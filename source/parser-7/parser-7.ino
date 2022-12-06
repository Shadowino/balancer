#include <SoftwareSerial.h>

#define STPCYCLEY 8600
#define TOSTEPY (65536/STPCYCLEY)

SoftwareSerial debug(2, 3);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  debug.begin(115200);
  debug.setTimeout(1);
  debug.println("115200");
  DDRD |= (1 << 4);
  DDRD |= (1 << 7);
  DDRB |= (1 << 4);
  DDRB |= (1 << 5);
  DDRB |= (1 << 3);
  PORTD &= ~(1 << 7);
  PORTD &= ~(1 << 4);
  PORTB &= ~(1 << 3);
  PORTB &= ~(1 << 4);
  PORTB &= ~(1 << 5);
  //  PORTB |= (1 << 3);
  debug.print("HS:");
  debug.print(((Serial) ? "true" : "false"));
  debug.print(" HS:");
  debug.println(((debug) ? "true" : "false"));
}

class message {
  public:
    uint8_t type;
    uint8_t data[20];
    uint8_t len;
    bool mready;
    void add(uint8_t d) {
      data[len] = d;
      len++;
    }
    uint8_t check() {
      if (data[0] == 0x50 and data[2] == 0x06) type = 2;
      else if (data[0] == 0x50 and data[2] == 0x00) type = 1;
      else if (data[0] == 0x55) type = 3;
      else type = 0;
      return type;
    }
    int16_t getY() {
      if (type == 2) {
        return (data[4] | (data[3] << 8));
      } else {
        return (data[5] | (data[6] << 8));
      }

    }

    int16_t getY_old() {
      if (type == 2) {
        return (data[6] | (data[5] << 8));
      } else {
        return (data[5] | (data[6] << 8));
      }

    }
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
unsigned int stpdel;
bool enabled = true;


message nw;
message pack;

void loop() {
  while (0) {
    PORTB |= (1 << 5);
    for (int i = 0; i < 100; i++) {
      PORTB &= ~(1 << 4);
      delay(40);
      PORTB |= (1 << 4);
      delay(40);
    }
    delay(500);
    PORTB &= ~(1 << 5);
    for (int i = 0; i < 100; i++) {
      PORTB &= ~(1 << 4);
      delay(40);
      PORTB |= (1 << 4);
      delay(40);
    }
    delay(500);
  }


  { // ================= RS485 protocol handler ===============
    if (Serial.available() and !msgc) {
      //      debug.println(String(nw.len) + ":\t");
      recv = Serial.read();
      if (recv == 0x50 or recv == 0x55) {
        nw.len = 0;
      }
      nw.add(recv);
      if ( nw.len > 3 and nw.len == ((nw.data[2] == 0) ? 8 : nw.data[2] + 5)) {
        for (int i = 0; i < nw.len; i++) {
          pack = nw;
        }
        pack.mready = true;
        pack.check();
        nw.len = 0;
        //        PORTD &= ~(1 << 7);
      }
    }

  }// =============== end of handler =============

  { // ================= PID controller ==============
    if (pack.mready and pack.type == 2) {
      AY = pack.getY();
      AY -= DY;
      SCY = abs(AY) / TOSTEPY;
      if (SCY >= 200) stpdel = 40;
      else if (SCY <= 10) stpdel = 1000;
      else stpdel = 10000 / SCY;
      if (AY < 0) {
        PORTB |= (1 << 5);
      } else {
        PORTB &= ~(1 << 5);
      }
    } else if (pack.mready and pack.type == 3) {
      if (pack.data[1] == 0x06) {
        DY = pack.getY();
//        DY = (DY > 8100) ? 8100 : ((DY < -8100) ? -8100 : DY);
      } else if (pack.data[1] == 0x01) {
        enabled = true;
      } else if (pack.data[1] == 0x02) {
        enabled = false;
      }
    }

  }

  dt = micros();
  { // ================== Step controller
    if (dt - stpdt > stpdel and SCY > 1 and enabled) {
      stpdt = dt;
      if (PINB & (1 << 4)) {
        PORTB &= ~(1 << 4);
        SCY--;
        if (SCY >= 125) stpdel = 40;
        else if (SCY <= 10) stpdel = 1000;
        else stpdel = 10000 / SCY;
      } else {
        PORTB |= (1 << 4);
      }
    }
  } // ================== asdcad ===============

  { // ================= SoftwareSerial debug ==============
    if (false and pack.mready) {
      String snd = String(pack.len) + ":\t";
      for (int i = 0; i < pack.len; i++) {
        snd += ((pack.data[i] < 16) ? " 0" : " ") + String(pack.data[i], HEX);
      }

      //      if (pack.type != 1) {
      //        String snd = String((AY + DY) / 182.041, 2) + " - " + String(DY / 182.041, 2) + " = " + String(AY / 182.041, 2);
      debug.println(snd);
      //      }
    }
  } // ================= SS debug END ==============

  pack.mready = false;

}
