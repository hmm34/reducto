# This is a makefile that Heather made. Heather doesn't know a whole lot about
# makefiles. There should be _great care_ taken when using this makefile. It
# is dangerous.
BIN=bin
SRC=src
TEST=test/src
CPPFLAGS=-Wall -std=c++11 -I /src -I /usr/local/Cellar/eigen/3.2.0/include/eigen3

all: reducto test

$(BIN)/test.o: 
	g++ $(CPPFLAGS) -c $(TEST)/test.cpp -o $(BIN)/test.o

reducto: $(BIN)/main.o $(BIN)/tools.o
	g++ -o $(BIN)/reducto $(BIN)/main.o $(BIN)/tools.o

$(BIN)/main.o: $(SRC)/main.cpp
	g++ $(CPPFLAGS) -c $(SRC)/main.cpp -o $(BIN)/main.o

$(BIN)/tools.o: $(SRC)/tools.cpp
	g++ $(CPPFLAGS) -c $(SRC)/tools.cpp -o $(BIN)/tools.o

test: $(BIN)/test.o $(BIN)/tools.o
	g++ -o $(BIN)/test $(BIN)/test.o $(BIN)/tools.o

clean: 
	rm -r $(BIN)/*
