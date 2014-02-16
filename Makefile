# This is a makefile that Heather made. Heather doesn't know a whole lot about
# makefiles. There should be _great care_ taken when using this makefile. It
# is dangerous.
CPPFLAGS=-Wall -std=c++0x
BIN=bin
SRC=src

all: reducto

reducto: $(BIN)/reducto.o
	g++ -o $(BIN)/reducto $(BIN)/reducto.o

$(BIN)/reducto.o: $(SRC)/reducto.cpp
	g++ $(CPPFLAGS) -c $(SRC)/reducto.cpp -o $(BIN)/reducto.o

clean: 
	rm -r $(BIN)/*
