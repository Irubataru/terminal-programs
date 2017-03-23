CXX      ?= g++
CXXFLAGS := -std=c++14 -Iinclude

SRCS     := src/table_mean.cpp
EXECS    := bin/table_mean

SRC_FOLDER := src
BIN_FOLDER := bin

all: $(EXECS)

.PHONY: clean all

bin/table_mean : src/table_mean.cpp
	$(CXX) $(CXXFLAGS) -lboost_program_options $< -o $@

clean:
	rm -f $(BIN_FOLDER)/*
