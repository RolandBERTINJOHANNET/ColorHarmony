//sampler.cpp

#include "Sampler.h"
#include "stdlib.h"


void Sampler::resize_GAN(){
	gansized = new Image(256);
	downsample(original,gansized);
	gansized->writefile("downed.png");
}

void Sampler::loadHarmo(const char* filename){
	harmonized = new Image();
	harmonized->readfile(filename);
}


void Sampler::buildImage(){
	Image* harmo_diff = new Image(w,h);
	upsample_no_diff(harmonized,harmo_diff);

	Image* harmo_grad = new Image(w,h);
	upsample_no_diff(harmonized,harmo_grad);


	final = new Image(w,h);

	int16_t* diff = new int16_t[h*w*2];
	int16_t* grad = new int16_t[h*w*4];

	build_diff(original,harmo_diff,diff);
	apply_diff(original,harmo_diff,diff);


	build_grad(original,grad);
	apply_grad(original,harmo_grad,grad);


	final = new Image(w,h);


	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			if(abs(harmo_grad->pixels[i*w+j].h - harmo_diff->pixels[i*w+j].h) > 30)
				final->pixels[i*w+j] = harmo_diff->pixels[i*w+j];
			else
				final->pixels[i*w+j] = harmo_grad->pixels[i*w+j];
		}
	}
}
