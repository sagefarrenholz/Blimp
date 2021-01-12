#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/libBMP.h"

TEST_CASE("Blank image construction", "[creation]") {

	SECTION("No arguments") {
		BMP img{};
		REQUIRE( img.get_width() == 64 );
		REQUIRE( img.get_height() == 64 );
		REQUIRE( img.get_bit_depth() == 24 );
	}	

	SECTION("With size parameters") {
		BMP img{128, 128};
		REQUIRE( img.get_width() == 128 );
		REQUIRE( img.get_height() == 128 );
		REQUIRE( img.get_bit_depth() == 24 );
	}
	

}

//TEST_CASE("Setting pixel values", "[manipulation]") {
	
