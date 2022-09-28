#pragma once
#include <iostream>
#include <random>

class SodtwareSerial
{
public:
	SodtwareSerial(int RX, int TX) {}
	uint8_t read() { return rand() % 256; }
	int readBytes(uint8_t *buff,int numBytes) { return rand() % 256; }
	int available(){ return rand() % 256; }
	void begin(int baud) {}
	operator bool() { return true; }
	void setTimeout(int ms) {};
	template <class t>
	void print(t msg) {};
	template <class t>
	void println(t msg) {};
};

template<class t>
std::string String(t val) {
	return "str"
}

void delay(int ms) {};
void delayMicroseconds(int us) {};