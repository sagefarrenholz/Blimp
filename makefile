libBMP.a: libBMP.o
	mkdir -p bin
	ar rcs bin/libBMP.a libBMP.o
	rm libBMP.o

debug: libBMPd.o
	mkdir -p bin/debug
	ar rcs bin/debug/libBMPd.a libBMPd.o
	rm libBMPd.o

libBMP.o: src/libBMP.cpp
	g++ -Wall -std=c++17 -c -o libBMP.o src/libBMP.cpp

libBMPd.o: src/libBMP.cpp
	g++ -Wall -std=c++17 -g1 -c -o libBMPd.o src/libBMP.cpp
