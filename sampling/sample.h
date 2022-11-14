/** sample.h
 * 
 * Header file containing methods to up and downsample images for CNN usage
 * 
 * 
 * 
 */
#include "../utils/Image.h"
#include <stdint.h>
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

void upsample_no_diff(const Image* in, Image* out){
	int wIn = in->width,hIn= in->height;
	int wOut= out->width,hOut= out->height;
	float hratio = (float)hOut/hIn;
	float wratio = (float)wOut/wIn;
	

	std::cout<<hratio<< " "<<wratio<<std::endl;
	std::cout<<hIn<< " "<<wIn<<std::endl;

	int i=0,j=0;
	int ki=0,kj=0;
	float rli=1.,rlj=1.;
	bool flagi=false,flagj=false;


	while(ki<=hratio*(i+1)&&i<256&&ki<hOut){
		if(flagi||(i+1)*hratio-ki>=1){
			j=0;
			kj=0;
			while(kj<=wratio*(j+1)&&j<256&&kj<wOut){
				if(flagj||(j+1)*wratio-kj>=1){


					out->pixels[ki*wOut+kj].r+= in->pixels[i*256+j].r*rlj*rli;
					out->pixels[ki*wOut+kj].g+= in->pixels[i*256+j].g*rlj*rli;
					out->pixels[ki*wOut+kj].b+= in->pixels[i*256+j].b*rlj*rli;
					/*
					std::cout<<"i = "<<i<<"ki = "<<ki<<" j = "<<j<<"kj = "<<kj<<" ratio = "<<rli<<" "<<rlj<< "Value : "<< in->pixels[i*256+j].r<<" "<< in->pixels[i*256+j].g<<" "<< in->pixels[i*256+j].b
					<< " cumulated value " <<out->pixels[i*256+j].r<<" "<<out->pixels[ki*wOut+j].g<<" "<<out->pixels[i*wOut+j].b<< std::endl;
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

	std::cout<<i<<" "<<j<<" "<<ki<<" "<<kj<<std::endl;

}

void build_diff(const Image* original, const Image* upsampled, int16_t* diffMap){
	int height = original->height;
	int width = original->width;
	for(int i=0;i<height*width;i++){
		diffMap[3*i] = original->pixels[i].r-upsampled->pixels[i].r;
		diffMap[3*i+1] = original->pixels[i].g-upsampled->pixels[i].g;
		diffMap[3*i+2] = original->pixels[i].b-upsampled->pixels[i].b;
	}
}

void apply_diff(Image* upsampled, int16_t* diffMap){
	for(int i=0;i<upsampled->height*upsampled->width;i++){
		int16_t r = diffMap[3*i]+upsampled->pixels[i].r;
		int16_t g = diffMap[3*i+1]+upsampled->pixels[i].g;
		int16_t b = diffMap[3*i+2]+upsampled->pixels[i].b;

		if(r<0)
			r=0;
		if(r>255)
			r=255;
		if(g<0)
			g=0;
		if(g>255)
			g=255
		;if(b<0)
			b=0;
		if(b>255)
			b=255;
		upsampled->pixels[i].r = r;
		upsampled->pixels[i].g = g;
		upsampled->pixels[i].b = b;
	}
}