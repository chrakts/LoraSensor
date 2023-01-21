#define EXTERNALS_H_

#include "LoraSensor.h"
#include "../Secrets/secrets.h"
#include "loraCmulti.h"

const char *Node = "LS";

volatile bool debugOn = false, oldDebugOn = true;

volatile double  fTemperatur=-999,fHumidity=-999,fDewPoint=-999,fAbsHumitdity=-999;

ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_COMMANDS,cnetInformation,NUM_INFORMATION,"W1",sendViaRelay);

uint8_t key[16] = {AES256_KEY};

SPI mySPI(0);
LoRaClass LoRa(&mySPI);

Communication cmulti(0,Node,5,true);

#ifdef SHTC3
TWI_MasterDriver_t twiE_Master;
TWI_MasterDriver_t twiC_Master;
shtc3 humiSensor;
#endif
