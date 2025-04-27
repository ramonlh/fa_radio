
#pragma once

#include "Adafruit_TPA2016.h"

#ifdef TPA2016
  Adafruit_TPA2016 audioamp = Adafruit_TPA2016();
#endif

void initPorts(){
  pinMode(ENC_A, INPUT_PULLUP);   // en ESP32 necesita pullup externo
  pinMode(ENC_B, INPUT_PULLUP);   // en ESP32 necesita pullup externo
  pinMode(FBUTTON, INPUT_PULLUP); // en ESP32 necesita pullup externo
  pinMode(PTT, INPUT_PULLUP);     // en ESP32 necesita pullup externo
  pinMode(ANALOG_KEYER, INPUT);   // Keyer

  pinMode(CW_TONE, OUTPUT);    digitalWrite(CW_TONE, 0);
  pinMode(TX_RX,OUTPUT);       digitalWrite(TX_RX, 0);
  pinMode(TX_LPF_A, OUTPUT);   digitalWrite(TX_LPF_A, 0);
  pinMode(TX_LPF_B, OUTPUT);   digitalWrite(TX_LPF_B, 0);
  pinMode(TX_LPF_C, OUTPUT);   digitalWrite(TX_LPF_C, 0);
  pinMode(CW_KEY, OUTPUT);     digitalWrite(CW_KEY, 0);
}

/* The settings are read from FILE  */
void initSettings(){
  memset(spval,0,sizeof(spval));
  if (btnDown())
    {
    ////////////////////////////////////////////////////
    tft.drawString("Reset Factory...",0,40);
    s2("Setting default values");s2(crlf);
    saveconf(); // para iniciar fichero 
    showSettings();
    }
  if (conf.cwKeyType==1) keyerControl &= ~IAMBICB; else if (conf.cwKeyType==2) keyerControl |= IAMBICB;   
  
  //Enable / Diable Check for CW Display Cofiguration Group 
  if ((conf.commonOption0 & 0x80) != 0x00)
    {
    //Adjust CW Mode Freq
    shiftDisplayAdjustVal = (conf.isShiftDisplayCWFreq & 0x3F) * 10;
    //check Minus
    if ((conf.isShiftDisplayCWFreq & 0x40) == 0x40)
      shiftDisplayAdjustVal = shiftDisplayAdjustVal * -1;
   //Shift Display Check (Default : 0)
    conf.isShiftDisplayCWFreq = (conf.isShiftDisplayCWFreq & 0x80) == 0?1:0;
    }
   //Stored IF Shift Option
  if ((conf.commonOption0 & 0x40) != 0x00)
    {
    isIFShift = conf.ifShiftValue != 0;
    }

  //default Value (for original hardware)
  if (conf.cwAdcSTFrom >= conf.cwAdcSTTo)
    {
    conf.cwAdcSTFrom = 0;
    conf.cwAdcSTTo = 50;
    }

  if (conf.cwAdcBothFrom >= conf.cwAdcBothTo) { conf.cwAdcBothFrom = 701; conf.cwAdcBothTo = 900;  }
  if (conf.cwAdcDotFrom >= conf.cwAdcDotTo) { conf.cwAdcDotFrom = 901; conf.cwAdcDotTo = 1300; }
  if (conf.cwAdcDashFrom >= conf.cwAdcDashTo) { conf.cwAdcDashFrom = 1800; conf.cwAdcDashTo = 2300; }
  //end of CW Keying Variables
  
  if (conf.cwDelayTime < 1 || conf.cwDelayTime > 250)  conf.cwDelayTime = 60;
//  if (conf.vfoA_mode < 2) conf.vfoA_mode = 2;   // LSB default
//  if (conf.isUSBB < 2) conf.isUSBB = 3;   // USB default
  
  //original code with modified by kd8cec
  if (conf.usbCarrier > 11060000l || conf.usbCarrier < 11048000l)
    conf.usbCarrier = 11056000l;   // EA4GZI
  if (conf.cwmCarrier > 11060000l || conf.cwmCarrier < 11048000l)
    conf.cwmCarrier = 11056000l;    // EA4GZI
  if (conf.frequencyA > 35000000l || 3500000l > conf.frequencyA) {
     conf.frequencyA = 7150000l;
     conf.isUSBA = 0; //LSB
    }
  if (conf.frequencyB > 35000000l || 3500000l > conf.frequencyB) {
    conf.frequencyB = 14150000l;  
    conf.isUSBB = 1; //USB
    }
  //end of original code section
  
  if (conf.sideTone < 100 || 2000 < conf.sideTone) conf.sideTone = 800;
  if (conf.cwSpeed < 10 || 1000 < conf.cwSpeed) conf.cwSpeed = 100;
  if (conf.sideTone < 300 || conf.sideTone > 1000) {
    sideTonePitch = 0;
    sideToneSub = 0;;
    }
  else{
    sideTonePitch = (conf.sideTone - 300) / 50;
    sideToneSub = conf.sideTone % 50;
    }
  initButtons();
}

