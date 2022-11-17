#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "../../harmony_solli/converter.cpp"

using std::filesystem::directory_iterator;
using namespace std;

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../utils/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../utils/stb_image.h"


void convert_image(std::string filename, std::string type){
	//load image
	int chan,w,h;
	std::string in_path = (type+std::string("/")+filename);
	std::cout<<"reading "<<filename<<" from : "<<type<<" path : "<<std::string(in_path)<<std::endl;
	uint8_t *data = stbi_load(in_path.c_str(), &w, &h, &chan, 3);

	//turn into lch
	std::cout<<"attempting to allocate "<<w*h*3<<" bytes of stack"<<std::endl;
	unsigned char img_lch[w*h*3];
	std::cout<<"allocation successful"<<std::endl;
	std::cout<<"value at the start : "<<img_lch[0]<<std::endl;

	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			double l,c,hue;
			int r = data[i*w*3+j*3];
			int g = data[i*w*3+j*3+1];
			int b = data[i*w*3+j*3+2];
		
			RGB_to_LCH(r,g,b,&l,&c,&hue);


			img_lch[i*w*3+j*3]=(int)l;
			img_lch[i*w*3+j*3+1]=(int)c;
			img_lch[i*w*3+j*3+2]=(int)(float(hue) * (256./360.));
		}
	}
	//save
	std::string out_path = (type+std::string("_lch/")+filename);
	std::cout<<"writing to : "<<out_path<<std::endl;
	stbi_write_png(out_path.c_str(), w, h, 3, img_lch, w*3);
}

int main(){
	std::vector<std::string> types(5);
	types[0]="mono";types[1]="analog";types[2]="comp";types[3]="triad";types[4]="rectangle";

	for(auto type : types){
		int i=0;
	    for (const auto & file : directory_iterator(type)){
	    	convert_image(file.path().filename().string(),type);
		}
		std::cout<<std::string("\nfinished ")+type+std::string(" directory")<<std::endl;
	}
}