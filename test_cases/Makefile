CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I../

POBJS = 

TEST_MAIN = test_menu

SRC = test_menu.c ../menu.c

TARGET_ALL = $(TEST_MAIN)

all: $(TARGET_ALL)

$(TEST_MAIN):
	gcc -o $@ $(SRC) $(CFLAGS)
	
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
