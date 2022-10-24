#include <SoftwareSerial.h>
#include <avr/io.h>

#define WAITHSACT false    // wait hardware serial activate
#define WAITSSACT false   // wait software serial activate
#define HSSPEED 115200    // hardware serial speed
#define SSSPEED 115200    // software serial speed
#define USESSDEB true   // sofwareSerial debug

#define TODEG (65535/360.0)
#define STPTODEG (STPCYCLEY/360.0)
#define STPTRIG 20   // step trigger 20 default
#define STPSPD 100     // Step time / 2
#define STPCYCLEY 28800   // number step for full cycle default 3200
#define STPCYCLEX 3200   // number step for full cycle default 3200
#define STPXRDC 1   // motorX Reducer
#define STPYRDC 1   // motorY Reducer
#define OFFSETY (40*TODEG)   // motorY Ofssets (Available angle*TODEG)
#define AXINVERS false
#define AYINVERS false
#define AZINVERS true
#define TOSTEPY (65535/STPCYCLEY)
#define TOSTEPX (65535/STPCYCLEX)
#define STPLIM 90
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

void setup() {
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
  delay(3000);
}

int SPDcur = 35;
#define SPDmax 50
#define SPDmin 30
#define SPDacc 30
#define SPDstr 40

inline void stepn(int n_stp) {
  SPDcur = SPDstr;
  for (int i = 0; i < n_stp; i++) {
    PORTB &= ~(1 << MYSTP);
    delayMicroseconds(SPDcur);
    PORTB |= (1 << MYSTP);
    delayMicroseconds(SPDcur - 1);
    if ( i % SPDacc == 0) {
      if (n_stp - i < (SPDmax - SPDmin)*SPDacc) {
        SPDcur = (SPDcur + 1 > SPDmax) ? SPDmax : SPDcur;
      } else {
        SPDcur = (SPDcur - 1 < SPDmin) ? SPDmin : SPDcur;
      }
    }
  }
}


#define CNTS 2600   
#define DELS 50   
#define DELSS 10000   

void loop() {
  PORTB &= ~(1 << MYDIR);
//  for (int stp = 0; stp < CNTS ; stp++) {
//    PORTB |= (1 << MYSTP);
//    delayMicroseconds(DELS);
//    PORTB &= ~(1 << MYSTP);
//    delayMicroseconds(DELS);
//  }
  stepn(CNTS);
  delay(DELSS);
  PORTB |= (1 << MYDIR);
  stepn(CNTS);
  delay(DELSS);
  PORTB |= (1 << MYDIR);
  stepn(CNTS);
  delay(DELSS);
  PORTB &= ~(1 << MYDIR);
  stepn(CNTS);
  delay(DELSS);
//  while(1){}
//  PORTB |= (1 << MYDIR);
//  for (int stp = 0; stp < CNTS ; stp++) {
//    PORTB |= (1 << MYSTP);
//    delayMicroseconds(DELS);
//    PORTB &= ~(1 << MYSTP);
//    delayMicroseconds(DELS);
//  }
//  delay(3000);
//  PORTB |= (1 << MYDIR);
//  for (int stp = 0; stp < CNTS ; stp++) {
//    PORTB |= (1 << MYSTP);
//    delayMicroseconds(DELS);
//    PORTB &= ~(1 << MYSTP);
//    delayMicroseconds(DELS);
//  }
//  delay(3000);
//  PORTB &= (1 << MYDIR);
//  for (int stp = 0; stp < CNTS ; stp++) {
//    PORTB |= (1 << MYSTP);
//    delayMicroseconds(DELS);
//    PORTB &= ~(1 << MYSTP);
//    delayMicroseconds(DELS);
//  }
//  delay(3000);
}
