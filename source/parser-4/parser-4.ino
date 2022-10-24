#include <SoftwareSerial.h>

/*
  ranges
  angle by WT901 -> 0-65535 (uint16)
  angle Phisics  -> 0-360.00
  angle Stepper  -> 0-xxx
  converting
  WT901 -> Phisics : WT901 / 182.041 (TOSTEP)
  phisics -> WT901 : Phisics * 182.041 (TOSTEP)
  WT901 -> Stepper : WT901 / TOSTEP
  Stepper -> WT901 : Stepper * TOSTEP
  TOSTEP = 65535(int16max) / "number step for full circle"
*/

#define WAITHSACT false    // wait hardware serial activate
#define WAITSSACT false   // wait software serial activate
#define HSSPEED 115200    // hardware serial speed
#define SSSPEED 115200    // software serial speed
#define USESSDEB true   // sofwareSerial debug

#define TODEG (65536/360.0)
#define STPTODEG (STPCYCLEY/360.0)
#define STPTRIG 20   // step trigger 20 default
#define STPSPD 40     // old ver. don`t work
// speed => (1 step time) / 2
#define SSmax 50     // max speed / 2
#define SSmin 30     // min speed / 2
#define SSstr 40     // start speed / 2
#define SSacc 10     // speed accelerate
#define STPCYCLEY 6400   // number step for full cycle default 3200
#define STPCYCLEX 3200   // number step for full cycle default 3200
#define STPXRDC 1   // motorX Reducer
#define STPYRDC 1   // motorY Reducer
#define OFFSETY (40*TODEG)   // motorY Ofssets (Available angle*TODEG)
#define AXINVERS false
#define AYINVERS false
#define AZINVERS true
#define TOSTEPY (65536/STPCYCLEY)
#define TOSTEPX (65536/STPCYCLEX)
#define STPLIM 240
// pinout (use PD 0~7 and PB 8~13 like PD0~7 and PB0~5)
// MX*** -> motorX controller
// MY*** -> motorY controller
// DB*   -> DeBug channel
// SS**   -> Software Serial RX TX

#define MXSTP 0
#define MXDIR 1
#define MXENA 2
#define MYSTP 4
#define MYDIR 5
#define MYENA 3
#define DBA 6
#define DBB 7
#define SSRX 2
#define SSTX 3
#define SSEN 5

SoftwareSerial debug(SSRX, SSTX);

int lim(int val, int mx) {
  return (val > mx) ? mx : val;
}

void setup() {
  delay(5000);
  Serial.begin(HSSPEED);
  debug.begin(SSSPEED);
  Serial.setTimeout(5);
  debug.setTimeout(5);
  //  debug.print("\nboot...");

  DDRB |= (1 << MXSTP);
  DDRB |= (1 << MXDIR);
  DDRB |= (1 << MXENA);
  DDRB |= (1 << MYSTP);
  DDRB |= (1 << MYDIR);
  DDRB |= (1 << MYENA);

  DDRD |= (1 << SSEN);
  DDRD |= (1 << DBA);
  DDRD |= (1 << DBB);
  delayMicroseconds(50);
  PORTB &= ~(1 << MXSTP);
  PORTB &= ~(1 << MXDIR);
  PORTB &= ~(1 << MXENA);
  PORTB &= ~(1 << MYSTP);
  PORTB &= ~(1 << MYDIR);
  PORTB &= ~(1 << MYENA);
  delayMicroseconds(50);
  //  debug.println("\tOK");
  debug.println("AngleY numstpY DefinitionAngle");
}

uint8_t answer[10];
uint8_t qwery[10];
uint8_t recv, Lrecv;
int16_t AX, AY, AZ;
int PX, PY, PZ;
int DX, DY, DZ;
int SCX;
int SCY;
int stpnum;
String deb;
int enabled = true;


inline void stepy(int n_stp) {
  int SScur = SSstr; // current speed start with start speed
  int iter = 0; // current speed start with start speed
  int dd = (SSmax - SSmin) * SSacc; // current speed start with start speed
  for (int i = 0; i < n_stp; i++) { // number step cycle
    PORTB |= (1 << MYSTP);
    delayMicroseconds(SScur);
    PORTB &= ~(1 << MYSTP);
    delayMicroseconds(SScur);
    iter++;
    if ( iter == SSacc) { // current speed update when `SSacc` step
      iter = 0;
      PORTD |= (1 << DBB);
      if (n_stp - i < dd) { // acelerate or slowdown
        SScur = (SScur + 1 > SSmax) ? SSmax : SScur + 1; // slowdown
      } else {
        SScur = (SScur - 1 < SSmin) ? SSmin : SScur - 1; // accelerate
      }
      PORTD &= ~(1 << DBB);
    }
  }
}
uint8_t ddll = 40;

void loop() {
//  while (1) {
//    stepy(240);
//    delay(2);
//  }

  if (Serial.available()) {
    recv = Serial.read();
    if (recv == 0x50) {
      Lrecv = recv;
    }
    else if (Lrecv == 0x50 and recv == 0x03) {
      while (!Serial.available()) {}
      if (Serial.read() == 0x06) {

        Serial.readBytes(answer, 8);
        //        AX = (int16_t)(answer[1] | (answer[0] << 8));// / 182.04;
        AY = (int16_t)(answer[3] | (answer[2] << 8));// / 182.04; //?
        //        AZ = (uint16_t)(answer[5] | (answer[4] << 8));// / 182.04;
        //        AY -= DY; // fix with default position
        SCY = abs(AY) / TOSTEPY;
        //        SCY =(abs(AY) / (65536/STPCYCLEY));
        //SCY =(abs(AY) * STPCYCLEY / 65536);
        //        SCY = abs(AY) * STPCYCLEY;
        //        stpnum = (SCX > SCY) ? SCX : SCY;
        SCY = lim(SCY, STPLIM);
        stpnum = SCY;
        //        float WTangle = (AY) / TODEG;
        //        float STangle = (SCY * ((AY > 0) ? 1 : -1)) / STPTODEG;
        //        debug.println(String(WTangle, 3) + ' ' + String(STangle, 3) + ' ' + String(STangle - WTangle, 3));

        if (enabled) {
          //          if (AX < 0) PORTB |= (1 << MXDIR);
          //          else PORTB &= ~(1 << MXDIR);
          if (AY < 0) PORTB |= (1 << MYDIR);
          else PORTB &= ~(1 << MYDIR);

          stepy(SCY); // func steping with support acelarate
          //        //old step code
          //          for (int stp = 0; stp < stpnum; stp++) {
          //            if (stp < SCY) PORTB |= (1 << MYSTP);
          //            delayMicroseconds(STPSPD);
          //            PORTB &= ~(1 << MYSTP);
          //            delayMicroseconds(STPSPD);
          //          }

        }
      }
      else {
        Serial.readBytes(qwery, 5);
      }
    }
    else if (recv == 0x55) {
      int msgln = Serial.readBytes(qwery, 6);
      if (qwery[0] == 0x01) enabled = true;
      if (qwery[0] == 0x02) enabled = false;
      if (qwery[0] == 0x06) {
        DX = qwery[1] | (qwery[2] << 8);
        DY = qwery[3] | (qwery[4] << 8);
        DZ = qwery[5] | (qwery[6] << 8);
      }
      //      debug.println("CMND:"+String(qwery[0])+"\tDXYZ\t"+String(DX)+"\t"+String(DY)+"\t"+String(DZ));
    }
    Lrecv = recv;
    PORTD &= ~(1 << DBA);
    PORTD &= ~(1 << DBB);
  }

}
