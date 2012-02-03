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
#include "background.h"
#include "gameworld.h"


void GameObject::setPos(float xx, float yy)
{
  x = xx;
  y = yy;
}

void GameObject::setColRect(int x, int y, int w, int h){
  this->colOffX = x;
  this->colOffY = y;
  this->collisionRect.x = this->x + x;
  this->collisionRect.y = this->y + y;
  this->collisionRect.w = w;
  this->collisionRect.h = h;
}

void GameObject::step()
{
}

Controllable::Controllable()
{

}
Controllable::Controllable(const Controllable& temp)
{
  x              = temp.x;
  y              = temp.y;
  w              = temp.w;
  h              = temp.h;
  type           = temp.type;
  animationFrame = temp.animationFrame;
  colOffX        = temp.colOffX;
  colOffY        = temp.colOffY;
  imageSurface   = temp.imageSurface;
  posRect        = temp.posRect;
  collisionRect  = temp.collisionRect;
  clippingRect   = temp.clippingRect;
  world          = temp.world;
  xSpeed         = temp.xSpeed;
  ySpeed         = temp.ySpeed;
  inAir          = temp.inAir;
  climbing       = temp.climbing;
  running        = temp.running;
  direction      = temp.direction;
  jumpSteps      = temp.jumpSteps;
  stepCounter    = temp.stepCounter;
  buttonHold     = temp.buttonHold;
  health         = temp.health;
  stamina        = temp.stamina;
  stateAI        = temp.stateAI;

}

Controllable::Controllable (int xx, int yy, int ww, int hh, SDL_Surface *i)
:GameObject()
{

  x = xx;
  y = yy;
  w = ww;
  h = hh;
  animationFrame = 0;

  jumpSteps = 10;
  stepCounter = 0;
  buttonHold = 0;

  colOffX = 0;
  colOffY = 0;

  xSpeed = 0;
  ySpeed = 0;

  type = 0;
  state = JumpingRight;
  stateAI = Idle;
  inAir = 1;
  climbing = 0;
  direction = 0;
  
  collisionRect.x = x;
  collisionRect.y = y;
  collisionRect.w = w;
  collisionRect.h = h;

  posRect.x = x;
  posRect.y = y;
  posRect.w = w;
  posRect.h = h;

  clippingRect.x = 0;
  clippingRect.y = 0;
  clippingRect.w = w;
  clippingRect.h = h;

  imageSurface = i;

}

void Controllable::setState(int s){
  state = s;
}

void Controllable::setSpeed( float xs, float ys){
  xSpeed = xs;
  ySpeed = ys;
}

void Controllable::control_down()
{
    if(climbing)
    {
      ySpeed = +12;
      climbing = 1;
    }
}

void Controllable::control_right()
{
   int tSpeed;
   if(running) tSpeed=14;
   else tSpeed = 11;

   if(xSpeed<tSpeed)
      setSpeed(xSpeed+3,ySpeed);
      direction=0;
   if(climbing) climbing = 0;
}
void Controllable::control_left()
{
    int tSpeed;
    if(running) tSpeed=-14;
    else tSpeed = -11;

    if(xSpeed>tSpeed)
      setSpeed(xSpeed-3,ySpeed);
    direction = 1;
    if(climbing) 
      setState(Idle);
      climbing = 0;
}

void Controllable::control_jump()
{
    if(((!inAir) || buttonHold) && (!climbing))
    {
      ySpeed = -16;
      buttonHold = 1;
      stepCounter++;
      if(stepCounter == jumpSteps)
      {
        buttonHold = 0;
        stepCounter = 0;
      }
    }
    if(climbing)
    {
      climbing = 2;
      ySpeed = -12;
    }
}

