#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "surfacehandler.h"
#include "gameobject.h"
#include "camera.h"
#include "gamesettings.h"
#include "gk.h"
#include "background.h"
#include "gameworld.h"
#include "math.h"

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

GameObject::GameObject(const GameObject& temp)
{
  climable = temp.climable;
  x              = temp.x;
  y              = temp.y;
  w              = temp.w;
  h              = temp.h;
  __class__      = temp.__class__;
  type           = temp.type;
  animationFrame = temp.animationFrame;
  colOffX        = temp.colOffX;
  colOffY        = temp.colOffY;
  sideColl       = temp.sideColl;
  imageSurface   = temp.imageSurface;
  posRect        = temp.posRect;
  collisionRect  = temp.collisionRect;
  clippingRect   = temp.clippingRect;
  world          = temp.world;
  state          = temp.state;
  objState       = temp.objState;
}


Controllable::Controllable()
{

}
Controllable::Controllable(const Controllable& temp)
{
  climable = temp.climable;
  x              = temp.x;
  y              = temp.y;
  w              = temp.w;
  h              = temp.h;
  __class__      = temp.__class__;
  type           = temp.type;
  animationFrame = temp.animationFrame;
  colOffX        = temp.colOffX;
  colOffY        = temp.colOffY;
  sideColl       = temp.sideColl;
  imageSurface   = temp.imageSurface;
  posRect        = temp.posRect;
  collisionRect  = temp.collisionRect;
  clippingRect   = temp.clippingRect;
  world          = temp.world;
  xSpeed         = temp.xSpeed;
  ySpeed         = temp.ySpeed;
  inAir          = temp.inAir;
  climbing       = temp.climbing;
  direction      = temp.direction;
  jumpSteps      = temp.jumpSteps;
  stepCounter    = temp.stepCounter;
  buttonHold     = temp.buttonHold;
  health         = temp.health;
  stamina        = temp.stamina;
  stateAI        = temp.stateAI;
  objState       = temp.objState;

}

Controllable::Controllable (string path, GameWorldC * wrld)
:GameObject( path, wrld)
{
  climable = 0;
  inAir  = 0;
  climbing = 0;
  direction = 0;

  jumpSteps = 15;
  stepCounter = 0;
  buttonHold = 0;

  health = 100;
  stamina = 100;
  stateAI = AnimIdle;
}

Controllable::Controllable (int xx, int yy, int ww, int hh, SDL_Surface *i)
:GameObject()
{
  climable = 0;

  objState = InAir;

  x = xx;
  y = yy;
  w = ww;
  h = hh;
  animationFrame = 0;

  jumpSteps = 15;
  stepCounter = 0;
  buttonHold = 0;

  colOffX = 0;
  colOffY = 0;

  xSpeed = 1;
  ySpeed = 1;

  type = 0;
  state = AnimJumpingRight;
  stateAI = AnimIdle;
  inAir = 1;
  climbing = 0;
  direction = 0;
  sideColl = 0;
  
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
    if(objState==Climbing)
    {
      ySpeed = 0 + world->settings.yClimbingSpeed;
    }
}

void Controllable::control_right()
{
   float tSpeed;
   if(objState==Running) tSpeed= world->settings.xSpeedRunning;
   else tSpeed = world->settings.xSpeedWalking;

   if(sideColl==1) sideColl=0;
   
   if(objState==Climbing)
     xSpeed = world->settings.xSpeedWalking;
   
   if(xSpeed<tSpeed)
     setSpeed(xSpeed+world->settings.xAcceleration,ySpeed);
     direction=0;
}
void Controllable::control_left()
{
   float tSpeed;
   if(objState==Running)
   {
     tSpeed= 0 - world->settings.xSpeedRunning;
   }
   else tSpeed = 0 -world->settings.xSpeedWalking;
   
   if(sideColl==2) sideColl=0;
   
   if(objState==Climbing)
     xSpeed = -world->settings.xSpeedRunning;
   
   if(xSpeed>tSpeed)
     setSpeed(xSpeed-world->settings.xAcceleration,ySpeed);
     direction = 1;
}

