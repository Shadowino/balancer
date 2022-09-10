
#define STP 8
#define DIR 9
#define ENA 10

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

void loop() {
  digitalWrite(STP, 1);
  delay(2);
  digitalWrite(STP, 0);
  delay(2);
  /*
   // example using doStep() with setDir() 
    setDir(1);
    while(1){
      doStep();
    }
   */

}
