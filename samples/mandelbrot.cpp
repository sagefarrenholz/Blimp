//Change this to your include directory.
#include "inc/BMPlib.h"

bool mandel(const double&, const double&, long double, long double, int);

int main(void)
{
	int32_t width, height;
	int depth;

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
	const double xres = 2.5 / width;
	const double yres = 2.5 / height;	

	BMP mandelbrot;
	mandelbrot.setDimensions(width,height);
	mandelbrot.setOutput("mandelbrot.bmp");

	//Here is the starting gradient color.
	color24 border = {0xFF, 0xFF, 0xFF};
	color24 black = {0};

	mandelbrot.fill(border);

	/*
	 * TODO: optimize mandelbrot generator, fix depth rendering and clean up mandel fucntion
	 * The Mandelbrot generator needs some optimization. When I have time I will clean this up
	 * as it is unbearably slow at high resolutions and bitdepths. Part of the reason is that
	 * the function redraws every pixel unnecessarily for every depth level. Another reason is
	 * that the mandelbrot function is passing huge numbers for no reason. Instead, I am going
	 * to move the actual depth testing to within the main function which should help speed things
	 * up and remove the need for passing long doubles and holding static variables.
	 */
	std::cout << "Estimating Mandelbrot set for this resolution..." << std::endl;
	//Go from top to bottom, left to right: checking if each pixel is contained within the mandelbrot set
	for(double clr = 1; clr <= depth; clr++){	
		//Customize the gradient here, the array number specifies which channel is changing.
		border[1] = static_cast<uint8_t>((depth - clr) * (255.0 / depth));
		for (double ity = height; ity >= 0; ity--)
			for (double itx = 0; itx < width; itx++)
				/*
				 * For every pixel the mandel function checks if it is within the set.
				 * The values that width and height are divided by are arbitrary and are used for adjusting the translation of 
				 * the pixel coordinates. Ex. -(width / 2), shifts the image right by half the width. An easier way to understand
				 * how these expressions work is to seperate the translation from the scaling. 
				 * The res values, as noted above, are used to scale the pixels to within certain bounds.
				 */
				if (mandel((itx-(width/1.5))*xres, (ity-(height/2.0))*yres,0,0,clr)){
					if(clr != depth){
						mandelbrot.changePixel(itx+1, ity+1, border);	
					} else {
						//At the the innermost depth, fill the image all black to clearly distungish the Mandelbrot set numbers.	
						mandelbrot.changePixel(itx+1, ity+1, black);	
					}
				}
	}		
	mandelbrot.generate();
	return 0;
}

bool mandel(const double& xo, const double& yo, long double x, long double y, int depth){
	/*
	 * The acutal mathematics behind the Mandelbrot set are too lengthy for comments,
	 * please check the wikipedia page for an explanation.
	 *
	 * Summary of the mandel function:
	 * x and y represent the real and imaginary components of the last (z) complex number, respecitvely.
	 * xo and yo represent the real and imaginary components of the c constant complex number, respectively. 
	 * If the function reaches the depth threshold (set by the user) then the number is accepted as
	 * part of the set. If either components of z exceed two, the number is ruled out, due to the
	 * fact that they are not bound (converging). If z meets these conditions and the depth threshold
	 * has not been met then we will perform another recursion and increase our depth.
	 */
	static int test = 0;
	if (test >= depth) {
		test = 0;
		return true;
	} else if (x*x + y*y >= 4) {
	  	test = 0;
		return false;
	} else {
		test++;
		return(mandel(xo, yo, x*x - y*y + xo, 2*x*y + yo, depth));
	}
}
