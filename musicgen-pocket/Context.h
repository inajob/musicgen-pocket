#ifndef Context_h
#define Context_h

struct Context{
  byte patternNo;
  byte pattern[16][16];
  byte modifier[16][16];
  byte trackNo;
  byte track[8][8];
  byte speed;
  byte seek;
  byte trackPos;
  unsigned int tones[32];
  char shift;
};

#endif
