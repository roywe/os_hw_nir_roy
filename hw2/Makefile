# Makefile for the smash program
CC = g++ -g

CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC)
TARGET = bank
OBJS = bank.o atm_machine.o bank_obj.o account.o
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) $(CFLAGS) -o $(TARGET) $(OBJS)
# Creating the object files
account.o: account.cpp account.h
bank.o:
atm_machine.o: atm_machine.cpp atm_machine.h
bank_obj.o: bank_obj.cpp bank_obj.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

