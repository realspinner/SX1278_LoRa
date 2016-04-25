#include "SX1278_SoftSPI.h"
#include <Arduino.h>

SX1278_SoftSPI* SX1278_SoftSPI::instance = NULL;

SX1278_SoftSPI* SX1278_SoftSPI::getInstance(SX1278_SoftSPI_Config_t *hwConfig){
	if (SX1278_SoftSPI::instance == NULL){
		if (hwConfig != NULL){
			SX1278_SoftSPI::instance = new SX1278_SoftSPI(hwConfig);
		}
	}
	return SX1278_SoftSPI::instance;
}

SX1278_SoftSPI::SX1278_SoftSPI(SX1278_SoftSPI_Config_t* hwConfig) {
	this->hwConfig = *(hwConfig);
	pinMode(this->hwConfig.pin_NSS, OUTPUT);
	pinMode(this->hwConfig.pin_CLK,  OUTPUT);
	pinMode(this->hwConfig.pin_MOSI, OUTPUT);
	pinMode(this->hwConfig.pin_MISO, INPUT);
	pinMode(this->hwConfig.pin_RESET, OUTPUT);
	pinMode(this->hwConfig.pin_DIO0, INPUT);
}

void SX1278_SoftSPI::HAL_SetNSS(bool value) {
	digitalWrite(this->hwConfig.pin_NSS, (value) ? HIGH : LOW);
}

void SX1278_SoftSPI::HAL_SetReset(bool value) {
	digitalWrite(this->hwConfig.pin_RESET, (value) ? HIGH : LOW);
}

void SX1278_SoftSPI::HAL_SPICommand(uint8_t cmd) {
	uint8_t bits;
	digitalWrite(this->hwConfig.pin_CLK, LOW);
	this->HAL_SetNSS(false);

	for(bits = 8; bits != 0; bits--) {
	    digitalWrite(this->hwConfig.pin_CLK, LOW);
	    digitalWrite(this->hwConfig.pin_MOSI, (cmd & 0x80) ? HIGH : LOW);
	    digitalWrite(this->hwConfig.pin_CLK, HIGH);
	    cmd <<= 1;
	  }
	  digitalWrite(this->hwConfig.pin_CLK, LOW);
	  digitalWrite(this->hwConfig.pin_MOSI, HIGH);
}

uint8_t SX1278_SoftSPI::HAL_SPIReadByte() {
	uint8_t rdByte = 0;
	uint8_t bits;

	digitalWrite(this->hwConfig.pin_MOSI, HIGH);
	digitalWrite(this->hwConfig.pin_CLK, LOW);
	this->HAL_SetNSS(false);

	for(bits = 8; bits != 0; bits--) {
		rdByte <<= 1;
		digitalWrite(this->hwConfig.pin_CLK, HIGH);
		rdByte |= (digitalRead(this->hwConfig.pin_MISO) == HIGH) ? 0x01 : 0x00;
		digitalWrite(this->hwConfig.pin_CLK, LOW);
	}

	return rdByte;
}

void SX1278_SoftSPI::HAL_Delay(uint32_t msec) {
	delay(msec);
}

bool SX1278_SoftSPI::HAL_GetDIO0() {
	return (digitalRead(this->hwConfig.pin_DIO0) == HIGH);
}
