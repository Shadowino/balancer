#pragma once
#include "WT901REG.h"
/*
это более корректна€ и полна€ верси€. рекомендуетьс€ использовать именно эту версию
здесь представлены все основные функции в классе WT901
класс WT901 относитс€ к WT901C485

!!!!!Ќ≈ ¬—≈ ‘”Ќ ÷»» –≈јЋ»«ќ¬јЌџ!!!!!

*/
using namespace std;


// вспомогательный класс 3х мерного вектора
template<class t>
class Vec3
{
public:
	t x, y, z;
	Vec3(t X, t Y, t Z) {
		x = X;
		y = Y;
		z = Z;
	}
};


class WT901
{
public:
	uint8_t addr = 0x50;
	WT901() {}
	~WT901() {}

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
	void setBaud(uint8_t) {
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
	}



	void readRegister();
	void writeRegister(uint8_t reg); // !warning!

	void getAccel(); // Acceleration
	void getGyro(); // gyroscope
	void getMagnetic(); // magnitometr
	void getPYR() {
		// пример из даташита
		// 0x50 0x03 0x00 0x3d 0x00 0x03 0x99 0x86
		// расшифровка (не проверено!)
		// devA|read|regAdress|regNumber|CRC code |
		// 0x50|0x03|0x00 0x3d|0x00 0x03|0x99 0x86|
		uint8_t arr[8] = { addr,0x03,0x00,0x3d,0x00,0x03,0x99,0x86 };
		sendBytes(arr, 8);
		// стабилизирующа€ задержка?
		//_getBytes();

	}


	// получение|установка даты|времени, 
	// год хранитьс€ в том же регистре что и мес€ц 
	// и занимает 8 (младших) бит в 16и битном регистре


	void getYear();
	void getMount();
	void getHour();
	void getDay();
	void getSecond();
	void getMinute();
	uint16_t getMillis() {
		uint8_t reqwest[] = { addr, 0x03, 0x00, MS, 0x00, 0x01, 0x00, 0x00 };
		sendBytes(reqwest, 8);
		return _getByte();
	}

	void setYear(uint8_t year);
	void setMount(uint8_t mount);
	void setHour(uint8_t hour);
	void setDay(uint8_t day);
	void setSec(uint8_t sec);
	void setMin(uint8_t Min);
	void setMillis(uint16_t ms);

	void sendBytes(uint8_t* msg, uint8_t length) {
		for (uint8_t i = 0; i < length; i++) {
			_send(msg[i]);
		}
	}
private:
	/// <summary>
	/// принимает ответ от датчика
	/// </summary>
	uint8_t _getBytes(uint8_t* buff, int lenght) {
		//	формат ответа
		//	Read X Y Z angle:180∞ 90∞ 30∞ 
		//			|		|		|								|		|
		//	dev adr	| readF	|num %N	|data %N						|CRCh	|CRCl
		//	0x50	|0x03	|0x06	|0x80 0x00 0x40 0x00 0x15 0x55	|0x14	|0x49
		//	data%N может иметь разную длину, указанную в num %N (как кол-во байт) будет записано в answer

		/* while (Serial.available()) { // чтение байтов
		byte
		}
		*/

		/*
		// предположительно рабочий код  (*_*) (*^*) (Х Х)
		while (Serial.available() < 3){}
		uint8_t par[3];
		Serial.readBytes(par, 3);
		// par[2] -> кол-во байт ответа
		if (par[2] > lenght) return 0b0001;
		if (par[2] == 0) return 0b0010;
		Serial.readBytes(buff, par[2]);
		*/
		return 0;
	}

	uint16_t _getByte() {
		// формат ответа см в функции _getBytes()
		/*
		// !код arduino!
		while (!Serial.available()>=10){}
		uint8_t answer[10];
		for (int i = 0; i < 10;i++){
			answer[i] = Serial.read();
		}
		*/
		// случайный массив ответа
		uint8_t answer[] = { 0x50, 0x03, 0x06, 0x80, 0x00, 0x14, 0x49};

		uint8_t res = ((answer[3] << 8) + (answer[4]));
		return res;
	}

	inline void _send(uint8_t byte) {
		//cout << std::hex << (int)byte << endl;
	}
};
