/** sample.h
 * 
 * Header file containing methods to up and downsample images for CNN usage
 * 
 * 
 * 
 */
#include "../utils/Image.h"
#include <iostream>




void downsample(const Image* in, Image* out){
	int wIn = in->width,hIn= in->height;
	int wOut= out->width,hOut= out->height;
	float hratio = (float)hIn/hOut;
	float wratio = (float)wIn/wOut;
	
	if(hratio<1||wratio<1){
		std::cout<<"Upsampling not supported yet"<<std::endl;
		return;
	}

	std::cout<<hratio<< " "<<wratio<<std::endl;
	std::cout<<hIn<< " "<<wIn<<std::endl;

	int i=0,j=0;
	int ki=0,kj=0;
	float rli=1.,rlj=1.;
	bool flagi=false,flagj=false;
	while(ki<=hratio*(i+1)&&i<256&&ki<hIn){
		if(flagi||(i+1)*hratio-ki>=1){
			j=0;
			kj=0;
			while(kj<=wratio*(j+1)&&j<256&&kj<wIn){
				if(flagj||(j+1)*wratio-kj>=1){


					out->pixels[i*256+j].r+= in->pixels[ki*wIn+kj].r*rlj*rli;
					out->pixels[i*256+j].g+= in->pixels[ki*wIn+kj].g*rlj*rli;
					out->pixels[i*256+j].b+= in->pixels[ki*wIn+kj].b*rlj*rli;
					
					/*std::cout<<"i = "<<i<<"ki = "<<ki<<" j = "<<j<<"kj = "<<kj<<" ratio = "<<rli<<" "<<rlj<< "Value : "<< in->pixels[ki*wIn+kj].r<<" "<< in->pixels[ki*wIn+kj].g<<" "<< in->pixels[ki*wIn+kj].b
					<< " cumulated value " <<out->pixels[i*256+j].r<<" "<<out->pixels[i*256+j].g<<" "<<out->pixels[i*256+j].b<< std::endl;
					*/
					if(!flagj){
						rlj=1;
						kj++;
					}
					else{
						rlj =1.-rlj;
						//std::cout<<"next one taking "<<rlj<<" leftover"<<std::endl;
						flagj = false;
						j++;
						
					}
				}
				else{
					rlj = (j+1)*wratio-kj;
					//std::cout<<"ratio put to"<<rlj<<std::endl;
					flagj = true;
				}

			}
			if(!flagi){
				rli=1;
				ki++;
			}
			else{
				rli=1.-rli;
				flagi = false;
				i++;
			}

		}
		else{
			rli = (i+1)*hratio-ki;
			flagi=true;

		}
	}
	
	for(i=0;i<hOut;i++)
		for(j=0;j<wOut;j++){

			out->pixels[i*256+j]/=(wratio*hratio);
		}


}