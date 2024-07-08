//
// Created by USER on 7/6/2024.
//

#include "bank.h"

using namespace std;

Bank::Bank(){

}
bool add_account(int atm_id, int account_id, int password, int current_balance){
    if(10000> password and password > 999 and 10000> account_id and account_id > 999 and current_balance>=0){
        Account new_ac = Account(account_id, password, current_balance);
        this->bank.insert ( std::pair<int,Account>(account_id,new_ac) );
        count << atm_id << " : New account id is "<<account_id<<" with password "<<password<<" and initial balance "<<current_balance<<endl
        return true;
    }
    cout << "Error "<< atm_id <<" : Your transaction failed – account with the same id exists" <<endl
    return false;
}
//TODO: validate params - un negetive balance, 4 digit password, account id that not strting with 0 and 4 digits
//TODO - also print when the account succeed
Bank::~Bank(){

}
bool Bank::change_balance(int atm_id, int account_id, int amount){
    Account a = this-> bank.find(account_id);
    printf("%d",a->get_current_balance());
} //TODO - locking the account id - lock account read+write mutex + bank_lock
bool Bank::check_password(int atm_id, int account_id, int other_password){
    Account a = this-> bank.find(account_id);
    printf("%d",a->check_password(other_password));
} //TODO: this action will be before all the other because found in 2 - 6 - maybe will be good to add atm_id to action..
// also we dont have to lock in order to check
void Bank::show_balance(int atm_id, int account_id){
    Account a = this-> bank.find(account_id);
    printf("%d",a->get_current_balance());
} //TODO: need to lock account becasue there are many cases that it can change - lock account write mutex + bank_lock
bool Bank::delete_account(int atm_id, int account_id){
    Account a = this-> bank.find(account_id);
    const auto count = std::erase_if(this->bank, [](const auto& item)
    {
        auto const& [key, value] = item;
        return key == account_id;
    });
    Account a = this-> bank.find(account_id);
    if( a == NULL){
        printf("%s","successfully deleted")
    }

} //TODO: need to lock account because if moving amount the it is not good - lock account write+read mutex + bank_lock
bool Bank::move_between_accounts(int source_account,int target_account, int balance){
    printf("%d %d %d",source_account,target_account,balance);
} // the action of the movement and print should be done atomically
//TODO: need to lock account a from reading+writing+bank_lock and lock the other from reading - sort the locks by ids size (to avoid deadlock)

void Bank::lower_random_balance(){
    for (auto it = this->bank.begin(); it != this->bank.end(); ++it)
        std::cout << it->account_id << '\n';
} // it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts

void Bank::print_all_accounts(){
    for (auto it = this->bank.begin(); it != this->bank.end(); ++it)
        std::cout << it->account_id << '\n';
} // it happened each 0.5 s  (locking all accounts) - we will need thread for this








int main(){
    // parse argument and check them - number of argument + all the files are valid
    // start bank object
    //  start atm - first without threads passing file and check it can handle it
    // till here essential so we know we are fine
    /////////////////////////////////////////////////////////////////////////////////
    //  after the first level - pthread and stuff (after objects are ready
}

