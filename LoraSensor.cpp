#include "LoraSensor.h"
#include "LoraSensorCommands.h"

class LoRaClass;

void setup()
{
	init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);

	PORTA_DIRSET = 0xf0; // 4x Eingang, dann 4x LED's
	PORTB_DIRSET = 0x00;; // nichts angeschlossen
	PORTC_DIRSET = 0b00001011; // 4x LED, dann 4x Ausgang
	PORTD_DIRSET = 0b10111000; //
	PORTE_DIRSET = 0x00; // nichts angeschlossen

	PORTA_OUTSET = 0xf0;

  // Pullup für debugPin
	PORTA.PIN1CTRL = 0b10011000;
	_delay_ms(1);

	//debug-Status ermitteln
  if( (DEBUGPIN_DATA)==0 )
    debugOn = true;
  else
    debugOn = false;

  // Einschaltblinken (gruen=Standard; rot=debuggen)
  for(uint8_t i=0;i<=3;i++)
  {
    if(debugOn)
      LED_ROT_ON;
    else
      LED_GRUEN_ON;
    _delay_ms(50);
    LED_GRUEN_OFF;
    LED_ROT_OFF;
    _delay_ms(50);
  }

	//Interrupts einschalten
	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();

	// cmulti initialisieren und Einschaltmeldung
	cmulti.open(Serial::BAUD_57600,F_CPU);
  cmulti.sendInfo("Hello from LORA-Sensor","BR");

  // I2C-Schnittstelle initialisieren
  TWI_MasterInit(&twiE_Master, &TWIE, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);

#ifdef SHTC3
  // Klimasensor initialisieren
  humiSensor.begin(&twiE_Master);
  _delay_ms(1);
  humiSensor.setMode(SHTC3_NORMAL_T_FIRST);
  humiSensor.wakeup();
  _delay_ms(1);
  cmulti.broadcastUInt16(humiSensor.getID(),'S','I','D');
#endif // SHTC3

  // Lora-Modul aufsetzen
  while (!LoRa.begin(868E6))
  {
    LED_ROT_TOGGLE;
    _delay_ms(100);
  }
  LoRa.setTxPower(14, PA_OUTPUT_RFO_PIN);
  loraCmulti.setChecksum();
  //LoRa.onReceive(onReceive);
  LoRa_rxMode();

  // RTC aufsetzen
  CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm; // 1kHz ULP
  if(debugOn)
    setRTC(10);
  else
    setRTC(30);
}



int main()
{
  uint8_t key[16] = {AES256_KEY};
  setup();

  while(1)
  {
    // Änderung des debug-Status behandeln
    if(oldDebugOn!=debugOn)
    {
      if(debugOn)
        setRTC(10);
      else
        setRTC(30);
      oldDebugOn = debugOn;
    }
    // Abfrage Debug-Pin
    if( DEBUGPIN_DATA==0 )
      debugOn = true;
    else
      debugOn = false;

    if(processRelaisInfos(&cmulti))
    {
      LED_BLAU_OFF;
      cmulti.sleep();
      LoRa.sleep();
      set_sleep_mode(SLEEP_MODE_PWR_SAVE);
      sleep_enable();
      sleep_cpu();
      sleep_disable();
      //LoRa.onReceive(onReceive);
      //LoRa_rxMode();
      LoRa_txMode();
      LED_BLAU_ON;
      cmulti.wakeup();
      doClima();
    }
  }

}

void setRTC(uint16_t time)
{
  RTC.INTCTRL = 0;
  RTC.CTRL = 0;
  while(RTC.STATUS & RTC_SYNCBUSY_bm)
  {
    ;
  }
  RTC.PER = time;
  RTC.CNTL = 0;
  RTC.CNTH = 0;
  RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
  RTC.INTCTRL = RTC_OVFINTLVL_HI_gc;
}


#ifdef SHTC3
void doClima()
{
  static uint8_t status;
  if(debugOn)
  {
    loraCmulti.clearEncryption();
  }
  else
  {
    loraCmulti.setEncryption(NULL);
    loraCmulti.encryptSetKey(key);
  }
  switch(status)
  {
    case 0:
      humiSensor.wakeup();
      _delay_ms(1);
      humiSensor.startMeasure();
      _delay_ms(14);
      humiSensor.readResults();
      humiSensor.getResults(fTemperatur,fHumidity);
      humiSensor.sleep();
      fAbsHumitdity =  humiSensor.calcAbsHumi(fTemperatur,fHumidity);
      fDewPoint =  humiSensor.calcDewPoint(fTemperatur,fHumidity);
      humiSensor.sleep();
      loraCmulti.broadcastDouble(fTemperatur,'C','1','t');
      status++;
    break;
    case 1:
      loraCmulti.broadcastDouble(fHumidity,'C','1','h');
      status++;
    break;
    case 2:
      loraCmulti.broadcastDouble(fAbsHumitdity,'C','1','a');
      status++;
    break;
    case 3:
      loraCmulti.broadcastDouble(fDewPoint,'C','1','d');
      status++;
    break;
    default:
      status = 0;
  }
  if( status > 3 )
    status = 0;
  sendViaRelay(loraCmulti.get());

}
#endif // SHTC3

ISR ( RTC_OVF_vect )
{

}


