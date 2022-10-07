#include <SoftwareSerial.h>
// ---------------- begin ----------------
// params
#define WAITHSACT false    // wait hardware serial activate
#define WAITSSACT false   // wait software serial activate
#define HSSPEED 115200    // hardware serial speed
#define SSSPEED 115200    // software serial speed
#define USESSDEB true   // sofwareSerial debug
#define TODEG 182.041
#define STPTRIG 183*5   // step trigger 182.041(6) default
#define STPSPD 100     // Step time / 2
#define STPCYCLEY 3200   // step for full cycle
#define STPCYCLEX 3200   // step for full cycle
#define AXINVERS false
#define AYINVERS false
#define AZINVERS true
#define TOSTEPY (65535/STPCYCLEY)
#define TOSTEPX (65535/STPCYCLEX)
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
  debug.print("\nboot...");

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
  debug.println("\tOK");

}

void CRCcheck() {}

uint8_t answer[10];
uint8_t qwery[10];
uint8_t recv, Lrecv;
int AX, AY, AZ;
int PX, PY, PZ;
int DX, DY, DZ;
int SCX;
int SCY;
int stpnum;
String deb;


void loop() {
  while (0) {
    if (true xor AXINVERS) PORTB |= (1 << MXDIR);
    else PORTB &= ~(1 << MXDIR);
    for (int stp = 1600 ; stp > 0; stp--) {
      PORTB |= (1 << MXSTP);
      delayMicroseconds(STPSPD);
      PORTB &= ~(1 << MXSTP);
      delayMicroseconds(STPSPD);
    }
    delay(500);
  }
  if (Serial.available()) {
    recv = Serial.read();
    if (recv == 0x50) {
      Lrecv = recv;
    } else if (Lrecv == 0x50 and recv == 0x03) {
      while (!Serial.available()) {}
      if (Serial.read() == 0x06) {
        Serial.readBytes(answer, 8);
        deb = "";
        AX = (uint16_t)(answer[1] | (answer[0] << 8));// / 182.04;
        AY = (uint16_t)(answer[3] | (answer[2] << 8));// / 182.04; //?
        AZ = (uint16_t)(answer[5] | (answer[4] << 8));// / 182.04;
        deb += " | " + String(AX / TODEG);
        deb += " | " + String(AY / TODEG);
        deb += " | " + String(AZ / TODEG);
        PORTD |= 1 << DBB;
        debug.println(deb);
        PORTD &= ~(1 << DBB);

        SCX = abs((AX - PX)) / TOSTEPX; //
        SCY = abs((AY - PY)) / TOSTEPY; //
        stpnum = (SCX > SCY) ? SCX : SCY;
        if ((AX - PX) > 0 xor AXINVERS) PORTB |= (1 << MXDIR);
        else PORTB &= ~(1 << MXDIR);
        if ((AY - PY) > 0 xor AYINVERS) PORTB |= (1 << MYDIR);
        else PORTB &= ~(1 << MYDIR);

        for (int stp = 0; stp < stpnum; stp++) {
          if (stp <= SCX) PORTB |= (1 << MXSTP);
          if (stp <= SCY) PORTB |= (1 << MYSTP);
          delayMicroseconds(STPSPD);
          PORTB &= ~(1 << MXSTP);
          PORTB &= ~(1 << MYSTP);
          delayMicroseconds(STPSPD);
        }
        PX = AX;
        PY = AY;
      } else {
        Serial.readBytes(qwery, 5);
      }
    }
    Lrecv = recv;
    PORTD &= ~(1 << DBA);
    PORTD &= ~(1 << DBB);
  }


}
