 

#define FIRMWARE_VERSION_INFO F("0.1")

// librerias 
#include "Wire.h"
#include "WiFi.h"
#include "TimeLib.h"                  // Local
#include <WebServer.h>
#include <HTTPClient.h>

// includes proyecto
#include <eeprom.h>
#include "defines.h"
#include "variables.h"
#include "basic_f.h"
#include "memo.h"
#include "files.h"
#include "conf.h"
#include "botones.h"
#include "ds18b20.h"
#include "si5351.h"
#include "display.h"
#include "cw_f.h"
//#include "cat.h"
#include "debug.h"
#include "ads1x15.h"
#include "html_f.h"
#include "net_serv.h"
#include "radio.h"
#include "atu.h"
#include "wifi_f.h"
#include "serial_f.h"
#include "keyer.h"

void setup()
{
  initSerial2(115200);          s2("Serial 2 started\n"); 
  //init_cat(38400, SERIAL_8N1);  s2("Serial 1 started\n");
  init_EEprom();
  initTFT();      s2("TFT iniciado\n");
  s2("Vers.:"); s2(FIRMWARE_VERSION_INFO); s2("\n");
  initConf();     s2("========== Init =========\n");
  //showSettings();
  initSPIFSS (false, false);   // par1: test  ficheros   par2: formatear
  initPorts();              s2("Ports OK\n");  
  initSettings(); 
  initSettingsAux();   
  Wire.begin(SDA,SCL);      s2("I2C started\n");
  initOscillators();        s2("Oscillators started\n");
  initTone();               s2("Pin Tone OK\n");
  setFrequency(conf.frequency);  
  initDS18B20();   showDS18B20(); 
  leevaloresOW();  showDS18B20values();
  initADS1x15();            s2("ADS1115 started\n");
  initATU();                s2("ATU started\n");
  updateDisplay(1);
  initWiFi();         
  initNetServices();
  //initTPA2016();            s2("TPA2016 started\n");
  //initWS();
  //initDecodeCW();     s2("initDecodeCW");s2(crlf);

  smetervalue=0;  maxsmeter=0;  minsmeter=32000;
  byte auxconnMode=conf.connMode;
  conf.connMode=auxconnMode;
  s2(F("END SETUP"));s2(crlf);  
  s2("============================");s2(crlf);
  s2(F("Type 'h' to help")); s2(crlf); 
  s2("----------------------------");s2(crlf);

}

unsigned countloop=0;

void handleWebclient() { 
  server.handleClient(); 
  }

void handleFTP() { 
  ftpSrv.handleFTP(); 
  } 

void loopaux()
{
  countloop++;  
  tini=millis();
  handleSerial();  

  if (conf.ftpenable) handleFTP();  
  if (conf.webenable) handleWebclient();
  //if (conf.wsenable) handleWS(); 
  //if (conf.debugenable) Debug.handle();
  if (isCWAutoMode==0)    //when CW AutoKey Mode, disable this process
    {
    if ((!txCAT) && (!txTFT)) { checkPTT(); }  
    handletfttouch();     // este proceso dura unos 11 ms
    }
  if (conf.cwMode!=0) 
    { 
    //decodeCW();
    cwKeyer(); 
    }

  if (tftpage==0)
    {
    readSmeter();     // este proceso dura unos 9 ms
    if (inTx==1)  // TX
      { 
      SWR=readSWR(1); 
      displaySWR(0); 
      }
    else    // RX
      {
      if (conf.memMode==0)
        {
        if (isCWAutoMode == 0 || cwAutoDialType == 1)
          {
          if (scanF>0) 
            { 
            doScanF();  
            readSmeter();   // valores de 0 a 90
            displayFrame();
            if (conf.scanmode>0)
              {
              if (smetervalue>conf.squelchval) { 
                setSCAN(0); 
                }
              }
            }
          else
            {
            if (conf.ritOn) {  
                doRIT();    
              }
            else 
              { 
              if (keylock==0) 
                {
                doTuningWithThresHold();   // este proceso dura unos 10 ms  
                }
              }
            }
          }
        }
      } //end of check TX Status
    //we check CAT after the encoder as it might put the radio into TX
    //Check_Cat(inTx?1:0); // s2("Check_Cat");s2(crlf);
    }
  else if (tftpage==22) { 
    doMem(); 
    }    // Mem Mode
}

void loop()
{ 
//  conf.tcpenable=false;
//  if (conf.tcpenable)
//    handletcpS();
//  else
    loopaux();
}
