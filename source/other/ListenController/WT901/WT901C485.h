#pragma once
#include <iostream>
#include "WT901REG.h"
using namespace std;

enum Example { ANGLE, ACCEL, ANGULAR };


class WT901С485
{
public:

	WT901С485(uint8_t adress = 0x50) {
		_adress = adress;
	}

	void sendMsgEx(Example V) {
		if (V == ANGLE) {
			uint8_t m[] = { 0x50,0x03,0x00,0x3d,0x00,0x03,0x99,0x86 };
			sendBytes(m, 8);
		}
		else if (V == ACCEL) {
			uint8_t m[] = { 0x50,0x03,0x00,0x34,0x00,0x03,0x49,0x84 };
			sendBytes(m, 8);
		}
	}

	/// <summary>
	/// отправляет датчику 
	/// </summary>
	/// <param name="REG">Регистр который нужно прочитать</param>
	void readReg(uint8_t REG) {
		/*
		0x50 0x03 0x00 0x3d 0x00 0x03 0x99 0x86

		DevAddr	Read	RegH	RegL	regNumH	regNumL	CRCH	CRCL
		0x50	0x03	0x00	0x3d	0x00	0x03	0x99	0x86

		*/

		uint8_t m[8] = { _adress, 0x03, 0x00, REG, 0x00, 0x01 };
		sendBytes(m, 8);
	}

	uint16_t waitReadReg() {

	}

	/// <summary>
	/// отправляет сообщение датчику
	/// </summary>
	/// <param name="msg">указатель на массив byte для отправки</param>
	/// <param name="length">длина массива отправки</param>
	/// <returns>
	/// 
	/// </returns>
	uint8_t sendBytes(uint8_t* msg, uint8_t length) {
		// формат msg =>
		// byte# 	|0		|1		|2		|3		|4		|5		|6		|7		| 
		// val		|addr	|CMD	|0x00	|REGL	|0x00	|REGnum	|CRChigh|CRClow	| 
		for (uint8_t i = 0; i < length; i++) {
			_send(msg[i]);
		}

		return 1;
	}

	void CRC(uint8_t* msg) {
		// формат msg =>
		// byte# 	|0		|1		|2		|3		|4		|5		|6		|7		|
		// val		|addr	|CMD	|0x00	|REGL	|0x00	|REGnum	|CRChigh|CRClow	|
		// код здесь должен вычислять значение CRC
		// и записывать его в последние 2 байта массива msg (msg[6], msg[8])
		// я не знаю алгоритм CRC
	}

private:
	inline void _send(uint8_t byte) {
		/// TODO: write serial send bytes alg
		//send byte
		if (byte <= 0xf) {
			cout << "\t0x0" << std::hex << (uint16_t)byte << endl;
		}
		else {
			cout << "\t0x" << std::hex << (uint16_t)byte << endl;
		}
	}

	uint8_t _adress;
};