void initSettingsAux() {
  conf.attLevel=0;      //
  conf.ifShiftValue=0;   //
  byte auxconnMode=conf.connMode;
  conf.connMode=0;
}

void initTone()
{
  ledcAttachChannel(CW_TONE, conf.sideTone, 12, 0);
  //ledcSetup(0,conf.sideTone,12);    Espressiff changed
  //ledcAttachPin(CW_TONE,0);   Espressiff changed
}

/* Below are the basic functions that control the uBitx. Understanding the functions before 
 * you start hacking around */

//-1 : not found, 0 ~ 9 : Hamband index
byte getIndexHambanBbyFreq(unsigned long f)
{
  for (byte i = 0; i<conf.useHamBandCount; i++)
    if ((conf.hamBandRange[i][0]*1000 <= f) && (f <= conf.hamBandRange[i][1]*1000))
      return i;
  return 99;
}

void setTXFilters(unsigned long freq){
    if (freq > 21000000L){  // the default filter is with 35 MHz cut-off
      digitalWrite(TX_LPF_A, 0);
      digitalWrite(TX_LPF_B, 0);
      digitalWrite(TX_LPF_C, 0);
      }
    else if (freq >= 14000000L){ //thrown the KT1 relay on, the 30 MHz LPF is bypassed and the 14-18 MHz LPF is allowd to go through
      digitalWrite(TX_LPF_A, 1);
      digitalWrite(TX_LPF_B, 0);
      digitalWrite(TX_LPF_C, 0);
      }
    else if (freq > 7000000L){
      digitalWrite(TX_LPF_A, 0);
      digitalWrite(TX_LPF_B, 1);
      digitalWrite(TX_LPF_C, 0);    
      }
    else {
      digitalWrite(TX_LPF_A, 0);
      digitalWrite(TX_LPF_B, 0);
      digitalWrite(TX_LPF_C, 1);    
      }
}

void setFrequency(unsigned long f) {
  setTXFilters(f);
  //alternative to reduce the intermod spur
  IF1=conf.firstIF;
  if (conf.isUSB==1)
    {
    OSC1 = IF1 + conf.usbCarrier;
    if (conf.cwMode) { OSC2 = IF1 + f + conf.sideTone; }
    else             { OSC2 = IF1 + f; }
    }
  else
    {       // LSB
    OSC1 = IF1 - conf.usbCarrier;
    if (conf.cwMode) { OSC2 = IF1 + f + conf.sideTone;  }
    else             { OSC2 = IF1 + f;    }
    }
  si5351bx_setfreq(2, OSC2); 
  si5351bx_setfreq(1, OSC1);
  tini=millis();
  conf.frequency=f;
  conf.actualBand=getIndexHambanBbyFreq(f);
  if (conf.actualBand != 99) 
    conf.freqbyband[conf.actualBand][conf.vfoActive==VFO_A?0:1]=f; 
  if (conf.vfoActive==VFO_A) {
    conf.frequencyA=f;
    sendwsData(tcpfrequencyA);
   } else {
     conf.frequencyB=f;  
     sendwsData(tcpfrequencyB);
   }
  if (scanF==0)
    {
    if (!readingspectrum)
      {
      sendwsData(tcpfrequencyA);
      //saveconf();
      }
    }
  else 
    {
    sendwsData(tcpfrequencyA);
    }
}

