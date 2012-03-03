#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "gameobject.h"
#include "camera.h"
#include "gamesettings.h"
#include "gk.h"
#include "background.h"



BackgroundC::BackgroundC()
{

}

BackgroundC::BackgroundC(int w, int h, SDL_Surface * i, int l, int x, int y)
{
  this->w = i->w;
  this->h = i->h;

  this->imageSurface = i;

  this->posRect.x = 0;
  this->posRect.y = 0;
  this->posRect.w = this->w;
  this->posRect.h = this->h;

  this->layer = l;
  this->xOffset = x;
  this->yOffset = y;

  this->clippingRect.x = 0;
  this->clippingRect.y = 0;
  this->clippingRect.w = this->w;
  this->clippingRect.h = this->h;
}

void BackgroundC::drawBackground(CameraC * cam)
{
  int offX,offY;
  int x = 0, y=0;
  offX = ((cam->x / (this->layer+3)) % this->w);
  offY = ((cam->y / (this->layer+4)) % this->h);
  if(offX <0)
    offX = this->w + offX;

  if(offY <0)
    offY = this->h + offY;


  x = 0 - offX;
  y = 0 - offY;
  if(this->xOffset<0 && this->yOffset<0)
    while(y < screen->h)
    {
      while(x < screen->w )
      {
        this->posRect.x = x;
        this->posRect.y = y;
        SDL_BlitSurface(this->imageSurface,&(this->clippingRect),screen,&(this->posRect));
        x += this->w;
      }
      y += this->h;
      x = 0 - offX;
    }
    else if(this->yOffset>=0)
    {
      y = this->yOffset - 2*cam->y/3;
      while(x < screen->w )
      {
        this->posRect.x = x;
        this->posRect.y = y;
        SDL_BlitSurface(this->imageSurface,&(this->clippingRect),screen,&(this->posRect));
        x += this->w;
      }
      y += this->h;
    }
}

