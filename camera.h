#ifndef CAMERA_H
#define CAMERA_H



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


#endif
