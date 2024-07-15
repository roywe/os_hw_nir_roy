//
// Created by USER on 7/6/2024.
//

#include "bank.h"
#include "atm_machine.h"
using namespace std;
std::string log_file="log.txt";

Bank::Bank(){
//    this->bank = {};
}
//bool Bank::add_account(int atm_id, int account_id, int password, int current_balance){
//    for (std::map<int, Account>::iterator it = bank.begin(); it != bank.end(); ++it) {
//        std::cout << "Key: " << it->first << ", Value: " << it->second.get_account_id() << std::endl;
//    }
//    if(bank.find(account_id) == bank.end()){
////        Account new_ac = Account(account_id, password, current_balance);
////        this->bank.insert ( std::pair<int,Account>(account_id,new_ac) );
//        this->bank[account_id] = Account(account_id, password, current_balance);
//        log << atm_id << " : New account id is "<< account_id <<" with password "<< password <<" and initial balance "<< current_balance <<endl;
//        return true;
//    }
//    log << "Error "<< atm_id <<" : Your transaction failed – account with the same id exists" <<endl;
//    return false;
//}
//TODO: validate params - un negetive balance, 4 digit password, account id that not strting with 0 and 4 digits
//TODO - also print when the account succeed
Bank::~Bank(){

}
//bool Bank::deposit(int atm_id, int account_id, int amount){
//    auto a = this->bank.find(account_id)->second;
//    int new_balance = a.deposit(amount);
//    return new_balance;
//} //TODO - locking the account id - lock account read+write mutex + bank_lock
//
//bool Bank::withdrawn(int atm_id, int account_id, int amount){
//    auto a = this->bank.find(account_id)->second;
//    int new_balance = a.withdrawn(amount);
//    return new_balance;
//} //TODO - locking the account id - lock account read+write mutex + bank_lock
//
//bool Bank::check_password(int atm_id, int account_id, int other_password){
//    Account a = this-> bank.find(account_id)->second;
//    if (a.check_password(other_password)){
//        return true;
//    }
//    return false;
//}
//// also we dont have to lock in order to check
//int Bank::show_balance(int atm_id, int account_id){
//    //TODO: need to lock account - lock account write mutex..
//    Account a = this->bank.find(account_id)->second;
//    int balance = a.get_current_balance();
//    return balance;
//}
//int Bank::delete_account(int atm_id, int account_id){
//    //TODO: need to lock account because if moving amount the it is not good - lock account write+read mutex + bank_lock
//    Account a = this->bank.find(account_id)->second;
//    int balance = a.get_current_balance();
//    printf("%s","try deleting");
//    this->bank.erase(account_id);
//    //TODO: check that actually erase and return answer if not...
//    a.print_account();
//
//    return balance;
//
//}
//bool Bank::move_between_accounts(int atm_id, int source_account,int target_account, int amount){
////TODO:   implement lock_account_by_order() //is it before we get them or after... because they can be deleted - maybe adding anothe lock will help
//    //getting 2 accounts and locking by account id, another option - get mapping and lock..
//    //we need the find to be somehow atomic - maybe external lock will help for this case
//    Account source = this->bank.find(source_account)->second;
//    Account dest = this->bank.find(target_account)->second;
//    int source_balance = source.withdrawn(amount);
//    if (source_balance<0){
//        return false;
//    }
//    else{
//        int target_balance = dest.deposit(amount);
//        log << atm_id<<": Transfer "<<amount<<" from account "<<source_account<<" to account "<<target_account<<" new account balance is " << source_balance << " new target account balance is "<< target_balance << endl;
//        return true;
//    }
//
//} // the action of the movement and print should be done atomically
//TODO: need to lock account a from reading+writing+bank_lock and lock the other from reading - sort the locks by ids size (to avoid deadlock)

void Bank::lower_random_balance(){
    //TODO: lock all the bank
    std::srand(std::time(nullptr));
    float randomPer = std::rand() % 5 + 1;
    printf("per: %f was chosen",randomPer);
    for (auto& pair : this->accounts) {
        int commission = pair.second.withdrawn_by_per(randomPer);
        log << "Bank: commissions of " <<  randomPer<< " % were charged, bank gained "<<commission << " from account "<< pair.first<<endl;
    }
    //TODO: Unlock all the bank

} // it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts

void Bank::print_all_accounts(){
//    for (auto it = this->bank.begin(); it != this->bank.end(); ++it)
//        std::cout << it->get_account_id() << '\n';
    for (const auto& pair : this->accounts) {
        pair.second.print_account();
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

