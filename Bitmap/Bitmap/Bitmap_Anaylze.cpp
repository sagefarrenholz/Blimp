#include "pch.h"
#include "Bitmap.h"

void analyzeBMP(std::ifstream& i) {

	char header[64];
	i.read(header, 64);
	BMP_HEADER* h = reinterpret_cast<BMP_HEADER*>(header);

	//Print out BMP DETAILS
	std::cout << "size: " << h->size << " bytes" << std::endl;
	std::cout << "offset: " << h->offset << " bytes" << std::endl;
	std::cout << "headersize: " << h->headersize << " bytes" << std::endl;
	std::cout << "bitdepth: " << h->bitdepth << std::endl;
	std::cout << "height: " << h->height << std::endl;
	std::cout << "width: " << h->width << std::endl;
	std::cout << "color palette size: " << h->colorcount << std::endl;

}