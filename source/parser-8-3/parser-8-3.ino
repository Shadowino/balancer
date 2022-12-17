//parser 8-1
//изменено направление AX
//
#include <SoftwareSerial.h>

#define STPCYCLEY 8600
#define TOSTEPY (65536/STPCYCLEY)

SoftwareSerial debug(2, 3);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  debug.begin(115200);
  debug.setTimeout(1);
  debug.println("\n115200");
  DDRD |= (1 << 4);
  pinMode(5, INPUT_PULLUP);
  DDRD |= (1 << 7);

  DDRB |= (1 << 4);
  DDRB |= (1 << 5);
  DDRB |= (1 << 3);

  DDRB |= (1 << 0); //dir
  DDRB |= (1 << 1); //pul
  DDRB |= (1 << 2); //ena

  PORTD &= ~(1 << 7);
  PORTD &= ~(1 << 4);
  PORTB &= ~(1 << 3);
  PORTB &= ~(1 << 4);
  PORTB &= ~(1 << 5);
  PORTB &= ~(1 << 0); //dir
  PORTB &= ~(1 << 1); //pul
  PORTB &= ~(1 << 2); //ena
  //  PORTB |= (1 << 3);
  debug.print("HS:");
  debug.print(((Serial) ? "true" : "false"));
  debug.print(" SS:");
  debug.println(((debug) ? "true" : "false"));
  if (!digitalRead(5)) {
    debug.println("INFO: Start selftest, check jumper and resetv arduino.");
    while (1) {
      PORTB &= ~(1 << 5); //13
      PORTB &= ~(1 << 0); //8
      for (int i = 0; i < 1500; i++) {
        PORTB &= ~(1 << 4); //12
        PORTB &= ~(1 << 1); //9
        delay(1);
        PORTB |= (1 << 4); //12
        PORTB |= (1 << 1); //9
        delay(1);
      }
      delay(500);
      PORTB |= (1 << 5); //13
      PORTB |= (1 << 0); //8
      for (int i = 0; i < 1500; i++) {
        PORTB &= ~(1 << 4); //12
        PORTB &= ~(1 << 1); //9
        delay(1);
        PORTB |= (1 << 4); //12
        PORTB |= (1 << 1); //9
        delay(1);
      }
      delay(500);

    }
  }

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

    int16_t getX() {
      if (type == 2) {
        return (data[6] | (data[5] << 8));
      } else {
        return (data[3] | (data[4] << 8));
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

int16_t AY, DY, AX, DX;
long int SCY, SCX;
long unsigned int stpdt, stpdtx, dtled;
long unsigned int dt, dtx, dtl;
unsigned int stpdel;
bool enabled = true;
uint32_t dtbyte;
uint32_t DTMILLIS;

message nw;
message pack;

void loop() {

  while (0) {
    dtl = millis();
    PORTD |= (1 << 7);
    delay(250);
    PORTD &= ~(1 << 7);
    delay(250);
    if (Serial.available()) {
      Serial.read();
      PORTD |= (1 << 7); //13
    } else {
      PORTD &= ~(1 << 7); //13
    }
  }


  DTMILLIS = millis();
  { // ================= LED INDICATOR =====
//    if (DTMILLIS - dtled > 500) {
//      dtled = DTMILLIS;
//      if (PIND & (1 << 7)) {
//        PORTD &= ~(1 << 7);
//      } else {
//        PORTD |= (1 << 7);
//      }
//    }
  }


  { // ================= RS485 protocol handler ===============
    if (DTMILLIS - dtbyte > 5) nw.len = 0;
    if (Serial.available() and !msgc) {
      if (PIND & (1 << 7)) {
        PORTD &= ~(1 << 7);
      } else {
        PORTD |= (1 << 7);
      }
      dtbyte = DTMILLIS;
      recv = Serial.read();
      //      if (recv == 0x50 or recv == 0x55) {
      //        nw.len = 0;
      //      }
      nw.add(recv);
      if (nw.len > 3 and nw.len == ((nw.data[2] == 0) ? 8 : nw.data[2] + 5)) {
        for (int i = 0; i < nw.len; i++) {
          pack = nw;
        }
        pack.mready = true;
        pack.check();
        nw.len = 0;
      }
    }

  }// =============== end of handler =============

  { // ================= PID controller ==============
    if (pack.mready and pack.type == 2) {
      AY = pack.getY();
      AY -= DY;
      SCY = abs(AY) / TOSTEPY;
      if (SCY <= 10) SCY = 0;
//      if (SCY >= 200) stpdel = 40;
//      else if (SCY <= 10) stpdel = 1000;
//      else stpdel = 10000 / SCY;

      AX = pack.getX();
      AX -= DX;
      SCX = abs(AX) / TOSTEPY;
      if (SCX <= 10) SCX = 0;

      if (AY > 0) {
        PORTB |= (1 << 5);
      } else {
        PORTB &= ~(1 << 5);
      }

      if (AX < 0) {
        PORTB |= (1 << 0); //dir
      } else {
        PORTB &= ~(1 << 0); //dir
      }


    } else if (pack.mready and pack.type == 3) {
      if (pack.data[1] == 0x06) {
        DY = pack.getY();
        DX = pack.getX();
        //        DY = (DY > 8100) ? 8100 : ((DY < -8100) ? -8100 : DY);
      } else if (pack.data[1] == 0x01) {
        enabled = true;
      } else if (pack.data[1] == 0x02) {
        enabled = false;
      }
    }

  }

  dt = micros();
  dtx = dt;
  //  dtl = millis();
  { // ================== Step controller
    //    if (dtl - dtled > 500) {
    //      dtled = dtl;
    //      if (PINB & (1 << 7)) {
    //        PORTD &= ~(1 << 7);
    //      } else {
    //        PORTD |= (1 << 7);
    //      }
    //
    //    }

    //    if (!enabled and dt - stpdt > 100) {
    //      stpdt = dt;
    //      if (PINB & (1 << 5)) {
    //        PORTB |= (1 << 5);
    //      } else {
    //        PORTB &= ~(1 << 5);
    //      }
    //    }

    if (dt - stpdt > 800 and SCY > 1 and enabled) {
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

    if (dtx - stpdtx > 800 and SCX > 1 and enabled) {
      stpdtx = dtx;
      if (PINB & (1 << 1)) {
        PORTB &= ~(1 << 1); //pul
        SCX--;
      } else {
        PORTB |= (1 << 1);
      }
    }


  } // ================== asdcad ===============

  { // ================= SoftwareSerial debug ==============
    if (true and pack.mready) {
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
