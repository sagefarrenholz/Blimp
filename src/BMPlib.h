/*
 - 
 == Sage Farrenholz, 2020
 -
 */

#pragma once

#include <cmath>
#include <cstdint>
#include <vector>
#include <string>

#include "BMP_header.h"

class BMP {

	/*
	 -
	 - I found the following links useful for information about the BMP image format, HSL, HSV, and bit depth.
	 -
	 == BMP File Format:
	 * Wikipedia link --> https://en.wikipedia.org/wiki/BMP_file_format#DIB_header_(bitmap_information_header)
	 * Archived link --> https://web.archive.org/web/20200508042056/https://en.wikipedia.org/wiki/BMP_file_format
         -
         == HSL, HSV, and HSB:
	 * Wikipedia link --> https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB
	 * Archived link --> https://web.archive.org/web/20200626151635/https://en.wikipedia.org/wiki/HSL_and_HSV
	 - 
	 == Color Bit Depth:
	 * Wikipedia link --> https://en.wikipedia.org/wiki/Color_depth  
	 * Archived link --> https://web.archive.org/web/20200623202619/https://en.wikipedia.org/wiki/Color_depth
	 - 
	 */

	//---------------- Image Data ---------------
	
	//Header required for generation
	BMP_Header header; 

	//Bit depth of the image
	uint8_t bd;

	//Dimensions of the image
	int32_t width;
	int32_t height;

	//Vector containing all of the image's pixels
	std::vector<uint32_t> data;

	//32bit palette, required for images <= 8 bit depth
	std::vector<uint32_t> palette;

	public:

	//--------------- Image attribute operations ---------------

	//Set Bitdepth
	int set_bit_depth(const unsigned& bd) {
		switch (bd) {
			case 1:
			case 2:
			case 4:
			case 8:
				palette.assign(pow_2(bd), 0);
				bitdepth = bd;
				return 0;
				break;
			case 16:
			case 24:
			case 32:
				palette.resize(0);
				bitdepth = bd;
				return 0;
				break;
			default:
				throw "Invalid Bitdepth";
				return 1;
		}
	}

	//Set Size
	void set_size(const int32_t& width, const int32_t& height) {
		this->width = width;
		this->height = height;
		pixeldata.resize(x*y);
		return 0;
	}

	//Add a color to the palette
	int set_palette(uint8_t index, uint32_t color) {
		palette[index].changeColor(incolor, 24);
		return 0;
	}

	//---------- Pixel operations -----------
	
	//Functions are defined in the BMP_operations.cpp file

	//By coordinate, indexed at (1,1)
	uint32_t set_pixel(int32_t x, int32_t y, color);
	//By index, indexed at 0
	uint32_t set_pixel(size_t index, color);

	//Fill the image with one color
	int fill(color);

	//Fill one row with a color
	int fill_row(int32_t row, color);

	//Fill one col with a color
	int fill_col(int32_t col, color);

	//Sets intensity of all pixels
	int set_lightness(double lightness);

	int set_saturation(double saturation);

	//Set intensity of a row of pixels
	//int set_lightness(int32_t row, double lightness);

	//---------- Information ----------

	unsigned get_bit_depth() const{	return bd; }

	int32_t get_width() const{ return width; }

	int32_t get_height() const{ return height; }

	long get_size() const{ return width * height; }

	//FIXME add bounds checking?

	//Returns a pixel by index, indexed at 0
	uint32_t get_pixel(const size_t& index) const{ return data[index]; }

	//Returns a pixel by coordinate, index begins at given arguement 
	uint32_t get_pixel(const int32_t& x, const int32_t& y, const int32_t& index = 1) const {
		return pixeldata[(width * (y - index)) + (x - index)];
	}

	//---------- I/O ----------

	//Copy from another image
	void copy(const std::string& file);

	//Generate the current image to the output directory
	void generate(const std::string& name);
	
	//--------------- Constructors ----------------
	
	BMP(int32_t width = 64, int32_t height = 64, uint8_t bitdepth = 24) {
		set_size(width, height);
		set_bitdepth(bitdepth);	
	};

	BMP(const std::string& filename) {
		copy(filename);
	};	
		
	private:

	//--------------- Internal Tools ---------------
	
	static constexpr pow_2(const unsigned& n) { return 1 << n; }
};

