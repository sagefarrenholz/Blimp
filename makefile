libBMP.a: libBMP.o
	mkdir -p bin
	ar rcs bin/libBMP.a libBMP.o
	rm -f samples/libBMP.a
	cp bin/libBMP.a samples/
	rm libBMP.o

debug: libBMPd.o
	mkdir -p bin
	ar rcs bin/libBMPd.a libBMPd.o
	rm -f samples/libBMPd.a
	cp bin/libBMPd.a samples/
	rm libBMPd.o

BMP_operations.o: src/BMP_operations.cpp
	g++ -std=c++17 -c -o BMP_operations.o src/BMP_operations.cpp

libBMP.o: src/libBMP.cpp
	g++ -std=c++17 -c -o libBMP.o src/libBMP.cpp

libBMPd.o: src/libBMP.cpp
	g++ -std=c++17 -g1 -c -o libBMPd.o src/libBMP.cpp

