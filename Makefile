SRC = main.cpp
OBJ = ${SRC:.c=.o}
OUT = build
RM = rm 

CC=  /usr/bin/g++
CFLAGS 	= -ansi -Wall -std=c++17
CFDEBUG = -ansi -Wall -g -std=c++17  -o ./main.o

%.o: %.c
	 $(CC) -c $(CFLAGS)

build: 
	$(CC) $(SRC) $(CFLAGS) -o main.o

run: $(SRC)
		$(CC) $(SRC) $(CFLAGS) -o main.o && ./main.o
	
clean: 
	$(RM) main.o out.graphml
