#include <algorithm>
#include <iterator>
//for the solli harmony prediction
#include "../harmony_scoring/harmony_solli/harmony_solli.cpp"
//for the yang harmony prediction
#include "../split_merge/utils.cpp"

//-------------------------------------------------------------------------------------------------------------------
//what the code does : 
//reads all the data from the Kuler dataset (harmony_yang/palettesKuler.csv and harmony_yang/scoresKuler.csv)
//makes a harmony prediction from the 4 pairs in each palette,
									//one prediction for the yang et al. and one for renz et al. method 
//writes these predictions to a .csv file for further processing down the line.
//-------------------------------------------------------------------------------------------------------------------


//CSVDatat is only used if use_solli is false
float prediction_from_palette(std::vector<float> palette, std::vector<float> CSVData, bool use_solli){
	float MLEScore = 0.;
	if(use_solli){
		//std::cout<<"using solli et al. harmony \nsize of palette : "<<palette.size()<<std::endl;
		for(int i=0;i<palette.size()-3;i+=3){
			int r1,g1,b1,r2,g2,b2;
			r1=palette[i]*255;g1=palette[i+1]*255;b1=palette[i+2]*255;
			r2=palette[i+3]*255;g2=palette[i+4]*255;b2=palette[i+5]*255;

			//std::cout<<"the two colors : "<<r1<<", "<<g1<<", "<<b1<<", "<<r2<<", "<<g2<<", "<<b2<<", "<<std::endl;

			float pairPrediction = compute_harmony(r1,g1,b1,r2,g2,b2);
			MLEScore+=pairPrediction;
			std::cout<<"solli pair prediction : "<<pairPrediction<<std::endl;
		}

	}else{
		//std::cout<<"using yang et al. harmony \nsize of palette : "<<palette.size()<<std::endl;
		for(int i=0;i<palette.size()-3;i+=3){
			float r1,g1,b1,r2,g2,b2;
			double l,c,h,l1,c1,h1;

			r1=palette[i]*255;g1=palette[i+1]*255;b1=palette[i+2]*255;
			r2=palette[i+3]*255;g2=palette[i+4]*255;b2=palette[i+5]*255;


			//std::cout<<"the two colors : "<<r1<<", "<<g1<<", "<<b1<<", "<<r2<<", "<<g2<<", "<<b2<<", "<<std::endl;

			RGB_to_LCH(r1,g1,b1,&l,&c,&h);
			RGB_to_LCH(r2,g2,b2,&l1,&c1,&h1);


			float pairPrediction = predictionFromCSV(h, h1, CSVData);
			MLEScore+=pairPrediction;
			std::cout<<"yang pair prediction : "<<pairPrediction<<std::endl;
		}
	}

	std::cout<<"using "<<(use_solli?"solli, ":"yang, ")<<" final score prediction : "<<MLEScore/5.<<std::endl;
	return MLEScore/4.;//the palette length will never not be 5 colors.
}

//specifically for reading the palettes.csv file
std::vector<float> readPalettes(std::string filename){
  std::vector<float> result;
  std::string line,word;

  std::fstream file (filename,std::ios::in);
  if(file.is_open()){
    int i=0;
    while(true){
      //delimiter should be \n one out of 15 times
      if(i%15<14){
        if(getline(file,line,',')){
          std::stringstream str(line);
          float tempFloat;
          str>>tempFloat;
        	//std::cout<<"read "<<tempFloat*255.<<std::endl;
          result.push_back(tempFloat);
        }else break;
      }else{
        if(getline(file,line)){
          std::stringstream str(line);
          float tempFloat;
          str>>tempFloat;
        	//std::cout<<"read "<<tempFloat*255.<<std::endl;
          result.push_back(tempFloat);
        }else break;
      }
      i++;
    }
  }
  return result;
}



int main(){
	//the csv file for yang et al. is "../harmony_scoring/harmony_yang/pairPlusPrediction.csv"
	//the csv file for the RGB palettes is "../harmony_scoring/harmony_yang/palettes.csv"

	std::vector<float> CSVData = readCSV("../harmony_scoring/harmony_yang/pairPlusPrediction.csv");
	std::vector<float> RGBPalettes = readPalettes("../harmony_scoring/harmony_yang/palettes.csv");
	
	//writing the computed palette scores to "harmony_yang/paletteScoresKuler.csv" and "harmony_solli/paletteScoresKuler.csv"
	std::string pathSol = "harmony_solli/paletteScoresKuler.csv";
	std::string pathYan = "harmony_yang/paletteScoresKuler.csv";
	std::ofstream fYan,fSol;
    fYan.open(pathYan,std::ios::app);
    fSol.open(pathSol,std::ios::app);


		//first compute solli et al harmony on all palettes
  std::vector<float>::iterator paletteStart,paletteEnd;
  paletteStart = RGBPalettes.begin();
  paletteEnd = std::next(RGBPalettes.begin(),15);

	while(paletteEnd<=RGBPalettes.end()){
		std::vector<float> palette(paletteEnd - paletteStart);
		std::copy(paletteStart,paletteEnd,palette.begin());
		float harmony_solli = prediction_from_palette(palette, CSVData, true);

		//write to fSol
		fSol<<harmony_solli<<std::endl;

		paletteStart = std::next(paletteStart,15);
		paletteEnd = std::next(paletteEnd,15);
	}

	//then yang on all palettes.
	paletteStart = RGBPalettes.begin();
  paletteEnd = std::next(RGBPalettes.begin(),15);

	while(paletteEnd<=RGBPalettes.end()){
		std::vector<float> palette(paletteEnd - paletteStart);
		std::copy(paletteStart,paletteEnd,palette.begin());
		float harmony_yang = prediction_from_palette(palette, CSVData, false);

		//write to fYan
		fYan<<harmony_yang<<std::endl;

		paletteStart = std::next(paletteStart,15);
		paletteEnd = std::next(paletteEnd,15);
	}


	fSol.close();
	fYan.close();
	return 1;
}