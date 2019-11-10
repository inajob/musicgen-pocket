#include "Player.h"
extern volatile unsigned int d[4];
extern volatile unsigned char vol[4];
extern volatile byte nf;
extern volatile byte nf2;

void Player::init(){
  context.seek = 0;
  context.trackPos = 0;
}

void Player::run(){
  if(arduboy.everyXFrames(1)){
    if(nf!=0){
      nf = (nf >> 1);
    }
    if(nf2!=0){
      nf2 = (nf2 >> 1);
    }
  }
  unsigned int now;
  unsigned int next;
  unsigned int nnext;
  if(arduboy.everyXFrames(1)){
    if(vol[0] > 0){ // melo decay
      next = context.tones[context.pattern[context.track[0][context.trackPos]][context.seek]];
      // if this tone isn't play next next frame.
      if(context.seek < 15 && (next != context.tones[context.pattern[context.track[0][context.trackPos]][context.seek + 1]])){
      vol[0] -= 8;
      }
    }
    next = context.tones[context.pattern[context.track[1][context.trackPos]][context.seek]];
    if(vol[1] > 0){ // base decay
      // if this tone isn't play next next frame.
      if(context.seek < 15 && (next != context.tones[context.pattern[context.track[1][context.trackPos]][context.seek + 1]])){
      vol[1] -= 4;
      }
    }
  }


  // sound generate
  if(context.track[0][context.trackPos] != 255){
    if(context.modifier[context.track[0][context.trackPos]][context.seek] == 2){//bend
      now = d[0];
      if(context.seek == 15){
        next = context.tones[context.pattern[context.track[0][(context.trackPos + 1)%2]][0]];
      }else{
        next = context.tones[context.pattern[context.track[0][context.trackPos]][context.seek]];
      }
      if(d[0] != 0 && next != 0){
        d[0] = (next + d[0])/2;
      }
    }
  }

  if(arduboy.everyXFrames(context.speed)){
    if(context.track[0][context.trackPos] != 255){
      next = context.tones[context.pattern[context.track[0][context.trackPos]][context.seek]];
      if(d[0] != next){
        d[0] = context.tones[context.pattern[context.track[0][context.trackPos]][context.seek]];
        vol[0] = 64;
      }    }else{
      vol[0] = 0;
    }
    if(context.track[1][context.trackPos] != 255){
      next = context.tones[context.pattern[context.track[1][context.trackPos]][context.seek]];
      if(d[1] != next){
        d[1] = next;
        vol[1] = 48;
      }
    }else{
      vol[1] = 0;
    }
    if(context.track[2][context.trackPos] != 255){
      d[2] = context.tones[context.pattern[context.track[2][context.trackPos]][context.seek]];
      vol[2] = 32;
    }else{
      vol[2] = 0;
    }
    if(context.seek%2 == 0){ // test rythm
      //nf2 = (0x3f | 0x80) & 0x7f;
    }

    context.seek ++;
    if(context.seek == 16){
      context.seek = 0;
      context.trackPos ++;
      if(context.trackPos == 2){ // how many track?
        context.trackPos = 0;
      }
    }
    if(context.seek&1){
      arduboy.setRGBled(16, 0, 0);
    }else{
      arduboy.setRGBled(0, 0, 0);
    }
  }

}
