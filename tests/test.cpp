#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/blimp.h"

constexpr static int MAX_BIT_DEPTH = 16;
constexpr static int DEFAULT_BIT_DEPTH = 24;
constexpr static int DEFAULT_WIDTH = 64;
constexpr static int DEFAULT_HEIGHT = 64;

TEST_CASE("Blank image with no args", "[creation]") {
	BMP img{};
	REQUIRE( img.get_width() == 64 );
	REQUIRE( img.get_height() == 64 );
	REQUIRE( img.get_bit_depth() == 24 );
	REQUIRE( img.get_pixel(0) == 0);
}

TEST_CASE("Blank image with size args", "[creation]") {
	BMP img{128, 128};
	REQUIRE( img.get_width() == 128 );
	REQUIRE( img.get_height() == 128 );
	REQUIRE( img.get_bit_depth() == 24 );
	REQUIRE( img.get_pixel(0) == 0);
}
	
TEST_CASE("Blank image with all args", "[creation]") {
	BMP img{32, 32, 8};
	REQUIRE( img.get_width() == 32 );
	REQUIRE( img.get_height() == 32 );
	REQUIRE( img.get_bit_depth() == 8 );
	REQUIRE( img.get_pixel(0) == 0);
}

TEST_CASE("Image resize", "[manipulation]") {
	BMP img{};
	REQUIRE( img.get_width() == 64 );
	REQUIRE( img.get_height() == 64 );
	REQUIRE( img.get_bit_depth() == 24 );
	REQUIRE( img.get_pixel(0) == 0);

	SECTION("Increase size") {
		img.set_size(128, 128);
		REQUIRE( img.get_width() == 128 );
		REQUIRE( img.get_height() == 128 );
		REQUIRE( img.get_bit_depth() == 24 );
		REQUIRE( img.get_pixel(0) == 0);
	}
	
	SECTION("Decrease size") {
		img.set_size(32, 32);
		REQUIRE( img.get_width() == 32 );
		REQUIRE( img.get_height() == 32 );
		REQUIRE( img.get_bit_depth() == 24 );
		REQUIRE( img.get_pixel(0) == 0);
	}
}


TEST_CASE("Image bit depth change", "[manipulation]") {
	BMP img{64, 64, 2};
	REQUIRE( img.get_width() == 64 );
	REQUIRE( img.get_height() == 64 );
	REQUIRE( img.get_bit_depth() == 2 );
	REQUIRE( img.get_pixel(0) == 0);

	SECTION("Increase bit depth") {
		img.set_bit_depth(24);
		REQUIRE( img.get_width() == 64 );
		REQUIRE( img.get_height() == 64 );
		REQUIRE( img.get_bit_depth() == 24 );
		REQUIRE( img.get_pixel(0) == 0);
	}
	
	SECTION("Decrease bit depth") {
		img.set_bit_depth(1);
		REQUIRE( img.get_width() == 64 );
		REQUIRE( img.get_height() == 64 );
		REQUIRE( img.get_bit_depth() == 1 );
		REQUIRE( img.get_pixel(0) == 0);
	}
}

TEST_CASE("Set, get test", "[operation]") {
	for (int i = 1; i <= MAX_BIT_DEPTH; i*=2) {
		REQUIRE(( i % 2 == 0 || i == 1));
		BMP img{DEFAULT_WIDTH, DEFAULT_HEIGHT, (uint16_t) i};
		REQUIRE( img.get_width() == DEFAULT_WIDTH );
		REQUIRE( img.get_height() == DEFAULT_HEIGHT );
		REQUIRE( img.get_bit_depth() == i );
		REQUIRE( img.get_pixel(0) == 0);

		img.set_pixel(0, 0, 1);
		REQUIRE( img.get_pixel(0) == 1); 

		img.set_pixel(DEFAULT_WIDTH - 1, DEFAULT_HEIGHT - 1, 1);
		REQUIRE( img.get_pixel(DEFAULT_WIDTH - 1, DEFAULT_HEIGHT - 1) == 1); 

		img.set_pixel(0, DEFAULT_HEIGHT - 1, 1);
		REQUIRE( img.get_pixel(0, DEFAULT_HEIGHT - 1) == 1); 

		img.set_pixel(DEFAULT_WIDTH - 1, 0, 1);
		REQUIRE( img.get_pixel(DEFAULT_WIDTH - 1, 0) == 1); 
	}
}
