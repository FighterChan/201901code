EXE = zone.elf
OBJ = main.o 
SRC = main.c 
CC = gcc
CFLAGS = -g
LDFLAGS =
EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(EXE) $^ 
OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $^
.PHONY:clean
clean:
	-$(RM) $(OBJ) $(EXE) 