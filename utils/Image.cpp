//Image.cpp
#include "Image.h"
#include "stdint.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Image::readfile(const char *filename){
	int chan;
	uint8_t *data = stbi_load(filename, &width, &height, &chan, 3);
	for(int i=0;i<height;i++){
		for(int j=0;j<width*3;j+=3){
			Pixel p;
			p.r =data[i*width*3+j];
			p.g =data[i*width*3+j+1];
			p.b =data[i*width*3+j+2];
			pixels.push_back(p);
		}
	}
	delete[] data;
}

void Image::writefile(const char *filename){
	std::vector<unsigned char> data;
	for(int i=0;i<pixels.size();i++){
		data.push_back(pixels[i].r);
		data.push_back(pixels[i].g);
		data.push_back(pixels[i].b);
	}
	stbi_write_png(filename, width, height, 3, &data[0], width*3);
	std::cout<<"done writing on file"<<filename<<std::endl;
}