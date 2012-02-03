#ifndef CAMERA_H
#define CAMERA_H


typedef struct{
  int x,y;
  int w,h;
  int rBoundary, lBoundary;
  int uBoundary, dBoundary;
  DynamicObject * focus;
}Camera;

class CameraC{
  public:
  int x,y;
  int w,h;
  int rBoundary, lBoundary;
  int uBoundary, dBoundary;
  GameObject * focus;

  void moveCamera();

  CameraC(Controllable * focus, int x, int y,int w, int h,
		int lBoundary, int rBoundary, int uBoundary, int dBoundary);
};


void cameraInit(Camera * cam, DynamicObject * focus, int x, int y,int w, int h,
		int lBoundary, int rBoundary, int uBoundary, int dBoundary);

void moveCamera(Camera * cam);
#endif
