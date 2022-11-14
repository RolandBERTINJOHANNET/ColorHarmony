#include <cmath>
#include <iostream>

const double pi = 3.141592653589793;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!only RGB->LCH has been fixed yet !

double gamma_expand(double value){
	return value<=0.04045?(value/12.92)
						:pow((value+0.055)/1.055, 2.4);
}
double gamma_compress(double value){
	return value<=0.0031308?(value*12.92)
						:pow(value*1.055, 1./2.4)-0.055;
}
unsigned char expand_to_byte(double value){
	int oct = 255*value;
	oct = oct>255?255:oct;
	oct = oct<0?0:oct;
    return (unsigned char)oct;
}


void convert_RGB_to_XYZ(unsigned char *img, double *XYZimg, int w, int h){
	for(int i=0;i<w*h*3;i+=3){
		double r,g,b;
		//bring to 0,1 range
		r=(double)img[i]/(double)255;
		g=(double)img[i+1]/(double)255;
		b=(double)img[i+2]/(double)255;
		
		//gamma expansion
		r = gamma_expand(r);
		g = gamma_expand(g);
		b = gamma_expand(b);

		//matrix multiplication
		double X = 0.4124564*r +0.3575761*g +0.1804375*b;
		double Y = 0.2126729*r  +0.7151522*g +0.0721750*b;
		double Z = 0.0193339*r +0.1191920*g +0.9503041*b;

		//output
		XYZimg[i]=X;
		XYZimg[i+1]=Y;
		XYZimg[i+2]=Z;
	}
}

void convert_XYZ_to_RGB(unsigned char *oimg, double *XYZimg, int w, int h){
	for(int i=0;i<w*h*3;i+=3){
		//matrix multiplication
		double X = XYZimg[i];
		double Y = XYZimg[i+1];
		double Z = XYZimg[i+2];

		//matrix multiplication
		double r = 3.2406*X -1.5372*Y -0.4986*Z;
		double g = -0.9689*X  +1.8758*Y +0.0415*Z;
		double b = 0.0557*X -0.2040*Y +1.0570*Z;
		
		//gamma compression
		r = gamma_compress(r);
		g = gamma_compress(g);
		b = gamma_compress(b);

		//output
		oimg[i]=expand_to_byte(r);
		oimg[i+1]=expand_to_byte(g);
		oimg[i+2]=expand_to_byte(b);
	}
}

double LABfun(double value){
	double delta = 6./29.;

	double t = value;

	return t>pow(delta, 3)?
				pow(t, 1./3.):
				(t/(3*pow(delta,2)))+(4./29.);
}
double LABfun_inv(double value){
	double delta = 6./29.;

	double t = value;

	return t>delta?
			pow(t, 3):
			3.*pow(delta,2)*(t-(4./29.));
}

void convert_RGB_to_LAB(unsigned char *img, double *XYZimg, int w, int h){
	convert_RGB_to_XYZ(img, XYZimg, w, h);

	for(int i=0;i<w*h*3;i+=3){
		double X = XYZimg[i];
		double Y = XYZimg[i+1];
		double Z = XYZimg[i+2];
		
		//transformation
		double L = 116.*LABfun(Y/100.) - 16.;
		double a = 500.*(LABfun(X/95.0489) - LABfun(Y/100.));
		double b = 200.*(LABfun(Y/100.) - LABfun(Z/108.8840));
	
		//output
		XYZimg[i]=L;
		XYZimg[i+1]=a;
		XYZimg[i+2]=b;
	}
}

