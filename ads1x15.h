
#pragma once

#include <Adafruit_ADS1X15.h>

void ajustarsmeter(int16_t valorleido)
{
  //s2("smeter :"); s2(valorleido);
  // todo lo que sigue hay que sacarlo de la task para ahorrar tiempo y hacerse fuera ??
  calSmeterReq=calSmeterReq || (valorleido<conf.sMeterLevels[0]) || (valorleido>conf.sMeterLevels[15]);
  //valorleido=valorleido>smeterlast?((smeterlast*7+valorleido*3))/10:   // valor ajustado para evitar variaciones rápidas
  //                              ((smeterlast*9+valorleido*1))/10;   // de VK2ETA
  smeterlast=valorleido;     // guardar último valor leído
  if (valorleido<=minsmeter) { minsmeter=valorleido; }    // buscar mínimo      
  if (valorleido>=maxsmeter) { maxsmeter=valorleido; }    // buscar máximo    
  // convertir valor a escala 0-90
  byte i=0; boolean encontrado=false;
  while ((i<16) && (!encontrado))           // busca intervalo de 0 a 16
    {
    if (valorleido<conf.sMeterLevels[i]) 
      encontrado=true;
    else
      i++;
    }
  if (i<15)
    smetervalue=((i-1)*6) + (valorleido-conf.sMeterLevels[i])*6 / (conf.sMeterLevels[i]-conf.sMeterLevels[i-1]);
  else
    smetervalue=90; 
  //s2("-"); s2(smetervalue); s2(crlf);
}

void ajustarswr(int16_t vforleido, int16_t vrefleido)
{
  //s2("swr:"); s2(vforleido); s2 ("/"); s2(vrefleido); s2 ("-");
  // ¿¿¿ sacar esto que sigue?
  vFORc=((float(vforleido)*0.1875/1000)+0.25)*11*0.707;
  vREFc=((float(vrefleido)*0.1875/1000)+0.25)*11*0.707;
  //wFORc=vFORc*vFORc/50; wREFc=vREFc*vREFc/50;
  wFORc=vFORc*vFORc*0.707/50; wREFc=vREFc*vREFc*0.707/50;
  if ((vFORc-vREFc)>0) SWRreal=(vFORc+vREFc)/(vFORc-vREFc); else SWRreal=1.0;
  float auxSWR=1;
  auxSWR=(SWRreal*conf.ATUFactor)+conf.ATUOffset;
  //if (auxSWR < 1) auxSWR=1;
  SWR = auxSWR;     // SWR es la variable global que se usa en otros sitios
  //s2(vFORc); s2("/"); s2(vREFc); s2("==>"); s2(SWR); s2(crlf);
}

void task_ads1115(void *pvParameters) {
  //esp_task_wdt_add(NULL);
  #ifdef ADS1X15
    Adafruit_ADS1115 adsA;  /* Use this for the 16-bit version */
    Adafruit_ADS1115 adsB;  /* Use this for the 16-bit version */
    adsA.begin();  //adsA.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V  1 bit = 0.1875mV (default)
    adsB.begin();  //adsB.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V  1 bit = 0.1875mV (default)
  #endif
  TickType_t xLastWakeTime = xTaskGetTickCount();
    //esp_task_wdt_reset();
    #ifdef ADS1X15
      while(1) {
        if (inTx==0) {
          // read smeter
          int16_t smeteradc=0;
          smeteradc = adsA.readADC_SingleEnded(SMETERp);    // es el valor leído del ADC sin convertir
          //s2("sme:"); s2(smeteradc);s2(crlf);
          ajustarsmeter(smeteradc);
          // read CW     ¡Sólo si en modo CW!
          cwcodevalue = adsB.readADC_SingleEnded(3); 
          }
        else {
          // read SWR ¡Sólo en modo TX!
          int16_t adc0, adc1;
          long ldc0=0; long ldc1=0; 
          for (byte i=0;i<conf.ATUIter;i++)
            {
            ldc0=ldc0+adsA.readADC_SingleEnded(VFORp); // VFORp=0
            ldc1=ldc1+adsA.readADC_SingleEnded(VREFp); // VREFp=1
            }
          adc0 = ldc0/conf.ATUIter; if (adc0<0) adc0=0;
          adc1 = ldc1/conf.ATUIter; if (adc1<0) adc1=0;
          ajustarswr(adc0, adc1);
          }

      //esp_task_wdt_reset();
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(task_ads1115_delay));
      }
    #endif
}
