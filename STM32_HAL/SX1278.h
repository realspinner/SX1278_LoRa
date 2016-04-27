#ifndef __SX1278_H__
#define __SX1278_H__

#include <SX1278_Base.h>
#include <stdlib.h>

//Change this to STM32 HAL library for your arch
#include <stm32f1xx_hal.h>
//#include <stm32f4xx_hal.h>


typedef struct _SX1278_GPIO_t {
	GPIO_TypeDef* port;
	uint16_t	  pin;
} SX1278_GPIO_t;


typedef struct _SX1278_Config_t {
	SX1278_GPIO_t pin_NSS;
	SX1278_GPIO_t pin_RESET;
	SX1278_GPIO_t pin_DIO0;
	SPI_HandleTypeDef* spi;
} SX1278_Config_t;


class SX1278 : public SX1278_Base {
private:
	SX1278(SX1278_Config_t* hwConfig);
	static SX1278* instance;

	SX1278_Config_t hwConfig;

protected:
	void HAL_init();
	void HAL_SetNSS(bool value);
	void HAL_Reset();
	void HAL_SPICommand(uint8_t cmd);
	uint8_t HAL_SPIReadByte();
	void HAL_DelayMs(uint32_t msec);
	bool HAL_GetDIO0();

	void SPIBurstRead(uint8_t addr, uint8_t *rxBuf, uint8_t length);

public:
	static SX1278* getInstance(SX1278_Config_t* hwConfig = NULL);
};

#endif
