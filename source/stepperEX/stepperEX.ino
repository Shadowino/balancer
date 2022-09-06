/*
  тестовая прошивка для работы с шаговым двигателем

*/
class stepper {
  public:
    stepper(int a, int b, int c, int d) {
      pin[0] = a;
      pin[1] = b;
      pin[2] = c;
      pin[3] = d;
      digitalWrite(pin[0], 0);  // по старту все ноги в ноль - чтобы убрать питание с драйвера  и мотора
      digitalWrite(pin[1], 0);
      digitalWrite(pin[2], 0);
      digitalWrite(pin[3], 0);
    }

    void forward(int dl) {
      digitalWrite(pin[0], 0);
      digitalWrite(pin[1], 0);
      digitalWrite(pin[2], 1);
      digitalWrite(pin[3], 1);
      delay(PPZ);
      digitalWrite(pin[0], 0);
      digitalWrite(pin[1], 1);
      digitalWrite(pin[2], 1);
      digitalWrite(pin[3], 0);
      delay(PPZ);
      digitalWrite(pin[0], 1);
      digitalWrite(pin[1], 1);
      digitalWrite(pin[2], 0);
      digitalWrite(pin[3], 0);
      delay(PPZ);
      digitalWrite(pin[0], 1);
      digitalWrite(pin[1], 0);
      digitalWrite(pin[2], 0);
      digitalWrite(pin[3], 1);
      delay(PPZ);
    }

    void backward(int dl) {

    }
  private:
    int pin[4];
};

void setup() {


}

void loop() {


}
