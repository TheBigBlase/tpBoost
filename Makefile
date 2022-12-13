SRC = main.cpp
OBJ = ${SRC:.c=.o}
OUT = build
RM = rm 

CC=  /usr/bin/g++
CFLAGS 	= -ansi -Wall -std=c++17 -O3
CFDEBUG = -ansi -Wall -g -std=c++17 -O0

%.o: %.c
	 $(CC) -c $(CFLAGS)

build: 
	$(CC) $(SRC) $(CFLAGS) -o main.o

run: 
	$(CC) $(SRC) $(CFLAGS) -o main.o && time ./main.o
	
debug: 
	$(CC) $(SRC) $(CFDEBUG) -o main.o && gdb ./main.o
clean: 
	$(RM) main.o out.graphml
