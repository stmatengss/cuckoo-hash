default: bin/test

CC=g++
CFLAG=-O3 -std=c++11
CLIBS=-libboost

bin/test: test/test.cpp lib/cuckoo_hash.hpp lib/header.hpp
	$(CC) $(CFLAG) -o $@ $<

clean:
	rm -rf bin/*


