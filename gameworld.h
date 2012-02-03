#ifndef GAMEWORLD_H
#define GAMEWORLD_H

class GameWorldC
{
  public:
    GameObject staticObjects[300];
    Controllable dynamicObjects[200];
    Controllable * player;
    GameSettings settings;
    CameraC * cam;
    BackgroundC  bg[3];
    int staticCount;
    int running;
    int dynamicCount;


    void worldStep();

    void addStaticObject(GameObject * object);

    void addBackground(int w, int h, SDL_Surface * i, int l, int x, int y);

    void handleInput() ;

    void handleCollision(Controllable *a);

    int loadMap(char *map);

    GameWorldC(Controllable * player, CameraC * cam);
    GameWorldC(char *map);

};

typedef struct GameWorld_t
{
  StaticObject staticObjects[300];
  DynamicObject dynamicObjects[200];
  DynamicObject * player;
  GameSettings * settings;
  Camera * cam;
  Background  bg[3];
  int staticCount;
  int running;
  int dynamicCount;
}GameWorld;

void createWorld(GameWorld * a, DynamicObject * player, GameSettings * settings, Camera * cam);

void worldStep(GameWorld * a, int gameSpeed);

void addStaticObject(GameWorld * a, StaticObject * object);

void addBackground(GameWorld  *a, int w, int h, SDL_Surface * i, int l, int x, int y);

void handleInput(GameWorld * a) ;

void handleCollision(DynamicObject *a);

int loadMap(char *map, GameWorld * a);


#endif
