/*
 -
 == Sage Farrenholz, 2020
 -
 */

#include "libBMP.h"

#include <fstream>
#include <iostream>
#include <cstdio>

void BMP::copy(const std::string& file) {
	
	char* strm_out = reinterpret_cast<char*>(&header);
	
	std::ifstream ifs{file, std::ios::binary};
	if (!ifs.good()) throw ifs.rdstate();

	// Read BMPHEADER
	ifs.read(strm_out, 14);
		
	// Read in DIBHEADER
	ifs.read(strm_out + 14, 4);

	// TODO Add support for V4, V5 headers eventually.
	// Currently, only supports 40 byte V1 DIB Header
	if (header.header_size > 40) throw "libBMP does not support V4 and V5 headers."; 
	
	// Read the rest of the DIBHEADER
	ifs.read(strm_out + 18, header.header_size - 4);

	// Allocates memory for palette and image data
	set_bit_depth(header.bit_depth);
	set_size(header.width, header.height);

	// Read into palette
	ifs.read(reinterpret_cast<char*>(palette), pal_size());

	// Read image dat, row reversed	
	for (int32_t r = get_height() - 1; r >= 0; r--)
		ifs.read(reinterpret_cast<char*>(get_row(r)), raw_width);

	ifs.close();
}


void BMP::generate(const std::string& file) {
	/*
	 -
	 -		BMP files are row reversed vertically meaning that the bottom is the top row in 
	 -	the actual data. Not only this but, padding bytes must be added to maintain a row 
	 -	width that is a multiple of 4 bytes. In libBMP, the pixel data is exposed primitively. The
	 -	generation step must compensate for this: all data must be correctly formatted. 
	 -
	 -		Following the BMP file structure spec the file must also possess a file header.
	 -	The header is defined in the libBMP.h header file. 
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
	std::ofstream ofs{file, std::ios::binary};

	if (!ofs.good()) { throw ofs.rdstate(); }

	// Size of bytes of this BMP image, this includes header, optional palette, and data
	header.size = sizeof(BMP_Header) + pal_size()  + data_size;
	// Offset to the actual picture data
	header.offset = sizeof(BMP_Header) + pal_size();
	// Number of colors in the palette
	header.color_count = static_cast<uint32_t>(pal_size());

	// Write the header to the BMP file
	ofs.write(reinterpret_cast<char*>(&header), sizeof(BMP_Header));

	std::cout << "Generating..." << std::endl;

	//Write the palette to the file if one exists
	/*for (unsigned i = 0; i < palette.size(); i++) {
		//Palette colors are in the order BGR
		ofs.write(reinterpret_cast<char*>(&palette[i]), 4);
	}*/

	if (palette != NULL) { ofs.write(reinterpret_cast<char*>(palette), pal_size()); }	

	std::cout << "Writing " << (height * raw_width)/1024.0 << "kB of image data." << std::endl;
	std::cout << "Total file size " << (header.size)/1024.0 << "kB." << std::endl;

	// Write image data from, rows vertically flipped
	for (int32_t r = get_height() - 1; r >= 0; r--)
		ofs.write(reinterpret_cast<char*>(get_row(r)), raw_width);

	ofs.close();
	
	std::cout << "Done." << std::endl;
}

void BMP::calc_raw_width() {
	// Size of actual row in bytes with padding
	raw_width = (unsigned)(std::ceil(get_bit_depth() * get_width() / 32.0)) * 4; 
}

uint8_t* BMP::get_row(const uint32_t& r) const {
	// Returns a pointer to the beginning of row 'r' of the image
	return data + raw_width * r;
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
uint32_t BMP::get_pixel(const size_t& i) const {
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
		return (uint32_t)(*ptr >> (7 - bd * bit_offset) & (pow_2(bd) - 1));	
	}
	return color;
}

uint32_t BMP::get_palette(const uint8_t& i) const { return palette[i]; }

void BMP::set_pixel(const size_t& i, const uint32_t& color) {
	uint8_t bd = get_bit_depth();
	uint8_t* ptr = get_pixel_ptr(i);
	if (bd >= 8){
		uint32_t* ptr_32 = reinterpret_cast<uint32_t*>(ptr);
		// Zero out unusued bytes
		if (bd != 32) {
			uint32_t mask = (pow_2(bd) - 1);
			if (i == get_width() * get_height() - 1) {
				// Precautions must be taken when setting the final pixel to prevent aliasing
				uint16_t* ptr_16 = reinterpret_cast<uint16_t*>(ptr); 	
				*ptr_16 = static_cast<uint16_t>(color);
				if (bd == 24) {
					*(ptr+2) = color >> 16;	
				}
			} else {
				// Creates a hole 
				*ptr_32 &= ~mask; 
				*ptr_32 |= mask & color;
			}
		} else {
			*ptr_32 = color;
		}
	} else {
	// If bitdepth < 8 some bit shifting magic is required to retrieve the color
		uint8_t bit_offset = i % get_width() % (8 / bd);
		// Bit mask
		uint8_t mask = (pow_2(bd) - 1);
		// Applies a hole to the byte where data will be places
		uint8_t ptr_hole = *ptr & ~(mask << (7 - bit_offset * bd));
		// Masked and shifted data, mask just in-case given data is larger
		uint8_t m_data = ((uint8_t)color & mask) << (7 - bd * bit_offset); 
		// Fill hole with masked and shifted data
		*ptr = ptr_hole | m_data;	
	}
}

void BMP::set_pixel(const int32_t& x, const int32_t& y, const uint32_t& color) {
	set_pixel(x + y * get_width(), color);
}

void BMP::set_palette(const int& i, const uint32_t& color){ palette[i] = color; }

void BMP::fill(const uint32_t& color){
	for (int64_t i = 0; i < get_size() - 1; i++){
		set_pixel(i, color);
	}
}

void BMP::fill_row(const int32_t& row, const uint32_t& color){
	for(int col = 0; col < get_width(); col++){
		set_pixel(col, row, color);
	}
}

void BMP::fill_col(const int32_t& col, const uint32_t& color){
	for (int row = 0; row < get_height(); row++){
		set_pixel(col, row, color);
	}
}

void BMP::fill_rect(const int32_t& x, const int32_t& y, const int32_t& width, const int32_t& height, const uint32_t& color){
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++){
			set_pixel(col + x, row + y, color);
		}
	}
}
