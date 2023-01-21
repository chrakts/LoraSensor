#ifndef STROMMESSUNG_H_INCLUDED
#define STROMMESSUNG_H_INCLUDED

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
#include <util/delay.h>


enum{ADCMODE_CF_NORMAL,ADCMODE_NORMAL,ADCMODE_CF_TEMP,ADCMODE_TEMP,ADCMODE_CF_OFFSET,ADCMODE_OFFSET,ADCMODE_LAST};



extern volatile uint16_t temperatureWert;
extern volatile uint16_t tempCalib85;
extern volatile uint8_t listPointer;


double doSpannungsMessung();
double doTemperatureMessung();

void initStromMessung(bool doOffset);
void startAdcMeasure();
uint16_t readStrom();
double calcTemperatur();
uint8_t SP_ReadCalibrationByte( uint8_t index );
#endif // STROMMESSUNG_H_INCLUDED
