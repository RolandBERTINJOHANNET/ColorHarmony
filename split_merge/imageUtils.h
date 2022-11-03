
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
//choices made with split&merge in mind

class Image;

class Pixel{
public:
	int i,j,block_label;
	unsigned char r,g,b;
	Pixel(int i, int j, unsigned char r,unsigned char g, unsigned char b, int block_label){
		this->i=i;this->j=j;
		this->r=r;this->g=g;this->b=b;
		this->block_label=block_label;
	}
};

class Block{
public :
	std::vector<Pixel*>::const_iterator start;
	int full_size,block_size,block_label;
	Image *image;
	Block(std::vector<Pixel*>::const_iterator pixelsStart,int blocksize, int imageWidth, int block_label, Image *image){
		this->start=pixelsStart;
		this->block_size=blocksize;
		full_size=imageWidth;
		block_label=block_label;
		this->image = image;
		assignPixelsToBlock();
	}
	void Split(int min_size,int max_x,int max_y);
	float variance();
	void assignPixelsToBlock();
	void mergeNeighbor(int directionx,int directiony,std::vector<Pixel*>::const_iterator starting_point,int max_x,int max_y);
	void Merge(int min_size,int max_x,int max_y);
	void colorFinalBlock();
};

class Image{
private:
	int w,h;
public:
	std::vector<Pixel*> pixels;
	std::vector<Block*> blocks;
	//from file
	Image(){}
	void fromFile(const char *filename);
	void save(const char *filename);
	void SplitAndMerge(int min_size);
};