#include "utils.h"

std::vector<int> argsort(const std::vector<int> &array) {
    std::vector<int> indices(array.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [&array](int left, int right) -> bool {
                  // sort indices according to corresponding array element
                  return array[left] < array[right];
              });

    return indices;
}

//for reading the PairsPlusPredictions.csv file.
std::vector<float> readCSV(std::string filename){
  std::vector<float> result;
  std::string line,word;

  std::fstream file (filename,std::ios::in);
  if(file.is_open()){
    int i=0;
    while(true){
      //delimiter should be \n one out of 3 times
      if(i%3<2){
        if(getline(file,line,',')){
          std::stringstream str(line);
          float tempFloat;
          str>>tempFloat;
          result.push_back(tempFloat);
        }else break;
      }else{
        if(getline(file,line)){
          std::stringstream str(line);
          float tempFloat;
          str>>tempFloat;
          result.push_back(tempFloat);
        }else break;
      }
      i++;
    }
  }
  return result;
}

float predictionFromCSV(float hue1, float hue2, std::vector<float> CSVData){
  bool PairExists=false;
  float finalScore;
  float minScore = 10000.;
  float minDist = 1000000.;
  //compute pair distance to all pairs, stopping if found the pair itself
  //if found pair itself, return its score
  //otherwise, return the score at the argmin of the distances.
  for(int i=0;i<CSVData.size();i+=3){
    float dist = pow(CSVData[i] - hue1,2) + pow(CSVData[i+1] - hue2,2);
    //std::cout<<"distance between "<<hue1<<", "<<hue2<<" and "<<CSVData[i]<<", "<<CSVData[i+1]<<" is : "<<dist<<std::endl;
    if(hue1==CSVData[i] && hue2==CSVData[i+1]){
      PairExists=true;
      finalScore=CSVData[i+2];
      break;
    }else if(dist<minDist){
      minDist=dist;
      finalScore = CSVData[i+2];
    }
  }
  return finalScore;
}
