/// здесь скетч по заупску шагового двигателя с редуктором и его драйвером из набора ардуино
/// сделан полный шаг и полушаг - микрошаг надо думать как делать

#define STA 8
#define STB 9
#define STC 10
#define STD 11
//#define dl 2   // это настройка скорости в милисекундах - меньше 2 милисекунд нельзя - иначе в полном шаге мотор не будет работать

void leftF(int dl = 2000);
void rightF(int dl = 2000);
void leftM(int dl = 2000);
void rightM(int dl = 2000);
void left(int dl = 2);
void right(int dl = 2);

void setup() {
  // put your setup code here, to run once:
  
  pinMode(STA, OUTPUT);    // настраиваем ноги как на схеме
  pinMode(STB, OUTPUT);
  pinMode(STC, OUTPUT);
  pinMode(STD, OUTPUT);

  digitalWrite(STA, LOW);  // по старту все ноги в ноль - чтобы убрать питание с драйвера  и мотора
  digitalWrite(STB, LOW);
  digitalWrite(STC, LOW);
  digitalWrite(STD, LOW);

  Serial.begin(9600);
  delay(300);
  Serial.println("Hello ROBOTOTECHNIKA! lets test stepper motor here! ");

}







void loop() {
  // put your main code here, to run repeatedly:
  int i;
  while (0) {
    digitalWrite(8, 1);
    digitalWrite(9, 1);
    digitalWrite(10, 1);
    digitalWrite(11, 1);
    delay(1);
    digitalWrite(8, 0);
    digitalWrite(9, 0);
    digitalWrite(10, 0);
    digitalWrite(11, 0);
    delay(2);
    //      digitalWrite(9, 0);
    digitalWrite(8, 1);
    delay(2);
    digitalWrite(8, 0);
    digitalWrite(9, 1);
    delay(2);
    digitalWrite(9, 0);
    digitalWrite(10, 1);
    delay(2);
    digitalWrite(10, 0);
    digitalWrite(11, 1);
    delay(2);
  }


  while (0) {
    Serial.println("max speed test");
    for (int i = 2000; i > 1300; i = i - 50) {
      Serial.println("speed => " + String(i));
      for (int j = 0 ; j < 128; j++)
      {
        leftF(i);
      }
    }
  }

  int spd = 10000;
  while (1)
  {

    Serial.println("Full step test left side...");

    for ( i = 0 ; i < 256; i++)
    {
      leftF(spd);
    }

    Serial.println("Full step test right side...");

    for ( i = 0 ; i < 256; i++)
    {
      rightF(spd);
    }

    Serial.println("Half step test left side...");
    for ( i = 0 ; i < 256; i++)
    {
      left(5);
    }

    Serial.println("Half step test right side...");

    for ( i = 0 ; i < 256; i++)
    {
      right(5);
    }

  }
}
