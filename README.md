# BMPlib
A simple C++ library that allows for the generation and manipulation of Bitmap images. Support for several bitdepths.

## Features
- BMP image generation at all BMP supported bitdepths: 1, 2, 4, 8, 16, 24.
- Simple image operations like: altering the image pixel by pixel, fill the image or by row or by column.

## Dependencies
None.

## Installing
Compile the library using any C++ compiler. Include BMPlib.h in your include directory. Link against the library. Add #include "BMPlib.h" to your code.
Easy as that.

## Getting Started
Intialize your image as a BMP object. 
```
BMP thebestpic;
```
Setup your image attributes. By default a new BMP image is 64x64, has a bitdepth of 24, and outputs in the same directory as your executable with the name "myimage.bmp".
```
thebestpic.setDimensions(512, 512);          
thebestpic.setBitdepth(16);                        
thebestpic.setOutputDirectory("thebestpic.bmp");  
```
Believe it or not, your image can already be generated.
```
thebestpic.generate();
```
If you want to add a little color create a color that matches your image bitdepth. BMPlib uses arrays of 1 byte integers to store color data. However, this is simplified for you with the built in types.
```
color16 red16bit = { 0b11111000, 0b00000000 };
color24 blue24bit = { 0x00, 0x00, 0xFF};
```
Fill the image with your new color!
```
thebestpic.fill(red16bit);
thebestpic.generate();
```
Sometimes you need to change just one pixel. For this use changePixel():
```
thebestpic.changePixel(50, 50, red16bit);
```
If you want more control over the pixel, for instance, to change its lightness, use getPixel():
```
thebestpic.getPixel(50,50).setLightness(1.0);
```
There are other ways to change your image. Please check the docs for more.
