//
// Created by USER on 7/6/2024.
//

#include "atm_machine.h"

ATM::ATM(int atm_id, Bank& bank,std::string file_name, std::string log_file){
    this->atm_id = atm_id;
    // we want to seperate between the part of initializing atm and running it therefore I think commands is fine idea
    // otherwise we should think how to return if the file is corrupted before running
    std::ifstream file1_content(file_name);
    string line_file1, tmp;
    while(getline(file1_content,line_file1)){
        tmp = line_file1;
        this->commands.push_back(tmp);
    }
    file1_content.close();
    this->log_file = log_file
    this->bank = bank

//TODO: this part will be at the parsing arguments in the bank main - atm will get valid(!) file
//    if (!file1_content.good()){
//        //need to save state of the atm
//    }
//    else{
//
//    }


}

ATM::run(){
    size_t atm_command_index = 0;

    const char* delimiters = " \t\n";
    std::string current_command;

    while (atm_command_index < this->commands.size()){
        atm_command_index++;
        current_command = this->commands[atm_command_index];

        int i = 0, num_arg = 0;
        char* args[5];
        //this is the max args - we need that for each command to get the arguments, so maybe it will be better to create function that return char*
        //doing the rows below
        cmd = strtok(current_command, delimiters);
        if (cmd == NULL)
            printf("%s","handle void command");
            //TODO: handle wrong cmd - by the question it is fine but for start to see that it is working for us we can use it
        args[0] = cmd;
        for (i=1; i<MAX_ARG; i++)
        {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
        }
        //till here is the args start

        if (!strcmp(cmd, "O")){

        }
        else{
            int account_id = atoi(args[1]);
            int password = atoi(args[2]);
            bool is_right_password = this->bank.check_password(this->atm_id, account_id, password);
            if (is_right_password){
                if (!strcmp(cmd, "O")){

                }
                else if (!strcmp(cmd, "D")){

                }
                else if (!strcmp(cmd, "W")){

                }
                else if (!strcmp(cmd, "B")){

                }
                else if (!strcmp(cmd, "Q")){

                }
                else if (!strcmp(cmd, "T")){

                }
                else{
                    //TODO: we can assume that every command is legal by the question...
                    printf("%s","wrong command");
                }
            }


        }

    }
    return 1;

}