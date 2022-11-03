
#include "imageUtils.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//choices made with split&merge in mind
//after the merge, Image's blocks array is only for knowing existing blocks labels,
			//block info is only attained through the pixels's assignment
//deux choses à paramétrer : la variance dans la descente (le split) et la distance dans le merge (tolérance pour l'union)



void Block::Split(int min_size){
	//std::cout<<"entered split"<<std::endl;
	//std::cout<<image<<std::endl;
	if(sqrt(variance())>20 && block_size>min_size){
		//std::cout<<"entered if "<<std::endl;
		//each block added to image blocks
		for(int x=0;x<2;x++){
			for(int y=0;y<2;y++){
				//std::cout<<"entered the loop for "<<x<<", "<<y<<std::endl;
				if(start + x*block_size/2 + y*full_size*block_size/2 < image->pixels.end()){
					//std::cout<<"		creating block "<<image->blocks.size()<<std::endl;
					image->blocks.push_back(new Block(
											start + x*block_size/2 + y*full_size*block_size/2,
											block_size/2,
											full_size,
											image->blocks.size(),
											image));
					//std::cout<<"calling Split on "<<image->blocks.size()<<"th block "<<std::endl;
					image->blocks[image->blocks.size()-1]->Split(min_size);
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

void Block::mergeNeighbor(int directionx,int directiony,std::vector<Pixel*>::const_iterator starting_point){
	std::vector<Pixel*>::const_iterator it = starting_point;
	//set it to one of the neighbors' starting points
	it+=directionx==1?directionx*block_size : directionx;
	it+=directiony==1?full_size*directiony*block_size : full_size*directiony;
	//vérifier qu'on ne pioche pas trop loin !
	if(it >= image->pixels.end()  || it < image->pixels.begin())
						 return;
	//std::cout<<"merging block of label "<<(*it)->block_label<<" to block of label "<<(*starting_point)->block_label<<std::endl;
	//check distance between mean colors
	int diff[3];
	diff[0]=abs((*it)->r - (*starting_point)->r);
	diff[1]=abs((*it)->g - (*starting_point)->g);
	diff[2]=abs((*it)->b - (*starting_point)->b);
	//std::cout<<"difference : "<<(diff[0]+diff[1]+diff[2])/9.<<std::endl;
	if((diff[0]+diff[1]+diff[2])/9.<10){
		//std::cout<<"looking for neighbor with label "<<(*it)->block_label<<std::endl;
		std::vector<Block*>::iterator blockIt = std::find_if(image->blocks.begin(),image->blocks.end(),
									[it](Block* &bl){return bl->iterator_inside_block(it);});
		//std::cout<<"found neighbor with label "<<(*blockIt)->block_label<<std::endl;
		(*blockIt)->assignLabel(block_label);

	}
}

void Block::assignLabel(int blocklabel){
	//std::cout<<"assigning label "<<blocklabel<<" to block "<<this->block_label<<std::endl;
	block_label = blocklabel;
	std::vector<Pixel*>::const_iterator it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			(*(it+i*full_size+j))->block_label = blocklabel;
		}
	}
}

void Block::fillWithColor(int r,int g,int b){
	//std::cout<<"assigning color "<<(int)r<<", "<<(int)g<<", "<<(int)b<<" to block "<<this->block_label<<std::endl;
	std::vector<Pixel*>::const_iterator it=start;
	for(int i=0;i<block_size;i++){
		for(int j=0;j<block_size;j++){
			(*(it+i*full_size+j))->r = r;
			(*(it+i*full_size+j))->g = g;
			(*(it+i*full_size+j))->b = b;
		}
	}
}

bool Block::iterator_inside_block(std::vector<Pixel*>::const_iterator iter){
	int i = (iter - image->pixels.begin()) % full_size;
	int j = (iter - image->pixels.begin()) / full_size;
	
	int start_offsetx = (start - image->pixels.begin()) % full_size;
	int start_offsety = (start - image->pixels.begin()) / full_size;

	return i<start_offsetx+block_size && i>=start_offsetx && j<start_offsety+block_size && j>=start_offsety;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   not checking for accessing mem outside of image because the block wouldn't have been created
//	but I might be wrong.
void Block::Merge(int min_size){
	//std::cout<<"merging "<<block_label<<" out of "<<image->blocks.size()<<std::endl;
	//call merge neighbor every min_size in both directions (along +-x, with directiony and along +-y, with directionx)
	int loop_helper[2]={-1,1};
	//along horizontal axis
	std::vector<Pixel*>::const_iterator it = start;
	for(int off=0;off<block_size;off+=min_size){
		for(int i=0;i<2;i++){
			mergeNeighbor(loop_helper[i],0,it+(off*full_size));
		}
	}
	//along vertical axis
	it = start;
	for(int off=0;off<block_size;off+=min_size){
		for(int i=0;i<2;i++){
			mergeNeighbor(0,loop_helper[i],it+off);
		}
	}
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

void Image::colorFromMerge(){
	for(int i=0;i<blocks.size();i++){
		//get list of blocks with current labels
		std::vector<Block*> thisLabelBlocks;
		for(auto block : blocks){
			if(block->block_label==i)
				thisLabelBlocks.push_back(block);
		}
		//std::cout<<"got "<<thisLabelBlocks.size()<<" blocks for label "<<i<<std::endl;
		if(thisLabelBlocks.size()>0){
			//get their mean color
			int means[3]={0,0,0};
			for(auto block : thisLabelBlocks){
				means[0]+=(*block->start)->r;
				means[1]+=(*block->start)->g;
				means[2]+=(*block->start)->b;
			}
			for(int k=0;k<3;k++)means[k]/=thisLabelBlocks.size();
			//std::cout<<"got color "<<(int)means[0]<<", "<<(int)means[1]<<", "<<(int)means[2]<<std::endl;
			//apply it to all of them.
			for(auto block : thisLabelBlocks){
				block->fillWithColor(means[0],means[1],means[2]);
			}
		}
	}
}

void Image::SplitAndMerge(int min_size){
	blocks.resize(0);
	blocks.push_back(new Block(pixels.begin(), w, w, blocks.size(), this));
	blocks[0]->Split(min_size);//splits all blocks recursively
	for(auto block : blocks){
		block->Merge(min_size);
	}
	colorFromMerge();
}