#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

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
#include "surfacehandler.h"
#include "gameworld.h"


GameWorldC::GameWorldC (Controllable * player, CameraC * cam)
{
  settings.gravity = 3 ;
  this->player = player;
  this->cam = cam;
  this->staticCount = 0;
  this->dynamicCount = 0;
  this->running = 1;
}


void GameWorldC::addBackground(int w, int h, SDL_Surface * i, int l, int x, int y){
  BackgroundC bg(w,h,i,l,x,y);
  this->bg[l] = bg; 
}


void GameWorldC::addStaticObject(GameObject * object)
{
  this->staticObjects[staticCount] = *object;
  this->staticCount++;
}


void GameWorldC::handleInput() 
{
  static SDL_Event event;
  Uint8 * keystates = SDL_GetKeyState(NULL);
  if(keystates[SDLK_UP])
  {
    player->control_jump2();
  }
  if(keystates[SDLK_DOWN])
  {
    player->control_down();
  }
  if(keystates[SDLK_r])
  {
    player->setPos(470 , 10);
    player->setSpeed(0,0);
        running = 0;
  }
  if(keystates[SDLK_LSHIFT])
  {
    if(player->stamina>0) 
      if(player->objState==Walking) player->objState = Running;
  }else if(player->objState!=Climbing) player->objState = Walking;
  if(keystates[SDLK_LEFT])
  {
    player->control_left();
  }
  if(keystates[SDLK_RIGHT])
  {
    player->control_right();
  }
  while( SDL_PollEvent( &event ) ) {
    switch (event.type) {

      case SDL_KEYUP:
        {
          switch(event.key.keysym.sym )
          {
            case SDLK_UP:
              {
                player->buttonHold = 0;
                player->stepCounter = 0;
                break;
              }
            default:
            {
  

              break;
            }
          }
          break;
        }
      /*case SDL_KEYDOWN:*/
      /*{*/
      /*switch(event.key.keysym.sym )*/
      /*{*/
      /*case SDLK_RIGHT: */
      /*{*/
      /*a->posRect.x += 5;*/
      /*break;*/
      /*}*/
      /*case SDLK_LEFT: */
      /*{*/
      /*a->posRect.x -= 5;*/
      /*break;*/
      /*}*/

      /*}*/
      /*break;*/
      /*}*/
      case SDL_QUIT:
        /*case SDL_MOUSEMOTION:*/
        running = 0;
        break;
    }
  }
}


void GameWorldC::handleCollision(Controllable *a){
  int i,sum = 0,coll = 0;
  for(i=0;i<a->world->staticCount;i++){
    if((a->x- a->world->staticObjects[i].x)*(a->x-a->world->staticObjects[i].x)+(a->y-a->world->staticObjects[i].y)*(a->y-a->world->staticObjects[i].y)<8192)
    {
      coll = a->checkCollision(&a->world->staticObjects[i]);
      if(coll)
      { 
        if(a->world->staticObjects[i].type == 2003)
        {
            if(a->climbing)
            {
              a->setState(AnimClimbing);
              if(a->x - a->world->staticObjects[i].x>3) a->xSpeed = -5;
              else if(a->x - a->world->staticObjects[i].x<-3) a->xSpeed = 5;
              else 
              {
                a->xSpeed = 0;
                a->x = a->world->staticObjects[i].x;
              }
            }
            sum+=coll;
            if(!a->climbing) a->climbing = 1;
            a->inAir = 1;
        }
        else
        {
          if(coll==1)
          {
            if(a->inAir && (a->climbing!=2 || a->ySpeed>0))
            {
              a->ySpeed =0;
              a->inAir=0;
              a->y = a->world->staticObjects[i].y-a->collisionRect.h-a->colOffY;
            } 
            sum+=coll;
          }
          if(coll==2)
          {
            a->xSpeed=0;
            a->x = a->world->staticObjects[i].x+a->world->staticObjects[i].w-a->collisionRect.w;
          }
          if(coll==3)
          {
            a->xSpeed=0;
            a->x = a->world->staticObjects[i].x-a->w+a->colOffX-0.1;
          }
          if(coll==4)
          {
            if(a->inAir)
            {
              a->ySpeed = 0;
              a->y = a->world->staticObjects[i].y+a->world->staticObjects[i].w-a->colOffY;
              a->buttonHold = 0;
            }
            else
            {
              a->ySpeed = 0;  
            }
          }
          if(coll==20 || coll==30)
          {
            if(!a->inAir)
            {
              a->setSpeed(a->xSpeed,0);
              a->y = a->world->staticObjects[i].y-a->collisionRect.h-a->colOffY;
            }
          }
        }
      }
    }
  }
  if(!sum)
  {
     a->inAir=1;
     a->climbing=0;
  }
}


