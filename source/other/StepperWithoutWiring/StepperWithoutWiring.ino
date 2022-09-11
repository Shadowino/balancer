#include <avr/io.h>
#include <util/delay.h>

#define SPD 35

#define STP 0
#define DIR 1
#define ENA 2


int main() {
  DDRB |= 1 << STP;
  DDRB |= 1 << DIR;
  DDRB |= 1 << ENA;
  _delay_us(500);
  PORTB &= ~(1 << STP);
  PORTB &= ~(1 << DIR);
  PORTB &= ~(1 << ENA);
  _delay_ms(5000);
  while (1) {
    PORTB &= ~(1 << DIR);
    for (int i = 3200; i > 0; i--) {
      PORTB |= 1 << STP;
      _delay_us(SPD);
      PORTB &= ~(1 << STP);
      _delay_us(SPD);
    }
    _delay_ms(500);
//    PORTB |= 1 << DIR;
    for (int i = 3200; i > 0; i--) {
      PORTB |= 1 << STP;
      _delay_us(SPD);
      PORTB &= ~(1 << STP);
      _delay_us(SPD);
    }
    _delay_ms(500);
  }
  return 0;
}
