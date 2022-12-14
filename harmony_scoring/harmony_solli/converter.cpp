#include <cmath>
#include <iostream>

const double pi = 3.141592653589793;


double gamma_expand(double value){
	return value<=0.04045?(value/12.92)
						:pow((value+0.055)/1.055, 2.4);
}
double gamma_compress(double value){
	return value<=0.0031308?(value*12.92)
						:1.055*pow(value, 1./2.4)-0.055;
}
unsigned char expand_to_byte(double value){
	int oct = 255*value;
	oct = oct>255?255:oct;
	oct = oct<0?0:oct;
    return (unsigned char)oct;
}

unsigned char real_to_byte(double value,double min, double max, bool verbose){
	if(verbose)
		std::cout<<"start : "<<value;
	int res = (value/(max-min)) * 255;
	if(verbose)
	std::cout<<"  before clip : "<< res<<std::endl;
	res = res>255?255:res;
	res = res<0?0:res;
	return (unsigned char)res;
}

double byte_to_real(unsigned char value,double min, double max, bool verbose){
	if(verbose)
		std::cout<<"start : "<< (int)value;
	double res = (double)value *(max-min) /255;

	if(verbose)
		std::cout<<"  after clip : "<< res<<std::endl;
	return res;
}


unsigned char quantify(unsigned char value,int in_max, int out_max){
	unsigned char res = (unsigned char)(value *((float)in_max /out_max));
	return res;
}



double LABfun(double value){
	double delta = 6./29.;

	double t = value;

	return t>pow(delta, 3)?
				pow(t, 1./3.):
				(7.787 * value)+(16./116.);
}
double LABfun_inv(double value){
	double delta = 6./29.;

	double t = value;

	return t>delta?
			pow(t, 3):
			3.*pow(delta,2)*(t-(4./29.));
}

//good help : https://www.easyrgb.com/en/convert.php#inputFORM

void RGB_to_LCH(int R, int G, int B, double *L, double *C, double *H){
	//first convert rgb to XYZ
	double r,g,b;
	//bring to 0,1 range
	r=(double)R/(double)255;
	g=(double)G/(double)255;
	b=(double)B/(double)255;
	
	//gamma expansion
	r = gamma_expand(r) * 100.;
	g = gamma_expand(g) * 100.;
	b = gamma_expand(b) * 100.;

	//matrix multiplication
	double X = 0.4124564*r +0.3575761*g +0.1804375*b;
	double Y = 0.2126729*r  +0.7151522*g +0.0721750*b;
	double Z = 0.0193339*r +0.1191920*g +0.9503041*b;

	//then convert to LAB

	double l = 116.*LABfun(Y/100.) - 16.;
	double a = 500.*(LABfun(X/95.0489) - LABfun(Y/100.));
	double b_ = 200.*(LABfun(Y/100.) - LABfun(Z/108.8840));

	//then to LCH

	double var_H = atan2(b_,a) * (180./pi);

	var_H = var_H>0?var_H:360+var_H;
	*L = l;
	*C = sqrt(a*a+b_*b_);
	*H = var_H;
}

//problem with the xyz->rgb transformation 
//see https://www.easyrgb.com/en/convert.php#inputFORM
//and https://www.easyrgb.com/en/math.php

void LCH_to_RGB(double l, double C, double H, int *r, int *g, int *b){
	//convert to LAB
	double H_rad = ((pi)/180.)*H;

	double L = l;
	double a = C*cos(H_rad);
	double b_ = C*sin(H_rad);

	//then convert to XYZ
	
	//transformation
	double X = 95.0489*LABfun_inv( ((L+16.)/116.)+(a/500.) ) /100.;
	double Y = 100.*LABfun_inv((L+16.)/116.) /100.;
	double Z = 108.8840*LABfun_inv(  ((L+16.)/116.)-(b_/200.) ) /100.;

	//matrix multiplication
	double R = 3.2406*X -1.5372*Y -0.4986*Z;
	double G = -0.9689*X  +1.8758*Y +0.0415*Z;
	double B = 0.0557*X -0.2040*Y +1.0570*Z;
	
	//gamma compression
	R = gamma_compress(R);
	G = gamma_compress(G);
	B = gamma_compress(B);


	//output
	*r=expand_to_byte(R);
	*g=expand_to_byte(G);
	*b=expand_to_byte(B);
}