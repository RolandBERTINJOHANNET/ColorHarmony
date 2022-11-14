//samplingtest.cpp

#include "sample.h"
#include <iostream>

int main(int argc,char** argv){
	Image* im = new Image();

	im->readfile("test.jpg");

	Image* downscaled = new Image(256);

	downsample(im,downscaled);

	std::cout<<downscaled->height<<" "<<downscaled->width<<" "<<downscaled->pixels.size()<<std::endl;

	downscaled->writefile("test1.png");


	Image* upscaled = new Image(im->width,im->height);

	upsample_no_diff(downscaled,upscaled);


	upscaled->writefile("upscaled.png");

	int16_t* diff = new int16_t[im->height*im->width*3];

	build_diff(im,upscaled,diff);

	apply_diff(upscaled,diff);


	upscaled->writefile("final.png");

	return 1;
}