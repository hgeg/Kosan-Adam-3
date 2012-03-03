#ifndef SURFACEHANDLER_H
#define SURFACEHANDLER_H

#include <string>

class SurfaceHandler
{
	private:
	SDL_Surface * textures[100];
	string files [100];
	int counter;
	public:
	SDL_Surface * loadImage(string filename);
	SDL_Surface * getImage(string filename);
	void freeSurfaces();
	SurfaceHandler();
	~SurfaceHandler();
};


#endif
