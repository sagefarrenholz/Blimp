#pragma once
#include "BMPlib.h"

int BMP::copy(std::string& file) {
	std::ifstream i(file);
	//i.see

	i.close();
	return 0;
}

int BMP::generate(void) {
	/*
	--
	--		BMP files are row reversed vertically meaning that the bottom is the top row in 
	--	the actual data. Not only this but, padding bytes must be added to maintain a row 
	--	width that is a multiple of 4 bytes. In BMPlib, the pixel data is exposed primitively. The
	--	generation step must compensate for this: all data must be correctly formatted. 
	--
	--		Following the BMP file structure spec the file must also possess a file header.
	--	The header is defined in the BMPlib.h header file. 
	--
	*/

	//Open file stream at output directory in binary mode
	outstream.open(output, std::ios::binary);

	std::cout << "Generating..." << std::endl;

	//Please check BMPlib.h for a list of the variables being defined here
	BMP_HEADER header = {
		{'B','M'},
		sizeof(BMP_HEADER) + palette.size() * 4U + (x_size * y_size * std::ceil((double) bitdepth / 8.0)),
		0,
		sizeof(BMP_HEADER) + palette.size() * 4U,
		40,
		x_size,
		y_size,
		1,
		bitdepth,
		0,
		0,
		0,
		0,
		0,
		0
	};
	outstream.write(reinterpret_cast<char*>(&header), sizeof(BMP_HEADER));

 	//The amount of needed bytes to create a multiple of 4 byte row
	if((x_size * bitdepth / 8) % 4)
		padding = 4 - static_cast<unsigned>(std::ceil(x_size * bitdepth / 8.0)) % 4;
	
	std::cout << (unsigned) padding << std::endl;

	//Write the palette to the file if one exists
	if (palette.size()) {
		for (unsigned i = 0; i < palette.size(); i++) {
			//Palette colors are in the order BGR
			outstream.write(reinterpret_cast<char*>(palette[i].color24bit), 3);
			/*outstream.write(reinterpret_cast<char*>(&palette[i].color24bit[2]), 1);
			outstream.write(reinterpret_cast<char*>(&palette[i].color24bit[1]), 1);
			outstream.write(reinterpret_cast<char*>(palette[i].color24bit), 1);*/
			//Palette colors require one reserve byte
			outstream.write(&padbyte, 1);
		}
	}

	//Iterate through each row starting from the bottom
	for (size_t row = y_size; row; row--) {
		//Write each pixel in the row from left to right
		for (int32_t col = 0; col < x_size; col++) {
			//Prints at least 1 byte, 2 for 16 bit depth, and 3 for 24 bit depth
			#define p (unsigned)pixeldata[(x_size * (row - 1)) + col]
			outstream.write(reinterpret_cast<char*>(pixeldata[(x_size * (row - 1)) + col].color24bit), std::ceil((double) bitdepth / 8.0));
		}
		//Add neccessary padding after each row
		if (padding)
			for(unsigned char i = 0; i < padding; i++)
				outstream.write(&padbyte, 1);
	}
	std::cout << "Done." << std::endl;

	outstream.close();
	return 0;
}