void convert_LAB_to_RGB(unsigned char *oimg, double *LABimg, int w, int h){

	for(int i=0;i<w*h*3;i+=3){
		double L = LABimg[i];
		double a = LABimg[i+1];
		double b = LABimg[i+2];
		
		//transformation
		double X = 95.0489*LABfun_inv( ((L+16.)/116.)+(a/500.) );

		double Y = 100.*LABfun_inv((L+16.)/116.);

		double Z = 108.8840*LABfun_inv(  ((L+16.)/116.)-(b/200.) );
	
		//output into LAB but they are XYZ coords by now
		LABimg[i]=X;
		LABimg[i+1]=Y;
		LABimg[i+2]=Z;

	}
	convert_XYZ_to_RGB(oimg, LABimg, w, h);
}


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

	double var_H = atan(b_/a) * (180./pi);
	std::cout<<"var h : "<<var_H<<"with mod : "<<int(var_H)%360<<std::endl;
	var_H = var_H>0?var_H:360+var_H;
	*L = l;
	*C = sqrt(a*a+b_*b_);
	*H = var_H;
}
void LCH_to_RGB(double l, double C, double H, int *r, int *g, int *b){
	//convert to LAB
	double H_rad = ((2.*pi)/360.)*H - pi;
	
	double L = l;
	double a = C*cos(H_rad);
	double b_ = C*sin(H_rad);


	//then convert to XYZ
	
	//transformation
	double X = 95.0489*LABfun_inv( ((L+16.)/116.)+(a/500.) );

	double Y = 100.*LABfun_inv((L+16.)/116.);

	double Z = 108.8840*LABfun_inv(  ((L+16.)/116.)-(b_/200.) );

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

void RGB_to_LAB(double *r, double *g, double *b){
	//first convert rgb to XYZ
	//bring to 0,1 range
	*r/=255.;
	*g/=255.;
	*b/=255.;
	
	//gamma expansion
	*r = gamma_expand(*r);
	*g = gamma_expand(*g);
	*b = gamma_expand(*b);

	//matrix multiplication
	double X = 0.4124564*(*r) +0.3575761*(*g) +0.1804375*(*b);
	double Y = 0.2126729*(*r)  +0.7151522*(*g) +0.0721750*(*b);
	double Z = 0.0193339*(*r) +0.1191920*(*g) +0.9503041*(*b);

	//then convert to LAB
	*r = 116.*LABfun(Y/100.) - 16.;
	*g = 500.*(LABfun(X/95.0489) - LABfun(Y/100.));
	*b = 200.*(LABfun(Y/100.) - LABfun(Z/108.8840));
}

void LAB_to_RGB(double *L, double *a, double *b){
	//convert to XYZ
	
	//transformation
	double X = 95.0489*LABfun_inv( (((*L)+16.)/116.)+((*a)/500.) );

	double Y = 100.*LABfun_inv(((*L)+16.)/116.);

	double Z = 108.8840*LABfun_inv(  (((*L)+16.)/116.)-((*b)/200.) );

	//matrix multiplication
	double R = 3.2406*X -1.5372*Y -0.4986*Z;
	double G = -0.9689*X  +1.8758*Y +0.0415*Z;
	double B = 0.0557*X -0.2040*Y +1.0570*Z;
	
	//gamma compression
	R = gamma_compress(R);
	G = gamma_compress(G);
	B = gamma_compress(B);

	//output
	*L=expand_to_byte(R);
	*a=expand_to_byte(G);
	*b=expand_to_byte(B);
}

void LAB_to_LCH(double *L, double *a, double *b){
	//convert to LCH

	double var_H = (pi+atan2(*b,*a))*(360./(2.*pi));  //Quadrant by signs, turned into degrees


	*a = sqrt((*a)*(*a)+(*b)*(*b));
	*b = var_H;
}

void LCH_to_LAB(double *L, double *a, double *b){
	//convert to LAB
	double C = *a;
	double H=*b;
	double H_rad = ((2.*pi)/360.)*H - pi;
	*a = C*cos(H_rad);
	*b = C*sin(H_rad);
}

//il faudrait supprimer toutes les autres fonctions que rgb->lch.
//dernier fix à faire : la composante H ne semble pas bonne.
int main(){
	int r,g,b;
	r=1;g=20;b=220;
	double l,c,h;
	RGB_to_LCH(r,g,b,&l,&c,&h);
	std::cout<<"lch : "<<l<<", "<<c<<", "<<h<<std::endl;
	return 1;
}