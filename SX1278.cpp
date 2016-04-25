#include "SX1278.h"
#include <string.h>

uint8_t SX1278_Base::SPIRead(uint8_t addr) {
	uint8_t tmp;
	this->HAL_SPICommand(addr);
	tmp = this->HAL_SPIReadByte();
	this->HAL_SetNSS(true);
	return tmp;
}

void SX1278_Base::SPIWrite(uint8_t addr, uint8_t cmd) {
	this->HAL_SetNSS(false);
	this->HAL_SPICommand(addr | 0x80);
	this->HAL_SPICommand(cmd);
	this->HAL_SetNSS(true);
}

void SX1278_Base::SPIBurstRead(uint8_t addr, uint8_t* rxBuf, uint8_t length) {
	  uint8_t i;
	  if(length <= 1){
		  return;
	  } else {
	     this->HAL_SetNSS(false);
	     this->HAL_SPICommand(addr);
	     for(i = 0; i < length; i++){
	    	 *(rxBuf + i) = this->HAL_SPIReadByte();
	     }
	     this->HAL_SetNSS(true);
	  }
}

void SX1278_Base::SPIBurstWrite(uint8_t addr, uint8_t* txBuf, uint8_t length) {
	  unsigned char i;
	  if(length <= 1){
		  return;
	  } else {
		  this->HAL_SetNSS(false);
		  this->HAL_SPICommand(addr | 0x80);
		  for(i = 0; i < length; i++){
			  this->HAL_SPICommand(*(txBuf + i));
		  }
		  this->HAL_SetNSS(true);
	  }
}

void SX1278_Base::defaultConfig(){
	this->config(this->frequency, this->power, this->LoRa_Rate, this->LoRa_BW);
}

void SX1278_Base::config(uint8_t frequency, uint8_t power, uint8_t LoRa_Rate, uint8_t LoRa_BW) {
	  this->sleep(); //Change modem mode Must in Sleep mode
	  this->HAL_Delay(15);

	  this->entryLoRa();
	  //SPIWrite(0x5904); //?? Change digital regulator form 1.6V to 1.47V: see errata note

	  this->SPIBurstWrite(LR_RegFrMsb, (uint8_t*)SX1278_Frequency[frequency], 3);    //setting  frequency parameter

	  //setting base parameter
	  this->SPIWrite(LR_RegPaConfig, SX1278_Power[power]); //Setting output power parameter

	  this->SPIWrite(LR_RegOcp, 0x0B);			//RegOcp,Close Ocp
	  this->SPIWrite(LR_RegLna, 0x23);			//RegLNA,High & LNA Enable
	  if(SX1278_SpreadFactor[LoRa_Rate] == 6) {	//SFactor=6
		  uint8_t tmp;
		  this->SPIWrite(
				  LR_RegModemConfig1,
				  ((SX1278_LoRaBandwidth[LoRa_BW] << 4) + (CR << 1) + 0x01 )); //Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)

		  this->SPIWrite(
				  LR_RegModemConfig2,
				  ((SX1278_SpreadFactor[LoRa_Rate] << 4) + (CRC << 2)  + 0x03));

		  tmp = this->SPIRead(0x31);
		  tmp &= 0xF8;
		  tmp |= 0x05;
		  SPIWrite(0x31, tmp);
		  SPIWrite(0x37, 0x0C);
	  } else {
		  this->SPIWrite(
				  LR_RegModemConfig1,
				  ((SX1278_LoRaBandwidth[LoRa_BW] << 4) + (CR << 1) + 0x00));	//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)

		  this->SPIWrite(
				  LR_RegModemConfig2,
				  ((SX1278_SpreadFactor[LoRa_Rate] << 4) + (CRC << 2) + 0x03));  //SFactor &  LNA gain set by the internal AGC loop
	  }

	  this->SPIWrite(LR_RegSymbTimeoutLsb, 0xFF); //RegSymbTimeoutLsb Timeout = 0x3FF(Max)
	  this->SPIWrite(LR_RegPreambleMsb, 0x00); //RegPreambleMsb
	  this->SPIWrite(LR_RegPreambleLsb, 12); //RegPreambleLsb 8+4=12byte Preamble
	  this->SPIWrite(REG_LR_DIOMAPPING2, 0x01); //RegDioMapping2 DIO5=00, DIO4=01
	  this->readBytes = 0;
	  this->standby(); //Entry standby mode
}

void SX1278_Base::reset() {
	this->HAL_SetNSS(true);
	this->HAL_SetReset(false);
	this->HAL_Delay(1);
	this->HAL_SetReset(true);
	this->HAL_Delay(100);
}

void SX1278_Base::standby(void) {
	this->SPIWrite(LR_RegOpMode, 0x09);
	this->status = STANDBY;
}

void SX1278_Base::sleep(void) {
	this->SPIWrite(LR_RegOpMode, 0x08);
	this->status = SLEEP;
}

void SX1278_Base::entryLoRa(void) {
	this->SPIWrite(LR_RegOpMode, 0x88);
}

void SX1278_Base::clearLoRaIrq(void) {
	this->SPIWrite(LR_RegIrqFlags, 0xFF);
}

