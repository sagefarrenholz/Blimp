libBMP.a: BMPlib.o
	mkdir -p bin
	ar rcs bin/libBMP.a BMPlib.o
	rm -f samples/libBMP.a
	cp bin/libBMP.a samples/
	rm BMPlib.o

debug: BMPlibd.o
	mkdir -p bin
	ar rcs bin/libBMPd.a BMPlibd.o
	rm -f samples/libBMPd.a
	cp bin/libBMPd.a samples/
	rm BMPlibd.o

BMP_operations.o: src/BMP_operations.cpp
	g++ -c -o BMP_operations.o src/BMP_operations.cpp

BMPlib.o: src/BMPlib.cpp
	g++ -c -o BMPlib.o src/BMPlib.cpp

BMPlibd.o: src/BMPlib.cpp
	g++ -g1 -c -o BMPlibd.o src/BMPlib.cpp

