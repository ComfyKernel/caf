CC=g++
CFLAGS=-std=c++11 -fPIC -shared -I$(INCLUDE)
OUT=libcaf.so

all:
	$(CC) $(shell find -type f -name "*.cpp") $(CFLAGS) -o $(BUILD)/$(OUT)
