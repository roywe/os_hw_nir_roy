# Makefile for the smash program
CC = g++ -g

CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC)
TARGET = bank
OBJS = bank.o atm_machine.o account.o read_write_lock.o
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) $(CFLAGS) -o $(TARGET) $(OBJS)
# Creating the object files
read_write_lock.o: read_write_lock.cpp read_write_lock.h
	$(CCLINK) $(CFLAGS) -c read_write_lock.cpp
account.o: account.cpp account.h
	$(CCLINK) $(CFLAGS) -c account.cpp
atm_machine.o: atm_machine.cpp atm_machine.h
	$(CCLINK) $(CFLAGS) -c atm_machine.cpp
bank.o: bank.cpp bank.h
	$(CCLINK) $(CFLAGS) -c bank.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

