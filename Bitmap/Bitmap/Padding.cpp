#include "pch.h"
#include "Bitmap.h"

bool isPadding(const int& height, const int& width, const int& c, const uint32_t& bit, const double padwidth) {

	//Integral padding
	int intpad = (int) padwidth;

	//Tells how far into the row we are
	auto index = c % width;

	//If we are in the padding bytes return true
	if (index >= (width - intpad)) 
		return true;

	return false;
}