
//==== шаговый режим с оптимизацией======
void leftM(int dl = 2000)
{
  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);

  digitalWrite(STB, 1);
  digitalWrite(STD, 0);
  delayMicroseconds(dl);

  digitalWrite(STA, 0);
  digitalWrite(STC, 1);
  delayMicroseconds(dl);

  digitalWrite(STB, 0);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);
}

void rightM(int dl = 2000)
{
  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);

  digitalWrite(STB, 1);
  digitalWrite(STD, 0);
  delayMicroseconds(dl);

  digitalWrite(STA, 1);
  digitalWrite(STC, 0);
  delayMicroseconds(dl);

  digitalWrite(STB, 0);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);
}

//==== шаговый режим======
void leftF(int dl = 2000)
{
  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 1);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delayMicroseconds(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 1);
  digitalWrite(STC, 1);
  digitalWrite(STD, 0);
  delayMicroseconds(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);
}

void rightF(int dl = 2000)
{
  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 1);
  digitalWrite(STC, 1);
  digitalWrite(STD, 0);
  delayMicroseconds(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 1);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delayMicroseconds(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delayMicroseconds(dl);
}


//==== полушаговый режим======


void left(int dl = 2)
{
  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delay(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 1);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 1);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 1);
  digitalWrite(STC, 1);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 1);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delay(dl);

}



void right(int dl = 2)
{

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 1);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 0);
  digitalWrite(STC, 1);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 1);
  digitalWrite(STC, 1);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 0);
  digitalWrite(STB, 1);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 1);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 0);
  delay(dl);

  digitalWrite(STA, 1);
  digitalWrite(STB, 0);
  digitalWrite(STC, 0);
  digitalWrite(STD, 1);
  delay(dl);
}
