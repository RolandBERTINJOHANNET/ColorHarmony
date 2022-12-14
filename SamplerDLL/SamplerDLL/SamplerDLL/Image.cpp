//Image.cpp
#include "pch.h"
#include "Image.h"
#include "stdint.h"
#include "converter.h"
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
			double l,c,h;
			RGB_to_LCH(data[i*width*3+j],data[i*width*3+j+1],data[i*width*3+j+2],&l,&c,&h);
			p.l =real_to_byte(l,0,100,false);
			p.c =real_to_byte(c,0,120,false);
			p.h =real_to_byte(h,0,360,false);
			pixels.push_back(p);
		}
	}
	delete[] data;
}

void Image::readfile_LCH(const char *filename){
	int chan;
	uint8_t *data = stbi_load(filename, &width, &height, &chan, 3);
	for(int i=0;i<height;i++){
		for(int j=0;j<width*3;j+=3){
			Pixel p;
			p.l =data[i*width*3+j];
			p.c =data[i*width*3+j+1];
			p.h =data[i*width*3+j+2];
			pixels.push_back(p);
		}
	}
	delete[] data;
}

void Image::writefile(const char *filename){
	std::vector<unsigned char> data;
	for(int i=0;i<pixels.size();i++){
		int r,g,b;
		double h;
		/*
		if(pixels[i].h>200)
			h = byte_to_real(pixels[i].h,0,360,true);*/
		h = byte_to_real(pixels[i].h,0,360,false);
		LCH_to_RGB(byte_to_real(pixels[i].l,0,100,false),byte_to_real(pixels[i].c,0,120,false),h,&r,&g,&b);
		data.push_back((char)r);
		data.push_back((char)g);
		data.push_back((char)b);
	}
	stbi_write_png(filename, width, height, 3, &data[0], width*3);
	std::cout<<"done writing on file"<<filename<<std::endl;
}

void Image::writefile_LCH(const char *filename){
	std::vector<unsigned char> data;
	for(int i=0;i<pixels.size();i++){
		double h;
		data.push_back(quantify(pixels[i].l,100,255));
		data.push_back(quantify(pixels[i].c,120,255));
		data.push_back(quantify(pixels[i].h,360,255));
	}
	stbi_write_png(filename, width, height, 3, &data[0], width*3);
	std::cout<<"done writing on file"<<filename<<std::endl;
}