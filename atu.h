
#include <ESP32Servo.h>  // Usa esta librería

//#ifdef SERVO
  Servo cap1, cap2;
//#endif

#pragma once
#include "display.h"

// display.h
uint32_t getValByKnob(int valueType, int targetValue, int minValue, int maxValue, int incStep, const char* Title, int Sensitivity);


void setupATU(uint8_t C) {
  if (C==1) { 
    conf.posATUC1=getValByKnob(3, conf.posATUC1, 0, 180, 1, "C1", 1); 
    }
  else { 
    conf.posATUC2=getValByKnob(4, conf.posATUC2, 0, 180, 1, "C2", 1); 
    }
  saveconf(); 
  return;
}

void initATU()
{
#ifdef UBITX_RADIO
#endif
#ifdef FA_RADIO
  cap1.setPeriodHertz(50);           // Establecer frecuencia PWM para servo (50 Hz)
  cap1.attach(TX_LPF_A, 500, 2400);        // Pin GPIO, ancho de pulso mínimo y máximo en microsegundos
  cap2.setPeriodHertz(50);           // Establecer frecuencia PWM para servo (50 Hz)
  cap2.attach(TX_LPF_B, 500, 2400);        // Pin GPIO, ancho de pulso mínimo y máximo en microsegundos
#endif

  // Allow allocation of all timers
  //ESP32PWM::allocateTimer(0);
  //ESP32PWM::allocateTimer(1);
  //ESP32PWM::allocateTimer(2);
  //ESP32PWM::allocateTimer(3);
  //cap1.setPeriodHertz(50);    // standard 50 hz servo
  //cap2.setPeriodHertz(50);    // standard 50 hz servo
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings for an accurate 0 to 180 sweep
  //cap1.write(conf.posATUC1);
  //cap2.write(conf.posATUC2);
}

int posmin1, posmin2;

void acopla()
{

  clearTFT();
  s2("Tunning"); s2(crlf);
  // cap1
  cap1.write(180);              // tell servo to go to position in variable '0'
  cap2.write(180);              // tell servo to go to position in variable '0'
  delay(500);
  swrmin=999; posmin1=180; 
  for (int i=180; i>0; i--) {   // goes from 0 degrees to 180 degrees
    //SWR=readSWR(0);
    //displaySWR2();
    displaySWR(1);
    cap1.write(i);              // tell servo to go to position in variable 'pos'
    if (SWRreal<swrmin) 
      { 
      posmin1=i-1; swrmin=SWRreal; 
      }
    }
  delay(100);
  conf.posATUC1=posmin1;
  cap1.write(conf.posATUC1);            // tell servo to go to position in variable 'pos'
  saveconf();

  // cap 2
  swrmin=999; posmin2=180;
  for (int i=180; i>0; i--) {   // goes from 0 degrees to 180 degrees
    //SWR=readSWR(0);
    //displaySWR2();  
    displaySWR(1);
    cap2.write(i);              // tell servo to go to position in variable 'pos'
    if (SWRreal<swrmin) 
      { 
      posmin2=i-1; 
      swrmin=SWRreal; 
      }
    }
  delay(100);
  conf.posATUC2=posmin2;
  saveconf();
  cap2.write(conf.posATUC2);            // tell servo to go to position in variable 'pos'
  delay(500);
}

