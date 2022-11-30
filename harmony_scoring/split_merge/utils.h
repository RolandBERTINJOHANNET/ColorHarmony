#include <numeric>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

std::vector<int> argsort(const std::vector<int> &array);

std::vector<float> readCSV(std::string filename);

float predictionFromCSV(float hue1, float hue2, std::vector<float> CSVData);