//samplinghd.cpp

//#include "sample.h"
#include "Sampler.h"
#include <iostream>
/*
int main(int argc,char** argv){
	Image* im = new Image();

	im->readfile("hd.jpg");

	Image* downscaled = new Image(256);

	downsample(im,downscaled);

	std::cout<<downscaled->height<<" "<<downscaled->width<<" "<<downscaled->pixels.size()<<std::endl;

	downscaled->writefile("hd_downscaled.png");
	Image* upscaled = new Image(im->width,im->height);

	upsample_no_diff(downscaled,upscaled);

	Image* harmo = new Image();
	harmo->readfile("hued.png");

	Image* harmoupscale = new Image(im->width,im->height);

	upsample_no_diff(harmo,harmoupscale);


	harmoupscale->writefile("hd_upscaled.png");

	Image* harmoupscale_g = new Image(im->width,im->height);

	upsample_no_diff(harmo,harmoupscale_g);


	harmoupscale->writefile("hd_upscaled.png");

	int16_t* diff = new int16_t[im->height*im->width*2];

	int16_t* grad = new int16_t[im->height*im->width*4];

	build_diff(im,upscaled,diff);


	apply_diff(im,harmoupscale,diff);


	build_grad(im,grad);


	apply_grad(im,harmoupscale_g,grad);


	harmoupscale->writefile("hd_upscaled_final.png");
	harmoupscale_g->writefile("hd_upscaled_final_gradient.png");

	return 1;
}
*/

int main(int argc,char** argv){
	Sampler* sampler = new Sampler("hd.jpg");

	sampler->resize_GAN();

	sampler->loadHarmo("hued.png");

	sampler->buildImage();

	sampler->final->writefile("hd_final.png");
	return 1;
}