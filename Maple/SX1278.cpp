#include "SX1278.h"
#include <string.h>

SX1278* SX1278::instance = NULL;

SX1278* SX1278::getInstance(SX1278_Config_t *hwConfig){
	if (SX1278::instance == NULL){
		if (hwConfig != NULL){
			SX1278::instance = new SX1278(hwConfig);
		}
	}
	return SX1278::instance;
}

SX1278::SX1278(SX1278_Config_t* hwConfig) {
	this->hwConfig = *(hwConfig);
	pinMode(this->hwConfig.pin_NSS, OUTPUT);
	digitalWrite(this->hwConfig.pin_NSS, HIGH);
	pinMode(this->hwConfig.pin_RESET, OUTPUT);
	digitalWrite(this->hwConfig.pin_RESET, HIGH);
	pinMode(this->hwConfig.pin_DIO0, INPUT);
}

void SX1278::HAL_init(){
	this->hwConfig.SPI->begin(SPI_562_500KHZ, MSBFIRST, SPI_MODE_0);
}


void SX1278::HAL_SetNSS(bool value) {
	digitalWrite(this->hwConfig.pin_NSS, (value) ? HIGH : LOW);
}

void SX1278::HAL_Reset() {
	digitalWrite(this->hwConfig.pin_NSS, HIGH);
	digitalWrite(this->hwConfig.pin_RESET, LOW);
	delay(1);
	digitalWrite(this->hwConfig.pin_RESET, HIGH);
	delay(100);
}

void SX1278::HAL_SPICommand(uint8_t cmd) {
	this->HAL_SetNSS(false);
	this->hwConfig.SPI->transfer(cmd);
}

uint8_t SX1278::HAL_SPIReadByte() {
	this->HAL_SetNSS(false);
	return this->hwConfig.SPI->transfer(0x00);
}

void SX1278::HAL_DelayMs(uint32_t msec) {
	delay(msec);
}

bool SX1278::HAL_GetDIO0() {
	return (digitalRead(this->hwConfig.pin_DIO0) == HIGH);
}
