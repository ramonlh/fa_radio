
#pragma once

#include "OneWire.h"                  // Local
#include "DallasTemperature.h"        // Local

OneWire owire(W0);
DallasTemperature sensors0(&owire);

uint8_t ds18b20active = 0;

int MbR[8]={0,0,0,0,0,0,0,0};      // 0-7 Temperaturas locales
int MbRant[8]={0,0,0,0,0,0,0,0};   // 0-7 Temperaturas locales anteriores
byte nTemp=0;                      // número sondas detectadas en cada puerto 1-wire
uint8_t addr1Wire[maxTemp][8];  // tiene los valores conectados

void showDS18B20() 
{
#ifdef DS18B20
  s2(F("DS18B20 probes\n"));
  s2("  "); s2("Sondas:");  
  s2(nTemp); s2(crlf); s2("  "); s2("Modo:");  s2(":");  s2("  "); 
  s2((sensors0.isParasitePowerMode())?"Parasite\n":"Power\n");
  for(byte i=0; i<maxTemp; i++)       {   // busca sondas conectadas
    if (sensors0.getAddress(addr1Wire[i], i))    {
      s2("  ");
      for(uint8_t j=0; j<8; j++) { if(addr1Wire[i][j]<16) { s2(0); } s2(addr1Wire[i][j]); }
      s2(crlf);
      }
    }
  s2(F("  Started\n"));
#endif
}

void task_ds18b20(void *pvParameters) {
  //esp_task_wdt_add(NULL);
  #ifdef DS18B20
    sensors0.begin();
    sensors0.setResolution(9);
    nTemp=sensors0.getDeviceCount();
    s2("Sondas:"); s2(nTemp); s2(crlf);
    if(nTemp>maxTemp) { nTemp=maxTemp; }
    for (int i=0;i<3;i++) {
      conf.nprobe[i] = i;
      }
    showDS18B20();
  #endif
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    //esp_task_wdt_reset();
    #ifdef DS18B20
      sensors0.requestTemperatures();
      for (uint8_t i=0; i<maxTemp; i++)  
        {
        if (conf.probecode[i][0] > 0)
          {
          int auxI = sensors0.getTempC(conf.probecode[i])*100;
          if (auxI>0)
            {
            MbR[i] = auxI;
            MbRant[i] = MbR[i];
            }
          }
        }
    #endif
    //esp_task_wdt_reset();
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(task_ds18b20_delay));
  }
}

