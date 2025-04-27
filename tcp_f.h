
#pragma once

#ifdef TCPCLIENT
  WiFiClient tcpclient;
#endif
/*
void sendtcpData(byte c)
{
  //if (keylock==1) return;
  char data[60];
  if (c==tcpsplitOn) { strcpy(data,itoa(conf.splitOn,buff,10)); }
  else if (c==tcpisUSB) { strcpy(data,itoa(conf.isUSB,buff,10)); }
  else if (c==tcpritOn) { strcpy(data,itoa(conf.ritOn,buff,10)); }
  else if (c==tcpcwMode) { strcpy(data,itoa(conf.cwMode,buff,10)); }
  else if (c==tcpvfoActive) { strcpy(data,itoa(conf.vfoActive,buff,10)); }
  else if (c==tcpisusbA) { strcpy(data,itoa(conf.isUSBA,buff,10)); }
  else if (c==tcpisusbB) { strcpy(data,itoa(conf.isUSBB,buff,10)); }
  else if (c==tcpcwmodeA) { strcpy(data,itoa(conf.cwModeA,buff,10)); }
  else if (c==tcpcwmodeB) { strcpy(data,itoa(conf.cwModeB,buff,10)); }
  else if (c==tcpCallSign) { strcpy(data,conf.CallSign); }
  else if (c==tcpfrequencyA) { strcpy(data,itoa(conf.frequencyA,buff,10)); }
  else if (c==tcpfrequencyB) { strcpy(data,itoa(conf.frequencyB,buff,10)); }
  else if (c==tcpwifi) { strcpy(data,WiFi.isConnected()?"1":"0"); }
  else if (c==tcpscanst) { strcpy(data,itoa(scanF,buff,10)); }
  else if (c==tcpkeylock) { strcpy(data,itoa(keylock,buff,10)); }
  else if (c==tcptemp1) { strcpy(data,itoa(MbR[0],buff,10)); }
  else if (c==tcptemp2) { strcpy(data,itoa(MbR[1],buff,10)); }
  else if (c==tcptemp3) { strcpy(data,itoa(MbR[2],buff,10)); }
  else if (c==tcptunestep) { strcpy(data,itoa(conf.tuneStepIndex,buff,10)); }
  else if (c==tcpminsmeter) { strcpy(data,itoa(minsmeter,buff,10)); }
  else if (c==tcpmaxsmeter) { strcpy(data,itoa(maxsmeter,buff,10)); }
  else if (c==tcpattlevel) { strcpy(data,itoa(conf.attLevel,buff,10)); }
  else if (c==tcpifShiftVal) { strcpy(data,itoa(conf.ifShiftValue,buff,10)); }
  else if (c==tcpvtotvalue) { dtostrf(vtotvalue,8,2,data); }
  else if (c==tcpitotvalue) { dtostrf(itotvalue,8,2,data); }
  else { strcpy(data,"999"); }
  if ((conf.connMode==1) || (conf.connMode==2))   // modos IP, enviar por Client
    {
    tcpclient.write(c); 
    if (c==tcpMEM) { tcpclient.write(buffmemo,sizeof(memo)); }
    else if (c==tcpALL) { tcpclient.write(buffconf,sizeof(conf)); }
    else { tcpclient.write(data); tcpclient.write('\n'); }
    }
}
*/