void Controllable::control_jump2()
{
    if(((objState == Walking || objState == Running) || buttonHold))
    {
      buttonHold = 1;
      stepCounter++;
      if(stepCounter >= jumpSteps)
      {
        buttonHold = 0;
      }
      else ySpeed = - world->settings.yJumpSpeed;
    }
    if(objState == Climbing)
    {
      ySpeed = 0 - world->settings.yClimbingSpeed;
    }
}

void Controllable::resolveCollision(GameObject * obj)
{
  float speed;
  float xStep,yStep;
  speed = sqrt(xSpeed * xSpeed + ySpeed * ySpeed);
  xStep = xSpeed / speed;
  yStep = ySpeed / speed;

  x = x - xStep;
  y = y - yStep;
  collisionRect.x = x + colOffX;
  collisionRect.y = y + colOffY;

  while(collision(obj))
  {
    x = x - xStep;
    y = y - yStep;
    collisionRect.x = x + colOffX;
    collisionRect.y = y + colOffY;
  }

}
void Controllable::step3()
{
  x = x + xSpeed;
  y = y + ySpeed;

  collisionRect.x = x + colOffX;
  collisionRect.y = y + colOffY;

  for (int i = 0; i < world->staticCount; i++) 
  {
    
    if(abs(x - world->staticObjects[i].x) + abs(y - world->staticObjects[i].y) > 300 || world->staticObjects[i].__class__ == 2)
    {
      continue;
    }

    if(checkBottom(&world->staticObjects[i]))
    {
      switch(world->staticObjects[i].climable)
      {
        case 1: //climb
        {
          if(objState==Climbing)
          {
            if(x - world->staticObjects[i].x>3) xSpeed = -5;
            else if(x - world->staticObjects[i].x<-3) xSpeed = 5;
            else 
            {
              xSpeed = 0;
              x = world->staticObjects[i].x;
            }
          }
          if(objState == InAir)
            objState = Climbing;
          break;
        }
        default:
        {
          while(checkBottom(&world->staticObjects[i]))
          {
            ySpeed = 0;
            if(objState!=Running) objState = Walking;
            y--;
            collisionRect.y = y + colOffY;
          }
          break;
        }
      }
    }
    else if(checkTop(&world->staticObjects[i]))
    {
      switch(world->staticObjects[i].climable)
      {
        case 1: //climb
        {
          break;
        }
        default:
        {
          while(checkTop(&world->staticObjects[i]))
          {
            ySpeed = 0;
            buttonHold = 0;
            y++;
            collisionRect.y = y + colOffY;
          }
          break;
        }
      }
    }
    else if(checkLeft(&world->staticObjects[i]))
    {
      switch(world->staticObjects[i].climable)
      {
        case 1: //climb
        {
          if(objState == InAir)
            objState = Climbing;
          break;
        }
        default:
        {
          while(checkLeft(&world->staticObjects[i]))
          {
            xSpeed = 0;
            sideColl = 1;
            x++;
            collisionRect.x = x + colOffX;
          }
          break;
        }
      }
    }
    else if(checkRight(&world->staticObjects[i]))
    {
      switch(world->staticObjects[i].climable)
      {
        case 1: //climb
        {
          if(objState == InAir)
            objState = Climbing;
          break;
        }
        default:
        {
          while(checkRight(&world->staticObjects[i]))
          {
            xSpeed = 0;
            sideColl = 2;
            x--;
            collisionRect.x = x + colOffX;
          }
          break;
        }
      }
    }
  }

  objState  = InAir;
  for (int i = 0; i < world->staticCount; i++) 
  {
    if(checkBottomPlus(&world->staticObjects[i]))
    {
      if(world->staticObjects[i].__class__ == 2)
      {
        Interactable * temp = (Interactable * )&world->staticObjects[i];
        temp->doWhenCollision(this);
        break;
      }
      switch(world->staticObjects[i].climable)
      {
        case 1: //climb
          {
            objState = Climbing;
            break;
          }
        default:
          {

            if(xSpeed>7 || xSpeed<-7) objState = Running;
            else objState = Walking;
            break;
          }
      }

    }
  }

  switch(objState)
  {
    case InAir:
    {
      //animation
      if(xSpeed>0)
      {
        setState(AnimJumpingRight);        
      }
      else if(xSpeed<0)
      { 
        setState(AnimJumpingLeft);
      }
      else
        if(direction) setState(AnimJumpingLeft);
        else setState(AnimJumpingRight);


      //ySpeed
      ySpeed = ySpeed + world->settings.gravity; 
      if(ySpeed>world->settings.ySpeedTerminal) ySpeed = world->settings.ySpeedTerminal; //terminal velocity;
      else if(ySpeed<0-world->settings.ySpeedTerminal - 30) ySpeed = 0-world->settings.ySpeedTerminal -30;

      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed-world->settings.airFriction,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+world->settings.airFriction,ySpeed);
      else
        setSpeed(0,ySpeed);

      break;
    }
    case Walking:
    {
      //animation
      if(xSpeed>0.0001)
          setState(AnimWalkingRight);
      else if(xSpeed<-0.0001)
          setState(AnimWalkingLeft);
      else
      {
        if(sideColl==2)
          setState(AnimLeaningRight);
        else if(sideColl==1)
          setState(AnimLeaningLeft);
        else
          setState(AnimIdle);
      }

      //ySpeed
      ySpeed = 0;
      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed-world->settings.groundFriction,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+world->settings.groundFriction,ySpeed);
      else
        setSpeed(0,ySpeed);
      break;
    }
    case Running:
    {
      //animation
      if(xSpeed>0.0001)
          setState(AnimRunningRight);
      else if(xSpeed<-0.0001)
          setState(AnimRunningLeft);
      else
      {
        if(sideColl==2)
          setState(AnimLeaningRight);
        else if(sideColl==1)
          setState(AnimLeaningLeft);
        else
          setState(AnimIdle);
      }

      //ySpeed
      ySpeed = 0;
      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed-world->settings.groundFriction,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+world->settings.groundFriction,ySpeed);
      else
        setSpeed(0,ySpeed);

      break;
    }
    case Climbing:
    {
      //animation
      setState(AnimClimbing);
      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed,0);
      else if(xSpeed<-1)
        setSpeed(xSpeed,0);
      else
        setSpeed(0,0);      break;
    }
  }

}

