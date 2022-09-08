/// здесь скетч по заупску шагового двигателя с редуктором и его драйвером из набора ардуино
/// сделан полный шаг и полушаг - микрошаг надо думать как делать


//#define dl 2   // это настройка скорости в милисекундах - меньше 2 милисекунд нельзя - иначе в полном шаге мотор не будет работать

void setup() {
  // put your setup code here, to run once:

  pinMode(8, OUTPUT);    // настраиваем ноги как на схеме
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(8, LOW);  // по старту все ноги в ноль - чтобы убрать питание с драйвера  и мотора
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  Serial.begin(9600);
  delay(300);
  Serial.println("Hello ROBOTOTECHNIKA! lets test stepper motor here! ");





}


//==== шаговый режим======
void leftF(int dl = 2000)
{
  digitalWrite(8,  1);
  digitalWrite(9,  0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delayMicroseconds(dl);

  digitalWrite(8,  1);
  digitalWrite(9,  1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delayMicroseconds(dl);

  digitalWrite(8,  0);
  digitalWrite(9,  1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delayMicroseconds(dl);

  digitalWrite(8,  0);
  digitalWrite(9,  0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delayMicroseconds(dl);
}

void rightF(int dl = 2000)
{
  digitalWrite(8,  0);
  digitalWrite(9,  0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delayMicroseconds(dl);

  digitalWrite(8,  0);
  digitalWrite(9,  1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delayMicroseconds(dl);

  digitalWrite(8,  1);
  digitalWrite(9,  1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delayMicroseconds(dl);

  digitalWrite(8,  1);
  digitalWrite(9,  0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delayMicroseconds(dl);
}


//==== полушаговый режим======


void left(int dl = 2)
{
  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(dl);

  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 1);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(dl);

}



void right(int dl = 2)
{

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 1);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(dl);

  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(dl);
}




void loop() {
  // put your main code here, to run repeatedly:
  int i;
  while(1){
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
  
  
  while (1) {
    Serial.println("max speed test");
    for (int i = 2000; i > 1300; i = i - 50) {
      Serial.println("speed => " + String(i));
      for (int j = 0 ; j < 128; j++)
      {
        leftF(i);
      }
    }
  }


  while (0)
  {

    Serial.println("Full step test left side...");

    for ( i = 0 ; i < 512; i++)
    {
      leftF(2);
    }

    Serial.println("Full step test right side...");

    for ( i = 0 ; i < 512; i++)
    {
      rightF(2);
    }

    Serial.println("Half step test left side...");
    for ( i = 0 ; i < 512; i++)
    {
      left(2);
    }

    Serial.println("Half step test right side...");

    for ( i = 0 ; i < 512; i++)
    {
      right(2);
    }

  }

}
