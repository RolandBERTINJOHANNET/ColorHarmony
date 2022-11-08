#include "imageUtils.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

using std::filesystem::directory_iterator;
using namespace std;

int main(void){
    string path = "../harmonies_database/mono/";
    std::ofstream f;
    f.open("harmonies_data.csv",std::ios::app);

    int i=0;
    for (const auto & file : directory_iterator(path)){
    	if(i<=100){
    		i++;
    		std::cout<<"did "<<i<<" out of "<<100<<std::endl;
			Image img;
			img.fromFile((path+file.path().filename().string()).c_str());
			std::cout<<"doing : "<<(path+file.path().filename().string())<<std::endl;
			img.SplitAndMerge(4);
			img.save("out.png");
			std::cout<<"SplitAndMerge"<<std::endl;
			double harm = img.computeHarmony();
			std::cout<<"total harmony is : "<<harm<<std::endl;
			f<<"mono,"<<harm<<std::endl;
		}
	}
	f.close();
}