



int Butt = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);    // настраиваем ноги как на схеме
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  digitalWrite(8, 0);  // по старту все ноги в ноль - чтобы убрать питание с драйвера  и мотора
  digitalWrite(9, 0);
  digitalWrite(10, 0);
}





void loop() {
  // put your main code here, to run repeatedly:


  if (digitalRead(11) == 0)
  {
    delay(200);
    if (Butt == 0)
    {
      Butt = 1;
    }
    else
    {
      Butt = 0;
    }

  }



}