void Controllable::step2()
{
  x = x + xSpeed;
  y = y + ySpeed;

  collisionRect.x = x + colOffX;
  collisionRect.y = y + colOffY;

  for (int i = 0; i < world->staticCount; i++) 
  {
    if(collision(&world->staticObjects[i]))
    {
      switch(world->staticObjects[i].climable)
      {
        case 1: //climb
        {
          if(objState == InAir)
            objState = Climbing;
          break;
        }
        default:
        {
          resolveCollision(&world->staticObjects[i]);
          break;
        }
      }

    }
  }

  if(objState != Climbing)
  {
    objState  = InAir;
    for (int i = 0; i < world->staticCount; i++) 
    {
      if(checkBottom(&world->staticObjects[i]))
      {
        switch(world->staticObjects[i].climable)
        {
          case 1: //climb
            {
              break;
            }
          default:
            {
              objState = Walking;
              break;
            }
        }

      }
    }

  }



  switch(objState)
  {
    case InAir:
    {
      //animation
      if(xSpeed>0)
      {
        setState(AnimJumpingRight);        
      }
      else if(xSpeed<0)
      { 
        setState(AnimJumpingLeft);
      }
      else
        if(direction) setState(AnimJumpingLeft);
        else setState(AnimJumpingRight);


      //ySpeed
      ySpeed = ySpeed + world->settings.gravity; 
      if(ySpeed>50) ySpeed = 50; //terminal velocity;
      else if(ySpeed<-50) ySpeed = -50;

      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed-0.6,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+0.6,ySpeed);
      else
        setSpeed(0,ySpeed);

      break;
    }
    case Walking:
    {
      //animation
      if(xSpeed>0.0001)
          setState(AnimWalkingRight);
      else if(xSpeed<-0.0001)
          setState(AnimWalkingLeft);
      else
        setState(AnimIdle);


      //ySpeed
      ySpeed = 0;
      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed-1.4,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+1.4,ySpeed);
      else
        setSpeed(0,ySpeed);


      break;
    }
    case Running:
    {
      //animation
      if(xSpeed>0.0001)
          setState(AnimRunningRight);
      else if(xSpeed<-0.0001)
          setState(AnimRunningLeft);
      else
        setState(AnimIdle);

      //ySpeed
      ySpeed = 0;
      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed-1.4,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed+1.4,ySpeed);
      else
        setSpeed(0,ySpeed);

      break;
    }
    case Climbing:
    {
      //animation
      setState(AnimClimbing);
      //xSpeed
      if(xSpeed>1)
        setSpeed(xSpeed,ySpeed);
      else if(xSpeed<-1)
        setSpeed(xSpeed,ySpeed);
      else
        setSpeed(0,ySpeed);
      break;
    }
  }


}
void Controllable::step(){}
/*{*/
  /*[>update state<]*/
  /*if (y > 300 && inAir )*/
  /*{*/
    /*inAir = 0;*/
    /*ySpeed = 0 ;*/
  /*}*/
  /*if(y < 300)*/
  /*{*/
    /*inAir = 1;*/
  /*}*/
  /*[>Update speed<]*/
  
  /*if(inAir)*/
  /*{ */
    /*if(!climbing) */
      /*ySpeed = ySpeed + world->settings.gravity; */
    /*if(xSpeed>0)*/
    /*{*/
      /*setState(AnimJumpingRight);        */
    /*}*/
    /*else if(xSpeed<0)*/
    /*{ */
      /*setState(AnimJumpingLeft);*/
    /*}*/
    /*else*/
      /*if(direction) setState(AnimJumpingLeft);*/
      /*else setState(AnimJumpingRight);*/
    /*if(ySpeed>50) ySpeed = 50; //terminal velocity;*/
    /*else if(ySpeed<-50) ySpeed = -50;*/
  /*}*/
  /*else*/
  /*{*/
    /*if(xSpeed>0.0001)*/
      /*if(!running)*/
        /*setState(AnimWalkingRight);*/
      /*else */
        /*setState(AnimRunningRight);*/
    /*else if(xSpeed<-0.0001)*/
      /*if(!running)*/
        /*setState(AnimWalkingLeft);*/
      /*else */
        /*setState(AnimRunningLeft);*/
    /*else*/
      /*setState(AnimIdle);*/
  /*}*/
  /*if(inAir)*/
  /*{*/
    /*if(climbing)*/
    /*{*/
      /*if(xSpeed>1)*/
        /*setSpeed(xSpeed,ySpeed);*/
      /*else if(xSpeed<-1)*/
        /*setSpeed(xSpeed,ySpeed);*/
      /*else*/
        /*setSpeed(0,ySpeed);*/
    /*}*/
    /*else*/
    /*{*/
      /*if(xSpeed>1)*/
        /*setSpeed(xSpeed-0.6,ySpeed);*/
      /*else if(xSpeed<-1)*/
        /*setSpeed(xSpeed+0.6,ySpeed);*/
      /*else*/
        /*setSpeed(0,ySpeed);*/
    /*}*/
  /*}*/
  /*else  */
  /*{  */
    /*if(xSpeed>1)*/
      /*setSpeed(xSpeed-1.4,ySpeed);*/
    /*else if(xSpeed<-1)*/
      /*setSpeed(xSpeed+1.4,ySpeed);*/
    /*else*/
      /*setSpeed(0,ySpeed);*/
  /*}*/
  /*if(climbing)*/
  /*{*/
     /*setState(AnimClimbing);*/
     /*if(ySpeed>3)*/
      /*setSpeed(xSpeed,ySpeed-5);*/
    /*else if(ySpeed<-3)*/
      /*setSpeed(xSpeed,ySpeed+5);*/
    /*else*/
      /*setSpeed(xSpeed,0);*/
  /*}*/
  /*[>update position<]*/
  /*x = x + xSpeed;*/
  /*y = y + ySpeed;*/

  /*[>posRect.x = x;<]*/
  /*[>posRect.y = y;<]*/

  /*collisionRect.x = x + colOffX;*/
  /*collisionRect.y = y + colOffY;*/


