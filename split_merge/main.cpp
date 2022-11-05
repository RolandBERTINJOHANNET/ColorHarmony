#include "imageUtils.h"


int main(void){
	Image img;
	img.fromFile("C:/Users/Orlando/Desktop/ColorHarmony/harmonies_database/mono/00000001_(6).jpg");
	img.SplitAndMerge(4);
	double harm = img.computeHarmony();
	std::cout<<"total harmony is : "<<harm<<std::endl;
	img.save("out.png");
}