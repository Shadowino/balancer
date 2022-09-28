#pragma once

/*
код определений имен и адресов регистров для WT901C485
все данные взяты из даташита

*/

//setting Registers
#define SAVE 0x00
#define CALSW 0x01
#define BAUD 0x04

// bias (offset)
#define AXOFFSET 0x05 //X axis Acceleration bias
#define AYOFFSET 0x06 //Y axis Acceleration bias
#define AZOFFSET 0x07 //Z axis Acceleration bias
#define GXOFFSET 0x08 //X axis angular velocity bias
#define GYOFFSET 0x09 //Y axis angular velocity bias
#define GZOFFSET 0x0a //Z axis angular velocity bias
#define HXOFFSET 0x0b //X axis Magnetic bias
#define HYOFFSET 0x0c //Y axis Magnetic bias
#define HZOFFSET 0x0d //Z axis Magnetic bias

//other
#define ADDR 0x1a //Modbus address
#define MMYY 0x30 //Month, Year
#define HHDD 0x31 //Hour, Day
#define SSMM 0x32 //Second, Minute
#define MS 0x33   //Millisecond

// XYZ sensor
#define AX 0x34 //X axis Acceleration
#define AY 0x35 //Y axis Acceleration
#define AZ 0x36 //Z axis Acceleration
#define GX 0x37 //X axis angular velocity
#define GY 0x38 //Y axis angular velocity
#define GZ 0x39 //Z axis angular velocity
#define HX 0x3a //X axis Magnetic
#define HY 0x3b //Y axis Magnetic
#define HZ 0x3c //Z axis Magnetic
#define Roll 0x3d //X axis Angle
#define Pitch 0x3e //Y axis Angle
#define Yaw 0x3f //Z axis Angle
#define TEMP 0x40 //Temperature

//Quaternion
#define Q0 0x51 //Quaternion Q0
#define Q1 0x52 //Quaternion Q1
#define Q2 0x53 //Quaternion Q2
#define Q3 0x54 //Quaternion Q3