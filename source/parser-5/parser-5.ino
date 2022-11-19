// avr-libc library includes
#include <avr/io.h>
#include <SoftwareSerial.h>

#define STPCYCLEY 20000
#define TOSTEPY (65536/STPCYCLEY)

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

uint32_t dt;
uint32_t ct;
uint8_t del = 40;

uint8_t answer[10];
uint8_t qwery[10];
uint8_t recv, Lrecv;
int16_t AX, AY, AZ;
int16_t DX, DY, DZ;
int SCY;
int stpnum;

SoftwareSerial debug(2, 3);
void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1);
  debug.begin(115200);
  debug.setTimeout(1);
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
}

bool enabled = true;

void loop() {
  if (Serial.available()) {
    recv = Serial.read();
    if (recv == 0x50) {
      Lrecv = recv;
    } else if (Lrecv == 0x50 and recv == 0x03) {
      while (!Serial.available()) {}
      if (Serial.read() == 0x06) {
        Serial.readBytes(answer, 8);
        AY = (int16_t)(answer[3] | (answer[2] << 8));// / 182.04; //?
        AY -= DY;
        SCY = abs(AY) / TOSTEPY;
        stpnum = SCY;
        debug.println("AY:" + String(AY+DY) + "\tDY:" + String(DY));

        if (stpnum >= 125) del = 40;
        else if (stpnum <= 10) del = 1000;
        else del = 10000 / stpnum;

        if (AY < 0) PORTB |= (1 << MYDIR);
        else PORTB &= ~(1 << MYDIR);
      }
    } else if (recv == 0x55) {
      while (!Serial.available()) {}
      DDRD |= (1 << DBB);
      int msgln = Serial.readBytes(qwery, 6);
      if (qwery[0] == 0x01) enabled = true;
      if (qwery[0] == 0x02) enabled = false;
      if (qwery[0] == 0x06) {
        DY = (int16_t)(qwery[3] | (qwery[4] << 8));
      }
      debug.println("CMND:" + String(qwery[0]) + "\tDY\t" + String(DY));
      DDRD &= ~(1 << DBB);

    } else {
      Serial.readBytes(qwery, 5);
    }
    Lrecv = recv;
  }

  ct = micros();
  if (ct - dt > del and stpnum > 1 and enabled) {
    dt = ct;
    if (PINB & (1 << MYSTP)) {
      PORTB &= ~(1 << MYSTP);
      stpnum--;
      //      if (stpnum >= 125) del = 40;
      //      else if (stpnum <= 10) del = 1000;
      //      else del = 10000 / stpnum;
    } else {
      PORTB |= (1 << MYSTP);
    }
  }
}
