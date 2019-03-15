#pragma once
#include "BMPlib.h"

//Change one pixel by index
int BMP::changePixel(size_t px, color incolor) {
	pixeldata[px].changeColor(incolor, bitdepth);
	return 0;
}

//Change one pixel by coordinate
int BMP::changePixel(int32_t x, int32_t y, color incolor) {
	pixeldata[(x_size * (y - 1)) + (x - 1)].changeColor(incolor, bitdepth);
	return 0;
}

//Changes all pixels to one color
int BMP::fill(color incolor) {
	for (pixel& p : pixeldata) 
		p.changeColor(incolor, bitdepth);
	return 0;
}

//Changes all pixels in a row to one color
int BMP::fillRow(int32_t row, color incolor) {
	for (int32_t i = 0; i < x_size; i++)
		pixeldata[(x_size * row) + i].changeColor(incolor, bitdepth);
	return 0;
}

//Changes all pixels in a column to one color
int BMP::fillCol(int32_t col, color incolor) {
	for (int32_t i = 0; i < y_size; i++)
		pixeldata[(x_size * i) + col].changeColor(incolor, bitdepth);
	return 0;
}

//Set intensity of a row of pixels
int BMP::setRowLightness(int32_t row, double lightness) {
	for (int32_t i = 0; i < x_size; i++)
		pixeldata[(x_size * row) + i].setLightness(lightness, bitdepth);
	return 0;
}

//---------- Image Operations ----------

//Sets instensity of all pixels
int BMP::setImageLightness(double lightness) {
	for (pixel& p : pixeldata)
		p.setLightness(lightness, bitdepth);
	return 0;
}

//Sets saturation of all pixels
int BMP::setImageSaturation(double saturation) {
	for (pixel& p : pixeldata)
		p.setSaturation(saturation, bitdepth);
	return 0;
}