# This is a makefile that Heather made. Heather doesn't know a whole lot about
# makefiles. There should be _great care_ taken when using this makefile. It
# is dangerous.
CPPFLAGS=-Wall -std=c++11
BIN=bin
SRC=src
TEST=test/src

all: reducto test

reducto: $(BIN)/reducto.o
	g++ -o $(BIN)/reducto $(BIN)/reducto.o

$(BIN)/reducto.o: $(SRC)/reducto.cpp
	g++ $(CPPFLAGS) -c $(SRC)/reducto.cpp -o $(BIN)/reducto.o

test: $(BIN)/test.o
	g++ -o $(BIN)/test $(BIN)/test.o

$(BIN)/test.o: 
	g++ $(CPPFLAGS) -c $(TEST)/test.cpp -o $(BIN)/test.o

clean: 
	rm -r $(BIN)/*
