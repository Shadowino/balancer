#include <iostream>
#include "SodtwareSerial.h"
#include <string>
using namespace std;
SodtwareSerial Serial(0, 1);
uint8_t DDRB, DDRC, DDRD;
uint8_t PORTB, PORTC, PORTD;
void setup();
void loop();
void main() {
	setup();
	while (1) { loop(); }
}

// ---------------- begin ----------------
// params
#define WAITHSACT false		// wait hardware serial activate
#define WAITSSACT false		// wait software serial activate
#define HSSPEED 115200		// hardware serial speed
#define SSSPEED 115200		// software serial speed
#define USESSDEB true		// sofwareSerial debug
#define TODEG 182.041
#define STPTRIG 183*5		// step trigger 182.041(6) default
#define STPSPD 35			// Step time / 2
#define STPCYCLE 3200		// step for full cycle
#define AXINVERS true
#define AYINVERS true
#define AZINVERS true
// pinout (use PD 0~7 and PB 8~13 like PD0~7 and PB0~5)
// MX*** -> motorX controller
// MY*** -> motorY controller
// DB*   -> DeBug channel
// SS**   -> Software Serial RX TX
#define MXSTP 0
#define MXDIR 0
#define MXENA 0
#define DBA
#define SSRX 2
#define SSTX 3

bool checkCRC(uint8_t* buff) { return true; } // check CRC code! always true ;)
SodtwareSerial debug(SSRX, SSTX);

void setup() {
	Serial.begin(HSSPEED);
	Serial.setTimeout(5);
	while (!Serial and WAITHSACT) {}
	debug.begin(SSSPEED);
	debug.setTimeout(5);
	while (!Serial and WAITHSACT) {}

	// use DDRB = 0b00000111
	DDRB |= (1 << MXSTP);
	DDRB |= (1 << MXDIR);
	DDRB |= (1 << MXENA);
	// use DDRB = 0b00000111
	PORTB &= ~(1 << MXSTP);
	PORTB &= ~(1 << MXDIR);
	PORTB &= ~(1 << MXENA);
	// _delay_us(100);
}
uint8_t recv[10];
string message;
// math data, using X only :(
int GX, GY, GZ; // gyro values
int PY, PZ; // position defoult values
int PX = 999; // !testing!
void loop() {
	while (true)
	{

	}

	while (Serial.available()) {
		if (Serial.read() != 0x50) { // check first byte
			break; // repeat if check fail
		}
		if (Serial.read() != 0x03) { // check second byte
			break; // repeat if check fail
		}
		if (Serial.read() == 0x03) {
			Serial.readBytes(recv, 8); // read message
			if (!checkCRC(recv)) break; // repeat if CRC check fail
			GX = (recv[1] | (recv[0] << 8)); // get XYZ value
			GY = (recv[3] | (recv[2] << 8)); 
			GZ = (recv[5] | (recv[4] << 8));
			if (USESSDEB) { // print to SoftwareSerial 
				message += "X:\t" + String(GX / TODEG);
				message += "\tY:\t" + String(GY / TODEG);
				message += "\tZ:\t" + String(GZ / TODEG);
				debug.println(message);
			}
			if (PX == 999) PX = GX; // calibration
			if (abs(PX - GX) > STPTRIG) { // check Step trigger
				// 0b0101 xor 0b0011;
				if (PX - GX > 0 xor AXINVERS) PORTB |= (1 << MXDIR);
				else PORTB &= ~(1 << MXDIR);
				PORTB |= (1 << MXSTP);
				delayMicroseconds(STPSPD);
				PORTB &= ~(1 << MXSTP);
				delayMicroseconds(STPSPD);
			}
		} else {
			Serial.readBytes(recv, 5); // read extra bytes
		}

	}

}