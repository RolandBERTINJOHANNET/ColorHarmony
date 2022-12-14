//sampler.h


#include "sample.h"



class Sampler{
private:
	int w,h;
public:
	Image* original;
	Image* gansized;
	Image* harmonized;
	Image* final;



	Sampler(const char* filename){
		original = new Image();
		original->readfile(filename);
		w = original->width;
		h = original->height;
	}

	void resize_GAN(){
	gansized = new Image(256);
	downsample(original,gansized);
	gansized->writefile("downed.png");
}

	void loadHarmo(const char* filename){
	harmonized = new Image();
	harmonized->readfile(filename);
}
	void buildImage(){
	Image* harmo_diff = new Image(w,h);
	upsample(harmonized,harmo_diff);

	Image* harmo_grad = new Image(w,h);
	upsample(harmonized,harmo_grad);


	Image* upscaled = new Image(w,h);
	upsample_no_diff(gansized,upscaled);

	upscaled->writefile("hd_up.png");

	final = new Image(w,h);

	int16_t* diff = new int16_t[h*w*2];
	int16_t* grad = new int16_t[h*w*4];

	build_diff(original,upscaled,diff);
	apply_diff(original,harmo_diff,diff);

	harmo_diff->writefile("hd_diff.png");


	build_grad(original,grad);
	apply_grad(original,harmo_grad,grad);


	harmo_grad->writefile("hd_grad.png");


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


};

