//
// Created by USER on 7/6/2024.
//

#include "bank.h"
#include "atm_machine.h"
#include <string>
using namespace std;
std::string log_file="log.txt";
pthread_mutex_t log_mutex;
#define DEBUG 0
#define BANK_PASSWORD 0
#define CLEAR_SCREEN "\033[2J"
#define START_SCREEN "\033[1;1H"

bool endBank =0;
Bank::Bank(){
	this->bank_account = Account(0, BANK_PASSWORD ,0);
	this->bank_lock = ReadWriteLock();
}

Bank::~Bank(){

}


void Bank::write_msg_to_log(string msg)
{
	/**
     * @brief - write msg for the bank
    */
	pthread_mutex_unlock(&log_mutex);
	log.open(log_file, std::ios::out | std::ios::app);
	if(log.is_open()){
		log << "Bank: " << msg << endl;
	}
	log.close();
	pthread_mutex_unlock(&(log_mutex));
}

//TODO: print as they want
void Bank::lower_random_balance(){
	/**
     * @brief - take commission from all bank users
    */
	while(!endBank){
		//TODO: lock all the bank
		this->read_lock();

		std::srand(std::time(nullptr));
		float randomPer = std::rand() % 5 + 1;
		//FIXME maybe need to check map is sorted
		for (auto it = this->accounts.begin() ; it != this->accounts.end() ; it++){
			Account* account = &(it->second);
			account->write_lock();
		}
		for (auto it = this->accounts.begin() ; it != this->accounts.end() ; it++) {
			Account* account = &(it->second);
			int commission = account->withdrawn_by_per(randomPer);
			this->bank_account.deposit(commission);
			string msg = "commissions of " +  to_string(int(randomPer)) + " % were charged, bank gained " + to_string(commission) + " from account " + to_string(it->first);
			this->write_msg_to_log(msg);
		}
		//FIXME maybe need to check map is sorted
		for (auto it = this->accounts.begin() ; it != this->accounts.end() ; it++){
			Account* account = &(it->second);
			account->write_unlock();
		}

		this->read_unlock();
		//TODO: Unlock all the bank
		sleep(3); // sleep for 3 sec
	}
}

// it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts
//TODO: print as they want
void Bank::print_all_accounts(){
	/**
     * @brief - print bank accounts and the account of the bank, lock users
    */
	while(!endBank){
		this->read_lock();
		printf(CLEAR_SCREEN);
		printf(START_SCREEN);
		//FIXME maybe need to check map is sorted
		for (auto it = this->accounts.begin() ; it != this->accounts.end() ; it++){
			Account* account = &(it->second);
			account->write_lock();
		}
		cout << "Current Bank Status " << endl;
		for (auto it = this->accounts.begin() ; it != this->accounts.end() ; it++) {
			Account* account = &(it->second);
			account->print_account();
		}
		//FIXME maybe need to check map is sorted
		for (auto it = this->accounts.begin() ; it != this->accounts.end() ; it++){
			Account* account = &(it->second);
			account->write_unlock();
		}
		int bank_balance = this->bank_account.get_current_balance();
		cout << "The bank has " << bank_balance << " $\n";

		this->read_unlock();
		usleep(500 * 1000); // sleep for 0.5 sec
	}
} // it happened each 0.5 s  (locking all accounts) - we will need thread for this




void* work_wrap(void* atm) {
    ATM* atm_ptr = static_cast<ATM*>(atm);
    atm_ptr->run();
	//pthread_detach(pthread_self());
    pthread_exit(NULL);
}

void* print_wrap(void* bank) {
    Bank* bankPtr = static_cast<Bank*>(bank);
    bankPtr->print_all_accounts();
    pthread_exit(NULL);
}

void* take_comm(void* bank) {
    Bank* bankPtr = static_cast<Bank*>(bank);
    bankPtr->lower_random_balance();
    pthread_exit(NULL);
}

void Bank::read_lock(){
	/**
     * @brief - lock function for read
    */
	if (DEBUG == 1) cout << "read lock bank" << endl;
	this->bank_lock.enter_read();
}
void Bank::read_unlock(){
	/**
     * @brief - lock function for read
    */
	if (DEBUG == 1) cout << "read unlock bank" << endl;
	this->bank_lock.leave_read();
}
void Bank::write_lock(){
	/**
     * @brief - lock function for write
    */
	if (DEBUG == 1) cout << "write lock bank" << endl;
	this->bank_lock.enter_write();
}
void Bank::write_unlock(){
	/**
     * @brief - lock function for write
    */
	if (DEBUG == 1) cout << "write unlock bank" << endl;
	this->bank_lock.leave_write();
}


int main (int argc, char *argv[]) {
	//checking arguments

	if ( argc == 1) {
		cerr << "Bank error: illegal arguments" << endl ;
        exit(1);
	}


    bool file_corrupted = false;
    for (int i=1;i<argc;i++){
        std::ifstream file1_content(argv[i]);
        if (!file1_content.good()){

            file_corrupted = true;
        }
        file1_content.close();
    }
    if (file_corrupted){
        std::cerr << "Bank error: illegal arguments" << std::endl;
        exit(1);
    }
    std::ofstream log;
    log.open(log_file);
	if (!log) {
		std::cerr << "Bank error: log.open failed" << std::endl;
        exit(1);
	}
	log.close();

	Bank bank = Bank();
	pthread_t *atm_threads = new pthread_t[argc -1];
	pthread_t comission_threads;
	pthread_t print_thread;
	vector <ATM> atm_arr;
	for (int i = 0; i<argc -1; ++i){
		ATM atm =ATM(i,&bank,argv[i+1]);
		atm_arr.push_back(atm);
	}

	for (int i = 0 ; i < argc-1 ; i++) {
		if ( pthread_create(&atm_threads[i] , NULL ,work_wrap , &(atm_arr[i]) ) ){
			perror("Bank error: pthread_create failed");
			delete[] atm_threads;
	        exit(1);
		}
	}
	if (pthread_create(&comission_threads, NULL, take_comm, &bank)){
		perror("Bank error: pthread create failed");
		delete[] atm_threads;
        exit(1);
	}
	if (pthread_create(&print_thread, NULL, print_wrap, &bank)){
		perror("Bank error: pthread create failed");
		delete[] atm_threads;
        exit(1);
	}

	for (int i = 0; i<argc -1; ++i){
		pthread_join(atm_threads[i], NULL);
	}
	endBank =1;
	pthread_join(comission_threads, NULL);
	pthread_join(print_thread, NULL);

	delete[] atm_threads;
    exit(0);

}

