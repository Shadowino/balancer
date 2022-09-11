/*
  распиновка от похожего драйвера мотора (есть совпадения)
  ► EN — включение и выключение модуля (0 — включен, 5 В — выключен).
  ► M0, M1 и M2 — выбор режима микро шаг (смотрите таблицу ниже).
  ► RST — сброс драйвера.
  ► SLP — вывод включения спящего режима, если подтянуть его к низкому состоянию драйвер перейдет в спящий режим.
  ► STEP — управляющий вывод, при каждом положительном импульсе, двигатель делает шаг
  (в зависимости от настройки микро шага), чем быстрее импульсы, тем быстрее вращаться двигатель.
  ► DIR — управляющий вывод, если подать +5 В двигатель будет вращается
  по часовой стрелке, а если подать 0 В против часовой стрелки.
  ► VMOT & GND MOT — питание шагового двигателя двигателя от 8.2 до 45 В
  (обязательное наличие конденсатора на 100 мкФ). Так же, нет необходим
  ► B2, B1, A1, и A2 — подключение обмоток двигателя.
  ► FAULT — Выход включения защиты, если состояние «0», значит полевые транзисторы
  H-моста отключены в результате защиты от перегрузки по току или был перегрев.
  ► GND LOGIC — заземление микроконтроллера.
  Источник: https://altarena.ru/kak-podklyuchit-drayver-shagovogo-dvigatelya-k-kontrolleru/
*/

// класс motor для подключения множества моторов одновременно (на данный момент не работает!)
class motor {
  public:
    uint8_t _dir;
    uint8_t _stp;
    uint8_t _ena;
    motor(uint8_t directPin, uint8_t stepPin, uint8_t enablePin) {
      _dir = directPin;
      _stp = stepPin;
      _ena = enablePin;
    }
    void ini() {
      pinMode(_dir, OUTPUT);
      pinMode(_stp, OUTPUT);
      pinMode(_ena, OUTPUT);
    }
};

void ini() {
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(ENA, OUTPUT);
}

void left() {
  setDir(0);
  doStep();
  delay(1);
}

void right() {
  setDir(1);
  doStep();
  delay(1);
}

void setDir(bool dir) {
  digitalWrite(DIR, (dir) ? 1 : 0);
}

void doStep() {
  digitalWrite(STP, 1);
  delay(1);
  digitalWrite(STP, 0);
}

void powerON() {
  digitalWrite(STP, 0);
}
void powerOFF() {
  digitalWrite(STP, 1);
}
