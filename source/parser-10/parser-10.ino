
#define PBon(x) PORTB |= (1 << x);
#define PBoff(x) PORTB &= ~(1 << x);
#define PDon(x) PORTD |= (1 << x);
#define PDoff(x) PORTD &= ~(1 << x);

// PINnum = BITnumber + 8
// BITnumber = PINnumber - 8
// пины(контакты) моторов указываються через BITnumber 
// и находяться на порту B
#define MXDIR 0 //8
#define MXSTP 1 //9
#define MXENA 2 //10

#define MXENA 3 //11
#define MXSTP 4 //12
#define MXDIR 5 //13

void setup() {


}

void loop() {


}
