#Makefile
OBJS = main.o functions.o
SOURCE = main.c functions.c
HEADER = headers.h functions.h
OUT = exec
CC = gcc
FLAGS = -g -c

# -g option enables debugging mode
# -c flag generates object code for separate files
all: $(OBJS)
	gcc -g main.c functions.c
	mv a.out $(OUT)

# create / compile the individual files separately
main.o: main.c
	$(CC) $(FLAGS) main.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

# cleaning
clean:
	rm -f $(OBJS) $(OUT)

# accounting
count:
	wc $(SOURCE) $(HEADER)
