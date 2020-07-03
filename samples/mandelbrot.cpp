//Change this to your include directory.
#include "../src/libBMP.h"

#include <iostream>

int main(void)
{
	int32_t width, height;
	double depth;

	std::cout << "Enter Width: ";
	std::cin >> width;

	std::cout << "Enter Height: ";
	std::cin >> height;

	std::cout << "Enter Depth: ";
	std::cin >> depth;

	/*
	 * The left value is the actual bound to each axis on the graph. Ex. If xres = 2 / width,
	 * the image visualizes -1 to 1 on the x axis. Feel free to play around with these to scale
	 * the image however you'd like. Smaller values will set the bounds closer, effectively zooming
	 * in.
	 */
	const double xres = 3.5 / width;
	const double yres = 2.0 / height;	

	long double xo, yo, temp, x = 0, y = 0;
       	long double test = 0;

	BMP mandelbrot(width, height);
	//mandelbrot.set_size(width,height);

	//Here is the starting gradient color.
	uint32_t border = 0; 
	uint32_t black = 0;
	
	mandelbrot.fill(black);

	std::cout << "Estimating Mandelbrot set for this resolution..." << std::endl;	
	//Go from top to bottom, left to right: checking if each pixel is contained within the mandelbrot set
	for (double ity = height; ity >= 0; ity--) {
		for (double itx = 0; itx < width; itx++) {
			/*
			 * For every pixel the mandel function checks if it is within the set.
			 * The values that width and height are divided by are arbitrary and are used for adjusting the translation of 
			 * the pixel coordinates. Ex. -(width / 2), shifts the image right by half the width. An easier way to understand
			 * how these expressions work is to seperate the translation from the scaling. 
			 * The res values, as noted above, are used to scale the pixels to within certain bounds.
			 */
			xo = xres * (itx - (width/1.5));
			yo = yres * (ity - (height/2.0));

			/*
			 * The acutal mathematics behind the Mandelbrot set are too lengthy for comments,
			 * please research online for an explanation.
			 *
			 * Summary of the mandel function:
			 * x and y represent the real and imaginary components of the last (z) complex number, respecitvely.
			 * xo and yo represent the real and imaginary components of the c constant complex number, respectively. 
			 * If the function reaches the depth threshold (set by the user) then the number is accepted as
			 * part of the set. If either components of z exceed two, the number is ruled out, due to the
			 * fact that they are not bound (converging). If z meets these conditions and the depth threshold
			 * has not been met then we will perform another recursion and increase our depth.
			 */
			while (test < depth && (x*x + y*y) < 4) {
				temp = x;
				x = x*x - y*y + xo;
				y = 2*temp*y + yo;
				test++;	
			}
			if (test != depth) { 
				border = static_cast<uint8_t>(test * (0xFFp1 / depth));
				if (itx + 1 >= width || ity + 1 >= height) continue; 
				mandelbrot.set_pixel(itx + 1, ity + 1, border);	
			} else { 
				if (itx + 1 >= width || ity + 1 >= height) continue; 
				mandelbrot.set_pixel(itx + 1, ity + 1, black);
			}
			test = 0;
			x = 0;
			y = 0;
		}	
	}

	mandelbrot.generate("mandelbrot.bmp");
	return 0;
}
