#ifndef LORASENSOR_H_INCLUDED
#define LORASENSOR_H_INCLUDED


#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "myConstants.h"
#ifdef SHTC3
#include "shtc3.h"
#include "twi_master_driver.h"
#endif // SHTC3
#include "Serial.h"
#include "External.h"
#include "../Secrets/secrets.h"
#include "Communication.h"
#include "ComReceiver.h"
#include "cmultiStandardCommands.h"
#include "LoraSensorCommands.h"
#include "../xmegaClocks/xmegaClocks.h"
#include "LoRa.h"
#include "Cmulti2Buffer_v02.h"
#include "loraCmulti.h"
#include "secrets.h"


void setup();
void setRTC(uint16_t);
#ifdef SHTC3
void doClima();
#endif // SHTC3

#endif // LORASENSOR_H_INCLUDED
