#pragma once

#include <cmath>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>

#pragma pack(push,2)

struct BMP_HEADER {

	//File Header Structure
	const char id[2];
	const uint32_t size;
	const uint32_t reserved;
	const uint32_t offset;

	//DIB Header Structure
	const uint32_t headersize;
	const int32_t width;
	const int32_t height;
	const uint16_t colorplanes;
	const uint16_t bitdepth;
	const uint32_t compress;
	const uint32_t imagesize;
	const int32_t xres;
	const int32_t yres;
	const uint32_t colorcount;
	const uint32_t importantcolors;

};
#pragma pack(pop)

/*
--
--		These typedefs are used to help users understand at what depth a certain color
--	is expected to be or assist users create colors.
--
*/
typedef uint8_t* color;		//Bitdepth Abstracted 
typedef uint8_t color8[1];  //8 bits
typedef uint8_t color16[2];	//16 bits
typedef uint8_t color24[3];	//24 bits

class BMP {

	//Bitdepth of our image
	unsigned bitdepth = 24;

	//This class simplifies pixels
	//Needed because managing color data at different bitdepths grows in complexity
	union pixel {
		uint8_t color1bit : 1;
		uint8_t color2bit : 2;
		uint8_t color4bit : 4;
		color8 color8bit;
		color16 color16bit;
		color24 color24bit = {0x00,0x00,0x00};

		//Change the color using pointer data
		int changeColor(color incolor, unsigned bit) {
			if (bit <= 8) {
				color8bit[0] = incolor[0];
			}
			if (bit == 16) {
				color16bit[0] = incolor[1];
				color16bit[1] = incolor[0];
			}
			if (bit == 24) {
				color24bit[0] = incolor[2];
				color24bit[1] = incolor[1];
				color24bit[2] = incolor[0];
			}
			return 0;		
		}

		//Changes the lightness of the color
		int setLightness(double lightness, unsigned bit) {
			if (bit == 24) {
				if (lightness <= 1) {
					double scalar = 1;
					//We must determine which channel is the main hue, in some cases multiple channels can be the same
					//If the first clause is true the first channel is the main hue or is equal to the others
					if (color24bit[0] >= color24bit[1] && color24bit[0] >= color24bit[2]){
						//This formula works by calculating a value that when multiplied by the driving channel gives a fraction of 255 equal to intensity
						scalar = (255 * lightness) / ((double) color24bit[0]);
					//The second channel is the main hue
					} else if (color24bit[1] >= color24bit[2]) {
						scalar = (255 * lightness) / ((double) color24bit[1]);
					//The third channel is the main hue
					} else {
						scalar = (255 * lightness) / ((double) color24bit[2]);
					}
					/*
					--
					--	All of these conditional statements are ensure that the color data doesn't overflow.
					--	If the color data overflows, for instance, 0xFF is multiplied by 2: the neighboring data
					--	in memory could be corrupted.
					--
					*/
					if (scalar * color24bit[0] < 0xFF)
						color24bit[0] *= scalar;
					else
						color24bit[0] = 0xFF;
					if (scalar * color24bit[1] < 0xFF)
						color24bit[1] *= scalar;
					else
						color24bit[1] = 0xFF;
					if (scalar * color24bit[2] < 0xFF)
						color24bit[2] *= scalar;
					else
						color24bit[2] = 0xFF;
				}
			}
			return 0;
		}

		//Retrieve the lightness of the color
		double getLightness() {
			if (color24bit[0] >= color24bit[1] && color24bit[0] >= color24bit[2]) {
				return ((double) color24bit[0]) / 255;
			} else if (color24bit[1] >= color24bit[2]) {
				return ((double) color24bit[1]) / 255;
			} else {
				return ((double) color24bit[2]) / 255;
			}
			return -1;
		}

		//Set the color based on an HSL value
		int setColorHSL(double hue, double saturation, double lightness) {

		}

		//Change the hue of the color 
		int setHue() {

		}

