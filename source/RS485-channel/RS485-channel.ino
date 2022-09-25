
#include <SoftwareSerial.h>

byte msg[] = { 0x50, 0x03, 0x00, 0x3d, 0x00, 0x03, 0x99, 0x86};

//Buff(0) = $50
//Buff(1) = $03
//Buff(2) = $00
//Buff(3) = $3D
//Buff(4) = $00
//Buff(5) = $03
//Buff(6) = $99
//Buff(7) = $86


SoftwareSerial sensor(2, 3); // RX, TX
long tm;


void setup() {
  Serial.begin(115200);
  Serial.println("Steel alive");
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(5, 0);
  digitalWrite(8, 0);
  Serial.println("boot...");
  delay(1000);

  sensor.begin(115200);
  Serial.println("Software Serial (sensor) -> UP\n");
  tm = millis();
  delay(1000);
}

int cnt;
String out;
void loop() {
  t();

  //  while (1) {
  //    out = "";
  //    if (Serial.available()) {
  //      cnt = Serial.available();
  //      for (int i = 0; i < cnt; i++) {
  //        out += "\t0x" + String(Serial.read(), HEX);
  //      }
  //      sensor.println("data recieve " + String(cnt) + " bytes: " + out);
  //    } else {
  //      digitalWrite(5, 1);
  //      Serial.write(msg, 8);
  //      digitalWrite(5, 0);
  //      delay(20);
  //      sensor.print("ByteSend ");
  //    }
  //    delay(10);
  //  }

  //  out = "";
  //  cnt = 0;
  //  while (!sensor.available()) {
  //    digitalWrite(5, 1);
  //    sensor.write(msg, 8);
  //    digitalWrite(5, 0);
  //    delay(20);
  //  }
  //
  //  cnt = sensor.available();
  //  for (int i = 0; i < cnt; i++) {
  //    out += "\t0x" + String(sensor.read(), HEX);
  //  }
  //  Serial.println("data recieve:\t" + out);

}

void t() {
//  if (sensor.available()) {
//    Serial.print('\t');
//    Serial.print(sensor.read(), HEX);
//    delay(5);
//  }//and millis() - tm > 200
//  if (!sensor.available() ) {
//    tm = millis();
    //    Serial.print("\nSend bytes...");
//    Serial.print('\n');
    digitalWrite(5, 1);
    digitalWrite(8, 1);
    sensor.write(msg, 8);
    digitalWrite(5, 0);
    digitalWrite(8, 0);
    delay(5);
//  }
}