bool SX1278_Base::LoRaEntryRx(uint8_t length, uint32_t timeout) {
	  uint8_t addr;

	  this->defaultConfig();		//Setting base parameter
	  this->SPIWrite(REG_LR_PADAC, 0x84);	//Normal and RX
	  this->SPIWrite(LR_RegHopPeriod, 0xFF);	//No FHSS
	  this->SPIWrite(REG_LR_DIOMAPPING1, 0x01);	//DIO=00,DIO1=00,DIO2=00, DIO3=01
	  this->SPIWrite(LR_RegIrqFlagsMask, 0x3F);	//Open RxDone interrupt & Timeout
	  this->clearLoRaIrq();
	  this->SPIWrite(LR_RegPayloadLength, length);	//Payload Length 21byte(this register must difine when the data long of one byte in SF is 6)
	  addr = this->SPIRead(LR_RegFifoRxBaseAddr); //Read RxBaseAddr
	  this->SPIWrite(LR_RegFifoAddrPtr, addr);	//RxBaseAddr->FiFoAddrPtr
	  this->SPIWrite(LR_RegOpMode, 0x8d);	//Mode//Low Frequency Mode
	  //SPIWrite(LR_RegOpMode,0x05);	//Continuous Rx Mode //High Frequency Mode
	  this->readBytes = 0;

	  while(true) {
		  if((this->SPIRead(LR_RegModemStat) & 0x04) == 0x04) {	//Rx-on going RegModemStat
			  this->status = RX;
			  return true;
		  }
		  if (--timeout == 0){
			  this->reset();
			  this->defaultConfig();
			  return false;
		  }
		  this->HAL_Delay(1);
	  }
}

uint8_t SX1278_Base::LoRaRxPacket() {
	  unsigned char addr;
	  unsigned char packet_size;

	  if (this->HAL_GetDIO0()) {
		memset(this->rxBuffer, 0x00, SX1278_MAX_PACKET);

	    addr = this->SPIRead(LR_RegFifoRxCurrentaddr); //last packet addr
	    this->SPIWrite(LR_RegFifoAddrPtr,addr);		   //RxBaseAddr -> FiFoAddrPtr

	    if(this->LoRa_Rate == SX1278_LORA_SF_6) { //When SpreadFactor is six,will used Implicit Header mode(Excluding internal packet length)
	      packet_size = this->packetLength;
	    } else {
	      packet_size = this->SPIRead(LR_RegRxNbBytes);     //Number for received bytes
	    }

	    this->SPIBurstRead(0x00, this->rxBuffer, packet_size);
	    this->readBytes = packet_size;
	    this->clearLoRaIrq();
	  }
	  return this->readBytes;
}

bool SX1278_Base::LoRaEntryTx(uint8_t length, uint32_t timeout) {
	  uint8_t addr;
	  uint8_t temp;

	  this->defaultConfig(); //setting base parameter
	  this->SPIWrite(REG_LR_PADAC, 0x87);	//Tx for 20dBm
	  this->SPIWrite(LR_RegHopPeriod, 0x00); //RegHopPeriod NO FHSS
	  this->SPIWrite(REG_LR_DIOMAPPING1, 0x41); //DIO0=01, DIO1=00,DIO2=00, DIO3=01
	  this->clearLoRaIrq();
	  this->SPIWrite(LR_RegIrqFlagsMask, 0xF7); //Open TxDone interrupt
	  this->SPIWrite(LR_RegPayloadLength, length); //RegPayloadLength 21byte
	  addr = this->SPIRead(LR_RegFifoTxBaseAddr); //RegFiFoTxBaseAddr
	  this->SPIWrite(LR_RegFifoAddrPtr,addr); //RegFifoAddrPtr

	  while(true) {
		  temp = this->SPIRead(LR_RegPayloadLength);
		  if(temp == 21) {
			  this->status = TX;
			  return true;
		  }

		  if (--timeout == 0) {
			  this->reset();
			  this->defaultConfig();
			  return false;
		  }
	  }
}

bool SX1278_Base::LoRaTxPacket(uint8_t* txBuffer, uint8_t length, uint32_t timeout) {
	  this->SPIBurstWrite(0x00, txBuffer, length);
	  SPIWrite(LR_RegOpMode,0x8b);	//Tx Mode
	  while(true) {
	    if(this->HAL_GetDIO0()) { //if(Get_NIRQ()) //Packet send over
	      this->SPIRead(LR_RegIrqFlags);
	      this->clearLoRaIrq(); //Clear irq
	      this->standby(); //Entry Standby mode
	      return true;
	    }

	    if (--timeout == 0){
	    	this->reset();
	    	this->defaultConfig();
	    	return false;
	    }
	    this->HAL_Delay(1);
	  }
}

void SX1278_Base::begin(uint8_t frequency, uint8_t power, uint8_t LoRa_Rate, uint8_t LoRa_BW, uint8_t paketLength) {
	this->frequency = frequency;
	this->power = power;
	this->LoRa_Rate = LoRa_Rate;
	this->LoRa_BW = LoRa_BW;
	this->packetLength = packetLength;
	this->defaultConfig();
}

bool SX1278_Base::transmit(uint8_t* txBuf, uint8_t length, uint32_t timeout) {
	if (this->LoRaEntryTx(length, timeout)){
		return this->LoRaTxPacket(txBuf, length, timeout);
	}
	return false;
}

bool SX1278_Base::receive(uint8_t length, uint32_t timeout){
	return this->LoRaEntryRx(length, timeout);
}

uint8_t SX1278_Base::available() {
	return this->LoRaRxPacket();
}

uint8_t SX1278_Base::read(uint8_t* rxBuf, uint8_t length) {
	if (length != this->readBytes) length = this->readBytes;
	memcpy(rxBuf, this->rxBuffer, length);
	return length;
}

uint8_t SX1278_Base::RSSI_LoRa(void) {
	   uint32_t temp = 10;
	   temp = this->SPIRead(LR_RegRssiValue); //Read RegRssiValue, Rssi value
	   temp = temp + 127 - 137; //127:Max RSSI, 137:RSSI offset
	   return (uint8_t) temp;
}

uint8_t SX1278_Base::RSSI(void) {
	  uint8_t temp = 0xff;
	  temp = this->SPIRead(0x11);
	  temp = 127 - (temp >> 1);	//127:Max RSSI
	  return temp;
}
