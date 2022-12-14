application : samplingtest.cpp Image.o 
	g++  -std=c++17 -o application.out samplingtest.cpp Image.o Sampler.h


Image.o : ../utils/Image.cpp
	g++ -std=c++17  -c ../utils/Image.cpp

Sampler.o : Sampler.cpp
	g++ -fPIC -std=c++17  -c Sampler.cpp

extern : Sampler.cpp Image.o
	g++ -c -fPIC Sampler.cpp -o Sampler.o 
	g++ -c -fPIC ../utils/Image.cpp -o Image.o 
	g++ -shared -fPIC -Wl,-soname,libfoo.so -o libsampler.so  Sampler.o Image.o 


clear:
	rm *.o