void checkPTT(){  
  //we don't check for ptt when transmitting cw
  //if (conf.cwTimeout > 0) return;
  if (digitalRead(PTT) == 0 && inTx == 0)
    {
    //startTx(TX_SSB, 1);  
    delay(50); //debounce the PTT
    }
  if (digitalRead(PTT) == 1 && inTx == 1)
    {
    //stopTx(); 
    }
}

/************************************
Replace function by KD8CEC
prevent error controls applied Threshold for reduct errors,  dial Lock, dynamic Step
 *************************************/
byte threshold = 2;  //noe action for count
unsigned long lastEncInputtime = 0;
int encodedSumValue = 0;
byte lastMovedirection = 0;     //0 : stop, 1 : cw, 2 : ccw
#define encodeTimeOut 1000

void setFreq(int s)
{
  conf.frequency += (conf.arTuneStep[conf.tuneStepIndex] * s);  //applied weight (s is speed) //if want need more increase size, change step size
  if (conf.vfoActive==VFO_A) conf.frequencyA=conf.frequency; else conf.frequencyB=conf.frequency;
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1);
}


void doTuningWithThresHold(){
  int s=0;
  //unsigned long prev_freq;
  s = enc_read();
  //if time is exceeded, it is recognized as an error, ignore exists values, because of errors
  if (s == 0) {
    if (encodedSumValue != 0 && (millis() - encodeTimeOut) > lastEncInputtime)
      encodedSumValue = 0;
    lastMovedirection = 0;
    return;
    }
  lastEncInputtime = millis();
  lasttimeknob=millis();        // reset temp to read spectrum
  //for check moving direction
  encodedSumValue += (s > 0 ? 1 : -1);
  //check threshold and operator actions (hold dial speed = continous moving, skip threshold check)
  //not use continues changing by Threshold
  if (((encodedSumValue *  encodedSumValue) <= (threshold * threshold)))
    return;
  //Valid Action without noise
  encodedSumValue = 0;
  setFreq(s);
  //conf.frequency += (conf.arTuneStep[conf.tuneStepIndex] * s);  //applied weight (s is speed) //if want need more increase size, change step size
  //if (conf.vfoActive==VFO_A) conf.frequencyA=conf.frequency; else conf.frequencyB=conf.frequency;
  //setFrequency(conf.frequency);
  //displayFreq(1,1,1,1);
}

void doScanF()
{
  if (scanF==1)   // down
    {
    conf.frequency -= (conf.arTuneStep[conf.tuneStepIndex]);
    if (conf.scanallf==0)
      if (conf.frequency<conf.hamBandRange[conf.actualBand][0]*1000)
        conf.frequency=conf.hamBandRange[conf.actualBand][1]*1000;
    }
  else if (scanF==2)     // up
    {
    conf.frequency += (conf.arTuneStep[conf.tuneStepIndex]);
    if (conf.scanallf==0)
      if (conf.frequency>conf.hamBandRange[conf.actualBand][1]*1000)
        conf.frequency = conf.hamBandRange[conf.actualBand][0]*1000;
    }
  setFrequency(conf.frequency);
  displayFreq(1,1,0,0);
}

void setSCAN(byte value)
{
  if (value==scanF) return;
  scanF=value; 
  sendwsData(tcpscanst); 
  displayFlot();
}