/*}*/

int Controllable::checkLeft(GameObject *b)
{

  float pointX, pointY;
  float  leftB;
  float  rightB;
  float  topB;
  float  bottomB;

  pointX = collisionRect.x - 3 ;
  pointY = collisionRect.y + collisionRect.h /2;

  leftB = b->collisionRect.x;
  rightB =b->collisionRect.x+b->collisionRect.w;
  topB = b->collisionRect.y;
  bottomB = b->collisionRect.y + b->collisionRect.h;

  if (pointY < topB) return 0;
  if (pointY > bottomB) return 0;

  if (pointX < leftB) return 0;
  if (pointX > rightB) return 0;

  return 1;
} 

int Controllable::checkRight(GameObject *b)
{

  float pointX, pointY;
  float  leftB;
  float  rightB;
  float  topB;
  float  bottomB;

  pointX = collisionRect.x + collisionRect.w + 3;
  pointY = collisionRect.y + collisionRect.h /2;

  leftB = b->collisionRect.x;
  rightB =b->collisionRect.x+b->collisionRect.w;
  topB = b->collisionRect.y;
  bottomB = b->collisionRect.y + b->collisionRect.h;

  if (pointY < topB) return 0;
  if (pointY > bottomB) return 0;

  if (pointX < leftB) return 0;
  if (pointX > rightB) return 0;

  return 1;
} 

