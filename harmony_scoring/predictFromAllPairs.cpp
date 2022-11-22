#include <algorithm>
#include <iterator>
//for the solli harmony prediction
#include "../harmony_scoring/harmony_solli/harmony_solli.cpp"
//for the yang harmony prediction
#include "../split_merge/utils.cpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../split_merge/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../split_merge/stb_image.h"

//-------------------------------------------------------------------------------------------------------------------
//what the code does : 
//for the two harmony scoring methods :
	//computes the pair-wise harmony score prediction for each possible pair in the 360*360 sized hue pair space
	//puts them in a 360*360 image
	//saves it to a png file.
//-------------------------------------------------------------------------------------------------------------------

unsigned char yang_to_byte(float harm){
	//yang harm usually goes between 1.5 and 3.1
	harm = harm>3.1?3.1:harm;
	harm = harm<1.5?1.5:harm;
	//min-max scale to 0,1
	harm = (harm-1.5)/(3.1-1.5);
	//expand to byte
	return (unsigned char)(harm*255);
}
unsigned char solli_to_byte(float harm){
	//solli harm usually goes between -1. and 1.
	harm = harm>1.?1.:harm;
	harm = harm<-1.?-1.:harm;
	//min-max scale to 0,1
	harm = (harm+1)/2.;
	//expand to byte
	return (unsigned char)(harm*255);
}


int main(){
	std::vector<unsigned char> yang_image;
	std::vector<unsigned char> solli_image;
	std::vector<float> CSVData = readCSV("../harmony_scoring/harmony_yang/pairPlusPrediction.csv");


	for(int hue1=0;hue1<360;hue1++){
		for(int hue2=0;hue2<360;hue2++){
			//get the rgb values for the hues, given the supposition L=40 and C=20 (for an average color)
			int r1,g1,b1,r2,g2,b2;
			LCH_to_RGB(40.,20.,hue1,&r1,&g1,&b1);
			LCH_to_RGB(40.,20.,hue2,&r2,&g2,&b2);
			//compute solli harmony for this pair
			float solli = compute_harmony(r1,g1,b1,r2,g2,b2);
			float yang = predictionFromCSV(hue1, hue2, CSVData);
//			std::cout<<"yang : "<<yang<<", "<<(int)yang_to_byte(yang)<<std::endl;
//			std::cout<<"solli : "<<solli<<", "<<(int)solli_to_byte(solli)<<std::endl;
			yang_image.push_back(yang_to_byte(yang));
			solli_image.push_back(solli_to_byte(solli));
		}
		std::cout<<"xaxis : did "<<hue1<<std::endl;
	}

	//write the images
	int w,h;w=h=360;
	stbi_write_png("yang_allpairs.png", w, h, 1, &yang_image[0], w);
	stbi_write_png("solli_allpairs.png", w, h, 1, &solli_image[0], w);

	return 1;
}