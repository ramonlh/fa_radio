
#pragma once

boolean filesok=false;

unsigned long tini=0;

char watermark[16]; // código de verificación uBitx

const char fileconf[]="/ubitx.cnf";
char filememo[]="/ubitx.mem";
char filecommon[]="/common.txt";
char filespanish[]="/spanish.txt";
char fileenglish[]="/english.txt";
char filegerman[]="/german.txt";

#define IAMBICB 0x10 // 0 for Iambic A, 1 for Iambic B
unsigned char keyerControl = IAMBICB;

int shiftDisplayAdjustVal = 0;  //
byte isIFShift = 0;             //1 = ifShift, 2 extend
//sideTonePitch + sideToneSub = sideTone
byte sideTonePitch = 0;
byte sideToneSub = 0;

unsigned long OSC0 = 0;
unsigned long OSC1 = 0;
unsigned long OSC2 = 0;
unsigned long IF1 = 0;
unsigned long IF2 = 0;

byte scanF = 0;   // 1: DW, 2: UP
boolean readingspectrum = false;
char buff[20];    // 20 bytes, auxiliar
char auxtft[30];                // 30 bytes, auxiliar para varios
String msg;                     // String donde se construye la respuesta HTML qu e se envía al cliente

float vtotvalue=0.0;    
float itotvalue=0.0; 

// smeter
int maxsmeter=0;
int minsmeter=29999;
int16_t smetervalue=0;
int16_t powervalue=0;
boolean calSmeterReq=false;

char auxchar[130];              // 130 bytes, auxiliar 

// spectrum values
int spval[250];
int maxvalspectrum=0;

char freqpartant[9][4]={"","","","","","","","",""};

int nAP=0;                      // 2 bytes, redes encontradas con scanAP
int nAPact=0;                   // 2 bytes, redes actual
boolean internetON = false;
unsigned long countfaulttime=29999;       // veces que ha fallado la sincronización
boolean txCAT = false;        //turned on if the transmitting due to a CAT command
boolean txTFT = false;        //turned on if the transmitting due to a TFT command
int lastframemode=0;
int mempos=0;   // posición actual de memoria
int memlin=0;   // posición actual en la pantalla de lista de memorias
byte tftapactual=0;
char flecha[4][3]={"<","<<",">>",">"};
unsigned long lasttimeknob=0;
int16_t smeterlast=0;
char udpAddress[16] = "192.168.000.000";
// SWR
int nSWR=100;
float SWR=1.0;
float SWRreal=1.0;
float swrmin=999.99;   
float vFORc; 
float vREFc; 
float wFORc;
float wREFc;
unsigned long maxRitdev=5000;  // max. deviation on RIT
char keyDown = 0;             //in cw mode, denotes the carrier is being transmitted
//DialLock
byte isTxType = 0;    //000000[0 - isSplit] [0 - isTXStop]
unsigned long lastswr=0;
unsigned long swrper=10; 
boolean firstmem=false;
int16_t cwcodevalue=0;

// space for save freq actual
  uint8_t vfoActiveAct;      // VFO 
  uint8_t isUSBAct;       // mode
  uint8_t cwModeAct;      // CW mode
  uint8_t ritOnAct;       // RIT
  uint8_t splitOnAct;     // SPL
  unsigned long frequencyAct;  // frequency
  unsigned long ritTxFrequencyAct;  // frequency TX Rit
  uint8_t isUsbsplAct;      // isUSB TX split mode
  uint8_t cwModesplAct;     // CW mode TX split
  unsigned long ftxsplAct;  // frequency TX Split

uint8_t isCWAutoMode = 0;          //0 : none, 1 : CW_AutoMode_Menu_Selection, 2 : CW_AutoMode Sending
byte keylock=0; // bloqueo teclado y botones
