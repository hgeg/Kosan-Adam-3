#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include <iostream>
using namespace std;
#include "surfacehandler.h"


SDL_Surface * SurfaceHandler::loadImage(string filename)
{
	int ignore = 0;
	SDL_Surface * tempSurface;
	for(int i = 0; i<counter;i++)
	{
		if(!files[i].compare(filename))
		{
			ignore = 1;
			break;
		}
	}

	if(!ignore)
	{
		tempSurface = (IMG_Load(filename.c_str()));
		textures[counter] = SDL_DisplayFormat(tempSurface);
		files[counter] = filename;
		counter++;
		SDL_FreeSurface(tempSurface);
		return textures[counter - 1];
	}
	return NULL;
}


SDL_Surface * SurfaceHandler::getImage(string filename)
{

	for(int i = 0; i<counter;i++)
	{
		if(!files[i].compare(filename))
		{
			return textures[i];
		}
	}

	return loadImage(filename);
}

void SurfaceHandler::freeSurfaces()
{
	for(int i = 0; i<counter;i++)
	{
		SDL_FreeSurface(textures[i]);
	}
	counter  = 0;
}

SurfaceHandler::SurfaceHandler()
{
	counter = 0;
}

SurfaceHandler::~SurfaceHandler()
{
	for(int i = 0; i<counter;i++)
	{
		SDL_FreeSurface(textures[i]);
	}
	counter  = 0;
}

