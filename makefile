libBlimp.a: blimp.o
	mkdir -p bin
	ar rcs bin/libBlimp.a blimp.o
	rm blimp.o

debug: blimp.o
	mkdir -p bin/debug
	ar rcs bin/debug/blimp.a blimp.o
	rm blimp.o

blimp.o: src/blimp.cpp
	g++ -Wall -std=c++17 -c -o blimp.o src/blimp.cpp

blimp.o: src/blimp.cpp
	g++ -Wall -std=c++17 -g1 -c -o blimp.o src/blimp.cpp
