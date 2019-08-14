ROOT_CONFIG := $(shell root-config --cflags)
ROOT_LIBS := $(shell root-config --libs)
RATROOT := $(shell echo $(RATROOT))
RAT_LIBRARY_PATH := $(RATROOT)/lib
RAT_INCLUDE_PATH := $(RATROOT)/include
RAT_LIBRARY := RATEvent_Linux
PROGRAM := ./src/skim.cc ./src/rand.cc ./src/main.cc
OUTPUT_FILE := main

standard: $(PROGRAM)
	g++ -Wall -std=c++11 $(ROOT_CONFIG) -I$(RAT_INCLUDE_PATH) -I$(RAT_INCLUDE_PATH)/libpq -L$(RAT_LIBRARY_PATH)  $(PROGRAM) -o $(OUTPUT_FILE) -l$(RAT_LIBRARY) $(ROOT_LIBS)

clean:
	rm -f $(OUTPUT_FILE) *_cc.d *_cc.so *.*~ *~
