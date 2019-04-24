BMPlib.a: BMP_operations.o BMP_generator.o
	mkdir -p bin
	ar rcs bin/BMPlib.a BMP_operations.o BMP_generator.o
	rm BMP_operations.o BMP_generator.o

BMP_operations.o: src/BMP_operations.cpp
	g++ -c -o BMP_operations.o src/BMP_operations.cpp

BMP_generator.o: src/BMP_generator.cpp
	g++ -c -o BMP_generator.o src/BMP_generator.cpp


