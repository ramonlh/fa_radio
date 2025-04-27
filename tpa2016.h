
#pragma once

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

void setTPAGain(uint8_t gain) // de -28 a +30,  (0-12)
{
  audioamp.setGain(gain);   setTPAenable(false); setTPAenable(true);
}

void setTPAMaxGain(uint8_t maxgain) // de -28 a +30,  (0-12)
{
  audioamp.setAGCMaxGain(maxgain);
}
