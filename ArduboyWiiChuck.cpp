#include <Arduino.h>
#include "ArduboyWiiChuck.h"
#include <Wire.h>

void ArduboyWiiChuck::init() {
  Wire.begin();              
  Wire.beginTransmission(address);
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0xFB); //disable encryption
  Wire.write(0x00);
  Wire.endTransmission();
}

uint8_t ArduboyWiiChuck::poll() {
	Wire.requestFrom(address, 6);// request data from nunchuck
      
	int cnt = 0;
	while (Wire.available ()) {
		i2cBuffer[cnt] = Wire.read();
		cnt++;
	}	
      
	// send request for next data payload
	Wire.beginTransmission(address);
	Wire.write(0x00); 
	Wire.endTransmission();
	return (cnt >= 5) ? 1: 0;
}