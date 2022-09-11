
#define DIR 8
#define STP 8
#define ENA 8

#define STPMUL 16

void powerON();
void powerOFF();
void left(); // step left
void right(); // step right
void setDir(bool dir = true); // use with doStep() true for left, false for right
void doStep(); // step with setDir() direct
void ini(); // initializate stepper

void setup() {
  Serial.begin(9600);
  Serial.println("Steel alive");
  ini();
}
int i;
void loop() {
  for (i = 0; i < 200*STPMUL; i++) {
    left();
  }
  for (i = 0; i < 200*STPMUL; i++) {
    right();
  }
  delay(1000);
}
