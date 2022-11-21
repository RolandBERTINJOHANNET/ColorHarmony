#include <algorithm>
//for the solli harmony prediction
#include "../harmony_scoring/harmony_solli/harmony_solli.cpp"
//for the yang harmony prediction
#include "../split_merge/utils.cpp"

//!!!!!!!!!!!!!!!!!!!!!!!!for solli prediction, since only the hues are known in the palettes, we're doing 
//palette will either be rgb or hues, based on use_solli (rgb if True, hues if False)
//CSVDatat is only used if use_solli is false
float prediction_from_palette(std::vector<int> palette, std::vector<float> CSVData, bool use_solli){
	float MLEScore = 0.;
	if(use_solli){
		std::cout<<"using solli et al. harmony \nsize of palette : "<<palette.size()<<std::endl;
		for(int i=0;i<palette.size()-3;i+=3){
			int r1,g1,b1,r2,g2,b2;
			r1=palette[i];g1=palette[i+1];b1=palette[i+2];
			r2=palette[i*3];g2=palette[i*3+1];b2=palette[i*3+2];

			std::cout<<"the two colors : "<<r1<<", "<<g1<<", "<<b1<<", "<<r2<<", "<<g2<<", "<<b2<<", "<<std::endl;

			float pairPrediction = compute_harmony(r1,g1,b1,r2,g2,b2);
			std::cout<<"solli pair prediction : "<<pairPrediction<<std::endl;
		}

	}else{
		std::cout<<"using yang et al. harmony \nsize of palette : "<<palette.size()<<std::endl;
		for(int i=0;i<palette.size()-1;i+=3){
			float h1,h2;
			h1=palette[i];h2=palette[i+1];//ignore the score (which is at i+2)

			std::cout<<"the two hues : "<<h1<<", "<<h2<<std::endl;

			float pairPrediction = predictionFromCSV(h1, h2, CSVData);
			std::cout<<"pair prediction : "<<pairPrediction<<std::endl;
		}
	}

	std::cout<<"use_solli : "<<use_solli<<" final score prediction : "<<MLEScore/5.<<std::endl;
	return MLEScore/5.;//the palette length will never not be 5 colors.
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
        	std::cout<<"read "<<line<<std::endl;
          result.push_back(tempFloat);
        }else break;
      }else{
        if(getline(file,line)){
          std::stringstream str(line);
          float tempFloat;
          str>>tempFloat;
        	std::cout<<"read "<<line<<std::endl;
          result.push_back(tempFloat);
        }else break;
      }
      i++;
    }
  }
  return result;
}


//reads all the data from the Kuler dataset (harmony_yang/palettesKuler.csv and harmony_yang/scoresKuler.csv)
//makes a harmony prediction from the 4 pairs in each palette,
									//one prediction for the yang et al. and one for renz et al. method 
//writes these predictions to a .csv file for further processing down the line.
int main(){
	//the csv file for yang et al. is "../harmony_scoring/harmony_yang/pairPlusPrediction.csv"
	//the csv file for the RGB palettes is "../harmony_scoring/harmony_yang/palettes.csv"

	std::vector<float> CSVData = readCSV("../harmony_scoring/harmony_yang/pairPlusPrediction.csv");
	std::vector<float> RGBPalettes = readPalettes("../harmony_scoring/harmony_yang/palettes.csv");
	
	//writing the computed palette scores to "harmony_yang/paletteScoresKuler.csv" and "harmony_solli/paletteScoresKuler.csv"
	std::string pathSol = "harmony_yang/paletteScoresKuler.csv";
	std::string pathYan = "harmony_yang/paletteScoresKuler.csv";
	std::ofstream fYan,fSol;
    fYan.open(pathYan,std::ios::app);
    fSol.open(pathSol,std::ios::app);

//--------------:::::::::::::!!!!!!!!!!!!!!!!!!!!!!!!!!!!TROUBLE WITH ITERATORS, TRYING TO GET THE PALETTE AS A STD::VECTOR.

	for(int i=0;i<15;i++){
		auto paletteStart = RGBPalettes.begin();
		auto paletteEnd = RGBPalettes.begin();
		paletteStart.next(i);
		paletteEnd.next(i+15);
		std::vector<float> palette = std::copy(paletteStart,paletteEnd);
		std::cout<<"palette length : "<<paletteEnd-paletteStart<<std::endl;
		float harmony_yang = prediction_from_palette(palette, CSVData, False);
		float harmony_solli = prediction_from_palette(palette, CSVData, True);

	}

	return 1;
}