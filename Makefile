# Makefile for soothsayer
TARGET = time
CFLAGS=-D __BOOK__

#LDFLAGS = -lncurses 
#CC = gcc
CC= /opt/cross/gcc-3.2.3-glibc-2.2.5/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-gcc

SRC = $(TARGET).c
#SRC += gnuplot_o.c


OBJ = $(SRC:.c=.o)

all: $(TARGET)
#	$(CC) -o $(TARGET) $(TARGET).c
	
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) -g $(OBJ) $(LDFLAGS)

obj: $(SRC)
	$(CC) $(CFLAGS) -c -g $(SRC) 
clean:
	rm -f *.o $(TARGET) 
	
