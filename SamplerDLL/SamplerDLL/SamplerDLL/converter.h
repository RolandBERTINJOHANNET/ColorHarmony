#pragma once
#pragma once
#include "pch.h"
#include <iostream>
#include <cmath>

const double pi = 3.141592653589793;


double gamma_expand(double value);
double gamma_compress(double value);
unsigned char expand_to_byte(double value);

unsigned char real_to_byte(double value, double min, double max, bool verbose);

double byte_to_real(unsigned char value, double min, double max, bool verbose);


unsigned char quantify(unsigned char value, int in_max, int out_max);


double LABfun(double value);
double LABfun_inv(double value);

//good help : https://www.easyrgb.com/en/convert.php#inputFORM

void RGB_to_LCH(int R, int G, int B, double* L, double* C, double* H);

//problem with the xyz->rgb transformation 
//see https://www.easyrgb.com/en/convert.php#inputFORM
//and https://www.easyrgb.com/en/math.php

void LCH_to_RGB(double l, double C, double H, int* r, int* g, int* b);