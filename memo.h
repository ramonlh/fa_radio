
#pragma once

typedef struct {    // datos memorias
      uint8_t act[maxMem];         // activa o no
      uint8_t vfoActive[maxMem];      // VFO 
      uint8_t isUSB[maxMem];       // mode
      uint8_t cwMode[maxMem];      // CW mode
      uint8_t ritOn[maxMem];       // RIT
      uint8_t splitOn[maxMem];     // SPL
      unsigned long frequency[maxMem];  // frequency
      unsigned long ritTxFrequency[maxMem];  // frequency TX Rit
      uint8_t LIBRE[maxMem];         //  
      uint8_t cwModespl[maxMem];        // CW mode TX split
      unsigned long ftxspl[maxMem];  // frequency TX Split
      char descr[maxMem][20];        // descripción
} memotype;
      memotype memo;
      uint8_t *buffmemo=(uint8_t *) &memo; // acceder a mem como bytes