		//Changes the saturation of the color 
		int setSaturation(double saturation, unsigned bit) {
			if (bit == 24) {
				if (saturation <= 1) {
					double scalar = 1;
					uint8_t lowByte = 0x00;
					uint16_t SatByte = 0x00;
					//Determine the lowest channel using in coalition with the highest to determine a fully desaturated average
					if (color24bit[0] <= color24bit[1] && color24bit[0] <= color24bit[2]) {
						lowByte = color24bit[0];
					} else if (color24bit[1] <= color24bit[2]) {
						lowByte = color24bit[1];
					} else {
						lowByte = color24bit[2];
					}
					//We must determine which channel is the main hue, in some cases multiple channels can be the same
					//If the first clause is true the first channel is the main hue or is equal to the others
					if (color24bit[0] >= color24bit[1] && color24bit[1] >= color24bit[2]) {
						/*
						--
						--	This formula works by calculating a value that when multiplied by the driving channel gives a 
						--	fraction of the difference between 255 and the average of the lowest and highest channel
						--
						*/
						if (color24bit[0]) {
							SatByte = (color24bit[0] + lowByte) / 2;
						}
					//The second channel is the main hue
					} else if (color24bit[1] >= color24bit[2]) {
						if (color24bit[1]) {
							SatByte = (color24bit[1] + lowByte) / 2;
						}
					} else {
						if (color24bit[2]) {
							SatByte = (color24bit[2] + lowByte) / 2;
						}
					}
					/*
					--
					--	All of these conditional statements are ensure that the color data doesn't overflow.
					--	If the color data overflows, for instance, 0xFF is multiplied by 2: the neighboring data
					--	in memory could be corrupted.
					--
					*/
					//TODO: Clean all of this up
					scalar = (((0xFF - SatByte) * saturation) + SatByte) / color24bit[0];
					if (scalar * color24bit[0] < 0xFF)
						color24bit[0] *= scalar;
					else
						color24bit[0] = 0xFF;
					//if (color24bit[0] >= SatByte) {
					//	if (scalar * color24bit[0] < 0xFF)
					//		color24bit[0] *= scalar;
					//	else
					//		color24bit[0] = 0xFF;
					//}
					//else {
					//	if (color24bit[0]) {
					//		if (color24bit[0] / scalar > 0x00)
					//			color24bit[0] /= scalar;
					//		else
					//			color24bit[0] = 0x00;
					//	}
					//}
					scalar = (((0xFF - SatByte) * saturation) + SatByte) / color24bit[1];
					if (scalar * color24bit[1] < 0xFF)
						color24bit[1] *= scalar;
					else
						color24bit[1] = 0xFF;
					//if (color24bit[1] >= SatByte) {
					//	if (scalar * color24bit[1] < 0xFF)
					//		color24bit[1] *= scalar;
					//	else
					//		color24bit[1] = 0xFF;
					//} else {

					//	if (color24bit[1]) {
					//		if (color24bit[1] / scalar > 0x00)
					//			color24bit[1] /= scalar;
					//		else
					//			color24bit[1] = 0x00;
					//	} 
					//}
					scalar = (((0xFF - SatByte) * saturation) + SatByte) / color24bit[2];
					if (scalar * color24bit[2] < 0xFF)
						color24bit[2] *= scalar;
					else
						color24bit[2] = 0xFF;
					//if (color24bit[2] >= SatByte) {
					//	if (scalar * color24bit[2] < 0xFF)
					//		color24bit[2] *= scalar;
					//	else
					//		color24bit[2] = 0xFF;
					//}
					//else {
					//	if (color24bit[2]) {
					//		if (color24bit[2] / scalar > 0x00)
					//			color24bit[2] /= scalar;
					//		else
					//			color24bit[2] = 0x00;
					//	}
					//}
				}
			}
			return 0;
		}
	};

	//Dimensions
	int32_t x_size = 64;
	int32_t y_size = 64;

	//Vector containing all image pixels
	std::vector<pixel> pixeldata = std::vector<pixel>(x_size*y_size);

	//color24 palette for low bit depth images
	std::vector<pixel> palette;

	//Output Directory
	std::string output = "myimage.bmp";

	//Output Filestream
	std::ofstream outstream;

	//Padding amount 0-4
	uint8_t padding = 0;
	//Byte used to pad rows
	char padbyte = 0x00;

	//Color memory, NOT PALETTE. Used internally to store color data
	std::vector<uint8_t> colormem;

	public:

	//--------------- Image attribute operations ---------------

	//Set Bitdepth
	int setBitDepth(const unsigned& bd) {
		pixel temppix;
		color24 tempblack = { 0,0,0 };
		temppix.changeColor(tempblack, 24);
		switch (bd) {
		case 1:
		case 2:
		case 4:
		case 8:
			palette.assign(std::pow(2, bd) , temppix);
			bitdepth = bd;
			return 0;
			break;
		case 16:
		case 24:
			palette.resize(0);
			bitdepth = bd;
			return 0;
			break;
		default:
			throw "Invalid Bitdepth";
			return 1;
		}
	}

	//Set Dimensions
	int setDimensions(const int32_t x, const int32_t& y) {
		x_size = x;
		y_size = y;
		pixeldata.resize(x*y);
		return 0;
	}

	//Set the output directory for image generation
	int setOutput(const std::string& inDir) {
		output = inDir;
		return 0;
	}

	//Manually set color24 palette
	/*int setPalette(std::vector<color24>& pal) {
		palette = pal;
		return 0;
	}*/

	//Manually set pixel data NOT RECOMMENDED
	int setPixeldata(const std::vector<pixel>& inpixels) {
		if (inpixels.size() == pixeldata.size()) {
			pixeldata = inpixels;
			return 0;
		}
		return 1;
	}

	//Change the default padding byte, maybe for a secret message? :D
	int setPadding(const unsigned char& c) {
		padbyte = c;
		return 0;
	}

	//Add a color to the palette
	int setPaletteColor(uint8_t index, color24 incolor) {
		palette[index].changeColor(incolor, 24);
		return 0;
	}

	//---------- Pixel operations -----------
	//Functions are defined in the BMP_operations.cpp file

	//Change one pixel
	//By coordinate
	int changePixel(int32_t x, int32_t y, color);
	//By index
	int changePixel(size_t index, color);

	//Fill the image with one color
	int fill(color);

	//Fill one row with a color
	int fillRow(int32_t row, color);

	//Fill one col with a color
	int fillCol(int32_t col, color);

	//Sets intensity of all pixels
	int setImageLightness(double lightness);

	int setImageSaturation(double saturation);

	//Set intensity of a row of pixels
	int setRowLightness(int32_t row, double lightness);

	//---------- Information ----------

	unsigned getBitdepth() const{
		return bitdepth;
	}

	int getWidth() const{
		return x_size;
	}

	int getHeight() const{
		return y_size;
	}

	long getSize() const{
		return x_size * y_size;
	}

	//Returns a pixel by index. A pixel is just an array of color data.
	pixel getPixel(size_t index) const{
		return pixeldata[index];
	}

	//Returns a pixel by coordinate. A pixel is just an array of color data.
	pixel getPixel(int32_t x, int32_t y) const {
		return pixeldata[(x_size * (y - 1)) + (x - 1)];
	}
	
	//---------- Tools ----------


	//---------- Generation ----------

	//Copy from another image
	int copy(std::string& file);

	//Generate the current image to the output directory
	int generate(void);

};