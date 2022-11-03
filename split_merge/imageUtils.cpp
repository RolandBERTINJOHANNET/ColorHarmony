
#include "imageUtils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//choices made with split&merge in mind
//after the merge, Image's blocks array is only for knowing existing blocks labels,
			//block info is only attained through the pixels's assignment
//deux choses à paramétrer : la variance dans la descente (le split) et la distance dans le merge (tolérance pour l'union)



void Block::Split(int min_size,int max_x,int max_y){
	//std::cout<<"entered split"<<std::endl;
	//std::cout<<image<<std::endl;
	if(sqrt(variance())>20 && block_size>min_size){
		//std::cout<<"entered if "<<std::endl;
		//each block added to image blocks
		for(int x=0;x<2;x++){
			for(int y=0;y<2;y++){
				//std::cout<<"entered the loop for "<<x<<", "<<y<<std::endl;
				if(start + x*block_size/2 + y*full_size*block_size/2 < image->pixels.begin() + max_x + max_y*full_size){
					image->blocks.push_back(new Block(
											start + x*block_size/2 + y*full_size*block_size/2,
											block_size/2,
											full_size,
											image->blocks.size(),
											image));
					//std::cout<<"calling Split on "<<image->blocks.size()<<"th block "<<std::endl;
					image->blocks[image->blocks.size()-1]->Split(min_size,max_x,max_y);
				}
				else{
					std::cout<<(x*block_size/2 + y*full_size*block_size/2) <<" is bigger than "<< (max_x + max_y*full_size)<<std::endl;
				}
			}

		}
		//remove current block from Image blocks
		std::remove(image->blocks.begin(),image->blocks.end(),this);
	}else{
		//std::cout<<"didn't enter the if "<<std::endl;
		colorFinalBlock();
	}
}

float Block::variance(){
	float means[3]={0,0,0};
	std::vector<Pixel*>::const_iterator it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			means[0]+=(*(it+i*full_size+j))->r;
			means[1]+=(*(it+i*full_size+j))->g;
			means[2]+=(*(it+i*full_size+j))->b;
		}
	}
	for(int i=0;i<3;i++){means[i]/=block_size*block_size;}
	float variances[3]={0,0,0};
	it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			variances[0]+=pow(means[0]-(*(it+i*full_size+j))->r,2);
			variances[1]+=pow(means[1]-(*(it+i*full_size+j))->g,2);
			variances[2]+=pow(means[2]-(*(it+i*full_size+j))->b,2);
		}
	}
	for(int i=0;i<3;i++){variances[i]/=block_size*block_size;}
	//std::cout<<"variance : "<<(variances[0]+variances[1]+variances[2])/3.<<std::endl;
	return (variances[0]+variances[1]+variances[2])/3.;
}

void Block::assignPixelsToBlock(){
	std::vector<Pixel*>::const_iterator it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			(*(it+i*full_size+j))->block_label=block_label;
		}
	}
}

void Block::colorFinalBlock(){
	float means[3]={0,0,0};
	std::vector<Pixel*>::const_iterator it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			means[0]+=(*(it+i*full_size+j))->r;
			means[1]+=(*(it+i*full_size+j))->g;
			means[2]+=(*(it+i*full_size+j))->b;
		}
	}
	for(int i=0;i<3;i++){means[i]/=block_size*block_size;}
	it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			(*(it+i*full_size+j))->r=means[0];
			(*(it+i*full_size+j))->g=means[1];
			(*(it+i*full_size+j))->b=means[2];
		}
	}
}

void Block::mergeNeighbor(int directionx,int directiony,std::vector<Pixel*>::const_iterator starting_point,int max_x,int max_y){
	std::vector<Pixel*>::const_iterator it = starting_point;
	int i,j;i=j=0;
	while((*it)->block_label==block_label){
		//vérifier qu'on ne pioche pas trop loin !
		if(it < image->pixels.begin() + max_x + max_y*full_size){
			it+=j*directionx;
			it+=i*full_size*directiony;
		}
	}
	//check distance between mean colors
	int diff[3];
	diff[0]=abs((*it)->r - (*it-j*directionx -i*full_size*directiony)->r);
	diff[1]=abs((*it)->g - (*it-j*directionx -i*full_size*directiony)->g);
	diff[2]=abs((*it)->b - (*it-j*directionx -i*full_size*directiony)->b);
	std::cout<<"differences : "<<diff[0]<<", "<<diff[1]<<", "<<diff[2]<<std::endl;
	if((diff[0]+diff[1]+diff[2])/9.<30){
		std::cout<<"looking for neighbor with label "<<(*it)->block_label<<std::endl;
		std::vector<Block*>::iterator blockIt = std::find_if(image->blocks.begin(),image->blocks.end(),
									[it](Block* &bl){return bl->block_label==(*it)->block_label;});
		std::cout<<"found neighbor with label "<<(*blockIt)->block_label<<std::endl;
		(*blockIt)->assignLabel(block_label);
		(*blockIt)->fillWithColor(
									(int((*it)->r) + int((*it-j*directionx -i*full_size*directiony)->r))/2,
									(int((*it)->g) + int((*it-j*directionx -i*full_size*directiony)->g))/2,
									(int((*it)->b) + int((*it-j*directionx -i*full_size*directiony)->b))/2
								);

	}
	std::remove(image->blocks.begin(),image->blocks.end(),(*blockIt));//remove merged block from image blocks
}

void Block::Merge(int min_size,int max_x,int max_y){
	//call merge neighbor every min_size in both directions (along +-x, with directiony and along +-y, with directionx)
}

void Image::fromFile(const char *filename){
	int chan;
	uint8_t *data = stbi_load(filename, &w, &h, &chan, 3);
	for(int i=0;i<h;i++){
		for(int j=0;j<w*3;j+=3){
			pixels.push_back(new Pixel(i,j/3,data[i*w*3+j],data[i*w*3+j+1],data[i*w*3+j+2],0));
		}
	}
	delete[] data;
}

void Image::save(const char *filename){
	std::vector<unsigned char> data;
	for(int i=0;i<pixels.size();i++){
		data.push_back(pixels[i]->r);
		data.push_back(pixels[i]->g);
		data.push_back(pixels[i]->b);
	}
	stbi_write_png(filename, w, h, 3, &data[0], w*3);
}

void Image::SplitAndMerge(int min_size){
	blocks.resize(0);
	blocks.push_back(new Block(pixels.begin(), w, w, blocks.size(), this));
	blocks[0]->Split(min_size,w,h);
}