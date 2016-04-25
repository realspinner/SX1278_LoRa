#ifndef __SX1278_H__
#define __SX1278_H__

#include <stdint.h>
#include <stdbool.h>

#define SX1278_MAX_PACKET	256
#define SX1278_DEFAULT_TIMEOUT		3000

//Error Coding rate (CR)setting
#define CR_4_5
//#define CR_4_6
//#define CR_4_7
//#define CR_4_8
#ifdef   CR_4_5
	#define CR	0x01
#else
	#ifdef   CR_4_6
		#define CR    0x02
	#else
		#ifdef   CR_4_7
			#define CR    0x03
		#else
			#ifdef   CR_4_8
				#define CR    0x04
			#endif
		#endif
	#endif
#endif
//CRC Enable
#define CRC_EN
#ifdef  CRC_EN
  #define CRC   0x01
#else
  #define CRC   0x00
#endif
//RFM98 Internal registers Address
/********************LoRa mode***************************/
#define LR_RegFifo                                  0x00
// Common settings
#define LR_RegOpMode                                0x01
#define LR_RegFrMsb                                 0x06
#define LR_RegFrMid                                 0x07
#define LR_RegFrLsb                                 0x08
// Tx settings
#define LR_RegPaConfig                              0x09
#define LR_RegPaRamp                                0x0A
#define LR_RegOcp                                   0x0B
// Rx settings
#define LR_RegLna                                   0x0C
// LoRa registers
#define LR_RegFifoAddrPtr                           0x0D
#define LR_RegFifoTxBaseAddr                        0x0E
#define LR_RegFifoRxBaseAddr                        0x0F
#define LR_RegFifoRxCurrentaddr                     0x10
#define LR_RegIrqFlagsMask                          0x11
#define LR_RegIrqFlags                              0x12
#define LR_RegRxNbBytes                             0x13
#define LR_RegRxHeaderCntValueMsb                   0x14
#define LR_RegRxHeaderCntValueLsb                   0x15
#define LR_RegRxPacketCntValueMsb                   0x16
#define LR_RegRxPacketCntValueLsb                   0x17
#define LR_RegModemStat                             0x18
#define LR_RegPktSnrValue                           0x19
#define LR_RegPktRssiValue                          0x1A
#define LR_RegRssiValue                             0x1B
#define LR_RegHopChannel                            0x1C
#define LR_RegModemConfig1                          0x1D
#define LR_RegModemConfig2                          0x1E
#define LR_RegSymbTimeoutLsb                        0x1F
#define LR_RegPreambleMsb                           0x20
#define LR_RegPreambleLsb                           0x21
#define LR_RegPayloadLength                         0x22
#define LR_RegMaxPayloadLength                      0x23
#define LR_RegHopPeriod                             0x24
#define LR_RegFifoRxByteAddr                        0x25
// I/O settings
#define REG_LR_DIOMAPPING1                          0x40
#define REG_LR_DIOMAPPING2                          0x41
// Version
#define REG_LR_VERSION                              0x42
// Additional settings
#define REG_LR_PLLHOP                               0x44
#define REG_LR_TCXO                                 0x4B
#define REG_LR_PADAC                                0x4D
#define REG_LR_FORMERTEMP                           0x5B
#define REG_LR_AGCREF                               0x61
#define REG_LR_AGCTHRESH1                           0x62
#define REG_LR_AGCTHRESH2                           0x63
#define REG_LR_AGCTHRESH3                           0x64

/********************FSK/ook mode***************************/
#define  RegFIFO                0x00
#define  RegOpMode              0x01
#define  RegBitRateMsb      	0x02
#define  RegBitRateLsb      	0x03
#define  RegFdevMsb             0x04
#define  RegFdevLsb             0x05
#define  RegFreqMsb             0x06
#define  RegFreqMid             0x07
#define  RegFreqLsb         	0x08
#define  RegPaConfig            0x09
#define  RegPaRamp              0x0a
#define  RegOcp                 0x0b
#define  RegLna                 0x0c
#define  RegRxConfig            0x0d
#define  RegRssiConfig      	0x0e
#define  RegRssiCollision 		0x0f
#define  RegRssiThresh      	0x10
#define  RegRssiValue           0x11
#define  RegRxBw                0x12
#define  RegAfcBw               0x13
#define  RegOokPeak             0x14
#define  RegOokFix              0x15
#define  RegOokAvg              0x16
#define  RegAfcFei              0x1a
#define  RegAfcMsb              0x1b
#define  RegAfcLsb              0x1c
#define  RegFeiMsb              0x1d
#define  RegFeiLsb              0x1e
#define  RegPreambleDetect  	0x1f
#define  RegRxTimeout1      	0x20
#define  RegRxTimeout2      	0x21
#define  RegRxTimeout3      	0x22
#define  RegRxDelay             0x23
#define  RegOsc                 0x24
#define  RegPreambleMsb     	0x25
#define  RegPreambleLsb     	0x26
#define  RegSyncConfig      	0x27
#define  RegSyncValue1      	0x28
#define  RegSyncValue2      	0x29
#define  RegSyncValue3      	0x2a
#define  RegSyncValue4      	0x2b
#define  RegSyncValue5      	0x2c
#define  RegSyncValue6      	0x2d
#define  RegSyncValue7      	0x2e
#define  RegSyncValue8      	0x2f
#define  RegPacketConfig1       0x30
#define  RegPacketConfig2       0x31
#define  RegPayloadLength       0x32
#define  RegNodeAdrs            0x33
#define  RegBroadcastAdrs       0x34
#define  RegFifoThresh      	0x35
#define  RegSeqConfig1      	0x36
#define  RegSeqConfig2      	0x37
#define  RegTimerResol      	0x38
#define  RegTimer1Coef      	0x39
#define  RegTimer2Coef      	0x3a
#define  RegImageCal            0x3b
#define  RegTemp                0x3c
#define  RegLowBat              0x3d
#define  RegIrqFlags1           0x3e
#define  RegIrqFlags2           0x3f
#define  RegDioMapping1			0x40
#define  RegDioMapping2			0x41
#define  RegVersion				0x42
#define  RegPllHop				0x44
#define  RegPaDac				0x4d
#define  RegBitRateFrac			0x5d

