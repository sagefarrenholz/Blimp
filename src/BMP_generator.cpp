#pragma once
#include "BMPlib.h"

int BMP::copy(std::string& file) {
	std::ifstream i(file);
	//i.see

	i.close();
	return 0;
}

int BMP::generateBMP(void) {
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
	outstream.open(outlocation, std::ios::binary);

	std::cout << "Generating..." << std::endl;

	//Please check BMPlib.h for a list of the variables being defined here
	BMP_HEADER header = {
		{'B','M'},
		sizeof(BMP_HEADER) + (x_size * y_size * std::ceil(bitdepth / 8)),
		0,
		sizeof(BMP_HEADER),
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
		padding = 4 - ((x_size * bitdepth / 8) % 4);

	//Write the palette to the file if one exists
	if (palettetoggle) {

	}

	//Iterate through each row starting from the bottom
	for (size_t row = y_size; row; row--) {
		//Write each pixel in the row from left to right
		for (int32_t col = 0; col < x_size; col++) {
			//Prints at least 1 byte, 2 for 16 bit depth, and 3 for 24 bit depth
			#define p (unsigned)pixeldata[(x_size * (row - 1)) + col]
			outstream.write(reinterpret_cast<char*>(pixeldata[(x_size * (row - 1)) + col].color24bit), std::ceil(bitdepth / 8));
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