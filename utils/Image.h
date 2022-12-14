//Image.h
#include <vector>
#define DOWN_SIZE 256

struct Pixel{
	int l,c,h;

	void operator+=(Pixel a){
		l+=a.l;
		c+=a.c;
		h+=a.h;
	}

	void operator/=(float f){
		l/=f;
		c/=f;
		h/=f;
	}

	Pixel operator*(const float f) const {
		Pixel res;
		res.l = l*f;
		res.c = l*c;
		res.h = l*h;
		return res;
	}
};


class Image{
	public:
		Image(){};
		Image(int size){
			width = size;
			height = size;
			pixels.resize(size*size);
			for(int i = 0;i<size;i++){
				pixels[i].l =0;pixels[i].c=0;pixels[i].h=0;
			}
		}
		Image(int w,int h){
			width = w;
			height = h;
			pixels.resize(w*h);
			for(int i = 0;i<w*h;i++){
				pixels[i].l =0;pixels[i].c=0;pixels[i].h=0;
			}
		}
		std::vector<Pixel> pixels;
		int height;
		int width;
		void readfile(const char* filename);
		void readfile_LCH(const char* filename);
		void writefile(const char* filename);
		void writefile_LCH(const char* filename);
};