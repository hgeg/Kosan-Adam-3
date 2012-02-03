#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "gamesettings.h"
#include "gameobject.h"
#include "camera.h"
#include "gk.h"
#include "background.h"
#include "gameworld.h"

#define JUMPSTEPS 10

//static SDL_Rect                     tempSurface = {0,0,65,113};
//elma


SDL_Surface* screen   = NULL      ;

unsigned int                 SCREEN_BPP = 32;
unsigned int                 SCREEN_WIDTH = 800;
unsigned int                 SCREEN_HEIGHT = 600;
int                          SURF_TYPE = SDL_HWSURFACE | SDL_DOUBLEBUF;
static int                          gameSpeed = 10;

//harita yukleyici

static void init(void) {
  srand( time(NULL) );

  SDL_Init( SDL_INIT_EVERYTHING );
  TTF_Init();
  SDL_WM_SetCaption("Kosan Adam 3", NULL); 

  /*freopen( "log", "w", stdout ); */
  /*freopen( "log", "w", stderr );*/

  printf("SDL initiated\n"); 

screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SURF_TYPE );
}


int main(int argc, const char *argv[])
{
  int t0,t1;
  init();
  /*TTF_Font *font12 = TTF_OpenFont("./res/fonts/FreeSans.ttf",12);*/
  TTF_Font *font32 = TTF_OpenFont("./res/fonts/FreeSans.ttf",32);
  SDL_Color blue = {60,90,220,0};

  SDL_Surface *text;
  SDL_Rect txt = {300,30,0,0};
	Controllable armut;

  GameWorldC worldC("./res/maps/level1.kam");


  /*int cMap = loadMap("./res/maps/level1.kam", &world);*/
  /*printf("Map Created: %d\n",cMap);*/

  /*fclose(stdout);*/

  text = TTF_RenderText_Solid(font32, "Kosan Adam 3", blue);

  t0 = SDL_GetTicks();
  while(worldC.running) {

    /*clear_screen();*/
    SDL_FillRect(screen,NULL, 0x000000); 


    /*printf ( " aa a a%d\n", kosan.imageSurface->h);*/
    
    SDL_BlitSurface(text,NULL,screen,&txt);
    /*worldStep(&world,gameSpeed);*/
    worldC.worldStep();
    t1 = SDL_GetTicks();
    gameSpeed = t1 - t0;
    if(gameSpeed < (1000/30))
      SDL_Delay((1000 / 30) - gameSpeed);
    t0 = SDL_GetTicks();
    SDL_Flip(screen);
  } 
  return 0;
}
