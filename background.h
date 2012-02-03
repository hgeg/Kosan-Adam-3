#ifndef BACKGROUND_H
#define BACKGROUND_H


class BackgroundC{
  public:
    int w, h;
    int xOffset, yOffset;
    int layer;
    SDL_Surface *imageSurface;
    SDL_Rect posRect;
    SDL_Rect clippingRect;
    BackgroundC();
    BackgroundC(int w, int h, SDL_Surface * i, int l, int x, int y);
    void drawBackground(CameraC * cam);

};

typedef struct{
  int w, h;
  int xOffset, yOffset;
  int layer;
  SDL_Surface *imageSurface;
  SDL_Rect posRect;
  SDL_Rect clippingRect;
}Background;

void createBackground(Background * back,int w, int h, SDL_Surface * i, int l, int x, int y);
void drawBackground(Background * back, Camera * cam);

#endif
