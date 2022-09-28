#pragma once
#include <iostream>
/*
это более корректна€ и полна€ верси€. рекомендуетьс€ использовать именно эту версию
здесь представлены все основные функции в классе WT901
класс WT901 относитс€ к WT901C485

!!!!!Ќ≈ ¬—≈ ‘”Ќ ÷»» –≈јЋ»«ќ¬јЌџ!!!!!

*/
using namespace std;






class WT901INC
{
public:
	WT901INC();

	// настройка
		/*
		скорость передачи (осторожно!)
		1 : 4800
		2 : 9600
		3 : 19200
		4 : 38400
		5 : 57600
		6 : 115200
		7 : 230400
		8 : 460800
		9 : 921600
		*/
	uint8_t setBaud(uint8_t);
		/*
		выдержка из даташита
		Assume the device address is 0x50. If you want to change the baud rate from 9600 to 115200. 
		Step 1.First you should send unlock command to your device use baud rate 9600.
		the unlock command is write 0xB588 to register 0x69. Send: 0x50 06 00 69 B5 88 22 A1
		2. Send baud rate set command:
		Send: 0x50 0x06 0x00 0x04 0x00 0x06 0x45 0x88
		3. Then change your master deviceТs baud rate to 115200.
		4. Send save config command:
		0x50 06 00 00 00 00 84 4B
		*/



	uint16_t readRegister();
	uint8_t writeRegister(uint8_t reg); // !warning!

	Vec3<uint16_t> getAccel(); // Acceleration
	Vec3<uint16_t> getGyro(); // gyroscope
	Vec3<uint16_t> getMagnetic(); // magnitometr
	Vec3<uint16_t> getPYR(); // pitch yaw roll


	// получение|установка даты|времени, 
	// год хранитьс€ в том же регистре что и мес€ц 
	// и занимает 8 (младших) бит в 16и битном регистре

	uint8_t getYear();
	uint8_t getMount();
	uint8_t getHour();
	uint8_t getDay();
	uint8_t getSecond();
	uint8_t getMinute();
	uint16_t getMillis();

	uint8_t setYear(uint8_t year);
	uint8_t setMount(uint8_t mount);
	uint8_t setHour(uint8_t hour);
	uint8_t setDay(uint8_t day);
	uint8_t setSec(uint8_t sec);
	uint8_t setMin(uint8_t Min);
	uint16_t setMillis(uint16_t ms);

	uint8_t sendBytes(uint8_t* msg, uint8_t length); // !Warning!
private:
	void _getBytes();
	uint16_t _getByte();
	inline void _send(uint8_t byte);
};
