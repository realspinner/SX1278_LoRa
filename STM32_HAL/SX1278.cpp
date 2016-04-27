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
}

void SX1278::HAL_init(){
	this->HAL_SetNSS(true);
	HAL_GPIO_WritePin(this->hwConfig.pin_RESET.port,
			          this->hwConfig.pin_RESET.pin,
					  GPIO_PIN_SET);
}


inline void SX1278::HAL_SetNSS(bool value) {
	HAL_GPIO_WritePin(this->hwConfig.pin_NSS.port,
			          this->hwConfig.pin_NSS.pin,
					  (value == true) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void SX1278::HAL_Reset() {
	this->HAL_SetNSS(true);
	HAL_GPIO_WritePin(this->hwConfig.pin_RESET.port,
			          this->hwConfig.pin_RESET.pin,
					  GPIO_PIN_RESET);

	this->HAL_DelayMs(1);

	HAL_GPIO_WritePin(this->hwConfig.pin_RESET.port,
			          this->hwConfig.pin_RESET.pin,
					  GPIO_PIN_SET);

	this->HAL_DelayMs(100);
}

void SX1278::HAL_SPICommand(uint8_t cmd) {
	this->HAL_SetNSS(false);
	HAL_SPI_Transmit(this->hwConfig.spi, &cmd, 1, 1000);
	while (HAL_SPI_GetState(this->hwConfig.spi) != HAL_SPI_STATE_READY);
}

uint8_t SX1278::HAL_SPIReadByte() {
	uint8_t txByte = 0x00;
	uint8_t rxByte = 0x00;

	this->HAL_SetNSS(false);
	HAL_SPI_TransmitReceive(this->hwConfig.spi, &txByte, &rxByte, 1, 1000);
	while(HAL_SPI_GetState(this->hwConfig.spi) != HAL_SPI_STATE_READY);
	return rxByte;
}

void SX1278::HAL_DelayMs(uint32_t msec) {
	HAL_Delay(msec);
}

bool SX1278::HAL_GetDIO0() {
	return (HAL_GPIO_ReadPin(this->hwConfig.pin_DIO0.port, this->hwConfig.pin_DIO0.pin) == GPIO_PIN_SET);
}

void SX1278::SPIBurstRead(uint8_t addr, uint8_t* rxBuf, uint8_t length) {
	  if(length <= 1){
		  return;
	  } else {
		 memset(rxBuf, 0x00, length);
	     this->HAL_SetNSS(false);
	     this->HAL_SPICommand(addr);
	     HAL_SPI_Receive(this->hwConfig.spi, rxBuf, length, 1000);
	     while(HAL_SPI_GetState(this->hwConfig.spi) != HAL_SPI_STATE_READY);
	     this->HAL_SetNSS(true);
	  }
}
