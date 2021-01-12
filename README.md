# Blimp
A dead simple library for handling BMP format images. Great for programs that need fast image generation without the complexity. Support for antiquated bit depths such as 1, 2, 4, 8, 16, as well as modern bit depths: 24 and 32.

## Features
- BMP image generation at all BMP supported bitdepths: 1, 2, 4, 8, 16, 24, and 32;
- Fast in-memory manipulation of images with lightweight getter and setter wrappers for pixels.
- A primitve but powerful interface that stays out of your way.

## Dependencies
Just the C standard library, C math library, and C++ standard library.

## Installing
1. Run `make` in the root directory of  the project. This creates the static library in the bin directory.
2. Include libBMP.h.
3. Link against libBMP.a.

## Getting Started
Intialize your image. By default a new BMP image is 64 x 64 and has a bit depth of 24.
```
BMP image;
```
Optionally, provide contructor arguments to set the size and bit depth.
```
BMP image(512, 512); // 24 bit depth
BMP image2(300, 200, 16); // 16 bit depth
```
You can create a BMP object from an existing BMP image with either a constructor overload or the `copy(string filename)` function.
```
BMP image3("../greatpicture.bmp");
image2.copy("someimage.bmp");
```
Resize or change the bit depth with the following functions: 
```
image.set_bit_depth(24);
image.set_size(512, 512);          
```
Generate your image with `generate(string filename)`.
```
image.generate("thebestpic.bmp");
```
libBMP uses 32-bit integers for color but provides a simple alias for readability: `color`. 
```
color red_24bit = 0xFF0000;
color blue_16bit = 0x001F;
```
Fill the image with your new color!
```
image.fill(red_24bit);
```
Sometimes you need to change just one pixel. Use `set_pixel(int x, int y, color c)`.
```
image.set_pixel(50, 50, red_24bit);
```
If you want to get the color of one pixel, use `color get_pixel(int x, int y)`.
```
image.get_pixel(50, 50);
```
You can also get and set pixels by index (starting at 0), if you are into that kinda thing:
```
image.set_pixel(20, red_24bit);
```
![Vapor Lena](https://github.com/sagefarrenholz/libBMP/blob/master/samples/lena_vapor.bmp)