int Controllable::checkBottomPlus(GameObject *b)
{

  float pointX, pointY;
  float  leftB;
  float  rightB;
  float  topB;
  float  bottomB;

  pointX = collisionRect.x + collisionRect.w /2;
  pointY = collisionRect.y + collisionRect.h + 1;

  leftB = b->collisionRect.x;
  rightB =b->collisionRect.x+b->collisionRect.w;
  topB = b->collisionRect.y;
  bottomB = b->collisionRect.y + b->collisionRect.h;

  /*cout << endl << endl;*/
  /*cout << "pointX "<< pointX << endl;*/
  /*cout << "pointY "<< pointY << endl;*/
  /*cout << "leftB "<< leftB << endl;*/
  /*cout << "rightB "<< rightB << endl;*/
  /*cout << "topB "<< topB << endl;*/
  /*cout << "bottomB "<< bottomB << endl;*/
  /*cout << endl << endl;*/

  if (pointY < topB) return 0;
  if (pointY > bottomB) return 0;

  if (pointX < leftB) return 0;
  if (pointX > rightB) return 0;

  return 1;
} 
int Controllable::checkBottom(GameObject *b)
{

  float pointX, pointY;
  float  leftB;
  float  rightB;
  float  topB;
  float  bottomB;

  pointX = collisionRect.x + collisionRect.w /2;
  pointY = collisionRect.y + collisionRect.h;

  leftB = b->collisionRect.x;
  rightB =b->collisionRect.x+b->collisionRect.w;
  topB = b->collisionRect.y;
  bottomB = b->collisionRect.y + b->collisionRect.h;

  if (pointY < topB) return 0;
  if (pointY > bottomB) return 0;

  if (pointX < leftB) return 0;
  if (pointX > rightB) return 0;

  return 1;
} 

