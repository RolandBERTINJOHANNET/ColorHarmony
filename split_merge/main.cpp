#include "imageUtils.h"


int main(void){
	Image img;
	img.fromFile("C:/Users/Orlando/Desktop/ColorHarmony/harmonies_database/mono/00000000.jpg");
	img.SplitAndMerge(1);
	img.save("out.png");
}