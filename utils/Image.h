//Image.h
#include <vector>
#define DOWN_SIZE 256

struct Pixel{
	int r,g,b;

	void operator+=(Pixel a){
		r+=a.r;
		g+=a.g;
		b+=a.b;
	}

	void operator/=(float f){
		r/=f;
		g/=f;
		b/=f;
	}

	Pixel operator*(const float f) const {
		Pixel res;
		res.r = r*f;
		res.g = r*g;
		res.b = r*b;
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
				pixels[i].r =0;pixels[i].g=0;pixels[i].b=0;
			}
		}
		Image(int w,int h){
			width = w;
			height = h;
			pixels.resize(w*h);
			for(int i = 0;i<w*h;i++){
				pixels[i].r =0;pixels[i].g=0;pixels[i].b=0;
			}
		}
		std::vector<Pixel> pixels;
		int height;
		int width;
		void readfile(const char* filename);
		void writefile(const char* filename);
};