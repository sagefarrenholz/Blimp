/*
 * =====================================================================================
 *
 *       Filename:  BMP_header.h
 *
 *    Description:  Contains header structs used in the preamble of .bmp files  
 *
 *        Version:  1.0
 *        Created:  06/29/2020 07:17:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sage Farrenholz, 
 *
 * =====================================================================================
 */

#pragma pack(push,2)
struct BMP_Header {

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
