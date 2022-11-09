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
	return 1;
}