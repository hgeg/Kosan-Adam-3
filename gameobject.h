#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//struct _Camera;
//typedef Camera _Camera;



enum AnimationStates 
{
  AnimIdle = 0,
  AnimWalkingRight = 1,
  AnimWalkingLeft = 2,
  AnimJumpingRight = 3,
  AnimJumpingLeft = 4,
  AnimClimbing = 5,
  AnimRunningLeft = 6,
  AnimRunningRight = 7, 
  AnimLeaningRight = 8,
  AnimLeaningLeft = 9 
};

enum ObjectStates 
{
  InAir = 0,
  Walking = 1,
  Running = 2,
  Climbing = 3,
  Idle = 4
};

class GameWorldC;

class GameObject{
  public:
  float x,y;
  int w,h;
  int type;
  int animationFrame;
  int sideColl;

  int colOffX;
  int colOffY;
  int state;
  int objState;

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
  int collision(GameObject *b);
  int  checkCollision(GameObject *b);
  int checkBottom(GameObject *b);
  int checkBottomPlus(GameObject *b);
  int checkTop(GameObject *b);
  int checkLeft(GameObject *b);
  int checkRight(GameObject *b);
  void  resolveCollision(GameObject * obj);
  void  step2();
  void  step3();

  void control_down();
  void control_right();
  void control_left();
  void control_jump();
  void control_jump2();

  Controllable();
  Controllable(const Controllable& temp);
  Controllable(int x, int y, int w, int h, SDL_Surface *i);
};


#include "camera.h"


#endif
