
#pragma once

#include <TFT_eSPI.h>     // Graphics and font library for ILI9341 driver chip

TFT_eSPI_Button btVFO[2];          // buttons vfo
TFT_eSPI_Button btMEM[3];          // buttons mem
TFT_eSPI_Button btFreq[9];         // buttons frequency display
TFT_eSPI_Button btFreqTx[9];       // buttons frequency display TX at ritOn & SplitOn
TFT_eSPI_Button btMain[10];        // buttons main display
TFT_eSPI_Button btNav[5];          // buttons Nav display
TFT_eSPI_Button btCAL[5];          // buttons Calibration display
TFT_eSPI_Button btSME[5];          // buttons S-METER display
TFT_eSPI_Button btCON[5];          // buttons Connections display
TFT_eSPI_Button btTEMP[5];         // buttons Temps display
TFT_eSPI_Button btPORTS[5];        // buttons Ports display
TFT_eSPI_Button btATU[5];          // buttons ATU 
TFT_eSPI_Button btTPA[5];          // buttons TPA2016 
TFT_eSPI_Button btCW[5];           // buttons CW setting
TFT_eSPI_Button btSet[5];          // buttons Setting 
TFT_eSPI_Button btSetRad[5];       // buttons Setting radio
TFT_eSPI_Button btMemMan[5];       // buttons mem manager
TFT_eSPI_Button btNet[5];          // buttons Setting Net display
TFT_eSPI_Button btFlot[5];         // buttons flotantes
TFT_eSPI_Button btKEYER[8];        // buttons KEYER setting
TFT_eSPI_Button btSta[8];          // buttons Status (Wifi,Hour, Min, Sec, Rst, TR1, TR2, T5v)
TFT_eSPI_Button btYN[3];           // buttons OK / Cancel / Backspace
TFT_eSPI_Button btKey[50];         // buttons Key 0..9, A..Z & symbols
TFT_eSPI_Button btSel[4];          // botones barra estado: "< Cancel OK >"
TFT_eSPI_Button btMemN[7];         // botones lista memorias visibles, mempos
TFT_eSPI_Button btMemF[7];         // botones lista memorias visibles, frequency
TFT_eSPI_Button btSmeter[2];       // botones Smeter
TFT_eSPI_Button btMenuNav[20];     // botones selección pantalla

byte btCWact[5]={1,1,1,1,1};
byte btCalact[5]={1,1,1,0,1};
byte btSmeact[5]={1,1,0,0,0};
byte btConact[5]={1,1,0,0,0};
byte btTempact[5]={1,1,1,0,0};
byte btPortsact[5]={1,1,1,1,1};
byte btMemManact[5]={0,0,0,0,1};
byte btATUact[5]={1,1,1,0,0};
byte btTPAact[5]={1,1,1,1,0};
byte btSetact[5]={1,1,1,1,1};
byte btSetRadact[5]={1,1,1,1,1};
byte btNetact[5]={1,1,1,1,1};
byte btNavact[5]={1,1,1,1,0};
byte btFlotact[5]={1,1,1,1,1};
byte btYNact[3]={1,1,1};
byte btMenuNavact[20]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0};

byte btMainact[10] = {0,0,0,0,0,0,0,0,0,0};

char btMaintext[10][6] = {"ATU","V/M","Band-","Band+","LSB","CW","RIT","SPL","IFS","ATT"};

int16_t btMainposx[10]={ 0,64,255,255, 64,   0,128,192,128,192}; // position mains buttons
int16_t btMainposy[10]={70,70, 35,  0,105, 105, 70, 70,105,105}; // position mains buttons
int16_t btMaintam[10]={60,60, 65, 65,60, 60, 60, 60,60,60}; // position mains buttons
uint16_t btMaincol[10]={TFT_RED,TFT_YELLOW,TFT_YELLOW,TFT_YELLOW,TFT_YELLOW,TFT_YELLOW,TFT_CYAN,TFT_ORANGE,TFT_YELLOW,TFT_YELLOW};

char btCaltext[5][11]={"Calibrati.","Adj. BFO","SI5351 Ad.","xxx","Reset Fact"};
char btSmetext[5][11]={"Min. Value","Max. Value","xxx","xxx","xxx"};
char btContext[5][11]={"Conn. Mode","Serial2","xxx","xxx","xxx"};
char btTemptext[5][11]={"Probe TR1","Probe TR2","Probe 5v","xxx","xxx"};
char btPortstext[5][11]={"TCP","UDP-S","WS","WEB","UDP-F"};
char btATUtext[5][11]={"Enable","C1","C2","",""};
char btTPAtext[5][11]={"Enable","AGC Comp.","Max Gain","Gain",""};
char btCWtext[5][11]={"Key type","WPM","Sidetone","Delay Time","Start Time"};
char btKEYERtext[8][11]={"from","from","from","from","to","to","to","to"};
char btSettext[5][11]={"Language","CallSign","Latitude","Longitude","Time Zone"};
char btSetRadtext[5][11]={"TX range","SSB auto","Scan range","Scan mode","Resume (s)"};
char btMemMantext[5][11]={"xxx","xxx","xxx","xxx","Clear All"};
char btNettext[5][11]={"Auto Conn.","Scan SSID","Password","WiFi Mode","Static IP"};
char btNavtext[5][8]={"H","<",">","M","xxx"};
char btFlottext[5][6]={"Ent","Scan-","Scan+","Lock",">MEM"};
char btYNtext[3][6]={"OK","ESC","<--"};
char btKeytextL[50][3]={"0","1","2","3","4","5","6","7","8","9",
                        "a","b","c","d","e","f","g","h","i","j",
                        "k","l","m","n","ñ","o","p","q","r","s",
                        "t","u","v","w","x","y","z","","","",
                        "-","=","_",".",",",";","/","(",")","U"};
char btKeytextU[50][3]={"0","1","2","3","4","5","6","7","8","9",
                        "A","B","C","D","E","F","G","H","I","J",
                        "K","L","M","N","Ñ","O","P","Q","R","S",
                        "T","U","V","W","X","Y","Z","", "", "",
                        "-","=","_",".",",",";","/","(",")","L"};
char keytypetext[3][11]={"Straigth","Iambic A","Iambic B"};
char btMenuNavtext[20][6]={"HOME","USER","RADIO","CW","Wifi",
                           "CALIB","KEYER","MEM","ATU","TPA",
                           "SMet","CONN","TEMP","PORTS","15",
                            "16","17","18","19","20"};

byte inTx = 0;                //it is set to 1 if in transmit mode (whatever the reason : cw, ptt or cat)

void  initButtons()
{
  btMainact[0] = inTx==1?1:0;                     // RX / TX
  btMainact[1] = 0;                               // V/M
  btMainact[2] = 0;                               // Band Down
  btMainact[3] = 0;                               // Band Up
  btMainact[4] = conf.isUSB==1?1:0;               // LSB / USB
  strcpy(btMaintext[0],inTx==1?"TX":"ATU");       // TX / ATU (only at RX
  strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB");// LSB / USB
  btMainact[5] = conf.cwMode>0?1:0;               // CW
  btMainact[6] = conf.ritOn==1?1:0;               // RIT
  btMainact[7] = conf.splitOn==1?1:0;             // Split
  btMainact[8] = conf.ifShiftValue!=0?1:0;        // IFS
  btMainact[9] = conf.attLevel>0?1:0;             // ATT
}

