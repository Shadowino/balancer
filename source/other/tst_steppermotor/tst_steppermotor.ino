/// здесь скетч по заупску шагового двигателя с редуктором и его драйвером из набора ардуино
/// сделан полный шаг и полушаг - микрошаг надо думать как делать 


#define PPZ 2   // это настройка скорости в милисекундах - меньше 2 милисекунд нельзя - иначе в полном шаге мотор не будет работать

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

void leftF()
{
  digitalWrite(8,  1);
  digitalWrite(9,  0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(PPZ);

  digitalWrite(8,  1);
  digitalWrite(9,  1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8,  0);
  digitalWrite(9,  1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8,  0);
  digitalWrite(9,  0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delay(PPZ);

}

void rightF()
{
  digitalWrite(8,  0);
  digitalWrite(9,  0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delay(PPZ);
  digitalWrite(8,  0);
  digitalWrite(9,  1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8,  1);
  digitalWrite(9,  1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ); 

  digitalWrite(8,  1);
  digitalWrite(9,  0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(PPZ);
  
  
}


//==== полушаговый режим======


void left()
{
  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(PPZ);

  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 1);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);


  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(PPZ);


  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delay(PPZ);



  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(PPZ);






}



void right()
{

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(PPZ);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 1);
  delay(PPZ);

  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 1);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 0);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 1);
  digitalWrite(9, 1);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 0);
  delay(PPZ);

  digitalWrite(8, 1);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  delay(PPZ);
}




void loop() {
  // put your main code here, to run repeatedly:
int i;


  while (1)
  {

    Serial.println("Full step test left side...");

    
    for ( i = 0 ; i < 512; i++)
    {
      leftF();
    }
    
    Serial.println("Full step test right side...");
    
    for ( i = 0 ; i < 512; i++)
    {

    }

    Serial.println("Half step test left side...");
    for ( i = 0 ; i < 512; i++)
    {
      left();
    }

    Serial.println("Half step test right side...");

    for ( i = 0 ; i < 512; i++)
    {
      right();
    }

  }

}




