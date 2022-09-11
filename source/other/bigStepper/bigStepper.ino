
#define STP 8
#define DIR 9
#define ENA 10

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
  digitalWrite(ENA, 0);
  digitalWrite(DIR, 0);
  delay(100);
}
int i;
void loop() {
  digitalWrite(STP, 1);
  digitalWrite(STP, 0);
  
//  for (i = 0; i < 200 * STPMUL; i++) {
//    left();
//  }
//  for (i = 0; i < 200 * STPMUL; i++) {
//    right();
//  }
////  delay(1000);
//  setDir(1);
//  for (i = 0; i < 200 * STPMUL; i++) {
//    doStep();
//  }
//  setDir(0);
//  for (i = 0; i < 200 * STPMUL; i++) {
//    doStep();
// }
 
}
