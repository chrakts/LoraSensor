#define EXTERNALS_H_

#include "LoraSensor.h"
#include "../Secrets/secrets.h"
#include "loraCmulti.h"

const char *Node = "LS";

volatile float fExternalTemperature = 5.0;

ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_COMMANDS,cnetInformation,NUM_INFORMATION,"W1",sendViaRelay);

uint8_t key[16] = {AES256_KEY};

SPI mySPI(0);
LoRaClass LoRa(&mySPI);

Communication cmulti(0,Node,5,true);
