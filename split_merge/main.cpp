#include "imageUtils.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

using std::filesystem::directory_iterator;
using namespace std;


void compute_harmony_folder(string harmonyType,string fout){
	string path = "../script_database/databaseOut2" + harmonyType + "/";
	std::ofstream f;
    f.open(fout,std::ios::app);

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
			f<<harmonyType<<","<<harm<<std::endl;
		}
	}
	f.close();
}

void classify_harmony_folder(string harmonyType,string fout){
	string path = "../script_database/databaseOut2/";
	std::ofstream f;
    f.open(fout,std::ios::app);

    int i=0;
    for (const auto & file : directory_iterator(path)){
		i++;
		std::cout<<"did "<<i<<" images "<<std::endl;
		Image img;
		img.fromFile((path+file.path().filename().string()).c_str());
		std::cout<<"doing : "<<(path+file.path().filename().string())<<std::endl;
		img.SplitAndMerge(4);
		img.save("out.png");
		std::cout<<"SplitAndMerge"<<std::endl;
		string harm = img.classifyHarmony();
		std::cout<<"harmony type is : "<<harm<<std::endl;
		f<<file.path().filename().string()<<","<<harm<<std::endl;
	}
	f.close();
}

int main(void){
    //compute_harmony_folder("comp" ,"harmonies_data.csv");
    classify_harmony_folder("comp", "harmony_types.csv");
	
	
}