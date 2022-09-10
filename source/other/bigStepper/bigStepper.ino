
#define DIR 8
#define STP 8
#define ENA 8

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

void loop() {
  /*
   // example using doStep() with setDir() 
    setDir(1);
    while(1){
      doStep();
    }
   */

}