void setSTEP(byte value)
{
  conf.tuneStepIndex=value; 
  sendwsData(tcptunestep); 
  displayFreq(1,1,1,1);  
}

/* RIT only steps back and forth by 100 hz at a time */
void doRIT() {
  int knob = enc_read();
  unsigned long old_freq = conf.frequency;
  if (conf.tuneStepIndex<6) {
    setSTEP(6);
    }
  if (knob < 0) {
    conf.frequency -= (conf.arTuneStep[conf.tuneStepIndex]);  
    }
  else if (knob > 0) {
    conf.frequency += (conf.arTuneStep[conf.tuneStepIndex]);  
    }
  if (conf.frequency>conf.ritTxFrequency+maxRitdev)conf.frequency=old_freq;
  if (conf.frequency<conf.ritTxFrequency-maxRitdev)conf.frequency=old_freq;
  if (old_freq != conf.frequency) {
    setFrequency(conf.frequency);
    displayFreq(0,1,1,1);
    }
}

void setCW(byte value) 
  { 
  conf.cwMode=value; 
  sendwsData(tcpcwMode); 
  if (conf.vfoActive==VFO_A) conf.cwModeA=conf.cwMode; else conf.cwModeB=conf.cwMode;
  btMainact[5]=(conf.cwMode>0);
  displayMain(); 
  }

void setRIT(byte value);

void setSPLIT(byte value) 
  { 
  conf.splitOn=value;
  btMainact[7]=value;
  sendwsData(tcpsplitOn); 
  if (conf.splitOn==1) 
    { 
    setRIT(0); 
    setFrequency(conf.ritTxFrequency);
    }
  displayMain();
  displayFreq(1,1,1,1);
  }


void setRIT(byte value) 
  { 
  conf.ritOn=value; 
  btMainact[6]=value;
  sendwsData(tcpritOn);   // reenvía estado a cliente
  if (conf.ritOn==1) 
    { 
    setSPLIT(0); 
    conf.ritTxFrequency = conf.frequency;
    }
  //setFrequency(conf.ritTxFrequency);
  displayMain();
  displayFreq(1,1,1,1);
  }
  

void setVFO(byte value) 
  { 
  conf.vfoActive=value; 
  sendwsData(tcpvfoActive); 
  if (conf.vfoActive==VFO_A) 
    {
    conf.isUSB=conf.isUSBA; 
    conf.frequency = conf.frequencyA;
    }
  else 
    {
    conf.isUSB=conf.isUSBB;
    conf.frequency = conf.frequencyB;
    }
  setUSB(conf.vfoActive==VFO_A?conf.isUSBA:conf.isUSBB);
  setCW(conf.vfoActive==VFO_A?conf.cwModeA:conf.cwModeB);
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1); 
  }

void setMEMtoVFO(int pos)
{
  if (memo.act[pos]==1)
    {
    setVFO(memo.vfoActive[pos]);
    setUSB(memo.isUSB[pos]);
    setCW(memo.cwMode[pos]);
    setRIT(memo.ritOn[pos]);
    setSPLIT(memo.splitOn[pos]);
    conf.frequency=memo.frequency[pos];
    conf.ritTxFrequency=memo.ritTxFrequency[pos];
    conf.cwModeB=memo.cwModespl[pos]=0;
    conf.frequencyB=memo.ftxspl[pos];
    setFrequency(conf.frequency);
    conf.lastmempos=pos;
    saveconf();
    }
}


void doMem()
{
  if (getBtnStatus()==1) 
    {
    if (memo.act[mempos]==1)
      {
      tftpage=0;
      conf.memMode=0;
      updateDisplay(1);
      return;
      }
    }
  boolean cambio=false;
  int knob = enc_read();
  if (knob < -1) 
    {
    if (memlin>0) memlin--;
    if (mempos>0) { mempos--; cambio=true; }
    }
  else if (knob > 1) 
    {
    if (memlin<5) memlin++;
    if (mempos<maxMem) { mempos++; cambio=true; }
    }
  Serial2.println(mempos);
  if ((cambio) || (firstmem))
    { 
    firstmem=false;
    displayFreq(0,1,1,1);
    displayYN(1,1,0);
    displayMemList(); 
    setMEMtoVFO(mempos); 
    }
}

