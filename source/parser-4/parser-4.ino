#include <SoftwareSerial.h>

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
int enabled = true;

void loop() {
  if (Serial.available()) {
    recv = Serial.read();
    if (recv == 0x50) {
      Lrecv = recv;
    } else if (Lrecv == 0x50 and recv == 0x03) {
      while (!Serial.available()) {}
      if (Serial.read() == 0x06) {
        Serial.readBytes(answer, 8);
//        deb = "";
        AX = (uint16_t)(answer[1] | (answer[0] << 8));// / 182.04;
        AY = (int16_t)(answer[3] | (answer[2] << 8));// / 182.04; //?
        AZ = (uint16_t)(answer[5] | (answer[4] << 8));// / 182.04;
//        PORTD |= 1 << DBB;
//        debug.println(deb + "\tDXYZ\t" + String(DX / TODEG) + "\t" + String(DY / TODEG) + "\t" + String(DZ / TODEG));
//        PORTD &= ~(1 << DBB);

//        SCX = abs(((AX + DX) - PX)) / TOSTEPX; //
        SCX = 0;//abs(((AX + DX) - PX)) / TOSTEPX; //
//        SCY = abs(((AY + DY) - PY)) / TOSTEPY; //
        SCY = abs(AY) / TOSTEPY; //
        stpnum = (SCX > SCY) ? SCX : SCY;

        if (enabled) {
//          if (((AX + DX) - PX) > 0 xor AXINVERS) PORTB |= (1 << MXDIR);
//          else PORTB &= ~(1 << MXDIR);
          if (AY < 0) PORTB |= (1 << MYDIR);
          else PORTB &= ~(1 << MYDIR);

          for (int stp = 0; stp < stpnum; stp++) {
//            if (stp <= SCX) PORTB |= (1 << MXSTP);
            if (stp <= SCY) PORTB |= (1 << MYSTP);
            delayMicroseconds(STPSPD);
//            PORTB &= ~(1 << MXSTP);
            PORTB &= ~(1 << MYSTP);
            delayMicroseconds(STPSPD);
          }

          PX = AX + DX;
          PY = AY + DY;
        }
      } else {
        Serial.readBytes(qwery, 5);
      }
    } else if (recv == 0x55) {
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
