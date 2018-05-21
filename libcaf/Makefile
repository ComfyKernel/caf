CC=g++
CFLAGS=-std=gnu++17 -I$(INCLUDE)
OUT=libcaf.a

SOURCES := $(shell find -type f -name "*.cpp")
BINS    := $(patsubst %.cpp,%.o, $(SOURCES))

all: $(BUILD)/$(OUT)

$(BUILD)/$(OUT): $(BINS)
	ar -rvs $(BUILD)/$(OUT) $(BINS)

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) -o $@
