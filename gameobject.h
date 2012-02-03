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


#include "camera.h"


#endif
