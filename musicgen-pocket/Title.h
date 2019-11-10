#include "Scene.h"

#ifndef Title_h
#define Title_h
extern Arduboy2 arduboy;
extern Context context;

class Title : public Scene
{
  byte cursor;
  byte cursorH;
  public:
  virtual void init();
  virtual SceneID run();
  virtual void draw();
};

#endif
