CC=g++
CFLAGS=-std=c++11 -I$(INCLUDE)
OUT=libcaf.a

all:
	$(CC) -c $(shell find -type f -name "*.cpp") $(CFLAGS) -o $(BUILD)/$(OUT).o
	ar -rvs $(BUILD)/$(OUT) $(BUILD)/$(OUT).o
