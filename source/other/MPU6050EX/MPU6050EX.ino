#include "I2Cdev.h"
#include "MPU6050.h"

#define TO_DEG 57.2957f
#define TIME_OUT 20

MPU6050 accgyro;

template <class t>
class vec3 {
  public:
    t _x, _y, _z;
    vec3() {
      _x = 0;
      _y = 0;
      _z = 0;
    }
    vec3(t x, t y, t z) {
      _x = x;
      _y = y;
      _z = z;
    }
};

float toDeg(int angle) {
  return angle / 32768.0 * 180;
}

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
  angleY = toDeg(ay);
  //angleY = ay / 32768.0 * 180;
  angleZ = az / 32768.0 * 180;
  vec3 pos = vec3<float>(angleX, angleY, angleZ); // vec3 object
  Serial.print(pos._x);
  Serial.print("\t");
  Serial.print(pos._y);
  Serial.print("\t");
  Serial.print(pos._z);
  Serial.print("\t");
  
//  Serial.print(angleX);
//  Serial.print("\t");
//  Serial.print(angleY);
//  Serial.print("\t");
//  Serial.print(angleZ);
//  Serial.print("\t");
  Serial.println("");


  //  angleX = gx / 32768.0 * 2;
  //  angleY = gy / 32768.0 * 2;
  //  angleZ = gz / 32768.0 * 2;
  //  Serial.print(angleX);
  //  Serial.print("\t");
  //  Serial.print(angleY);
  //  Serial.print("\t");
  //  Serial.print(angleZ);
  //  Serial.println("\t");
  delay(20);
}
