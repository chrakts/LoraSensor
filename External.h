#ifndef EXTERNAL_H_INCLUDED
#define EXTERNAL_H_INCLUDED

#include "Serial.h"
#include "myConstants.h"
#include "Communication.h"
#include "CRC_Calc.h"
#include "SPI.h"
#include "LoRa.h"
#include "Cmulti2Buffer_v02.h"
#include "ComReceiver.h"
#include "LoraSensor.h"

extern const char *Node;

extern volatile bool debugOn, oldDebugOn;

extern volatile double  fTemperatur,fHumidity,fDewPoint,fAbsHumitdity;

extern CRC_Calc crcGlobal;

extern SPI mySPI;

extern Communication cmulti;
extern ComReceiver cmultiRec;
class LoRaClass;

extern LoRaClass LoRa;

//extern volatile  uint8_t txIsReady;
//extern volatile  uint8_t rxIsReady;

//extern volatile int8_t rxRssi;
//extern  char rxMessage[LORAMESSAGELENGTH];

extern uint8_t key[];

extern char LoraCmultiBuffer[];
extern Cmulti2Buffer_v02 loraCmulti;

#ifdef SHTC3
extern TWI_MasterDriver_t twiC_Master;
extern TWI_MasterDriver_t twiE_Master;
extern shtc3 humiSensor;
#endif

#endif // EXTERNAL_H_INCLUDED


