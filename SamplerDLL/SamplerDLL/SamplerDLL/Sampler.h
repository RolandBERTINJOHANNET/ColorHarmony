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

	void resize_GAN();
	void loadHarmo(const char* filename);
	void buildImage();


};

