 

// sólo debe haber una línea sin comentar
//#define PLACAVERDE          // no recuerdo que placa es ésta
//#define PLACAAMARILLA       // CYD integrada con esp32
//#define RADUINO32           // Raduino32
//#define RADUINO32_4P           // Raduino32 con pantalla 4 pulgadas ILI9488
//#define PLACA4MOTORES       // placa 4 motores coche Diego
//#define PLACA4MOTORES       // placa 4 motores coche Diego

//#define PANTALLA28P          // 
#define PANTALLA40P          //
//#define UBITX_RADIO
#define FA_RADIO

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
#include "cat.h"
#include "tcp_f.h"
#include "atu.h"
#include "display.h"
#include "ads1x15.h"
#include "keyer.h"
#include "cw_f.h"
#include "debug.h"
#include "html_f.h"
#include "ws_server.h"
#include "net_serv.h"
#include "mux_mcp23017.h"
#include "radio.h"

#include "wifi_f.h"
#include "serial_f.h"

void setup()
{
  initSerial2(115200);          
  s2("========== Init =========\n");
                                s2("Serial 2 OK\n"); 
  Init_Cat(38400, SERIAL_8N1);  s2("Serial 1 OK\n");
  init_EEprom();                s2("EEPROM OK\n");
  initConf();   // se asignan valores iniciales por si falla la lectura

  initTFT();                    s2("TFT OK\n");
  s2("Version:"); s2(FIRMWARE_VERSION_INFO); s2("\n");
  initFFat (true, true);
  s2("CONF\n");
  int err = readconf();
  if (err == 0)
    s2("  OK\n");
  else if (err == 2)
    s2("  El archivo /ubitx.cnf no existe\n");
  else if (err == 3)
    s2("  Error al abrir /ubitx.cnf\n");
  else if (err == 4)
    s2("  Error tamano fichero /ubitx.cnf\n");

  Wire.begin(SDA,SCL);      s2("I2C OK\n");
  //showSettings();
  initPorts();              s2("Ports OK\n");  
  initSettings(); 
  initSettingsAux();   
  initOscillators();        s2("Oscilador OK\n");
  initTone();               s2("Pin Tone OK\n");
  setFrequency(conf.frequency);  
  initATU();                s2("ATU OK\n");
  updateDisplay(1);
  initWiFi();         
  initNetServices();
  initTPA2016();            s2("TPA2016 OK\n");
  initDecodeCW();           s2("initDecodeCW\n");
  smetervalue=0;  maxsmeter=0;  minsmeter=32000;
  byte auxconnMode=conf.connMode;
  conf.connMode=auxconnMode;
  s2("MCP23017 "); s2(init_MCP23017()==0?"OK\n":"ERROR\n");
  xTaskCreate(task_ds18b20, "Task DS18B20", task_ds18b20_size, NULL, task_ds18b20_priority, NULL);
  xTaskCreate(task_ads1115, "Task ADS1115", task_ads1115_size, NULL, task_ads1115_priority, NULL);

  s2(F("END SETUP\n"));
  s2("----------------------------\n");
  s2(F("Type 'h' to help\n")); 
  s2("============================\n");
  }


unsigned countloop=0;

void handleWebclient() { 
  server.handleClient(); 
  }

void handleFTP() { 
  ftpSrv.handleFTP(); 
  } 

long timetask01= 0; //
long timetask1= 0; //
long timetask10= 0; //

void task01()
{
  tini=millis();
  if ((inTx==0) && (tftpage==0) && (conf.framemode<=1))
    {
    if (conf.framemode==0)  {
      displaySmeter(1);
      }
    else if (conf.framemode==1) {
      displaybarSmeter(40,186,0,90,69);
      }
    }
  if (tftpage==23) { updateDisplay(0); }
  timetask01 = millis();
}

void sendTemperatureData() {
  String jsonData = "{\"t\":\"temp\",\"v\":{\"temp1\":\"" + String(MbR[0]) + "\",\"temp2\":\"" + String(MbR[0]) + "\",\"temp3\":\"" + String(MbR[0]) + "\"}}";
  // Envía los tres datos de temperatura a todos los clientes conectados
  wsserver.broadcastTXT(jsonData);
  //s2(jsonData); s2(crlf);
}
void sendsmeterData() {
  String jsonData = "{\"t\":\"s\",\"v\":{\"s\":\"" + String(smetervalue) + "\"}}";
  wsserver.broadcastTXT(jsonData);  // Envía el dato smeter
  //s2(jsonData); s2(crlf);
}
void sendpowerData() {
  String jsonData = "{\"t\":\"pwr\",\"v\":{\"p\":\"" + String(powervalue) + "\",\"w\":\"" + String(SWR) + "\"}}";
  wsserver.broadcastTXT(jsonData);
  //s2(jsonData); s2(crlf);
}

void task1()
{
  tini=millis();
  displayStatus();
  if (inTx==0)  {
    sendsmeterData();
    }
  else {
    sendpowerData();
    sendTemperatureData();  
    }
  timetask1 = millis();
}

void task10()
{

  tini=millis();
  sendTemperatureData();  
  timetask10 = millis();
}

void loopaux()
{
  countloop++;  
  tini=millis();
  handleSerial();  

  if (conf.ftpenable) handleFTP();  
  if (conf.webenable) handleWebclient();
  if (conf.wsenable) wsserver.loop(); 
  
  //if (conf.debugenable) Debug.handle();
  
  if (isCWAutoMode==0)    //when CW AutoKey Mode, disable this process
    {
    if ((!txCAT) && (!txTFT)) { 
      checkPTT(); 
      }  
    handletfttouch();     // este proceso dura unos 11 ms
    }
  if (conf.cwMode!=0) 
    { 
    decodeCW();
    cwKeyer(); 
    }
  if (tftpage==0)
    {
    if (inTx==1)  // TX
      { 
      displaySWR(0); 
      }
    else    // RX
      {
      if (conf.memMode==0)
        {
        //displaySmeter(190,210,50,1);
        //updateDisplay(0);
        if (isCWAutoMode == 0 || cwAutoDialType == 1)
          {
          if (scanF > 0) 
            { 
            doScanF();  
            displayFrame();
            if (conf.scanmode>0) {
              if (smetervalue > conf.squelchval) { 
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
              if (keylock==0) {
                doTuningWithThresHold();   // este proceso dura unos 10 ms  
                }
              }
            }
          }
        }
      } //end of check TX Status
    // we check CAT after the encoder as it might put the radio into TX
    Check_Cat(inTx?1:0); // s2("Check_Cat");s2(crlf);
    }
  else if (tftpage==22) { 
    doMem(); 
    }    // Mem Mode
  else if (tftpage==23) { 
    updateDisplay(0); 
    }    // Mem Mode
  if((millis() > (timetask01 + 100))) { task01(); }                     // tareas que se hacen cada 0.1 segundo
  if((millis() > (timetask1 + 1000))) { task1(); }                     // tareas que se hacen cada 0.1 segundo
  if((millis() > (timetask10 + 10000))) { task10(); }                     // tareas que se hacen cada 0.1 segundo
}

void loop()
{ 
  loopaux();
}