/**********************************************************
**Parameter table define
**********************************************************/
#define SX1278_433MHZ			0

static const uint8_t SX1278_Frequency[1][3] = {
		{0x6C, 0x80, 0x00}, //434MHz
};


#define SX1278_POWER_20DBM		0
#define SX1278_POWER_17DBM		1
#define SX1278_POWER_14DBM		2
#define SX1278_POWER_11DBM		3

static const uint8_t SX1278_Power[4] = {
		0xFF, //20dbm
		0xFC, //17dbm
		0xF9, //14dbm
		0xF6, //11dbm
};

#define SX1278_LORA_SF_6		0
#define SX1278_LORA_SF_7		1
#define SX1278_LORA_SF_8		2
#define SX1278_LORA_SF_9		3
#define SX1278_LORA_SF_10		4
#define SX1278_LORA_SF_11		5
#define SX1278_LORA_SF_12		6

static const uint8_t SX1278_SpreadFactor[7] = {
		6,
		7,
		8,
		9,
		10,
		11,
		12
};


#define	SX1278_LORA_BW_7_8KHZ		0
#define	SX1278_LORA_BW_10_4KHZ		1
#define	SX1278_LORA_BW_15_6KHZ		2
#define	SX1278_LORA_BW_20_8KHZ		3
#define	SX1278_LORA_BW_31_2KHZ		4
#define	SX1278_LORA_BW_41_7KHZ		5
#define	SX1278_LORA_BW_62_5KHZ		6
#define	SX1278_LORA_BW_125KHZ		7
#define	SX1278_LORA_BW_250KHZ		8
#define	SX1278_LORA_BW_500KHZ		9

static const uint8_t SX1278_LoRaBandwidth[10] =
{
		0, //   7.8KHz,
		1, //  10.4KHz,
		2, //  15.6KHz,
		3, //  20.8KHz,
		4, //  31.2KHz,
		5, //  41.7KHz,
		6, //  62.5KHz,
		7, // 125.0KHz,
		8, // 250.0KHz,
		9  // 500.0KHz
};

typedef enum _SX1278_STATUS {
	SLEEP,
	STANDBY,
	TX,
	RX
} SX1278_Status_t;



class SX1278_Base {
protected:
	uint8_t frequency;
	uint8_t power;
	uint8_t LoRa_Rate;
	uint8_t LoRa_BW;
	uint8_t packetLength;

	SX1278_Status_t status;


	//HAL Functions
	virtual void HAL_SetNSS(bool value) = 0;
	virtual void HAL_SetReset(bool value) = 0;
	virtual void HAL_SPICommand(uint8_t cmd) = 0;
	virtual uint8_t HAL_SPIReadByte() = 0;
	virtual void HAL_Delay(uint32_t msec) = 0;
	virtual bool HAL_GetDIO0() = 0;

	uint8_t rxBuffer[SX1278_MAX_PACKET];
	uint8_t readBytes;


	// Common Private Functions
	uint8_t SPIRead(uint8_t addr);
	void SPIWrite(uint8_t addr, uint8_t cmd);
	void SPIBurstRead(uint8_t addr, uint8_t *rxBuf, uint8_t length);
	void SPIBurstWrite(uint8_t addr, uint8_t *txBuf, uint8_t length);
	void DIO0_InterruptHandler();


	void config(uint8_t frequency, uint8_t power, uint8_t LoRa_Rate, uint8_t LoRa_BW);
	void defaultConfig();
	void reset();

	void standby();
	void sleep();
	void entryLoRa();
	void clearLoRaIrq();
	bool LoRaEntryRx(uint8_t length, uint32_t timeout = SX1278_DEFAULT_TIMEOUT);
	uint8_t LoRaRxPacket();
	bool LoRaEntryTx(uint8_t length, uint32_t timeout = SX1278_DEFAULT_TIMEOUT);
	bool LoRaTxPacket(uint8_t *txBuf, uint8_t length, uint32_t timeout = SX1278_DEFAULT_TIMEOUT);

public:
	virtual ~SX1278_Base() {};
	void begin(uint8_t frequency, uint8_t power, uint8_t LoRa_Rate, uint8_t LoRa_BW, uint8_t packetLength);

	bool transmit(uint8_t *txBuf, uint8_t length, uint32_t timeout = SX1278_DEFAULT_TIMEOUT);
	bool receive(uint8_t length, uint32_t timeout = SX1278_DEFAULT_TIMEOUT);
	uint8_t available();
	uint8_t read(uint8_t *rxBuf, uint8_t length);

	uint8_t RSSI_LoRa();
	uint8_t RSSI();
};

#endif