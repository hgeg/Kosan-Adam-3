#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "gameobject.h"
#include "camera.h"
#include "gamesettings.h"
#include "gk.h"

CameraC::CameraC(Controllable * focus, int x, int y,int w, int h,
		int lBoundary, int rBoundary, int uBoundary, int dBoundary)
{
  this->x          = x;
  this->y          = y;
  this->w          = w;
  this->h          = h;
  this->lBoundary  = lBoundary;
  this->rBoundary  = rBoundary;
  this->uBoundary  = uBoundary;
  this->dBoundary  = dBoundary;
  this->focus      = focus;

}

void CameraC::moveCamera()
{
   static int relX, relY;
   relX = this->focus->x - this->x + this->focus->w/2;
   relY = this->focus->y - this->y + this->focus->h/2;
   if(relX < this->lBoundary )
   {
     /*this->x = this->focus->x + this->focus->w/2 - this->lBoundary;*/
     if(this->lBoundary-relX >120)
       this->x -= 14;
     else if(this->lBoundary-relX >70)
       this->x -= 11;
     else if(this->lBoundary-relX >40)
       this->x -= 7;
     else if(this->lBoundary-relX>20)
       this->x -= 4;
     else if(this->lBoundary-relX>10)
       this->x -= 2;
   }
   else if(relX > (this->w - this->rBoundary))
   {
     if(relX - (this->w - this->rBoundary) > 120)
       this->x += 14;
     else if(relX - (this->w - this->rBoundary) > 70)
       this->x += 11;
     else if(relX - (this->w - this->rBoundary) > 40)
       this->x += 7;
     else if(relX - (this->w - this->rBoundary) > 20)
       this->x += 4;
     else if(relX - (this->w - this->rBoundary) > 10)
       this->x += 2;
       /*this->x -= 2;this->x = this->focus->x + this->focus->w/2 - (this->w - this->rBoundary) ;*/
   }

   if(relY < this->dBoundary )
   {
     if(this->dBoundary-relY >70)
       this->y -= 15;
     else if(this->dBoundary-relY >40)
       this->y -= 10;
     else if(this->dBoundary-relY>20)
       this->y -= 6;
     else if(this->dBoundary-relY>10)
       this->y -= 2;
     /*this->y = this->focus->y +this->focus->h/2 - this->uBoundary;*/
   }
   else if(relY > (this->h - this->dBoundary))
   {
     if(relY - (this->h - this->dBoundary) > 70)
       this->y += 15;
     else if(relY - (this->h - this->dBoundary) > 40)
       this->y += 10;
     else if(relY - (this->h - this->dBoundary) > 20)
       this->y += 6;
     else if(relY - (this->h - this->dBoundary) > 10)
       this->y += 2;
     /*this->y = this->focus->y + this->focus->h/2- (this->h - this->dBoundary)  ;*/
   }
   /*this->x = this->focus->x + this->focus->w/2 -400;*/
   /*this->y = this->focus->y + this->focus->h/2;*/

}


