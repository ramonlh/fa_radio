
#pragma once

#include <Adafruit_ADS1X15.h>

#ifdef ADS1X15
  Adafruit_ADS1115 adsA;  /* Use this for the 16-bit version */
  Adafruit_ADS1115 adsB;  /* Use this for the 16-bit version */
#endif

void initADS1x15()
{
#ifdef ADS1X15
  adsA.begin();  //adsA.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V  1 bit = 0.1875mV (default)
  adsB.begin();  //adsB.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V  1 bit = 0.1875mV (default)
#endif
}
