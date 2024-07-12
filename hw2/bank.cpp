//
// Created by USER on 7/6/2024.
//

#include "bank.h"
#include "atm_machine.h"
using namespace std;
std::string log_file="log.txt";

Bank::Bank(){
    this->bank = {};
}
bool Bank::add_account(int atm_id, int account_id, int password, int current_balance){
    if(true){
//        Account new_ac = Account(account_id, password, current_balance);
//        this->bank.insert ( std::pair<int,Account>(account_id,new_ac) );
        this->bank[account_id] = Account(account_id, password, current_balance);
        log << atm_id << " : New account id is "<< account_id <<" with password "<< password <<" and initial balance "<< current_balance <<endl;
        return true;
    }
    log << "Error "<< atm_id <<" : Your transaction failed – account with the same id exists" <<endl;
    return false;
}
//TODO: validate params - un negetive balance, 4 digit password, account id that not strting with 0 and 4 digits
//TODO - also print when the account succeed
Bank::~Bank(){

}
bool Bank::change_balance(int atm_id, int account_id, int amount){
    auto a = this->bank.find(account_id)->second;
    printf("%d",a.get_current_balance());
    return true;
} //TODO - locking the account id - lock account read+write mutex + bank_lock
bool Bank::check_password(int atm_id, int account_id, int other_password){
    Account a = this-> bank.find(account_id)->second;
    printf("%d",a.check_password(other_password));
    return true;
} //TODO: this action will be before all the other because found in 2 - 6 - maybe will be good to add atm_id to action..
// also we dont have to lock in order to check
void Bank::show_balance(int atm_id, int account_id){
    Account a = this->bank.find(account_id)->second;
    printf("%d",a.get_current_balance());
} //TODO: need to lock account becasue there are many cases that it can change - lock account write mutex + bank_lock
bool Bank::delete_account(int atm_id, int account_id){
    Account a = this->bank.find(account_id)->second;
//    const auto count = std::erase_if(this->bank, [](const auto& item)
//    {
//        auto const& [key, value] = item;
//        return (key) == account_id;
//    });
//    if( a == nullptr){
//        printf("%s","successfully deleted");
//    }
    return true;

} //TODO: need to lock account because if moving amount the it is not good - lock account write+read mutex + bank_lock
bool Bank::move_between_accounts(int atm_id, int source_account,int target_account, int balance){
    printf("%d %d %d",source_account,target_account,balance);
    return true;
} // the action of the movement and print should be done atomically
//TODO: need to lock account a from reading+writing+bank_lock and lock the other from reading - sort the locks by ids size (to avoid deadlock)

void Bank::lower_random_balance(){
    for (const auto& pair : this->bank) {
        std::cout << "Key: " << pair.first << ", Value: ";//<< pair.second.get_account_id();
    }
//    for (auto it = this->bank.begin(); it != this->bank.end(); ++it)
//        std::cout << it->get_account_id() << '\n';
} // it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts

void Bank::print_all_accounts(){
//    for (auto it = this->bank.begin(); it != this->bank.end(); ++it)
//        std::cout << it->get_account_id() << '\n';
    for (const auto& pair : this->bank) {
        std::cout << "Key: " << pair.first << ", Value: " ;//<< pair.second.get_account_id();

    }

} // it happened each 0.5 s  (locking all accounts) - we will need thread for this








int main (int argc, char *argv[]) {
	//checking arguments
	if ( argc == 1) {
		cerr << "Bank error: illegal arguments" << endl ;
        exit(1);
	}
    // parse argument and check them - number of argument + all the files are valid
    // start bank object
    //  start atm - first without threads passing file and check it can handle it
    // till here essential so we know we are fine
    /////////////////////////////////////////////////////////////////////////////////
    //  after the first level - pthread and stuff (after objects are ready

    // TODO: cause segmentation error: need to fix it!!!!!!!!!!!!!!!!!
//    log.open(log_file, std::ios::out | std::ios::trunc);
//    if (!log) {
//        std::cerr << "Error opening output file!" << std::endl;
//        exit(1);
//    }
//    cout<<"fine";
//    log.close();
    Bank bank = Bank();

    bool file_corrupted = false;
    for (int i=1;i<argc;i++){
        std::ifstream file1_content(argv[i]);
        if (!file1_content.good()){

            file_corrupted = true;
        }
        file1_content.close();
    }
    if (file_corrupted){
        std::cerr << "Error opening input file!" << std::endl;
        exit(1);
    }
    std::ofstream log;
    log.open(log_file);
	if (!log) {
		std::cerr << "Error opening file: " << log_file << std::endl;
		return 1;
	}


    std::string atm_command_file = argv[1];
    ATM atm0 = ATM(0, bank, atm_command_file);
    atm0.run();
    log.close();

}

