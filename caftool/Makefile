CC=g++
CFLAGS=-std=c++11 -L$(BUILD)/ -lcaf -ltinyxml -I$(INCLUDE)
OUT=caf

all:
	$(CC) $(shell find -type f -name "*.cpp") $(CFLAGS) -o $(BUILD)/$(OUT)