void setATT(int value, byte local)
{
  conf.attLevel=value;
  setFrequency(conf.frequency);
  SetCarrierFreq();
  if (local==1) {
    updateDisplay(1);
    sendwsData(tcpattlevel);
    }
  else
    {
    btMaincol[9]=conf.attLevel>0?TFT_YELLOW:TFT_WHITE; // ATT
    displayMain();
    displayATT(0,40,150);
    }
}

void setIFS(int value, uint8_t local)
{
  conf.ifShiftValue=value;
  setFrequency(conf.frequency);
  SetCarrierFreq();
  if (local==1) {
    updateDisplay(1);
    sendwsData(tcpifShiftVal);
  }
  else
    {
    btMaincol[8]=conf.ifShiftValue!=0?TFT_YELLOW:TFT_WHITE; // IFS
    displayMain();
    displayIFS(0,40,185);
    }
}

void startTx(byte txMode, byte isDisplayUpdate) {
  //Check Hamband only TX //Not found Hamband index by now frequency
  tftpage=0;
  unsigned long auxfreq;
/**  if ((isTxType & 0x01) != 0x01)
    {
    digitalWrite(TX_RX, 1);
    }
  inTx = 1;**/
  
  if (conf.ritOn)
    {
    //save the current as the rx frequency
    conf.ritRxFrequency = conf.frequency;
    auxfreq=conf.ritTxFrequency;
//    setFrequency(conf.ritTxFrequency);
    }
  else 
    {
    if (conf.splitOn == 1) { setVFO(conf.vfoActive==VFO_A?VFO_B:VFO_A); }
    auxfreq=conf.frequency;
    } //end of else
  byte auxf=getIndexHambanBbyFreq(auxfreq);
  if (conf.TXall==0)    // TX only ham bands
    if ((auxfreq<conf.hamBandRange[auxf][0]*1000) || (auxfreq>conf.hamBandRange[auxf][1]*1000)) 
      {
      tftErrormsg("OUT OF BAND","Modify parameter","TX range");
      //tftpage=2;
      return;
      } 
  if ((isTxType & 0x01) != 0x01) { digitalWrite(TX_RX, 1); }
  inTx = 1;
  
  setFrequency(auxfreq);
  SetCarrierFreq();
  if (txMode == TX_CW)
    {
    //turn off the second local oscillator and the bfo
    OSC0 = 0;  OSC1 = 0;
    si5351bx_setfreq(0, OSC0);
    si5351bx_setfreq(1, OSC1);

    //shif the first oscillator to the tx frequency directly
    //the key up and key down will toggle the carrier unbalancing
    //the exact cw frequency is the tuned frequency + sidetone
    if (conf.cwMode == 0)
      {
      if (conf.isUSB==1)
        OSC2 = conf.frequency + conf.sideTone;
      else
        OSC2 = conf.frequency - conf.sideTone;
      }
    else if (conf.cwMode == 1) //CWL
      {
      OSC2 = conf.frequency - conf.sideTone;
      }
    else  //CWU
      {
      OSC2 = conf.frequency + conf.sideTone;
      }
    si5351bx_setfreq(2, OSC2);
  }
  //reduce latency time when begin of CW mode
  if (isDisplayUpdate == 1)
    updateDisplay(1);
}

void stopTx(void) {
  inTx = 0;
  digitalWrite(TX_RX, 0);           //turn off the tx
  SetCarrierFreq();
  if (conf.ritOn)
    setFrequency(conf.ritRxFrequency);
  else
    {
    if (conf.splitOn == 1) { setVFO(conf.vfoActive==VFO_A?VFO_B:VFO_A); }
    setFrequency(conf.frequency);
    } //end of else
  updateDisplay(1);
}

