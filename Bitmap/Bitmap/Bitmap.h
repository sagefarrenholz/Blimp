#pragma once

#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#pragma pack(push,2)
#pragma pack(show)
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

bool isPadding(const int&, const int&, const int&, const uint32_t&, const double);
void analyzeBMP(std::ifstream&);