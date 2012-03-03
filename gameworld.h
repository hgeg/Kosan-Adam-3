#ifndef GAMEWORLD_H
#define GAMEWORLD_H


class GameWorldC
{
  public:
    GameObject staticObjects[300];
    Controllable dynamicObjects[200];
    GameObject *objects[256];
    Controllable * player;
    GameSettings settings;
    CameraC * cam;
    BackgroundC  bg[3];
    SurfaceHandler  surfaces;
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
    GameWorldC(const char *map);

};




#endif