// when Band change step = just hamband,  moveDirection : 1 = next, -1 : prior
void setNextHamBandFreq(unsigned long f, int moveDirection)
{
  unsigned long resultFreq = 0;
  if (conf.actualBand == 99) 
    {
    conf.actualBand=3; 
    }   //out of hamband
  else if (moveDirection==1)
    {
    if (conf.actualBand < conf.useHamBandCount-1)
      conf.actualBand++;
    else
      conf.actualBand=0;
    }
  else if (moveDirection==-1)
    {
    if (conf.actualBand > 0)
      conf.actualBand--;
    else
      conf.actualBand=conf.useHamBandCount-1;
    }
  else
  {
    conf.actualBand = 99;
  }
  resultFreq = resultFreq & 0x1FFFFFFF;
  if ((resultFreq/1000) < conf.hamBandRange[conf.actualBand][0] || (resultFreq / 1000) > conf.hamBandRange[conf.actualBand][1])
    resultFreq = (unsigned long)(conf.hamBandRange[conf.actualBand][0]) * 1000;
  if (conf.actualBand != 99)
    {
    setFrequency(conf.freqbyband[conf.actualBand][conf.vfoActive==VFO_A?0:1]);
    }
  else
  {
    setFrequency(resultFreq);
  }
    
  if (conf.autoMode==1) { setUSB(resultFreq>=10000000?1:0); }
  displayFreq(0,1,1,1);
  saveconf();
}

