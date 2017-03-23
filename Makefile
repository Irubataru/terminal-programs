CXX      ?= g++
CXXFLAGS := -std=c++14 -Iinclude

SRCS     := src/stat_table.cpp
EXECS    := bin/stat_table

SRC_FOLDER := src
BIN_FOLDER := bin

all: $(EXECS)

.PHONY: clean all

bin/stat_table : src/stat_table.cpp | $(BIN_FOLDER)
	$(CXX) $(CXXFLAGS) -lboost_program_options $< -o $@

clean:
	rm -f $(BIN_FOLDER)/*

$(BIN_FOLDER):
	@mkdir $(BIN_FOLDER)
