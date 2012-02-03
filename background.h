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


#endif
