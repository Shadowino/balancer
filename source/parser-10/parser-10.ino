#include <SoftwareSerial.h>

#define PBon(x) PORTB |= (1 << x);
#define PBoff(x) PORTB &= ~(1 << x);
#define PDon(x) PORTD |= (1 << x);
#define PDoff(x) PORTD &= ~(1 << x);

// PINnum = BITnumber + 8
// BITnumber = PINnumber - 8
// пины(контакты) моторов указываються через BITnumber
// и находяться на порту B
#define MXDIR 0 //8
#define MXSTP 1 //9
#define MXENA 2 //10

#define MYENA 3 //11
#define MYSTP 4 //12
#define MYDIR 5 //13


#define STPCYCLEY 8600
#define TOSTEPY (65536/STPCYCLEY)

#define SSENA true

SoftwareSerial debug(2, 3);

void movtest();

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  debug.begin(115200);
  debug.setTimeout(1);
  debug.println("STEEL @LIVE");
  // DDRx |= (1 << y); => PinMode(y, OUTPUT);
  DDRD |= (1 << 4); //
  DDRD |= (1 << 5);
  DDRD |= (1 << 6);
  DDRD |= (1 << 7);

  DDRB |= (1 << 0);
  DDRB |= (1 << 1);
  DDRB |= (1 << 2);
  DDRB |= (1 << 3);
  DDRB |= (1 << 4);
  DDRB |= (1 << 5);
  PDoff(4)
  PDoff(5)
  PDoff(6)
  PDoff(7)
  PBoff(MXDIR);
  PBoff(MXSTP);
  PBoff(MXENA);
  PBoff(MYENA);
  PBoff(MYSTP);
  PBoff(MYDIR);
  if (SSENA) {
    debug.print("HS:");
    debug.print(((Serial) ? "true" : "false"));
    debug.print(" SS:");
    debug.println(((debug) ? "true" : "false"));
  }
}
