#include <SoftwareSerial.h>

#define STPCYCLEY 8600
#define TOSTEPY (65536/STPCYCLEY)

SoftwareSerial debug(2, 3);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  debug.begin(115200);
  debug.setTimeout(1);
  DDRD |= (1 << 5);
  DDRD |= (1 << 7);
  DDRB |= (1 << 4);
  DDRB |= (1 << 5);
  PORTD &= ~(1 << 7);
  PORTD &= ~(1 << 5);
  PORTB &= ~(1 << 4);
  PORTB &= ~(1 << 5);
}

class message {
  public:
    uint8_t data[20];
    uint8_t len;
    uint8_t n;
};

uint8_t recv;
uint8_t msg[30];
uint8_t msgl = 0;
uint8_t msgll = 0;
long unsigned int msgdt;
bool msgc = false;

uint8_t packet[30];
uint8_t plen;
uint8_t pready;

bool dp = false;

int16_t AY, DY;
long int SCY;
long unsigned int stpdt;
long unsigned int dt;
bool enabled = true;


void loop() {
  dt = micros();

  { // ================= RS485 protocol handler ===============
    if (Serial.available() and !msgc) {
      PORTD |= (1 << 7);
      recv = Serial.read();
      if (recv == 0x50 or recv == 0x55) {
        msgl = 0;
      }
      msg[msgl] = recv;
      if ( msgl > 3 and msgl == ((msg[2] == 0) ? 8 : msg[2] + 5)) {
        //        while (pready == 1) {};
        for (int i = 0; i < msgl; i++) {
          packet[i] = msg[i];
        }
        plen = msgl;
        pready = 1;
        msgl = 0;
        PORTD &= ~(1 << 7);
      }
      msgl++;

    }

  }// =============== end of handler =============

  { // ================= SoftwareSerial debug ==============
    if (pready and true) {
      String snd = String(plen) + "\t";//String(SCY) + " " + String(AY / 182.041, 2) + " " + String(packet[2], HEX);
      for (int i = 0; i < plen; i++) {
        snd += ((packet[i] < 16) ? " 0" : " ") + String(packet[i], HEX);
      }
      //      msgl = 0;
      //      msgc = false;
      debug.println(snd);
    }
  } // ================= SS debug END ==============

  { // ================= PID controller ==============
    if (pready and packet[0] == 0x50 and packet[2] == 0x06) {
      PORTD |= (1 << 7);
      AY = (int16_t)(packet[6] | (packet[5] << 8));// / 182.04; //?
      // AY -= DY;
      SCY = abs(AY) / TOSTEPY;
      // debug.println("AY:" + String(AY + DY) + "\tDY:" + String(DY));
      if (AY < 0) {
        PORTB |= (1 << 5);
      } else {
        PORTB &= ~(1 << 5);
      }
      PORTD &= ~(1 << 7);
    }
  }

  { // ================== Step controller
    if (stpdt - dt > 40 and SCY > 1 and enabled) {
      stpdt = dt;
      if (PINB & (1 << 4)) {
        PORTB &= ~(1 << 4);
        SCY--;
        //      if (stpnum >= 125) del = 40;
        //      else if (stpnum <= 10) del = 1000;
        //      else del = 10000 / stpnum;
      } else {
        PORTB |= (1 << 4);
      }
    }
  } // ================== asdcad ===============

  pready = 0;

}