int Controllable::checkTop(GameObject *b)
{

  float pointX, pointY;
  float  leftB;
  float  rightB;
  float  topB;
  float  bottomB;

  pointX = collisionRect.x + collisionRect.w /2;
  pointY = collisionRect.y  ;

  leftB = b->collisionRect.x;
  rightB =b->collisionRect.x+b->collisionRect.w;
  topB = b->collisionRect.y;
  bottomB = b->collisionRect.y + b->collisionRect.h;

  if (pointY < topB) return 0;
  if (pointY > bottomB) return 0;

  if (pointX < leftB) return 0;
  if (pointX > rightB) return 0;

  return 1;
} 

int Controllable::collision(GameObject *b)
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

  if (bottomA < topB) return 0;
  if (topA > bottomB) return 0;

  if (rightA < leftB) return 0;
  if (leftA > rightB) return 0;

  /*printf ( "A: %f %f %f %f\n",leftA, rightA, topA, bottomA );*/
  /*printf ( "B: %f %f %f %f\n",leftB, rightB, topB, bottomB );*/


  return 1;
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
  if((objState!=Climbing) || ((objState==Climbing) && (xSpeed!=0 || ySpeed!=0))) animationFrame++;
  clippingRect.y = h*state;
  if(animationFrame%4==0)
  {
    clippingRect.x = (clippingRect.x + w) % imageSurface->w;
    animationFrame++;
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

GameObject::GameObject(string path, GameWorldC * wrld)
{
  climable = 0;
  world = wrld;
  string word;
  string value;
  x = 0;
  y = 0;
  type = 0;
  animationFrame = 0;
  sideColl = 0;
  colOffX = 0;
  colOffY = 0;
  state = 0;
  objState = 0;

 

  ifstream objfile(path.c_str());

  if(objfile.is_open())
  {
    while(objfile.good())
    {
      objfile >> word;
      objfile >> value;
      objfile >> value;
      if(word == "type"   )
      {
      }
      else if(word == "player" )
      {

      }
      else if(word == "climbable" )
      {
        climable = atoi(value.c_str());
      }
      else if(word == "texture")
      {
        imageSurface = world->surfaces.getImage(value);
      }
      else if(word == "w"      )
      {
        w = atoi(value.c_str());
      }
      else if(word == "h"      )
      {
        h = atoi(value.c_str());
      }
      else if(word == "coloffX")
      {
        colOffX = atoi(value.c_str());
      }
      else if(word == "coloffY")
      {
        colOffY = atoi(value.c_str());
      }
      else if(word == "colW"   )
      {
        collisionRect.w = atoi(value.c_str());
      }
      else if(word == "colH"   )
      {
        collisionRect.h = atoi(value.c_str());
      }

      /*cout << line << endl;*/
    }
    this->clippingRect.x = 0;
    this->clippingRect.y = 0;
    this->clippingRect.w = this->w;
    this->clippingRect.h = this->h;
    objfile.close();
  }
  else
  {
    
  }



}
GameObject::GameObject(int x, int y, int w, int h, SDL_Surface *i,int t)
{
  climable = 0;
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

GameObject * readObjectFile(string path, GameWorldC * wrld)
{
  string word;
  string value;

  ifstream objfile(path.c_str());

  if(objfile.is_open())
  {
    while(objfile.good())
    {
      objfile >> word;
      objfile >> value;
      objfile >> value;
      if(word == "type"   )
      {
        objfile.close();
        if(value == "Controllable")
        {
          Controllable * newObj;
          newObj = new Controllable(path,wrld);
          newObj->xSpeed = 1;
          newObj->ySpeed = 1;
          newObj->__class__ = 1;
          return newObj;
        }
        else if(value == "Basic")
        {
          GameObject * newObj;
          newObj = new GameObject(path,wrld);
          newObj->__class__ = 0;
          return newObj;
        }
        else if(value == "Interactable")
        {
          Interactable * newObj;
          newObj = new Interactable(path,wrld);
          newObj->__class__ = 2;
          return newObj;
        }
      }
    }
  }
  return NULL;


}


Interactable::Interactable (string path, GameWorldC * wrld)
:GameObject( path, wrld)
{
  __class__ = 2;
}

void Interactable::doWhenCollision (Controllable * other)
{
  other->ySpeed = -30;
}



