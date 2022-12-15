//sampler.cpp
#include "pch.h"
#include "sample.h"
#include "stdio.h"


class Sampler{
public:
	int w,h;
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
	gansized->writefile("downscaled.png");
}

	void loadHarmo(const char* filename){
	harmonized = new Image();
	harmonized->readfile(filename);
}
	void buildImage(){
	Image* harmo_grad = new Image(w,h);
	upsample(harmonized,harmo_grad);


	Image* upscaled = new Image(w,h);
	upsample_no_diff(gansized,upscaled);

	final = new Image(w,h);

	int16_t* grad = new int16_t[h*w*4];



	build_grad(original,grad);
	apply_grad(original,harmo_grad,grad);


	harmo_grad->writefile("result.png");

}


};

extern "C" {
	__declspec(dllexport) Sampler* Sampler_new(const char* filename){return new Sampler(filename); }
	__declspec(dllexport) void Sampler_resize_GAN(Sampler* s){s->resize_GAN();  }
	__declspec(dllexport) void Sampler_loadHarmo(Sampler* s,const char* filename){ s->loadHarmo(filename); }
	__declspec(dllexport) void Sampler_buildImage(Sampler* s){ s->buildImage(); }
}