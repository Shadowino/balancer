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
long unsigned int stpdt, stpdtx;
long unsigned int dt, dtx;
unsigned int stpdel;
bool enabled = true;


message nw;
message pack;

void loop() {
  while (0) {
    if (Serial.available()) {
      Serial.read();
      PORTB |= (1 << 5); //13
    } else {
      PORTB &= ~(1 << 5); //13
    }
  }

  while (0) {
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



  { // ================= RS485 protocol handler ===============
    if (Serial.available() and !msgc) {
      PORTD |= (1 << 7);
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
  PORTD &= ~(1 << 7);

  { // ================= PID controller ==============
    if (pack.mready and pack.type == 2) {
      PORTD |= (1 << 7);
      AY = pack.getY();
      AY -= DY;
      SCY = abs(AY) / TOSTEPY;
      if (SCY >= 200) stpdel = 40;
      else if (SCY <= 10) stpdel = 1000;
      else stpdel = 10000 / SCY;

      AX = pack.getX();
      SCX = abs(AX) / TOSTEPY;

      if (AY > 0) {
        PORTB |= (1 << 5);
      } else {
        PORTB &= ~(1 << 5);
      }

      if (AX > 0) {
        PORTB |= (1 << 0); //dir
      } else {
        PORTB &= ~(1 << 0); //dir
      }


    } else if (pack.mready and pack.type == 3) {
      if (pack.data[1] == 0x06) {
        //        DY = pack.getY();
        //        DY = (DY > 8100) ? 8100 : ((DY < -8100) ? -8100 : DY);
      } else if (pack.data[1] == 0x01) {
        enabled = true;
      } else if (pack.data[1] == 0x02) {
        enabled = false;
      }
    }

  }
  PORTD &= ~(1 << 7);

  dt = micros();
  dtx = dt;
  { // ================== Step controller
    PORTD |= (1 << 7);

    //    if (!enabled and dt - stpdt > 100) {
    //      stpdt = dt;
    //      if (PINB & (1 << 5)) {
    //        PORTB |= (1 << 5);
    //      } else {
    //        PORTB &= ~(1 << 5);
    //      }
    //    }

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

    if (dtx - stpdtx > 40 and SCX > 1 and enabled) {
      stpdtx = dtx;
      if (PINB & (1 << 1)) {
        PORTB &= ~(1 << 1); //pul
        SCX--;
      } else {
        PORTB |= (1 << 1);
      }
    }


  } // ================== asdcad ===============
  PORTD &= ~(1 << 7);

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
