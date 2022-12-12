SRC = main.c
OBJ = ${SRC:.c=.o}
OUT = build

CC 			= /usr/bin/g++
CFLAGS 	= -ansi -Wall -std=c99 
CFDEBUG = -ansi -Wall -g -std=c99  -o ./main.o

%.o: %.c
	 $(CC) -c $(CFLAGS)

clean:
		$(RM) $(OBJ) a.out $(OUT)/*

debug: $(SRC)
		$(CC) $(SRC) $(CFDEBUG) -o $(OUT)/a.out

run: $(SRC)
		$(CC) $(SRC) $(CFLAGS) -o $(OUT)/a.out && ./$(OUT)/main.o
