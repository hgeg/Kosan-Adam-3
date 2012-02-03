#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//struct _Camera;
//typedef Camera _Camera;



enum objectStates 
{
  Idle = 0,
  WalkingRight = 1,
  WalkingLeft = 2,
  JumpingRight = 3,
  JumpingLeft = 4,
  Climbing = 5,
  RunningLeft = 6,
  RunningRight = 7 
};

class GameWorldC;

class GameObject{
  public:
  float x,y;
  int w,h;
  int type;
  int animationFrame;

  int colOffX;
  int colOffY;
  int state;

  SDL_Surface *imageSurface;
  SDL_Rect posRect;
  SDL_Rect collisionRect;
  SDL_Rect clippingRect;
  //struct GameWorld_t * world;
  GameWorldC * world;
  int  checkCollision(GameObject *b);
  void  draw();
  virtual void  animate();
  virtual void  step();
  void setColRect(int x, int y, int w, int h);
  void setPos(float x, float y);
  GameObject();
  GameObject(int x, int y, int w, int h, SDL_Surface *i,int t);
};

class Controllable : public GameObject
{
  public:
  float xSpeed, ySpeed;
  int inAir;
  int climbing;
  int running;
  int direction;

  int jumpSteps;
  int stepCounter;
  int buttonHold;

  int health;
  int stamina;
  int stateAI;

  void animate();
  void step();
  void setSpeed( float xs, float ys);
  void setState(int s);
  int  checkCollision(GameObject *b);

  void control_down();
  void control_right();
  void control_left();
  void control_jump();

  Controllable();
  Controllable(const Controllable& temp);
  Controllable(int x, int y, int w, int h, SDL_Surface *i);
};

struct GameWorld_t;

typedef struct{
  float x,y;
  float xSpeed, ySpeed;
  int w,h;

  int state;
  int animationFrame;

  int inAir;
  int climbing;
  int running;
  int direction;
  int type;

  int jumpSteps;
  int stepCounter;
  int buttonHold;

  int colOffX;
  int colOffY;

  int health;
  int stamina;

  int stateAI;

  SDL_Surface *imageSurface;
  SDL_Rect posRect;
  SDL_Rect collisionRect;
  SDL_Rect clippingRect;
  struct GameWorld_t * world;

}DynamicObject;



typedef struct{
  float x,y;
  int w,h;
  int type;
  int animationFrame;

  int colOffX;
  int colOffY;

  SDL_Surface *imageSurface;
  SDL_Rect posRect;
  SDL_Rect collisionRect;
  SDL_Rect clippingRect;
  struct GameWorld_t * world;

}StaticObject;



#include "camera.h"

void animateD(DynamicObject *a, int gameSpeed);
void createD(DynamicObject *a, int x, int y, int w, int h, SDL_Surface *i);
void drawD(DynamicObject *a, Camera * cam, int gameSpeed);
void stepD(DynamicObject *a, int timePassed);
void setPosD(DynamicObject *a, float x, float y);
void setSpeedD(DynamicObject *a, float xs, float ys);
void setSizeD(DynamicObject *a, int w, int h);
void setStateD(DynamicObject *a, int s);
void setColRectD(DynamicObject *a, int x, int y, int w, int h);
int  checkCollisionDS(DynamicObject *a, StaticObject *b);
int  checkCollisionDD(DynamicObject *a, DynamicObject *b);

void animateS(StaticObject *a);
void createS(StaticObject *a, int x, int y, int w, int h, SDL_Surface *i,int t);
void drawS(StaticObject *a, Camera * cam);
void setPosS(StaticObject *a, float x, float y);
void setSizeS(StaticObject *a, int w, int h);
void setColRectS(StaticObject *a, int x, int y, int w, int h);


void control_down(DynamicObject * a);
void control_right(DynamicObject * a);
void control_left(DynamicObject * a);
void control_jump(DynamicObject * a);

#endif
