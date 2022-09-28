#pragma once
#include <iostream>
#include "WT901REG.h"
using namespace std;

enum Example { ANGLE, ACCEL, ANGULAR };


class WT901�485
{
public:

	WT901�485(uint8_t adress = 0x50) {
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
	/// ���������� ������� 
	/// </summary>
	/// <param name="REG">������� ������� ����� ���������</param>
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
	/// ���������� ��������� �������
	/// </summary>
	/// <param name="msg">��������� �� ������ byte ��� ��������</param>
	/// <param name="length">����� ������� ��������</param>
	/// <returns>
	/// 
	/// </returns>
	uint8_t sendBytes(uint8_t* msg, uint8_t length) {
		// ������ msg =>
		// byte# 	|0		|1		|2		|3		|4		|5		|6		|7		| 
		// val		|addr	|CMD	|0x00	|REGL	|0x00	|REGnum	|CRChigh|CRClow	| 
		for (uint8_t i = 0; i < length; i++) {
			_send(msg[i]);
		}

		return 1;
	}

	void CRC(uint8_t* msg) {
		// ������ msg =>
		// byte# 	|0		|1		|2		|3		|4		|5		|6		|7		|
		// val		|addr	|CMD	|0x00	|REGL	|0x00	|REGnum	|CRChigh|CRClow	|
		// ��� ����� ������ ��������� �������� CRC
		// � ���������� ��� � ��������� 2 ����� ������� msg (msg[6], msg[8])
		// � �� ���� �������� CRC
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

