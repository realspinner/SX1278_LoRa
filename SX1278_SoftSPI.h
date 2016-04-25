#ifndef __SX1278_SOFT_SPI_H__
#define __SX1278_SOFT_SPI_H__

#include "SX1278.h"
#include <stdlib.h>

typedef struct _SX1278_SoftSPI_Config_t {
	int	pin_NSS;
	int pin_CLK;
	int pin_MISO;
	int pin_MOSI;
	int pin_RESET;
	int pin_DIO0;
} SX1278_SoftSPI_Config_t;


class SX1278_SoftSPI : public SX1278_Base {
private:
	SX1278_SoftSPI(SX1278_SoftSPI_Config_t* hwConfig);

	static SX1278_SoftSPI* instance;

	SX1278_SoftSPI_Config_t hwConfig;

protected:
	void HAL_SetNSS(bool value);
	void HAL_SetReset(bool value);
	void HAL_SPICommand(uint8_t cmd);
	uint8_t HAL_SPIReadByte();
	void HAL_Delay(uint32_t msec);
	bool HAL_GetDIO0();

public:
	static SX1278_SoftSPI* getInstance(SX1278_SoftSPI_Config_t* hwConfig = NULL);
};



#endif
