#include "../src/blimp.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <iostream>

color white = 0x1;

void create_test_image(int32_t, int32_t, uint16_t);

int main(){
	
	// seed random generator
	srand(time(NULL));

	for (int i = 1; i <= 16; i *= 2){
		if (i == 16)
			white = 0xffff;
		create_test_image(64, 64, i);
	}

	white = 0xffffff;
	create_test_image(64, 64, 24);
	
	return 0;
}

void bw(BMP& img){
	if (img.get_bit_depth() <= 8)
		img.set_palette(1, 0xffffffff);
}

void create_test_image(int32_t width, int32_t height, uint16_t bit_depth) {

	BMP base{width, height, bit_depth}; base.generate("test_images/"+std::to_string(bit_depth)+"/blank.bmp"); // blank 24 bit 
	bw(base);
	
	BMP top_left{base};
	top_left.set_pixel(0, white); top_left.generate("test_images/"+std::to_string(bit_depth)+"/top_left.bmp"); // image with top left pixel as white

	BMP top_right{base};
	top_right.set_pixel(width - 1, white); top_right.generate("test_images/"+std::to_string(bit_depth)+"/top_right.bmp"); // image with top right pixel as white
	
	BMP bottom_left{base};
	bottom_left.set_pixel(0, height - 1, white); bottom_left.generate("test_images/"+std::to_string(bit_depth)+"/bottom_left.bmp"); // image with bottom left pixel as white

	BMP bottom_right{base};
	bottom_right.set_pixel(width - 1, height - 1, white); bottom_right.generate("test_images/"+std::to_string(bit_depth)+"/bottom_right.bmp"); // image with bottom right pixel as white

	BMP left_col{base};
	left_col.fill_col(0, white); left_col.generate("test_images/"+std::to_string(bit_depth)+"/left_col.bmp");

	BMP right_col{base};
	right_col.fill_col(width - 1, white); right_col.generate("test_images/"+std::to_string(bit_depth)+"/right_col.bmp");

	BMP top_row{base};
	top_row.fill_row(0, white); top_row.generate("test_images/"+std::to_string(bit_depth)+"/top_row.bmp");

	BMP bottom_row{base};
	bottom_row.fill_row(height - 1, white); bottom_row.generate("test_images/"+std::to_string(bit_depth)+"/bottom_row.bmp");

	BMP fill{base};
	fill.fill(white); fill.generate("test_images/"+std::to_string(bit_depth)+"/fill.bmp");
}
