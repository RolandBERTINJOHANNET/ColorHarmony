/** sample.h
 * 
 * Header file containing methods to up and downsample images for CNN usage
 * 
 * 
 * 
 */
#include "Image.h"
#include <stdint.h>
#include <iostream>
#include <cmath>

void downsample(const Image* in, Image* out){
	int wIn = in->width,hIn= in->height;
	int wOut= out->width,hOut= out->height;
	float hratio = (float)hIn/hOut;
	float wratio = (float)wIn/wOut;
	
	if(hratio<1||wratio<1){
		std::cout<<"Upsampling not supported yet"<<std::endl;
		std::cout << "height = " << hIn << "width = " << wIn << std::endl;
		return;
	}

	std::cout<<hratio<< " "<<wratio<<std::endl;
	std::cout<<hIn<< " "<<wIn<<std::endl;

	int i=0,j=0;
	int ki=0,kj=0;
	float rli=1.,rlj=1.;
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			int blocsum = 0;
			int transblocsum = 0;

			int blocount = 0;

			for(ki=floor(i*hratio);ki<ceil((i+1)*hratio);ki++){
				if(ki<hIn){
					for(kj=floor(j*wratio);kj<ceil((j+1)*wratio);kj++){
						if(kj<wIn){


							out->pixels[i*256+j].l+= in->pixels[ki*wIn+kj].l;
							out->pixels[i*256+j].c+= in->pixels[ki*wIn+kj].c;
							blocsum += in->pixels[ki*wIn+kj].h;
							transblocsum += (in->pixels[ki*wIn+kj].h+128)%256;
							blocount++;
						}
					}

				}
			}
			int avgblocsum = blocsum/blocount;
			int avgtblocsum = transblocsum/blocount;


			int eq = 0;
			int teq = 0;

			for(ki=floor(i*hratio);ki<ceil((i+1)*hratio);ki++){
				if(ki<hIn){
					for(kj=floor(j*wratio);kj<ceil((j+1)*wratio);kj++){
						if(kj<wIn){
							eq += sqrt((in->pixels[ki*wIn+kj].h-avgblocsum)*(in->pixels[ki*wIn+kj].h-avgblocsum));
							teq += sqrt( (((in->pixels[ki*wIn+kj].h+128)%256)-avgtblocsum) * (((in->pixels[ki*wIn+kj].h+128)%256)-avgtblocsum));
						}
					}

				}
			}

			out->pixels[i*256+j].l /= blocount;
			out->pixels[i*256+j].c /= blocount;

			out->pixels[i*256+j].h = eq<teq ? avgblocsum :(avgtblocsum+128)%256 ;
		}
	}
}




void upsample(const Image* in, Image* out){
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


	for(ki=0;ki<hOut;ki++){
		for(kj=0;kj<wOut;kj++){
			i = floor(ki/hratio);
			j = floor(kj/wratio);
			out->pixels[ki*wOut+kj].l+= in->pixels[i*256+j].l;
			out->pixels[ki*wOut+kj].c+= in->pixels[i*256+j].c;
			out->pixels[ki*wOut+kj].h+= in->pixels[i*256+j].h;
		}
	}

}






void build_diff(const Image* original, const Image* upsampled, int16_t* diffMap){
	int height = original->height;
	int width = original->width;
	for(int i=0;i<height*width;i++){
		diffMap[2*i] = original->pixels[i].c-upsampled->pixels[i].c;
		diffMap[2*i+1] = original->pixels[i].h-upsampled->pixels[i].h;
	}
}


void build_grad(const Image* original, int16_t* gradMap){

	int height = original->height;
	int width = original->width;
	for (int i=1; i < height; i++)
    	for (int j=1; j < width; j++)
    	{
    	    gradMap[(4*(i*width+j))] = original->pixels[i*width+j-1].c-original->pixels[i*width+j].c;
    	    gradMap[(4*(i*width+j))+1] = original->pixels[(i-1)*width+j].c-original->pixels[i*width+j].c;
    	    gradMap[(4*(i*width+j))+2] = original->pixels[i*width+j-1].h-original->pixels[i*width+j].h;
    	    gradMap[(4*(i*width+j))+3] = original->pixels[(i-1)*width+j].h-original->pixels[i*width+j].h;

    	}
    }

