//parser 8-2
//PID регулятор
//изменено направление AX

// PID struct
// тут все коэфиценты
uint16_t PIDKP = 0.1297;
uint16_t PIDKI = 0;
uint16_t PIDKD = 0;
uint16_t PIDI;
uint16_t PIDD;
uint32_t PIDdt;
uint16_t PIDerr;


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

    //    новые функции (корректные но опасные! нет совметсимости с прошлыми версиями!)
    //    int16_t getX() {
    //      return (data[4] | (data[3] << 8));
    //    }
    //    int16_t getY() {
    //      return (data[6] | (data[5] << 8));
    //    }
    //    int16_t getY() {
    //      return (data[8] | (data[7] << 8));
    //    }

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

    uint16_t getPID() {
      return ((data[3] << 8) | (data[4]));
    }
    uint16_t getPIDall() {
      PIDKP = ((data[3] << 8) | (data[4]));
      PIDKI = ((data[5] << 8) | (data[6]));
      PIDKD = ((data[7] << 8) | (data[8]));
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
unsigned int stpdely;
bool enabled = true;
uint32_t dtbyte;
uint32_t DTMILLIS;
int PIDmode = 3;
int SSS = 0;

message nw;
message pack;

void loop() {

  DTMILLIS = millis();



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
      // PID Регулятор -------- AY ONLY
      // SCY = AY*PIDKP + PIDI*PIDKI + PIDD*PIDKD // управляющий сигнал
      // PIDI = PIDI + AY*(DTMILLIS - PIDdt) // I состовляющая
      // PIDdt = DTMILLIS  | (time) // прошлое время измерения
      // PIDD = (AY - PIDerr)/(DTMILLIS - PIDdt) // D Состовляющая
      // время везде измеряеться в миллисикундах
      AY = pack.getY();
      AY -= DY;
      SCY = 0;
      if (PIDmode == 1) { // stepmode
        PIDI = PIDI + AY * (DTMILLIS - PIDdt);
        PIDD = (AY - PIDerr) * (DTMILLIS - PIDdt);
        SCY = (uint32_t)(AY * PIDKP + PIDI * PIDKI + PIDD * PIDKD);
        PIDerr = AY;
        PIDdt = DTMILLIS;
        if (SCY >= 125) stpdely = 40;
        else if (SCY <= 10) stpdely = 1000;
        else stpdely = 10000 / SCY;
      } else if (PIDmode == 2) { // speedmode
        PIDI = PIDI + AY * (DTMILLIS - PIDdt);
        PIDD = (AY - PIDerr) * (DTMILLIS - PIDdt);
        stpdely = (uint32_t)(AY * PIDKP + PIDI * PIDKI + PIDD * PIDKD);
        PIDerr = AY;
        PIDdt = DTMILLIS;
        stpdely = (stpdely < 40) ? 40 : ((stpdely > 1000) ? 1000 : stpdely );
        SCY = (DTMILLIS - PIDdt) / stpdely;
      } else { // linear (defoult)
        SCY = abs(AY) / TOSTEPY;
      }

      if (AY > 0) {
        PORTB |= (1 << 5);
      } else {
        PORTB &= ~(1 << 5);
      }

    } else if (pack.mready and pack.type == 3) {
      if (pack.data[1] == 0x06) { // сдвиг по двум осям
        DY = pack.getY();
        DX = pack.getX();
      } else if (pack.data[1] == 0x01) { // старт
        enabled = true;
      } else if (pack.data[1] == 0x02) { // стоп
        enabled = false;
      } else if (pack.data[1] == 0x03) { // установить PIDP
        PIDKP = pack.getPID();
      } else if (pack.data[1] == 0x04) { // установить PIDI
        PIDKI = pack.getPID();
      } else if (pack.data[1] == 0x05) { // установить PIDD
        PIDKD = pack.getPID();
      } else if (pack.data[1] == 0x07) { // установить все параметры PID (PIDP PIDI PIDD)
        pack.getPIDall();
      } else if (pack.data[1] == 0x08) {
        // выбор режима работы PID
        // (1 -> PIDSTEP) (2 -> PIDSPEED) (3 -> LINEAR)
        PIDmode = pack.getPID();
        PIDI = 0;
      } else if (pack.data[1] == 0x09) {
        // SoftwareSerialSetting
        SSS = pack.data[3];
      }
    }

  }

  dt = micros();
  { // ================== Step controller

    if (dt - stpdt > stpdely and SCY > 1 and enabled) {
      stpdt = dt;
      if (PINB & (1 << 4)) {
        PORTB &= ~(1 << 4);
        SCY--;
        //if (SCY >= 125) stpdel = 40;
        //else if (SCY <= 10) stpdel = 1000;
        //else stpdel = 10000 / SCY;
      } else {
        PORTB |= (1 << 4);
      }
    }

    //    if (dt - stpdtx > stpdelx and SCX > 1 and enabled) {
    //      stpdtx = dt;
    //      if (PINB & (1 << 1)) {
    //        PORTB &= ~(1 << 1); //pul
    //        SCX--;
    //      } else {
    //        PORTB |= (1 << 1);
    //      }
    //  }


  } // ================== Step controller ===============

  { // ================= SoftwareSerial debug ==============
    if (true and pack.mready and SSS) {
      String snd = "";
      if (SSS == 1) {
        snd = String(pack.len) + ":\t";
        for (int i = 0; i < pack.len; i++) {
          snd += ((pack.data[i] < 16) ? " 0" : " ") + String(pack.data[i], HEX);
        }
      } else if (SSS == 2) {
        if (pack.type != 1) {
          snd = "A: " + String((AY + DY) / 182.041, 2) + " - " + String(DY / 182.041, 2) + " = " + String(AY / 182.041, 2);
        }
      } else if (SSS == 3) {
        if (pack.type != 1) {
          snd = "P: " + String(PIDKP) + " " + String(PIDKI) + " " + String(PIDKD) + " " + String(SCY) + " " + String(stpdely);
        }
      }
      debug.println(snd);
    }
  } // ================= SS debug END ==============

  pack.mready = false;

}
