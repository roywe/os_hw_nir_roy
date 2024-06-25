# Makefile for the smash program
CC = g++ -g

CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC)
TARGET = smash
OBJS = smash.o commands.o signals.o job_class.o
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CCLINK) $(CFLAGS) -o $(TARGET) $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h job_class.h
smash.o: commands.h signals.h smash.h smash.cpp
signals.o: signals.cpp signals.h
job_class.o: job_class.cpp job_class.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