void apply_grad(const Image* original, Image* upsampled,int16_t* gradMap){

	int height = upsampled->height;
	int width = upsampled->width;
	for(int i=0;i<height;i++){
		upsampled->pixels[i*width].l = original->pixels[i*width].l;
		upsampled->pixels[i*width].c = original->pixels[i*width].c;
		upsampled->pixels[i*width].h = original->pixels[i*width].h;
	}
	for(int i=1;i<width;i++){
		upsampled->pixels[i].l = original->pixels[i].l;
		upsampled->pixels[i].c = original->pixels[i].c;
		upsampled->pixels[i].h = original->pixels[i].h;
	}
	for (int i=1; i < height; i++)
    	for (int j=1; j < width; j++)
    	{

			int16_t c = (gradMap[(4*(i*width+j))]+upsampled->pixels[i*width+j-1].c)*0.33+(gradMap[(4*(i*width+j))+1]+upsampled->pixels[i*width+j].c)*0.33+upsampled->pixels[i*width+j].c*0.33;
			int16_t h = ((gradMap[(4*(i*width+j))+2]+upsampled->pixels[i*width+j-1].h)%255)*0.33+((gradMap[(4*(i*width+j))+3]+upsampled->pixels[i*width+j].h)%255)*0.33+upsampled->pixels[i*width+j].h*0.33;

			if(c<0)
				c=0;
			if(c>255)
				c=255;
			if(h<0)
				h=0;
			if(h>255)
				h=255;

		upsampled->pixels[i*width+j].l = original->pixels[i*width+j].l;
		upsampled->pixels[i*width+j].c = c;
		upsampled->pixels[i*width+j].h = h;

    	}
    }

void apply_diff(const Image* original, Image* upsampled, int16_t* diffMap){

	for(int i=0;i<upsampled->height*upsampled->width;i++){
		int16_t c = diffMap[2*i]+upsampled->pixels[i].c;
		int16_t h = diffMap[2*i+1]+upsampled->pixels[i].h;

		if(c<0)
			c=0;
		if(c>255)
			c=255;
		if(h<0)
			h=0;
		if(h>255)
			h=255;
		upsampled->pixels[i].l = original->pixels[i].l;
		upsampled->pixels[i].c = c;
		upsampled->pixels[i].h = h;
	}
}



void downsample_pix(const Image* in, Image* out){
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


					out->pixels[i*256+j].l+= in->pixels[ki*wIn+kj].l*rlj*rli;
					out->pixels[i*256+j].c+= in->pixels[ki*wIn+kj].c*rlj*rli;
					out->pixels[i*256+j].h+= in->pixels[ki*wIn+kj].h*rlj*rli;
					
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
	int mem = out->pixels[0].h;
	for(i=0;i<hOut;i++)
		for(j=0;j<wOut;j++){
			out->pixels[i*256+j]/=(wratio*hratio);
			if(mem-out->pixels[i*256+j].h <-200|| mem-out->pixels[i*256+j].h>200){
				std::cout<<i<<" "<<j<<" " <<out->pixels[i*256+j].l<<	" "<<	out->pixels[i*256+j].c<<" "<<out->pixels[i*256+j].h<<std::endl;
			}
			mem = out->pixels[i*256+j].h;	

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


					out->pixels[ki*wOut+kj].l+= in->pixels[i*256+j].l*rlj*rli;
					out->pixels[ki*wOut+kj].c+= in->pixels[i*256+j].c*rlj*rli;
					out->pixels[ki*wOut+kj].h+= in->pixels[i*256+j].h*rlj*rli;
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

}
