ORIGIN  := $(realpath ./)
BUILD   := $(ORIGIN)/build
INCLUDE := $(ORIGIN)/include

export BUILD
export INCLUDE

all:
	$(MAKE) -C libcaf/
	$(MAKE) -C caftool/
