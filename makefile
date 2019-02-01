EXE = zone.elf
OBJ = zone.o ip_match.o
SRC = zone.c ip_match.c
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