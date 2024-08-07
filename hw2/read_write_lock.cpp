//
// Created by USER on 7/15/2024.
//

#include "read_write_lock.h"
#include <iostream>
using namespace std;
ReadWriteLock::ReadWriteLock(){
    this->rd_counter = 0;
    pthread_mutex_init(&this->read_mutex, nullptr);
	pthread_mutex_init(&this->write_mutex, nullptr);

}

void ReadWriteLock::enter_read(){
    //when enter reading we want lock reader in order to atomically increase counter and check the write
    //then unlock it
    pthread_mutex_lock(&this->read_mutex);
    this->rd_counter++;
    if (rd_counter == 1){
        pthread_mutex_lock(&this->write_mutex);
    }
    pthread_mutex_unlock(&this->read_mutex);
}

void ReadWriteLock::leave_read(){
    //leave reader is the opposite of the enter one cause we need to unlock
	pthread_mutex_lock(&this->read_mutex);
    this->rd_counter -= 1;
    if (rd_counter == 0){
        pthread_mutex_unlock(&this->write_mutex);
    }
    pthread_mutex_unlock(&this->read_mutex);
}

void ReadWriteLock::enter_write(){
    pthread_mutex_lock(&this->write_mutex);
}

void ReadWriteLock::leave_write(){
    pthread_mutex_unlock(&this->write_mutex);
}