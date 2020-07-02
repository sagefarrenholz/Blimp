# libBMP
A lightweight C++ library that allows for the generation and manipulation of Bitmap images. Supports modern and low bit depths.

## Features
- BMP image generation at all BMP supported bitdepths: 1, 2, 4, 8, 16, 24, and 32;
- Fast in-memory manipulation of images with lightweight getter and setter wrappers for pixels.
- A primitve but powerful interface that stays out of your way.

## Dependencies
Just the C standard library, C math library, and C++ standard library.

## Installing
Compile the library using any C++ compiler. Include libBMP.h in your include directory. Link against the library. Add #include "libBMP.h" to your code.
Easy as that.

## Getting Started
Intialize your image as a BMP object. 
```
BMP thebestpic;
```
Setup your image attributes. By default a new BMP image is 64 x 64 and has a bitdepth of 24.
```
thebestpic.set_bit_depth(24);
thebestpic.set_size(512, 512);          
```
Believe it or not, your image can already be generated.
```
thebestpic.generate("thebestpic.bmp");
```
libBMP uses 32bit integers for color but provides a simple typedef wrapper: "color". libBMP handles aliasing of bits, so worry not about using higher bit depth colors at low bit depths.
```
color red24bit = 0xFF0000;
color blue16bit = 0x001F;
```
Fill the image with your new color!
```
thebestpic.fill(red24bit);
thebestpic.generate("thebestpic.bmp");
```
Sometimes you need to change just one pixel. For this use set_pixel():
```
thebestpic.set_pixel(50, 50, red16bit);
```
If you want to see the color of just one pixel, use get_pixel():
```
thebestpic.get_pixel(50, 50);
```
You can also get and set pixels by index (starting at 0), if you are into that kinda thing:
```
thebestpic.set_pixel(20, red24bit);
thebestpic.get_pixel(100);
```
![Mandelbrot](https://i.ibb.co/FXMkyq6/mandelbrot.png)
