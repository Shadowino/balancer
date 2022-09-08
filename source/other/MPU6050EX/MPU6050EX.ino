#include "I2Cdev.h"
#include "MPU6050.h"

#define TO_DEG 57.2957f
#define TIME_OUT 20

MPU6050 accgyro;


void setup() {
  Serial.begin(9600);
  // инициализация датчика
  accgyro.initialize();
}
float angleX, angleY, angleZ;
int16_t ax, ay, az, gx, gy, gz;
void loop() {
  accgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  angleX = ax / 32768.0 * 180;
  angleY = ay / 32768.0 * 180;
  angleZ = az / 32768.0 * 180;
  //  Serial.print(ax);
  //  Serial.print(" = ");
  Serial.print("\t");
  Serial.print(angleX);
  Serial.print("\t");
  Serial.print(angleY);
//  Serial.print(angleZ);
//  Serial.print("\t");
  Serial.println("");


//  angleX = gx / 32768.0 * 2;
//  angleY = gy / 32768.0 * 2;
//  angleZ = gz / 32768.0 * 2;
//  //  Serial.print(ax);
//  //  Serial.print(" = ");
//  Serial.print(angleX);
//  Serial.print("\t");
//  Serial.print(angleY);
//  Serial.print("\t");
//  Serial.print(angleZ);
//  Serial.println("\t");
  delay(20);
}