void handleAI(Controllable *a)
{
  if(a->stateAI==AnimIdle)
  {
    if(!(rand()%30))
    {  
      if(rand()%2)
        a->stateAI = AnimWalkingLeft;
      else
        a->stateAI = AnimWalkingRight;
    }
  }
  if(a->stateAI==AnimWalkingRight)
  {
      if(!(rand()%20))
        a->stateAI = AnimIdle;
      else
        a->control_right();
        if(!(rand()%6)) a->control_jump2();
        a->control_right();
        a->control_right();
  }
  if(a->stateAI==AnimWalkingLeft)
  {
      if(!(rand()%20))
        a->stateAI = AnimIdle;
      else
        a->control_left();
        if(!(rand()%6)) a->control_jump2();
        a->control_left();
        a->control_left();
  }
}

void GameWorldC::worldStep()
{
  int i;
  /*handleCollision(player);*/
  player->step3();
  cam->moveCamera();
  for(i=2;i>=0;i--)
  {  
    bg[i].drawBackground(cam);
  }
  for(i=0;i<staticCount;i++)
  {
    staticObjects[i].draw();
    staticObjects[i].animate();
  }
  for(i=0;i<dynamicCount;i++)
  {
    dynamicObjects[i].step3();
    handleAI(&(dynamicObjects[i]));
    dynamicObjects[i].draw();
    dynamicObjects[i].animate();
  }
  handleInput();
  player->draw();
  player->animate();
}


GameWorldC::GameWorldC(const char *map)
{
  string line;
  unsigned int x=0,y=0;
  unsigned char del;
  /*Controllable * elma = (Controllable *)readObjectFile("./res/objects/kosan", this);*/
  /*GameObject elma("./res/objects/kosan", this);*/
  /*SDL_Surface *tempSurface;*/
  /*SDL_Surface *tempSurfaceOpt;*/

  dynamicCount = 0;
  staticCount = 0;

  srand(time(NULL));

  ifstream loader(map);
  
  string sPath;
  unsigned char symbol;

  //background loader
  int layer,offX,offY,w,h;
  getline(loader,line);
  while(line!="")
  {
  stringstream ss (stringstream::in |stringstream::out);
    ss << line;
    ss >> layer >> sPath >> offX >> offY >> w >> h;
    /*cout << " layer: "<< layer << " path: " << sPath << " offset x: "  <<  offX << " offset y: " << offY << endl;*/
    addBackground(w,h,surfaces.getImage(sPath),layer,offX,offY);
    getline(loader,line);
  }
  
  string objPath = "./res/objects/";
   
  //object loader
  getline(loader,line);
  while(line!="")
  {
  stringstream ss (stringstream::in |stringstream::out);
    ss << line;
    ss >> symbol >> sPath;
    cout << line << endl;
    /*cout << "symbol " << symbol << endl;*/
    /*cout << "sPath " << objPath+sPath << endl;*/
    objects[symbol] = readObjectFile(objPath+sPath,this); 
    /*cout << symbol << " asdasdasd " << objects[symbol]->__class__ << endl;*/
    getline(loader,line);
  }
  while(loader.good())
  {
    getline(loader,line);
    for(x=0;x<line.length();x++)
    {
      /*cout << " asdasdasd " << objects['L']->w << endl;*/
       del = line[x];
       if(del==' ')
       {
         continue;
       }
       if(del=='@')
       {
        player = new Controllable();
        *player = *((Controllable *)objects[del]);
        player->x = x*64;
        player->y = y*64;
        cam = new CameraC( player, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,380,380,270,270);

        continue;
       }
       if(objects[del]->__class__ == 1)
       { 
         /*printf ( "%c hello\n", del );*/
         dynamicObjects[dynamicCount] = *((Controllable *)objects[del]);
         dynamicObjects[dynamicCount].x = x*64;
         dynamicObjects[dynamicCount].y = y*64;
         dynamicCount++;
       }
       if(objects[del]->__class__ == 2)
       { 
         /*printf ( "%c hello\n", del );*/
         staticObjects[staticCount] = *objects[del];
         staticObjects[staticCount].x = x*64;
         /*staticObjects[staticCount].y = y*64;*/
         staticObjects[staticCount].y = y*64+staticObjects[staticCount].colOffY;
         staticObjects[staticCount].collisionRect.x = x*64 + staticObjects[staticCount].colOffX;
         staticObjects[staticCount].collisionRect.y = y*64 + staticObjects[staticCount].colOffY;
         staticCount++;
       }
       else
       { 
         staticObjects[staticCount] = *objects[del];
         staticObjects[staticCount].x = x*64;
         staticObjects[staticCount].y = y*64+staticObjects[staticCount].colOffY;
         staticObjects[staticCount].collisionRect.x = x*64 + staticObjects[staticCount].colOffX;
         staticObjects[staticCount].collisionRect.y = y*64 + staticObjects[staticCount].colOffY;
         staticCount++;
         /*cout << staticCount << endl;*/
       }
    }
    x=0;
    y++;
  }
  loader.close();
  running = 1;
  player->world = this;
  settings.gravity = 1.4;
  settings.airFriction = 0.00;
  settings.groundFriction = 0.4;
  settings.ySpeedTerminal=  20;
  settings.yJumpSpeed = 8;
  settings.xSpeedRunning = 7.5;
  settings.xSpeedWalking = 5.0;
  settings.xAcceleration = 1.2;
  settings.yClimbingSpeed = 5.5;

}

