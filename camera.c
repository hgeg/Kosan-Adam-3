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
     if(this->lBoundary-relX >70)
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
     if(relX - (this->w - this->rBoundary) > 70)
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

void cameraInit(Camera * cam, DynamicObject * focus, int x, int y,int w, int h,
		int lBoundary, int rBoundary, int uBoundary, int dBoundary)
{
  cam->x          = x;
  cam->y          = y;
  cam->w          = w;
  cam->h          = h;
  cam->lBoundary  = lBoundary;
  cam->rBoundary  = rBoundary;
  cam->uBoundary  = uBoundary;
  cam->dBoundary  = dBoundary;
  cam->focus      = focus;
}

void moveCamera(Camera * cam)
{
   static int relX, relY;
   relX = cam->focus->x - cam->x + cam->focus->w/2;
   relY = cam->focus->y - cam->y + cam->focus->h/2;
   if(relX < cam->lBoundary )
   {
     /*cam->x = cam->focus->x + cam->focus->w/2 - cam->lBoundary;*/
     if(cam->lBoundary-relX >70)
       cam->x -= 11;
     else if(cam->lBoundary-relX >40)
       cam->x -= 7;
     else if(cam->lBoundary-relX>20)
       cam->x -= 4;
     else if(cam->lBoundary-relX>10)
       cam->x -= 2;
   }
   else if(relX > (cam->w - cam->rBoundary))
   {
     if(relX - (cam->w - cam->rBoundary) > 70)
       cam->x += 11;
     else if(relX - (cam->w - cam->rBoundary) > 40)
       cam->x += 7;
     else if(relX - (cam->w - cam->rBoundary) > 20)
       cam->x += 4;
     else if(relX - (cam->w - cam->rBoundary) > 10)
       cam->x += 2;
       /*cam->x -= 2;cam->x = cam->focus->x + cam->focus->w/2 - (cam->w - cam->rBoundary) ;*/
   }

   if(relY < cam->dBoundary )
   {
     if(cam->dBoundary-relY >70)
       cam->y -= 15;
     else if(cam->dBoundary-relY >40)
       cam->y -= 10;
     else if(cam->dBoundary-relY>20)
       cam->y -= 6;
     else if(cam->dBoundary-relY>10)
       cam->y -= 2;
     /*cam->y = cam->focus->y +cam->focus->h/2 - cam->uBoundary;*/
   }
   else if(relY > (cam->h - cam->dBoundary))
   {
     if(relY - (cam->h - cam->dBoundary) > 70)
       cam->y += 15;
     else if(relY - (cam->h - cam->dBoundary) > 40)
       cam->y += 10;
     else if(relY - (cam->h - cam->dBoundary) > 20)
       cam->y += 6;
     else if(relY - (cam->h - cam->dBoundary) > 10)
       cam->y += 2;
     /*cam->y = cam->focus->y + cam->focus->h/2- (cam->h - cam->dBoundary)  ;*/
   }
   /*cam->x = cam->focus->x + cam->focus->w/2 -400;*/
   /*cam->y = cam->focus->y + cam->focus->h/2;*/
}
