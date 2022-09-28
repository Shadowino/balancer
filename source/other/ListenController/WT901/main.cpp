//#include <avr/io.h>
//#include <Arduino.h>
#include "WT901C485.h"
#include "WT901.h"
#include <iostream>

using namespace std;

void SB(uint8_t* msg, uint8_t lenght) {
	for (uint8_t i = 0; i < lenght; i++)
	{
		cout << msg[i];
	}
}

WT901С485 WW = WT901С485();
WT901 newWT = WT901();

uint8_t arr[] = "steel\nalive";


uint16_t ModRTU_CRC(uint8_t* buf, int len)
{
	uint16_t crc = 0xFFFF;

	for (int pos = 0; pos < len; pos++) {
		crc ^= (uint16_t)buf[pos];

		for (int i = 8; i != 0; i--) {
			if ((crc & 0x0001) != 0) {
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
				crc >>= 1;
		}
	}
	// Помните, что младший и старший байты поменяны местами, используйте соответственно (или переворачивайте)

	// перевернул ^)
	crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
	return crc;
}

//uint8_t m[] = { 0x50,0x03,0x00,0x3d,0x00,0x03,0x99,0x86 };
uint8_t m[] = { 0x50,0x03,0x00,0x3d,0x00,0x03};

Vec3<uint8_t> v = Vec3<uint8_t>(3, 4, 5);

int main() {
	cout << "steel alive\n";
	//WW.sendBytes(arr, sizeof arr);
	//WW.sendMsgEx(ACCEL);
	//WW.readReg(AX);
	cout << std::hex << ModRTU_CRC(m, 6);
	return 0;
}