void setupFreq() {
  clearTFT();
  int knob = 0;
  //round off the the nearest khz
  conf.frequency = (conf.frequency/1000l)* 1000l;
  setFrequency(conf.frequency);
  delay(100);
  tft.drawNumber(conf.calibration,180,40);
  tft.drawString("You should have a signal",0,130);
  tft.drawString("exactly at      Khz",0,150);
  tft.drawNumber(conf.frequency/1000l,135,150);
  tft.drawString("Rotate to zerobeat",0,170);
  displayYN(1,1,0);
  int result=-1;
  while (result==-1)
    {
    tft.fillRect(190, 40, 160, 30, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawNumber(conf.calibration,180,40);
    if (btnDown()) result=0; else result=checkYN();
    knob = enc_read();
    if (knob != 0)
      {
      conf.calibration += knob * 875;
      OSC0 = conf.usbCarrier;
      si5351bx_setfreq(0, OSC0);  //set back the carrrier oscillator anyway, cw tx switches it off  
      si5351_set_calibration(conf.calibration);
      setFrequency(conf.frequency);
      }
    }
  if (result==0)
    {
    saveconf();
    initOscillators();
    si5351_set_calibration(conf.calibration);
    }
  setFrequency(conf.frequency);    
}

void setupBFO() {
  clearTFT();
  int knob = 0;
  //unsigned long prevCarrier;
  //prevCarrier = conf.usbCarrier;
  tft.drawNumber(conf.usbCarrier,180,70);
  tft.drawString("Set BFO",0,130);
  tft.drawString("Press TUNE to Save",0,170);
  displayYN(1,1,0);
  OSC0 = conf.usbCarrier;
  si5351bx_setfreq(0, OSC0);
  //while (!btnDown()){
  int result=-1;
  while (result==-1)
    {
    tft.fillRect(190, 70, 160, 30, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawNumber(conf.usbCarrier,180,70);
    if (btnDown()) result=0; else result=checkYN();
    knob = enc_read();
    if (knob != 0)
      {
      conf.usbCarrier += 50 * knob;
      OSC0 = conf.usbCarrier;
      si5351bx_setfreq(0, OSC0);
      setFrequency(conf.frequency);
      delay(100);
      }
  }
  saveconf();
  OSC0 = conf.usbCarrier;
  si5351bx_setfreq(0, OSC0);          
  setFrequency(conf.frequency);    
}

void deleteMemo()
{
  memset(buffmemo,0,sizeof(memo));
  savememo(); 
}

void setTPAenable(boolean enablevalue)
{   // los dos canales a la vez
  audioamp.enableChannel(enablevalue, enablevalue);
}  

void setTPAComp()
{   
//#define TPA2016_AGC_OFF 0x00 ///<  Turn off AGC
//#define TPA2016_AGC_2 0x01   ///< AGC compression ratio 1:2
//#define TPA2016_AGC_4 0x02   ///< AGC compression ratio 1:4
//#define TPA2016_AGC_8 0x03   ///< AGC compression ratio 1:8
  audioamp.setAGCCompression(conf.TPA2016Compvalue);
  if (conf.TPA2016Compvalue==0) audioamp.setReleaseControl(0); 
  else   audioamp.setReleaseControl(11);
}  

void setTPAGain(uint8_t gain) // de -28 a +30,  (0-12)
{
  audioamp.setGain(gain);   setTPAenable(false); setTPAenable(true);
}

void setTPAMaxGain(uint8_t maxgain) // de -28 a +30,  (0-12)
{
  audioamp.setAGCMaxGain(maxgain);
}

void calcSmeterScale()
{
  const float D0[16]={1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0};
  const float D1[16]={1.0,2.0,4.0,8.0,16.0,32.0,63.0,126.0,251.0,502.0,1600.0,5000.0,16000.0,50000.0,160000.0,500000.0};
  if (conf.modecalsmeter==0)
    {
    for (int i=1;i<15;i++)
      {
      float auxf=D0[i]/D0[15];
      conf.sMeterLevels[i]=((conf.sMeterLevels[15]-conf.sMeterLevels[0])*auxf) + conf.sMeterLevels[0];
      }
    }
  else  if(conf.modecalsmeter==1)
    {
    for (int i=1;i<15;i++)
      {
      float auxf=D1[i]/D1[15];
      conf.sMeterLevels[i]=((conf.sMeterLevels[15]-conf.sMeterLevels[0])*auxf) + conf.sMeterLevels[0];
      }
    }
}

void initTPA2016()
{
  audioamp.begin();
  audioamp.setAGCCompression(TPA2016_AGC_OFF);  // Turn off AGC for the gain test
  audioamp.setReleaseControl(0);    // we also have to turn off the release to really turn off AGC
  // See Datasheet page 23 for value -> ms conversion table
  audioamp.setAttackControl(5);
  setTPAGain(30);
  setTPAenable(conf.TPA2016enabled);
  audioamp.setGain(conf.TPA2016Gain);
}

void setupconnmode()
{
  conf.connMode=conf.connMode<4?conf.connMode+1:0;
  saveconf();
}

void setupserial2mode()
{
  conf.serial2Mode=conf.serial2Mode==0?1:0;
  saveconf();
}

void saveVFOtoMem()   
{
  int i=0;
  while ((i<maxMem) && (memo.act[i]==1)) i++;
  if (i<maxMem)
    {
    strcpy(memo.descr[i], "");
    int auxI=getCharTFT(memo.descr[i],10); 
    if (auxI !=-1) 
      {
      strcpy(memo.descr[i],auxtft);  
      memo.act[i]=1;
      memo.vfoActive[i]=conf.vfoActive;
      memo.isUSB[i]=conf.isUSB;
      memo.cwMode[i]=conf.cwMode;
      memo.ritOn[i]=conf.ritOn;
      memo.splitOn[i]=conf.splitOn;
      memo.frequency[i]=conf.frequency;
      memo.ritTxFrequency[i]=conf.ritOn==1?conf.ritTxFrequency:conf.frequency;
      memo.cwModespl[i]=conf.cwModeB;
      memo.ftxspl[i]=conf.frequencyB;
      savememo();
      }
    }
}


