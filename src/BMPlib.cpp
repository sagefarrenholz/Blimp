/*
 -
 == Sage Farrenholz, 2020
 -
 */

#include "BMPlib.h"

#include <fstream>
#include <iostream>
#include <cstdio>

/* 
int BMP::copy(std::string& file) {
	std::ifstream i(file);
	//i.see

	i.close();
	return 0;
}
*/

void BMP::generate(const std::string& filename) {
	/*
	 -
	 -		BMP files are row reversed vertically meaning that the bottom is the top row in 
	 -	the actual data. Not only this but, padding bytes must be added to maintain a row 
	 -	width that is a multiple of 4 bytes. In BMPlib, the pixel data is exposed primitively. The
	 -	generation step must compensate for this: all data must be correctly formatted. 
	 -
	 -		Following the BMP file structure spec the file must also possess a file header.
	 -	The header is defined in the BMPlib.h header file. 
	 -
	 */

	// Bit depth
	const auto& bit_depth = get_bit_depth();
	// Image width
	const auto& width = get_width();
	// Image height
	const auto& height = get_height();

	// Size of the pixel data (includes padding), in bytes
	const size_t data_size = raw_width * height;

	// Open file stream at output directory in binary mode
	std::ofstream ofs{filename.c_str(), std::ios::binary};

	// Size of bytes of this BMP image, this includes header, optional palette, and data
	header.size = sizeof(BMP_Header) + palette.size() * 4U + data_size;
	// Offset to the actual picture data
	header.offset = sizeof(BMP_Header) + palette.size() * 4U;
	// Number of colors in the palette
	header.color_count = static_cast<uint32_t>(palette.size());

	// Write the header to the BMP file
	ofs.write(reinterpret_cast<char*>(&header), sizeof(BMP_Header));

	std::cout << "Generating..." << std::endl;

	//Write the palette to the file if one exists
	for (unsigned i = 0; i < palette.size(); i++) {
		//Palette colors are in the order BGR
		ofs.write(reinterpret_cast<char*>(palette[i]), 4);
	}

	std::cout << "Writing " << (height * raw_width)/1000.0 << "kB of image data." << std::endl;
	std::cout << "Total file size " << (header.size)/1000.0 << "kB." << std::endl;

	// Write image data from, rows vertically flipped
	for (int32_t r = get_height() - 1; r >= 0; r--)
		ofs.write(reinterpret_cast<char*>(data1 + r * raw_width), raw_width);

	ofs.close();
	
	std::cout << "Done." << std::endl;
}

void BMP::fill(const uint32_t& color){
	for (int64_t i = 0; i < get_size(); i++){
		set_pixel(i, color);
	}
}

void BMP::calc_raw_width() {
	// Size of actual row in bytes with padding
	raw_width = (unsigned)(std::ceil(get_bit_depth() * get_width() / 32.0)) * 4; 
}

uint8_t* BMP::get_row(const uint32_t& r) const {
	// Returns a pointer to the beginning of row 'r' of the image
	return data1 + raw_width * r;
}

// Returns a pointer to the byte containing the pixel
uint8_t* BMP::get_pixel_ptr(const size_t& i) const {
	uint8_t bd = get_bit_depth();
	// Points to the row of this pixel
	uint8_t* row = get_row(i / get_width());
	// The offset from the beginning of the row to this pixel, NOT in bytes
	size_t offset = i % get_width();
	// offset * bd / 8 gives byte offset (e.g. 1 offset at 8 bit depth = 1 byte, 9 offset at 1 bit depth = 1 byte)
	return row + offset * bd / 8;
}

// Returns the color value of a pixel
uint32_t BMP::get_pixel(const size_t& i) {
	uint8_t bd = get_bit_depth();
	uint32_t color;
	const uint8_t* ptr = get_pixel_ptr(i);
	if (bd >= 8){
		const uint32_t* ptr_32 = reinterpret_cast<const uint32_t*>(ptr);
		if (bd != 32)
			color = (pow_2(bd) - 1) & *ptr_32;
		else 
			color = *ptr_32;
	} else {
	// If bitdepth < 8 some bit shifting magic is required to retrieve the color
		uint8_t bit_offset = i % get_width() % (8 / bd);
		return (uint32_t)(*ptr >> bd * bit_offset & (pow_2(bd) - 1));	
	}
	return color;
}

void BMP::set_pixel(const size_t& i, const uint32_t& color) {
	uint8_t bd = get_bit_depth();
	uint8_t* ptr = get_pixel_ptr(i);
	if (bd >= 8){
		uint32_t* ptr_32 = reinterpret_cast<uint32_t*>(ptr);
		// Zero out unusued bytes
		if (bd != 32) {
			uint32_t mask = (pow_2(bd) - 1);
			// Creates a hole 
			*ptr_32 &= ~mask;
			*ptr_32 |= mask & color;
		} else {
			*ptr_32 = color;
		}
	} else {
	// If bitdepth < 8 some bit shifting magic is required to retrieve the color
		uint8_t bit_offset = i % get_width() % (8 / bd);
		// Bit mask
		uint8_t mask = (pow_2(bd) - 1);
		// Applies a hole to the byte where data will be places
		uint8_t ptr_hole = *ptr & ~(mask << (bit_offset * bd));
		// Masked and shifted data, mask just in-case given data is larger
		uint8_t m_data = ((uint8_t)color & mask) << (bd * bit_offset); 
		// Fill hole with masked and shifted data
		*ptr = ptr_hole | m_data;	
	}
}

void BMP::set_pixel(const int32_t& x, const int32_t& y, const uint32_t& color) {
	set_pixel(x + y * get_width(), color);
}
