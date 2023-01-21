#include "adcMessung.h"

#define TSteigung 0.1292763158
#define TOffset   -255.0

volatile uint16_t temperatureWert;
volatile uint16_t tempCalib85;
volatile uint8_t listPointer=0;
volatile uint8_t adcMode=ADCMODE_CF_NORMAL;

double doSpannungsMessung()
{
  ADCA.CTRLB     = 0;// 12bit, single ended, kein Freerun, ADC_CONMODE_bm | ADC_FREERUN_bm;
  ADCA.REFCTRL   = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm;
  ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
  ADCA.CH0.CTRL    = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
  ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN9_gc;
  ADCA.CALL = SP_ReadCalibrationByte( PRODSIGNATURES_ADCACAL0 );
	ADCA.CALH = SP_ReadCalibrationByte( PRODSIGNATURES_ADCACAL1 );
  ADCA.CTRLA     = ADC_ENABLE_bm;
  _delay_ms(1);
  uint16_t result = 0;
  for(uint8_t i=0;i<16;i++)
  {
    ADCA.CH0.CTRL    |= ADC_CH_START_bm;
    while( !(ADCA.INTFLAGS & 1) )
      ;
    ADCA.INTFLAGS = 1;
    result += ADCA.CH0.RES;
  }
  result = result >> 4;
  return(6.13*(double)(result-175)/4095.0);
}

double doTemperatureMessung()
{
  listPointer = 0;
  ADCA.CTRLB     = 0; // 12bit, single ended, kein Freerun, ADC_CONMODE_bm | ADC_FREERUN_bm;
  ADCA.REFCTRL   = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm | ADC_TEMPREF_bm;
  ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
  ADCA.CH0.CTRL    = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
  ADCA.CH0.MUXCTRL = 0;
  ADCA.CTRLA     = ADC_ENABLE_bm;
  _delay_ms(10);
  uint16_t result = 0;
  for(uint8_t i=0;i<16;i++)
  {
    ADCA.CH0.CTRL    |= ADC_CH_START_bm;
    while( !(ADCA.INTFLAGS & 1) )
      ;
    ADCA.INTFLAGS = 1;
    result += ADCA.CH0.RES;
  }
  result = result >> 4;
  return( TSteigung*(double)result + TOffset);
}


void startAdcMeasure()
{
  ADCA.CH0.CTRL    |= ADC_CH_START_bm;
}

uint16_t readStrom()
{
int16_t result;
  result = ADCA.CH0.RES;
  return(abs(result));
}

void waitMeasure()
{

}

double calcTemperatur()
{
  //return( TSteigung*(double)temperatureWert + TOffset);
  return( (double)temperatureWert );
}

uint8_t SP_ReadCalibrationByte( uint8_t index )
{
	uint8_t result;

	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(index);

	/* Clean up NVM Command register. */
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return result;
}
