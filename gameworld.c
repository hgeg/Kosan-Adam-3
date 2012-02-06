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

static SDL_Surface                  textures[20];

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
  }
  if(keystates[SDLK_LSHIFT])
  {
    if(player->stamina>0) 
      player->running = 1;
  }else player->running = 0;
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
        if(!(rand()%6)) a->control_jump();
        a->control_right();
        a->control_right();
  }
  if(a->stateAI==AnimWalkingLeft)
  {
      if(!(rand()%20))
        a->stateAI = AnimIdle;
      else
        a->control_left();
        if(!(rand()%6)) a->control_jump();
        a->control_left();
        a->control_left();
  }
}


void GameWorldC::worldStep()
{
  int i;
  handleInput();
  /*handleCollision(player);*/
  printf ( "y %f\n",player->x );
  player->step2();
  printf ( "y %f\n",player->x );
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
    handleAI(&(dynamicObjects[i]));
    handleCollision(&(dynamicObjects[i]));
    dynamicObjects[i].step();
    dynamicObjects[i].draw();
    dynamicObjects[i].animate();
  }
  player->draw();
  player->animate();
}


GameWorldC::GameWorldC(char *map)
{
  FILE *mapFile;
  int object_id;
  char path[80];
  char line[100];
  int object,index,x=0,y=0;
  char del;
  SDL_Surface *tempSurface;
  SDL_Surface *tempSurfaceOpt;

  SDL_Surface *kosanText ;
  kosanText = SDL_DisplayFormat(IMG_Load("./res/gfx/kosan.gif"));
  dynamicCount = 0;
  staticCount = 0;

  srand(time(NULL));

  mapFile = fopen(map, "r");
  /*if(mapFile == NULL)*/
    /*return 1; //dosya okuma hatasi*/

  printf("Reading map file: %s\n",map);
  sscanf(fgets(line, 100, mapFile),"%d %s %c",&object_id,path,&del);
  while((char)del != '!')
  {
    tempSurface = (IMG_Load(path));
    tempSurfaceOpt = SDL_DisplayFormat(tempSurface);
    textures[object_id] = *tempSurfaceOpt;   
    sscanf(fgets(line, 100, mapFile),"%d %s %c",&object_id,path,&del);
  }

  object = fgetc(mapFile);
  while(object != EOF)
  {
    /*printf ( "%d\n",staticCount );*/
    object = fgetc(mapFile);
    if(object<58 && object>48 )
    {
      index = object-48;
      
      /*printf ( "%c %d\n",object,object );*/
      if(index==5)
        staticObjects[staticCount] = GameObject(64*x,64*y-32,64,64,&textures[index],index);
      else if(index==4)
        staticObjects[staticCount] = GameObject(64*x,64*y-16,64,64,&textures[index],index);
      else
        staticObjects[staticCount] = GameObject(64*x,64*y,64,64,&textures[index],index);
      if(index==3)
        staticObjects[staticCount].setColRect(16,48,32,8);

      staticObjects[staticCount].world = this;
      staticCount++;
    }
    else if((char)object == '|')
    {
      x=0;
      y++;
    }
    else if((char)object == '@')
    {
      player = new Controllable(64*x,64*y,64,64,kosanText);
      cam = new CameraC( player, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,380,380,270,270);
      player->setColRect(22,14,20,50);
    }
    else if((char)object == 'U')
    {
      printf ( "%d\n",dynamicCount );
      this->dynamicObjects[dynamicCount]= Controllable(64*x,64*y,64,64,kosanText);
      this->dynamicObjects[dynamicCount].setColRect(22,14,20,50);
      this->dynamicObjects[dynamicCount].world = this;
      dynamicCount++;
    }
    else if((char)object == '*')
    {
      break;
    }
    x++;
  }
  fclose(mapFile);

  running = 1;
  addBackground(500,500,&textures[2],2,-1,-1);
  addBackground(600,500,&textures[6],1,-1,120);
  addBackground(499,500,&textures[7],0,-1,150);
  player->world = this;
  settings.gravity = 3;

}

