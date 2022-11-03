#include "imageUtils.h"


int main(void){
	Image img;
	img.fromFile("C:/Users/Orlando/Desktop/ColorHarmony/harmonies_database/analog/00000013_(5).jpg");
	img.SplitAndMerge(1);
	img.save("out.png");
}