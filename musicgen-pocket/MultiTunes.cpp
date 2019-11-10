#include <Arduboy2.h>

const unsigned int timerLoadValue = 180;//50;//220;
volatile unsigned int level = 0;
volatile unsigned int d[4] = {0,0,0,0}; // default value( for debug)
volatile unsigned int dn[4] = {0,0,0,0};       // work variable
//volatile unsigned int lfo = 0;
volatile byte nf;
volatile byte nf2;
int noise;
int noise2;

volatile unsigned char vol[4] = {0,0,0,0}; // volume par channel


int (*levelFunc)();

int rawCalcLevel(){
  return ((dn[0]&(1<<15))?vol[0]:0) +
        ((dn[1]&(1<<15))?vol[1]:0) +
        ((dn[2]&(1<<15))?vol[2]:0) +
        ((dn[3]&(1<<15))?vol[3]:0);// +
        //((dn[4]&(1<<15))?vol[4]:0) +
        //((dn[5]&(1<<15))?vol[5]:0) +
        //((dn[6]&(1<<15))?vol[6]:0) +
        //((dn[7]&(1<<15))?vol[7]:0);
}

void soundSetup(){
  //mkWave(0);

  levelFunc = rawCalcLevel;

  pinMode(5,OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  // 3-pwm ,pwm
  TCCR3A = _BV(COM3A1) | _BV(WGM30);
  // clk/1 prescale
  TCCR3B = /*_BV(WGM12) |*/ _BV(CS30); //1      31250
  TIMSK3 = 1<<TOIE3; // Timer/Counter2 Overflow Interrupt Enable
  OCR3A = 80; // for debug
}

ISR(TIMER3_OVF_vect) {    // Timer/Counter1 Overflow
  static byte realcount = 0;
  static byte timeCount = 0;
  constexpr byte interruptsPerTick = 8;

  TCNT3 = timerLoadValue; // Reset the timer
  dn[0] = dn[0] + d[0] /*+ vf * ((*(wave[15] + (lfo >> 10))) << 2)*/;
  dn[1] = dn[1] + d[1];
  dn[2] = dn[2] + d[2];
  dn[3] = dn[3] + d[3];
  //dn[4] = dn[4] + d[4];
  //dn[5] = dn[5] + d[5];
  //dn[6] = dn[6] + d[6];
  //dn[7] = dn[7] + d[7];
  
  if((realcount & 0x3f) == 0x3f)noise = (noise>>1) + ((bitRead(noise, 13) xor bitRead(noise, 3) xor 1) << 15);
  noise2 = 2100005341  * noise2 + 1651869;
  realcount ++;

  level = (*levelFunc)() + ((noise) & nf) + ((noise2>>8) &nf2);

  //level = ((preLevel + level) >> 1); // filter
  //preLevel = level;
  OCR3A = (level);
}

