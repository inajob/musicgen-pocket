#include "Title.h"

PROGMEM const unsigned char logo[] = {0xc4, 0x44, 0x47, 0x01, 0x9f, 0x90, 0x0f, 0xe1, 0x4f, 0x90, 0x1f, 0x01, 0x1f, 0x10, 0x1c, 0x04,
0x05, 0x05, 0x07, 0x00, 0x4f, 0xea, 0xe0, 0x7f, 0x00, 0x04, 0x03, 0x1c, 0x22, 0x32, 0x3c, 0x00};

extern volatile byte nf;
extern volatile byte nf2;

void Title::init(){
  cursor = 0;
  context.speed = 5;
}

SceneID Title::run(){
  if(arduboy.justPressed(A_BUTTON)){
    nf2 = (0x3f | 0x80) & 0x7f;
    switch(cursor){
      case 0: context.patternNo = 0; return STAY;
      case 1: context.patternNo = 1; return STAY;
      case 2: context.patternNo = 2; return STAY;
      case 3: context.patternNo = 3; return STAY;
      case 4: context.patternNo = 4; return STAY;
      case 5: context.patternNo = 5; return STAY;
    }
  }
  if(arduboy.justPressed(B_BUTTON)){
    nf = 0x3f;
  }
  if(arduboy.justPressed(UP_BUTTON)){
    if(cursor > 0){
      cursor --;
    }
  }
  if(arduboy.justPressed(DOWN_BUTTON)){
    if(cursor < 5){
      cursor ++;
    }
  }
  if(arduboy.justPressed(LEFT_BUTTON)){
    switch(cursor){
      case 0:
        if(context.speed > 3)context.speed --;
        break;
    }
  }
  if(arduboy.justPressed(RIGHT_BUTTON)){
   switch(cursor){
      case 0:
        if(context.speed < 10)context.speed ++;
        break;
   }
  }

  return STAY;
}

void Title::draw(){
  arduboy.clear();
  arduboy.setCursor(0,0);
  arduboy.print(F("MUSIC GEN POCKET"));

  arduboy.setCursor(5*2 ,9);
  arduboy.print(F("SPEED"));
  arduboy.setCursor(5*9,9);
  arduboy.print(context.speed);

  for(byte j = 0; j < 16; j++){
    arduboy.fillRect(j*8, 64 - context.pattern[context.track[0][context.trackPos]][j]*2, 8, 2);
    arduboy.fillRect(j*8, 64 - context.pattern[context.track[1][context.trackPos]][j]*2, 8, 2);
  }
  arduboy.fillRect(context.seek*8, 0, 1, 64);

  arduboy.setCursor(0,9 + 9 * cursor);
  if(arduboy.everyXFrames(2)){
    arduboy.print(F(">"));
  }
}
