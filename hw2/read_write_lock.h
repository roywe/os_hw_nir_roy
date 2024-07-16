//
// Created by USER on 7/15/2024.
//

#ifndef OS_HW_NIR_ROY_READ_WRITE_LOCK_H
#define OS_HW_NIR_ROY_READ_WRITE_LOCK_H

#include <pthread.h>

class ReadWriteLock {
	public:
		int rd_counter;
		pthread_mutex_t write_mutex;
		pthread_mutex_t read_mutex;
		ReadWriteLock();
		void enter_read();
		void leave_read();
		void enter_write();
		void leave_write();
};




#endif //OS_HW_NIR_ROY_READ_WRITE_LOCK_H
