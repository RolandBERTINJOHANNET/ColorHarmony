#include "converter.cpp"
#include <math.h>


double compute_deltaC(double C1, double H1, double C2, double H2){
	return sqrt(pow(abs(H1-H2),2) + pow(abs(C1-C2)/1.46,2));
}

double compute_HdeltaL(double L1, double L2){
	return 0.14 + 0.15 * tanh(-2+0.2 * abs(L1-L2));
}

double compute_Hlsum(double L1, double L2){
	return 0.28 + 0.54 * tanh(-3.88 + 0.029 * (L1+L2));
}

double compute_Ec(double C1){
	return 0.5 + 0.5 * tanh(-2+0.5 * C1);
}

double compute_Hs(double H1){
	return -0.08 - 0.14 * sin(((pi)/180.)*(H1+50)) - 0.07 * sin(((pi)/180.)*(2*H1 + 90));
}

double compute_Ey(double L1, double H1){
	double operand = (90-H1)/(10.);
	return ((0.22 * L1 - 12.8)/(10.))*exp(operand-exp(operand));
}

double compute_Hsy(double L1, double C1, double H1){
	return compute_Ec(C1) * (compute_Hs(H1) + compute_Ey(L1,H1));
}

double Hc(double C1, double H1, double C2, double H2){
	return 0.04 + 0.53 * tanh(0.8 - 0.045 * compute_deltaC(C1,H1,C2,H2));
}
double Hh(double L1, double C1, double H1, double L2, double C2, double H2){
	return compute_Hsy(L1,C1,H1)+compute_Hsy(L2,C2,H2);
}
double Hl(double L1,double L2){
	return compute_Hlsum(L1,L2) + compute_HdeltaL(L1,L2);
}

double compute_harmony(int r1, int g1, int b1, int r2, int g2, int b2){
	double L1,C1,H1,L2,C2,H2;
	RGB_to_LCH(r1,g1,b1,&L1,&C1,&H1);
	RGB_to_LCH(r2,g2,b2,&L2,&C2,&H2);
	return Hc(C1,H1,C2,H2) + 
			Hl(L1,L2) + 
			Hh(L1,C1,H1,L2,C2,H2);
}