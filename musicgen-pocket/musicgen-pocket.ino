#include <Arduboy2.h>
#include "ArduboyTones.h"
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "Context.h"
#include "Player.h"
#include "Title.h"
#include "MultiTunes.h"

Scene* scene;

Player player;
Title title;
Context context;
extern volatile unsigned int d[4];
extern volatile unsigned char vol[4];

unsigned int tones[] = {
NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,NOTE_G4,NOTE_A4,NOTE_B4,
NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5,NOTE_G5,NOTE_A5,NOTE_B5,
NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6,NOTE_G6,NOTE_A6,NOTE_B6,
NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7,NOTE_G7,NOTE_A7,NOTE_B7,
NOTE_C8, NOTE_D8, NOTE_E8, NOTE_F8,NOTE_G8,NOTE_A8,NOTE_B8,
};

#define R_RESUME 0
#define R_TONE 1
#define R_BEND 2


void setup() {
  soundSetup();

  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(60); // super fast

  // setup standard tones
  context.tones[0] = 0;
  for(int i = 0; i < 31; i ++){
    context.tones[i + 1] = tones[i + context.shift];
  }
  // setup track
  for(byte i = 0; i < 8; i ++){
    for(byte j = 0; j < 8; j ++){
      context.track[i][j] = 255;
    }
  }
  // setup
  for(byte i = 0; i < 16; i ++){
    for(byte j = 0; j < 16; j ++){
      context.pattern[i][j] = 0;
      context.modifier[i][j] = 0;
    }
  }


  arduboy.initRandomSeed();

  genMusic();

  title.init();

  scene = &title;
}

byte id = 0; // id for rythm make

byte genId(){
  return id ++;
}
void fill(byte *r, byte n, byte pos ,byte len){
  for(byte i = 0; i < len; i ++){
    r[pos + i] = n;
  }
}

void genRythm(byte *r,byte pos, byte len){
  int rr = random(10);
  switch(len){
    case 16:
      if(rr < 2){
        genRythm(r, pos, 2);
        genRythm(r, pos + 2, 14);
        return;
      }else if(rr < 5){
        genRythm(r, pos, 4);
        genRythm(r, pos + 4, 12);
        return;
      }else{
        genRythm(r, pos, 8);
        genRythm(r, pos + 8, 8);
        return;
      }
      break;
    case 8:
      if(rr < 4){
        fill(r, genId(), pos, 8);
        return;
      }else if(rr < 6){
        genRythm(r, pos, 2);
        genRythm(r, pos + 2, 6);
        return;
      }else{
        genRythm(r, pos, 4);
        genRythm(r, pos + 4, 4);
        return;
      }
      break;
    case 6:
      if(rr < 4){
        fill(r, genId(), pos, 6);
        return;
      }else{
        genRythm(r, pos, 2);
        genRythm(r, pos + 2, 4);
        return;
      }
      break;
    case 4:
      if(rr < 5){
        fill(r, genId(), pos, 4);
        return;
      }else{
        genRythm(r, pos, 2);
        genRythm(r, pos + 2, 2);
        return;
      }
      break;
    case 2:
      if(rr < 8){
        fill(r, genId(), pos, 2);
        return;
      }else{
        genRythm(r, pos, 1);
        genRythm(r, pos + 1, 1);
        return;
      }
      break;
    case 1:
      fill(r, genId(), pos, 1);
      return;
      break;
  }
}

void genMusic(){
  byte rythm[16];
  byte rrythm[16];
  // melo rythm
  id = 0;
  genRythm(rythm, 0, 16);

  byte baseMode = random(4);

  byte preRythm = 128;
  bool toneExists = false;
  while(toneExists == false){  // avoid no tone sequence
    for(int i = 0; i < 16; i ++){
        if(preRythm != rythm[i]){
          int r = random(100);
          if(r < 40){
            rrythm[i] = R_TONE;
            toneExists = true;
          }else if(r < 80 && (i > 0 && rrythm[i - 1] == R_TONE)){ // bend 40%
            rrythm[i] = R_BEND;
          }else{
            rrythm[i] = R_RESUME;
          }
        }else{
          rrythm[i] = rrythm[i - 1];
        }
        preRythm = rythm[i];
    }
  }

  // create similer melody in this loop
  for(int j = 0; j < 8; j ++){
    context.track[0][j] = j;

    // chord
    byte c = 1 + random(7); // C

    // assign tone
    byte preId = 128;
    unsigned int preTone  = 0;
    for(int i = 0; i < 16; i ++){
      if(preId != rythm[i]){
        if(rrythm[i] == R_RESUME){
          context.pattern[j][i] = 0; // no tone
        }else{ // R_TONE (R_BEND is not here)
          if(random(10) < 2){
            context.pattern[j][i] = 14 + c + random(7); // random
          }else{
            context.pattern[j][i] = 14 + c + random(3) * 2; // ex. C, E, G
          }
          if(random(2) == 0){
            context.pattern[j][i] -= 7;
          }
          preTone = context.pattern[j][i];
        }
      }else{
        context.pattern[j][i] = preTone;
      }
      if(rrythm[i] == R_BEND){
        context.modifier[j][i] = R_BEND;
      }else{
        context.modifier[j][i] = 0;
      }

      preId = rythm[i];
    }

    context.track[1][j] = j + 8;
    // base
    for(int i = 0; i < 16; i ++){
      switch(baseMode){
        case 0:
        context.pattern[j+8][i] = c;
        break;
        case 1:
        if(i%4 == 0){
          context.pattern[j+8][i] = c + (2*i/4%6);
        }else{
          context.pattern[j+8][i] = context.pattern[j+8][i - 1];
        }
        break;
        case 2:
        if(i%8 == 0){
          context.pattern[j+8][i] = c + (2*i/8%6);
        }else{
          context.pattern[j+8][i] = context.pattern[j+8][i - 1];
        }
        break;
        case 3:
        if(i%2 == 0){
          context.pattern[j+8][i] = c + (2*i/2%6);
        }else{
          context.pattern[j+8][i] = context.pattern[j+8][i - 1];
        }
        break;
        //case 4: // no base
        //  context.pattern[j+8][i] = 0;
        //break;
      }
    }
  }
}

void loop() {
  if (!(arduboy.nextFrame()))
  return;

  arduboy.pollButtons();

  player.run();

  if(arduboy.everyXFrames(context.speed)){
    if(context.seek == 0 && context.trackPos == 0){
      genMusic();
    }
  }

  SceneID nextSceneID = scene->run();
  scene->draw();

  arduboy.display();

  switch(nextSceneID){
    case STAY:
      break;
    case TITLE:
      scene = &title;
      break;
  }
}