void Controllable::step()
{
  /*update state*/
  /*if (y > 300 && inAir )
  {
    inAir = 0;
    ySpeed = 0 ;
  }
  if(y < 300)
  {
    inAir = 1;
  }*/
  /*Update speed*/
  
  if(!running) stamina++;
  if(stamina>=100) stamina=100;
  else if(stamina<=0) stamina = 0;

  if(inAir)
  { 
    if(!climbing) 
      ySpeed = ySpeed + world->settings.gravity; 
    if(xSpeed>0)
    {
      setState(JumpingRight);        
    }
    else if(xSpeed<0)
    { 
      setState(JumpingLeft);
    }
    else
      if(direction) setState(JumpingLeft);
      else setState(JumpingRight);
    if(ySpeed>50) ySpeed = 50; //terminal velocity;
    else if(ySpeed<-50) ySpeed = -50;
  }
  else
  {
    if(xSpeed>0.0001)
      if(!running)
        setState(WalkingRight);
      else 
        setState(RunningRight);
    else if(xSpeed<-0.0001)
      if(!running)
        setState(WalkingLeft);
      else 
        setState(RunningLeft);
    else
      setState(Idle);
  }
  if(inAir)
  {
    if(climbing)
    {
      if(xSpeed>1)
        setSpeed(xSpeed,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed,ySpeed);
      else
        setSpeed(0,ySpeed);
    }
    else
    {
      if(xSpeed>1)
        setSpeed(xSpeed-0.6,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+0.6,ySpeed);
      else
        setSpeed(0,ySpeed);
    }
  }
  else  
  {  
    if(xSpeed>1)
      setSpeed(xSpeed-1.4,ySpeed);
    else if(xSpeed<-1)
      setSpeed(xSpeed+1.4,ySpeed);
    else
      setSpeed(0,ySpeed);
  }
  if(climbing)
  {
     setState(Climbing);
     if(ySpeed>3)
      setSpeed(xSpeed,ySpeed-5);
    else if(ySpeed<-3)
      setSpeed(xSpeed,ySpeed+5);
    else
      setSpeed(xSpeed,0);
  }
  /*update position*/
  x = x + xSpeed;
  y = y + ySpeed;

  /*posRect.x = x;*/
  /*posRect.y = y;*/

  collisionRect.x = x + colOffX;
  collisionRect.y = y + colOffY;


}

int Controllable::checkCollision(GameObject *b)
{

  float leftA, leftB;
  float rightA, rightB;
  float topA, topB;
  float bottomA, bottomB;

  leftA = collisionRect.x;
  rightA = collisionRect.x+collisionRect.w;
  topA = collisionRect.y;
  bottomA = collisionRect.y + collisionRect.h;

  leftB = b->collisionRect.x;
  rightB =b->collisionRect.x+b->collisionRect.w;
  topB = b->collisionRect.y;
  bottomB = b->collisionRect.y + b->collisionRect.h;
  //ustten
  if(((((bottomA+ySpeed > topB) && (topA < topB)) && (bottomA-topB <= colOffY)) || ((bottomA<=topB) && (bottomA+ySpeed>bottomB)))){
    if(rightA >= leftB && leftA <= rightB)
    {
      return 1;
    }
  }
  //alttan
  if((topA+ySpeed < bottomB) && (bottomA > bottomB) && (bottomB-topA <= colOffY)){
    if(rightA >= leftB && leftA <= rightB)
    {
      return 4;
    }
  }
   //soldan
  if((rightA+xSpeed >= leftB) && (leftA < leftB)){
    if((bottomA > topB) && (topA < bottomB))
    { 
      if(bottomA-topB<=(collisionRect.h/2.0)) 
        return 30;
      else  
        return 3;
    } 
  }
  //sagdan
  if((leftA+xSpeed <= rightB) && (rightA > rightB)){
    if((bottomA > topB) && (topA < bottomB))
    { 
      if(bottomA-topB<=(collisionRect.h/2.0)) 
        return 20;
      else  
        return 2;
    }
  }
  //ustten
  if((bottomA+1 >= topB) && (topA <= topB)){
    if(rightA > leftB && leftA < rightB){
      return 1;
    }
  }

  return 0;

} 

void GameObject::animate()
{
   clippingRect.x = (clippingRect.x + w) % imageSurface->w;
}

void Controllable::animate()
{
  if(climbing && ((!ySpeed) && (!xSpeed))) 
  {
    clippingRect.y = h*state;
  }
  else
  {
    animationFrame++;
    clippingRect.y = h*state;
    /*if(count%3!=0)*/
    if(animationFrame%3!=0)
      clippingRect.x = (clippingRect.x + w) % imageSurface->w;
  }
}

void GameObject::draw(){

  SDL_Rect temp;

  temp = collisionRect;

  temp.x = temp.x - world->cam->x;
  temp.y = temp.y - world->cam->y;


  posRect.x = x - world->cam->x ;
  posRect.y = y - world->cam->y ;

   
  /*SDL_FillRect(screen, &temp,0xAA0000);*/
  SDL_BlitSurface(imageSurface,&(clippingRect),screen,&(posRect));
}
 


GameObject::GameObject()
{

}
GameObject::GameObject(int x, int y, int w, int h, SDL_Surface *i,int t)
{
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  this->animationFrame = 0;

  this->colOffX = 0;
  this->colOffY = 0;

  this->type = 2000+t;

  this->collisionRect.x = this->x;
  this->collisionRect.y = this->y;
  this->collisionRect.w = this->w;
  this->collisionRect.h = this->h;

  this->posRect.x = this->x;
  this->posRect.y = this->y;
  this->posRect.w = this->w;
  this->posRect.h = this->h;

  this->clippingRect.x = 0;
  this->clippingRect.y = 0;
  this->clippingRect.w = this->w;
  this->clippingRect.h = this->h;

  this->imageSurface = i;

}





