
#pragma once

#include "atu.h"
void acopla();


// ws_server
void sendwsData(uint8_t c);
// funciones externas
// radio.h
void setRIT(uint8_t value);
void setSPLIT(uint8_t value);
void setFrequency(unsigned long f);
void setNextHamBandFreq(unsigned long f, int moveDirection);
void setCW(uint8_t value);
void setIFS(int value, uint8_t local);
void setATT(int value, byte local);
void startTx(uint8_t txMode, uint8_t isDisplayUpdate);
void setSTEP(byte value);
void setVFO(byte value);
void setupFreq();
void setupBFO();
void deleteMemo();
void setTPAenable(boolean enablevalue);
void setTPAComp();
void setTPAGain(uint8_t gain); // de -28 a +30,  (0-12)
void setTPAMaxGain(uint8_t maxgain); // de -28 a +30,  (0-12)
void calcSmeterScale();
void setupconnmode();
void setupserial2mode();
void setSCAN(byte value);
void setMEMtoVFO(int pos);
void saveVFOtoMem();
uint8_t delay_background(unsigned delayTime, byte fromType, byte swr);

// atu.h
void setupATU(uint8_t C);

// wifi_f.h
void setWiFi();

// cw_f.h
void cwKeydown();
void cwKeyUp();


/********************************************
// you must define this pins at file "User_setup.h" from TFT_eSPI library
// these lines have no effect and are only informative
// TFT pins for uBITX
#define BITX        // for use  at user_setup.h from TFT_eSPI library
#define TFT_RST  -1 // Reset pin (could connect to RST pin)
#define TFT_DC   21 // Data Command control pin   
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   32 // Chip select control pin    
#define TOUCH_CS  5 // Chip select pin (T_CS) of touch screen
*********************************************/
 
#include <TFT_eSPI.h>     // Graphics and font library for ILI9341 driver chip

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

uint8_t tftpage = 0;

void clearTFT()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void DisplayVersionInfo(const __FlashStringHelper * fwVersionInfo)
{
  tft.print("Version: ");
  tft.print(FIRMWARE_VERSION_INFO);
}

void initTFT() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.println("Iniciando Radio...");
  DisplayVersionInfo(FIRMWARE_VERSION_INFO);
}

//returns true if the button is pressed
int btnDown(void) { 
  return digitalRead(FBUTTON) == HIGH?0:1; 
  }

void displayTime()
{
  //if (!WiFi.isConnected()) return;
  tft.setTextDatum(MR_DATUM);     // derecha
  char auxh[3]=""; if (hour()<10) strcpy(auxh,"0"); strcat(auxh,itoa(hour(),buff,10)); 
  char auxm[3]=""; if (minute()<10) strcpy(auxm,"0"); strcat(auxm,itoa(minute(),buff,10)); 
  char auxs[3]=""; if (second()<10) strcpy(auxs,"0"); strcat(auxs,itoa(second(),buff,10));
  for (int i=1;i<4;i++)
    {
    btSta[i].initButtonUL(&tft,btStaposx[i],btStaposy[i],btStatamx[i],btStatamy[i],1,TFT_BLACK,TFT_WHITE,auxh,2);
    btSta[i].drawButton();
    }
  tft.setTextSize(2);  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(ML_DATUM);     // izquierda
}

void displayTemp()
{
  //for (int i=0; i<3; i++) { s2("i:"); s2(i); s2("-->"); s2(MbR[i]); s2(crlf); }
  tft.setTextDatum(MR_DATUM);     // derecha
  tft.setTextColor(TFT_WHITE);
  for (int i=5; i<=7; i++) {
    btSta[i].initButtonUL(&tft,btStaposx[i],btStaposy[i],btStatamx[i],btStatamy[i],2,MbR[i-5]>4000?TFT_ORANGE:MbR[i-5]>4500?TFT_RED:TFT_GREEN,TFT_BLACK,itoa(MbR[i-5]/100,buff,10),1);
    btSta[i].drawButton();
    }
  tft.setTextDatum(ML_DATUM);     // izquierda
}

void displayVI(int tam, int x, int y)
{
  tft.setTextDatum(TL_DATUM);     // izquierda
  tft.setTextSize(2); tft.setTextColor(TFT_GREEN); 
  tft.fillRect(x, y, 80, tam, TFT_BLACK);
  tft.drawFloat(vtotvalue, 1,  x, y);
  tft.drawFloat(itotvalue, 3, x+60, y);
}

void displayStatus()
{
  displayTime();
  displayTemp();
  displayVI(20, 60, 300);
}

void displayMain()
{
  if (tftpage==0) {
    initButtons();
    // botones superiores
    for (uint8_t i=0;i<10;i++)
      {
      btMain[i].initButtonUL(&tft,btMainposx[i],btMainposy[i],btMaintamx[i],btMaintamy[i],2,btMainact[i]==1?btMaincol[i]:TFT_WHITE,TFT_BLACK,btMaintext[i],2);
      btMain[i].drawButton();
      }
    }
}

void displayNav()   // botones navegación
{
  strcpy(btNavtext[1],tftpage==22?"Ed":"<");
  strcpy(btNavtext[2],tftpage==22?"Ok":">");
  strcpy(btNavtext[3],"M");
  #ifdef PANTALLA28P
   strcpy(btNavtext[3],"x");    
  #endif
  for (uint8_t i=0;i<5;i++) if (btNavact[i]==1)
    {
    btNav[i].initButtonUL(&tft,btNavposx[i],btNavposy[i],btNavtamx[i],btNavtamy[i],2,TFT_WHITE,TFT_BLACK,btNavtext[i],2);
    btNav[i].drawButton();
    }
}

void displayFlot()
{
  // botones flotantes
  btFlot[1].initButtonUL(&tft,btFlotposx[1],btFlotposy[1],btFlottamx[1],btFlottamy[1],2,scanF==1?TFT_CYAN:TFT_WHITE,TFT_BLACK,btFlottext[1],2);
  btFlot[1].drawButton();
  btFlot[2].initButtonUL(&tft,btFlotposx[2],btFlotposy[2],btFlottamx[2],btFlottamy[2],2,scanF==2?TFT_CYAN:TFT_WHITE,TFT_BLACK,btFlottext[2],2);
  btFlot[2].drawButton();
  btFlot[3].initButtonUL(&tft,btFlotposx[3],btFlotposy[3],btFlottamx[3],btFlottamy[3],2,keylock>0?TFT_RED:TFT_WHITE,TFT_BLACK,btFlottext[3],2);
  btFlot[3].drawButton();
  if (conf.framemode!=3)
    {
    btFlot[0].initButtonUL(&tft,btFlotposx[0],btFlotposy[0],btFlottamx[0],btFlottamy[0],2,TFT_WHITE,TFT_BLACK,btFlottext[0],2);
    btFlot[0].drawButton();
    btFlot[4].initButtonUL(&tft,btFlotposx[4],btFlotposy[4],btFlottamx[4],btFlottamy[4],2,TFT_WHITE,TFT_BLACK,btFlottext[4],2);
    btFlot[4].drawButton();
    }
}

int xant=190; int yant=210;
int xantR=190; int yantR=210;
int backcol=TFT_BLACK; int textcol=TFT_WHITE; 
int dialcol=TFT_YELLOW; int needlecol=TFT_GREEN;

void displayneedle(int value,int xcen,int ycen,int rad,int min,int max,int angle)
{
  float factorg=float((max-min))/float(angle); // unidades por grado
  int grados=(90+angle/2)-((value-min)/factorg);
  float radi=grados*degtoradf;
  int x=xcen+(rad*8/10)*cos(radi);
  int y=ycen-(rad*8/10)*sin(radi);
  tft.drawLine(xcen,ycen,xant,yant,backcol); 
    tft.drawLine(xcen+1,ycen,xant+1,yant,backcol); 
  tft.drawLine(xcen,ycen,x,y,needlecol); 
    tft.drawLine(xcen+1,ycen,x+1,y,needlecol); 
  xant=x; yant=y;
}

void displaygauge_ubitx(byte tipo,int value,int xcen,int ycen,int rad,byte fondo,int min,int max,int angle,int ndiv)
{
  displayneedle(value,xcen,ycen,rad,min,max,angle);
  if (angle>180) angle=180; if (value<min) value=min; if (value>max) value=max;
  int x0=rad>=100?0:xcen-rad*7/6; 
  int y0=rad>100?0:ycen-rad*7/5;
  int wx=rad>100?320:rad*7/3;
  int wy=rad>100?240:rad*3/2;
  tft.drawRect(x0,y0,wx,wy,textcol);
  tft.fillCircle(xcen,ycen,2,needlecol);
  int dgrdiv=angle/ndiv;      // grados por división
  for (byte i=0; i<ndiv+1;i++) 
    if (i%2==0)
      {
      int dgr=(90+angle/2)-dgrdiv*i;
      float drad=dgr*degtoradf;
      float auxcos=cos(drad);
      float auxsin=sin(drad);
      int x=xcen+(rad*(rad>100?9:10)/10)*auxcos;
      int y=ycen-(rad*(rad>100?9:10)/10)*auxsin;
      int colorline=value==conf.squelchval?TFT_WHITE:TFT_YELLOW;
      tft.drawLine(x-5*auxcos,y+5*auxsin,x,y,colorline);
      tft.drawLine(x-5*auxcos+1,y+5*auxsin,x+1,y,colorline);
      tft.setTextColor(textcol, backcol); tft.setTextSize(rad<100?1:2);
      if (tipo==0) { 
        tft.drawNumber(min+(max-min)*i/ndiv, x-5+5*auxcos, y-10); }     // generic
      else if (tipo==1)   // smeter
        { 
        if (calSmeterReq) tft.setTextColor(TFT_RED, backcol);  
        tft.drawString(conf.smeterTit[i], x+(rad/8)*auxcos, y-rad*auxsin/5); 
        }
      else if (tipo==2) { 
        tft.drawNumber(min+i,x+(rad/10)*auxcos, y-rad*auxsin/8); } // SWR
      else if (tipo==3) { 
        tft.drawNumber(i*15,x+(rad/10)*auxcos, y-rad*auxsin/8); }  // CAP1 & CAP2
      else if (tipo==5) { 
        -tft.drawString(conf.smeterTit[i],x+(rad/10)*auxcos, y-rad*auxsin/8); }  // adjust squelch level 
      }   
  if (tipo==1)    // S-meter
    {
    char auxC[4]="OFF";
    if (conf.scanmode==1) 
      {
      strcpy(auxC,"SR"); 
      
      }
    else if (conf.scanmode==2)
      {
      strcpy(auxC,"St");  
      }
    btSmeter[0].initButtonUL(&tft,xcen-(rad+7),ycen-16,30,20,2,conf.scanmode==0?TFT_LIGHTGREY:conf.scanmode==1?TFT_GREEN:TFT_ORANGE,TFT_BLACK,auxC,1);
    btSmeter[0].drawButton();
    btSmeter[1].initButtonUL(&tft,xcen+rad-23,ycen-16,30,20,2,TFT_ORANGE,TFT_BLACK,"Lev.",1);
    btSmeter[1].drawButton();
    }
  else if (tipo==2)  // SWR
    { tft.drawString("SWR",x0+3,y0+10); }   
}

void displaygauge_FA(byte tipo,int value,int xcen,int ycen,int rad,byte fondo,int min,int max,int angle,int ndiv)
{
  displayneedle(value,xcen,ycen,rad,min,max,angle);
  if (angle>180) angle=180; if (value<min) value=min; if (value>max) value=max;
  int x0=rad>=100?0:xcen-rad*7/6; 
  int y0=rad>100?0:ycen-rad*7/5;
  int wx=rad>100?320:rad*7/3;
  int wy=rad>100?240:rad*3/2;
  tft.drawRect(x0,y0,wx,wy,textcol);
  tft.fillCircle(xcen,ycen,2,needlecol);
  int dgrdiv=angle/ndiv;      // grados por división
  for (byte i=0; i<ndiv+1;i++) 
    if (i%2==0)
      {
      int dgr=(90+angle/2)-dgrdiv*i;
      float drad=dgr*degtoradf;
      float auxcos=cos(drad);
      float auxsin=sin(drad);
      int x=xcen+(rad*(rad>100?9:10)/10)*auxcos;
      int y=ycen-(rad*(rad>100?9:10)/10)*auxsin;
      int colorline=value==conf.squelchval?TFT_WHITE:TFT_YELLOW;
      tft.drawLine(x-5*auxcos,y+5*auxsin,x,y,colorline);
      tft.drawLine(x-5*auxcos+1,y+5*auxsin,x+1,y,colorline);
      tft.setTextColor(textcol, backcol); tft.setTextSize(rad<100?1:2);
      if (tipo==0) { 
        tft.drawNumber(min+(max-min)*i/ndiv, x-5+5*auxcos, y-10); }     // generic
      else if (tipo==1)   // smeter
        { 
        if (calSmeterReq) tft.setTextColor(TFT_RED, backcol);  
        tft.drawString(conf.smeterTit[i], x+(rad/8)*auxcos, y-rad*auxsin/5); 
        }
      else if (tipo==2) { 
        tft.drawNumber(min+i,x+(rad/10)*auxcos, y-rad*auxsin/8); } // SWR
      else if (tipo==3) { 
        tft.drawNumber(i*15,x+(rad/10)*auxcos, y-rad*auxsin/8); }  // CAP1 & CAP2
      else if (tipo==5) { 
        -tft.drawString(conf.smeterTit[i],x+(rad/10)*auxcos, y-rad*auxsin/8); }  // adjust squelch level 
      }   
  if (tipo==1)    // S-meter
    {
    char auxC[4]="OFF";
    if (conf.scanmode==1) 
      {
      strcpy(auxC,"SR"); 
      
      }
    else if (conf.scanmode==2)
      {
      strcpy(auxC,"St");  
      }
    btSmeter[0].initButtonUL(&tft,xcen-(rad+7),ycen-16,30,20,2,conf.scanmode==0?TFT_LIGHTGREY:conf.scanmode==1?TFT_GREEN:TFT_ORANGE,TFT_BLACK,auxC,1);
    btSmeter[0].drawButton();
    btSmeter[1].initButtonUL(&tft,xcen+rad-23,ycen-16,30,20,2,TFT_ORANGE,TFT_BLACK,"Lev.",1);
    btSmeter[1].drawButton();
    }
  else if (tipo==2)  // SWR
    { tft.drawString("SWR",x0+3,y0+10); }   
}

void displaygauge(byte tipo,int value,byte fondo,int min,int max,int angle,int ndiv)
{
#ifdef UBITX_RADIO
  displaygauge_ubitx(tipo,value,180,200,50,fondo,min,max,angle,ndiv);
#endif
#ifdef FA_RADIO
  displaygauge_FA(tipo,value,295,175,75,fondo,min,max,angle,ndiv);
#endif
}

void displaySmeter(byte fondo)
{
  displaygauge(1,smetervalue,fondo,0,90,90,15);
}

void displaysetupSmeter()
{
  for (byte i=0;i<5;i++) if (btSmeact[i]==1)
    {
    btSME[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btSmetext[i],2);
    btSME[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("10 - S-Meter Settings",50,0);
  tft.drawNumber(conf.sMeterLevels[0],230,40);
  tft.drawNumber(conf.sMeterLevels[15],230,75);

  tft.drawNumber(minsmeter,50,110);
  tft.drawString("<--->",230,110);
  tft.drawNumber(maxsmeter,300,110);
}

int ifShiftValueant=0;

void displayIFS_ubitx(byte tam,int x, int y)
{
 tft.setTextDatum(TL_DATUM);     // izquierda
  if (tam==0)   // tamaño pequeño
    {
    int auxvalue=conf.ifShiftValue/160+12;
    for (byte i=0; i<25; i++)
      {
      tft.fillRect(x+i*3,y,2,10,(auxvalue>=i-1)&&(auxvalue<=i+1)?conf.ifShiftValue!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
      }
    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
    tft.drawString("IFS",x,y-7);
    tft.drawNumber(conf.ifShiftValue,x+30,y-7);
    }
  else      // tamaño grande
    {
    int auxvalue=conf.ifShiftValue/40+50;
    for (byte i=0; i<100; i++)
      {
      tft.fillRect(10+i*3,50,2,0,(auxvalue>=i-1) && (auxvalue<=i+1)?TFT_CYAN:TFT_ORANGE);
      }
    tft.drawRect(156,45,10,4,TFT_CYAN);
    tft.drawRect(156,81,10,4,TFT_CYAN);
    }
}

void displayIFS_FA(byte tam,int x, int y)
{
 tft.setTextDatum(TL_DATUM);     // izquierda
 if (tam==0)   // tamaño pequeño
    {
    int auxvalue=conf.ifShiftValue/160+12;
    for (byte i=0; i<25; i++)
      {
      tft.fillRect(x+i*3,y,2,20,(auxvalue>=i-1)&&(auxvalue<=i+1)?conf.ifShiftValue!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
      }
    tft.setTextSize(2); tft.setTextColor(TFT_WHITE);
    tft.drawString("IFS",x-40,y+5);
    tft.drawNumber(conf.ifShiftValue,x+80,y+5);
    tft.drawString("Hz",x+100,y+5);
    }
  else      // tamaño grande
    {
    int auxvalue=conf.ifShiftValue/40+50;
    for (byte i=0; i<100; i++)
      {
      tft.fillRect(x+i*3,y,2,30,(auxvalue>=i-1) && (auxvalue<=i+1)?TFT_CYAN:TFT_ORANGE);
//      tft.fillRect(10+i*3,50,2,30,(auxvalue>=i-1) && (auxvalue<=i+1)?TFT_CYAN:TFT_ORANGE);
      }
    tft.drawRect(x,y,10,4,TFT_CYAN);
    tft.drawRect(x,y+30,10,4,TFT_CYAN);
    }
}

void displayIFS(byte tam)
{
#ifdef UBITX_RADIO
  displayIFS_ubitx(tam, 35, 190);
#endif
#ifdef FA_RADIO
  displayIFS_FA(tam, 90, 130);
#endif
}

void displayWiFiSt()
{
  if ((tftpage>0) || (conf.framemode>1)) return;
  btSta[0].initButtonUL(&tft,btStaposx[0],btStaposy[0],btStatamx[0],btStatamy[0],2,WiFi.isConnected()?TFT_GREEN:TFT_RED,TFT_BLACK,"WiFi",1);
  btSta[0].drawButton();
}

void displayRstBt()
{
  btSta[4].initButtonUL(&tft,btStaposx[4],btStaposy[4],btStatamx[4],btStatamy[4],2,TFT_WHITE,TFT_BLACK,"Rst",1);
  btSta[4].drawButton();
}

void drawStringB(char* cad,int x, int y, int w, int h, int backcolor, int textcolor, int size)    // draw Number as utton 
{
  TFT_eSPI_Button btN;          // button
  btN.initButtonUL(&tft,x,y,w,h,2,backcolor,textcolor,cad,size);
  btN.drawButton();
}

void drawNumberB(long value,int x, int y, int w, int h, int backcolor, int textcolor, int size)    // draw Number as utton 
{
  TFT_eSPI_Button btN;          // button
  btN.initButtonUL(&tft,x,y,w,h,2,backcolor,textcolor,itoa(value,buff,10),size);
  btN.drawButton();
}

void displaybarSmeter(int x,int y,int min,int max, int divi)   // display bar
{
  if (smetervalue<min) smetervalue=min; if (smetervalue>max) smetervalue=max;
  int bkcolor=TFT_GREEN;
  for (int i=0;i<16;i++)    // titles
    if (i%2==0)
      {
      drawStringB(conf.smeterTit[i],x+i*14-6,y-12,20,12,TFT_BLACK,calSmeterReq?TFT_RED:TFT_WHITE,1);  
      }
  for (int i=0;i<divi;i++)
    {
    bkcolor=i<smetervalue*divi/90?TFT_GREEN:TFT_LIGHTGREY;
    tft.fillRect(x+i*3, y, 2, 20,bkcolor); 
    }
}

void displaySpectrum()
{
  tft.fillRect(0,140,250,50,TFT_BLACK);
  tft.setTextSize(1);
  drawStringB("Khz",60,197,30,12,TFT_BLACK,TFT_WHITE,1);
  drawStringB("Khz",180,197,30,12,TFT_BLACK,TFT_WHITE,1);
  drawNumberB(conf.frequency/1000-125,0,197,40,12,TFT_BLACK,TFT_WHITE,1);
  drawNumberB(conf.frequency/1000,105,197,40,12,TFT_BLACK,TFT_WHITE,1);
  drawNumberB(conf.frequency/1000+125,210,197,40,12,TFT_BLACK,TFT_WHITE,1);
  for (int i=0; i<250; i++) { tft.drawLine(i,194,i,194-spval[i],TFT_GREEN);  } // spval entre 0 y 45
  if (maxvalspectrum>0)
    {
    tft.drawLine(maxvalspectrum,140,maxvalspectrum,194-spval[maxvalspectrum],TFT_YELLOW);  // valor máximo
    drawNumberB(conf.frequency/1000-125+maxvalspectrum,maxvalspectrum+1,140,40,12,TFT_DARKGREY,TFT_YELLOW,1);
    }
  tft.drawLine(125,140,125,195,TFT_WHITE);
  tft.drawLine(0,195,250,195,TFT_WHITE);
}

void displayFreqs()
{
  tft.setTextSize(2);  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("OSC0",0,153); drawNumberB(OSC0,55,140,100,22,TFT_BLACK,TFT_WHITE,2); 
  tft.drawString("OSC1",0,173); drawNumberB(OSC1,55,160,100,22,TFT_BLACK,TFT_YELLOW,2); 
  tft.drawString("OSC2",0,193); drawNumberB(OSC2,55,180,100,22,TFT_BLACK,TFT_YELLOW,2); 

  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("IF1",160,153); drawNumberB(IF1,215,140,100,22,TFT_BLACK,TFT_WHITE,2); 
 // tft.drawString("IF2",160,173); drawNumberB(IF2,215,160,100,22,TFT_BLACK,TFT_WHITE,2); 
  tft.drawString("Cal",160,193); drawNumberB(conf.calibration,215,180,100,22,TFT_BLACK,TFT_WHITE,2); 
}

void displayATT_ubitx(byte tam, int x, int y)
{
  tft.setTextDatum(TL_DATUM);     // izquierda
  if (tam==0)   // pequeño
    {
    for (byte i=0; i<25;i++)
      tft.fillRect(x+i*3,y, 2, 10, conf.attLevel*60/250>i*3?conf.attLevel!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
    tft.drawString("ATT",x,y-7);
    tft.drawNumber(conf.attLevel,x+30,y-7);
    }
  else    // grande
    {
    for (byte i=0; i<25;i++)
      tft.fillRect(35+i*10,50, 9, 30, conf.attLevel>i*10?conf.attLevel!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
    }
}

void displayATT_FA(byte tam, int x, int y)
{
  tft.setTextDatum(TL_DATUM);     // izquierda
  if (tam==0)   // pequeño
    {
    for (byte i=0; i<25;i++)
      tft.fillRect(x+i*3, y, 2, 20, conf.attLevel*60/250>i*3?conf.attLevel!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
    tft.setTextSize(2); tft.setTextColor(TFT_WHITE);
    tft.drawString("ATT", x-40, y+5);
    tft.drawNumber(conf.attLevel, x+80, y+5);
    tft.drawString("db", x+100, y+5);
    }
  else    // grande
    {
    for (byte i=0; i<25;i++)
      tft.fillRect(x+i*10, y, 9, 30, conf.attLevel>i*10?conf.attLevel!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
    }
}

void displayATT(byte tam)
{
#ifdef UBITX_RADIO
  displayATT_ubitx(tam, 35, 150);
#endif
#ifdef FA_RADIO
  displayATT_FA(tam, 90, 130);
#endif
}

void displayFreq_ubitx(byte fast, byte fA, byte fB, byte fAux)
{
  if ((tftpage>0) && (tftpage!=22)) return;
  char freqpart[9][4]={"","","","","","","","",""};
  char freqpartSec[9][4]={"","","","","","","","",""};
  unsigned long f=conf.frequency;
  unsigned long fsec;
  int16_t colf;
  if (conf.ritOn==1) colf=TFT_CYAN; else if (conf.splitOn==1) colf=TFT_ORANGE; else colf=TFT_GREEN;
  if (conf.ritOn==1) fsec=conf.ritTxFrequency;
  else if (conf.splitOn==1) fsec=conf.frequencyB;
  else fsec=conf.vfoActive==VFO_A?conf.frequencyB:conf.frequencyA;
  for (byte i=0;i<9;i++)    // separa cifras
    {
    long auxL=long(pow(10,8-i));
    if ((i==0)&&(f<100000000)) strcat(freqpart[i]," "); 
    else if ((i==1)&&(f<10000000)) strcat(freqpart[i]," "); 
    else strcat(freqpart[i],itoa((f/auxL)%10,buff,10));
    if ((i==0)&&(fsec<100000000)) strcat(freqpartSec[i]," "); 
    else if ((i==1)&&(fsec<10000000)) strcat(freqpartSec[i]," "); 
    else strcat(freqpartSec[i],itoa((fsec/auxL)%10,buff,10));
    }
  tft.setTextSize(2); tft.setTextColor(colf);
  tft.setTextDatum(ML_DATUM);
  char auxchar[2][5]={"A","B"};
  
  if (fAux==1)
    {
    if (conf.vfoActive==VFO_B) { strcpy(auxchar[0],"B"); strcpy(auxchar[1],"A"); }
    btVFO[1].initButtonUL(&tft,0,0,20,20,2,TFT_BLACK,colf,auxchar[1],2);
    btVFO[1].drawButton();
    if ((conf.ritOn==1) || (conf.splitOn==1))
      {
      tft.drawString("TX",21,0);
      if (conf.splitOn==1) tft.drawString(conf.isUSBB==0?"LSB":"USB",80,0);
      }
    else if ((conf.ritOn==0) && (conf.splitOn==0))
      {
      tft.setTextColor(TFT_BLACK);
      tft.drawString("TX",21,0);
      if (conf.vfoActive==VFO_A)
        {
        tft.drawString(conf.cwModeB==0?"   ":"CW-",45,0);
        tft.drawString(conf.isUSBB==0?"LSB":"USB",80,0);
        }
      else
        {
        tft.drawString(conf.cwModeA==0?"   ":"CW-",45,0);
        tft.drawString(conf.isUSBA==0?"LSB":"USB",80,0);
        }
      }
  
    }
  if (fB==1)
    {
    for (byte i=0;i<9;i++)    // frecuencia secundaria 
      {
      btFreqTx[i].initButtonUL(&tft,140+12*i,0,12,20,2,TFT_BLACK,colf,freqpartSec[i],2);
      btFreqTx[i].drawButton();
      }
    tft.fillCircle(173,14,1,colf);    // punto de miles
    btVFO[0].initButtonUL(&tft,0,30,30,30,2,TFT_BLACK,TFT_WHITE,auxchar[0],3);
    btVFO[0].drawButton();
    }

  if (fA==1)
    {
    for (byte i=0;i<9;i++)      // frecuencia principal  ping 192.168.1.149
    
      {
      if ((fast==0) || (strcmp(freqpartant[i],freqpart[i]) != 0))    // son diferentes
        {
        strcpy(freqpartant[i],freqpart[i]);
        if (i<6) btFreq[i].initButtonUL(&tft,35+26*i,30,26,40,2,TFT_BLACK,conf.tuneStepIndex==i?TFT_YELLOW:TFT_WHITE,freqpart[i],4);
        else btFreq[i].initButtonUL(&tft,71+20*i,38,20,30,2,TFT_BLACK,conf.tuneStepIndex==i?TFT_YELLOW:TFT_WHITE,freqpart[i],3);
        btFreq[i].drawButton();
        }
      }
    tft.fillCircle(110,62,2,TFT_WHITE);    // punto de miles
    }
}

void displayFreq_FA(byte fast, byte fA, byte fB, byte fAux)
{
  tft.setTextDatum(TL_DATUM);     // izquierda
  if ((tftpage>0) && (tftpage!=22)) return;
  char freqpart[9][4]={"","","","","","","","",""};
  char freqpartSec[9][4]={"","","","","","","","",""};
  unsigned long f=conf.frequency;
  unsigned long fsec;
  int16_t colf;
  if (conf.ritOn==1) colf=TFT_CYAN; else if (conf.splitOn==1) colf=TFT_ORANGE; else colf=TFT_GREEN;
  if (conf.ritOn==1) fsec=conf.ritTxFrequency;
  else if (conf.splitOn==1) fsec=conf.frequencyB;
  else fsec=conf.vfoActive==VFO_A?conf.frequencyB:conf.frequencyA;
  for (byte i=0;i<9;i++)    // separa cifras
    {
    long auxL=long(pow(10,8-i));
    if ((i==0)&&(f<100000000)) strcat(freqpart[i]," "); 
    else if ((i==1)&&(f<10000000)) strcat(freqpart[i]," "); 
    else strcat(freqpart[i],itoa((f/auxL)%10,buff,10));
    if ((i==0)&&(fsec<100000000)) strcat(freqpartSec[i]," "); 
    else if ((i==1)&&(fsec<10000000)) strcat(freqpartSec[i]," "); 
    else strcat(freqpartSec[i],itoa((fsec/auxL)%10,buff,10));
    }
  tft.setTextSize(2); tft.setTextColor(colf);
  char auxchar[2][5]={"A","B"};
  
  if (fAux==1)
    {
    if (conf.vfoActive==VFO_B) { strcpy(auxchar[0],"B"); strcpy(auxchar[1],"A"); }
    btVFO[1].initButtonUL(&tft, 353, 10, 30,20,2,TFT_BLACK,colf,auxchar[1],2);
    btVFO[1].drawButton();
    if ((conf.ritOn==1) || (conf.splitOn==1))
      {
      tft.drawString("TX",21,0);
      if (conf.splitOn==1) tft.drawString(conf.isUSBB==0?"LSB":"USB",80,0);
      }
    else if ((conf.ritOn==0) && (conf.splitOn==0))
      {
      tft.setTextColor(TFT_BLACK);
      tft.drawString("TX",21,0);
      if (conf.vfoActive==VFO_A)
        {
        tft.drawString(conf.cwModeB==0?"   ":"CW-",45,0);
        tft.drawString(conf.isUSBB==0?"LSB":"USB",80,0);
        }
      else
        {
        tft.drawString(conf.cwModeA==0?"   ":"CW-",45,0);
        tft.drawString(conf.isUSBA==0?"LSB":"USB",80,0);
        }
      }
  
    }
  // frecuencia secundaria
  if (fB==1)
    {
    for (byte i=0;i<9;i++)    // frecuencia secundaria 
      {
      btFreqTx[i].initButtonUL(&tft,212+14*i,8,20,20,2,TFT_BLACK,colf,freqpartSec[i],2);
      btFreqTx[i].drawButton();
      }
    tft.fillCircle(256,21,1,colf);    // punto de miles
    btVFO[0].initButtonUL(&tft, 353,35,30,30,2,TFT_LIGHTGREY,TFT_WHITE,auxchar[0],3);
    tft.setTextDatum(MC_DATUM);     // izquierda
    btVFO[0].drawButton();
    }
  // frecuencia principal
  if (fA==1)
    {
    for (byte i=0;i<9;i++)      // frecuencia principal  
    
      {
      if ((fast==0) || (strcmp(freqpartant[i],freqpart[i]) != 0))    // son diferentes
        {
        strcpy(freqpartant[i],freqpart[i]);
        if (i<6) btFreq[i].initButtonUL(&tft,125+26*i,30,26,40,2,TFT_BLACK,conf.tuneStepIndex==i?TFT_YELLOW:TFT_WHITE,freqpart[i],4);
        else btFreq[i].initButtonUL(&tft,161+20*i,38,20,30,2,TFT_BLACK,conf.tuneStepIndex==i?TFT_YELLOW:TFT_WHITE,freqpart[i],3);
        btFreq[i].drawButton();
        }
      }
    tft.fillCircle(201,60,2,TFT_WHITE);    // punto de miles
    }
}

void displayFreq(byte fast, byte fA, byte fB, byte fAux)
{
#ifdef UBITX_RADIO
  displayFreq_ubitx(fast, fA, fB, fAux);
#endif
#ifdef FA_RADIO
  displayFreq_FA(fast, fA, fB, fAux);
#endif
}

void displayUSERSet()
{
  // botones setting
  for (byte i=0;i<5;i++) if (btSetact[i]==1)
    {
    btSet[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btSettext[i],2);
    btSet[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("1 - USER setting",50,0);
  tft.drawString(conf.lang==0?"Español":"English",230,40);
  tft.drawString(conf.CallSign,230,75);
  tft.drawNumber(conf.latitud,230,110);
  tft.drawNumber(conf.longitud,230,145);
  tft.drawString(timezonetext[conf.timezone],230,180);
}

void displaySetRad()
{
  // botones setting radio
  int backcolor=TFT_WHITE;
  for (byte i=0;i<5;i++) if (btSetRadact[i]==1)
    {
    if (i==1) backcolor=conf.autoMode==1?TFT_YELLOW:TFT_WHITE;
    else backcolor=TFT_WHITE;
    btSetRad[i].initButtonUL(&tft,50,35*i+30,160,30,2,backcolor,TFT_BLACK,btSetRadtext[i],2);
    btSetRad[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("2 - RADIO Setup",50,0);
  tft.drawString(conf.TXall==0?"Ham Bands":"All Range",230,40);
  tft.drawString(conf.autoMode==0?"NO":"YES",230,75);
  tft.drawString(conf.scanallf==0?"Ham Bands":"All Range",230,110);
  tft.drawString(conf.scanmode==0?"No Stop":conf.scanmode==1?"Stop-Resume":"Stop",230,145);
  tft.drawNumber(conf.scandelay,230,180);
}

void displayCWSet()
{
  // botones setting
  for (byte i=0;i<5;i++) if (btCWact[i]==1)
    {
    btCW[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btCWtext[i],2);
    btCW[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("3 - CW setting",50,0);
  tft.drawString(keytypetext[conf.cwKeyType],230,40);
  tft.drawNumber(1200/conf.cwSpeed,230,75);
  tft.drawNumber(conf.sideTone,230,110);
  tft.drawNumber(conf.cwDelayTime,230,145);
  tft.drawNumber(conf.delayBeforeCWStartTime,230,180);
}

void displayNet()
{
  // botones setting Net
  btNet[0].initButtonUL(&tft,50,35*0+30,160,30,2,conf.autoWiFi==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btNettext[0],2);
  btNet[0].drawButton();
  for (byte i=1;i<4;i++) if (btNetact[i]==1)
    {
    btNet[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btNettext[i],2);
    btNet[i].drawButton();
    }
  btNet[4].initButtonUL(&tft,50,35*4+30,160,30,2,conf.staticIP==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btNettext[4],2);
  btNet[4].drawButton();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("4 - WiFi setting",50,0);
  tft.drawString(conf.autoWiFi==1?"YES":"NO",230,40);
  tft.drawString(conf.ssidSTA,230,75);
  tft.drawString(conf.passSTA,230,110);
  tft.drawString(wifimodetext[conf.wifimode],230,145);
  tft.drawString(conf.staticIP==1?itoa(conf.EEip[3],buff,10):"NO",230,180);
}

void displayCal()
{
  // botones setting
  for (byte i=0;i<5;i++) if (btCalact[i]==1)
    {
    btCAL[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btCaltext[i],2);
    btCAL[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("5 - ADJUST",50,0);
  tft.drawNumber(conf.calibration,230,40);
  tft.drawNumber(conf.usbCarrier,230,75);
  tft.drawNumber(conf.SI5351BX_ADDR,230,110);
}

void displayKEYERSet()    // botones KEYER setting
{
  for (byte i=0;i<4;i++) 
    {
    btKEYER[i].initButtonUL(&tft,100,35*i+30,60,30,2,TFT_WHITE,TFT_BLACK,btKEYERtext[i],2);
    btKEYER[i].drawButton();
    btKEYER[i+4].initButtonUL(&tft,230,35*i+30,60,30,2,TFT_WHITE,TFT_BLACK,btKEYERtext[i+4],2);
    btKEYER[i+4].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("6 - analog KEYER setting",50,0);
  tft.drawString("DOT",50,45);
    tft.drawNumber(conf.cwAdcDotFrom,170,45);
    tft.drawNumber(conf.cwAdcDotTo,300,45);
  tft.drawString("DASH",50,80);
    tft.drawNumber(conf.cwAdcDashFrom,170,80);
    tft.drawNumber(conf.cwAdcDashTo,300,80);
  tft.drawString("BOTH",50,115);
    tft.drawNumber(conf.cwAdcBothFrom,170,115);
    tft.drawNumber(conf.cwAdcBothTo,300,115);
  tft.drawString("ST",50,150);
    tft.drawNumber(conf.cwAdcSTFrom,170,150);
    tft.drawNumber(conf.cwAdcSTTo,300,150);
}

void displayMemMan()
{
  for (byte i=0;i<5;i++)  if (btMemManact[i]==1)
    {
    btMemMan[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btMemMantext[i],2);
    btMemMan[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("7 - Manage Memories",50,0);
}

void displayATU()
{
  // botones setting
  int backcolor=TFT_WHITE;
  for (byte i=0;i<5;i++) if (btATUact[i]==1)
    {
    if (i==0) backcolor=conf.ATUZM2enabled==1?TFT_YELLOW:TFT_WHITE;
    else backcolor=TFT_WHITE;
    btATU[i].initButtonUL(&tft,50,35*i+30,160,30,2,backcolor,TFT_BLACK,btATUtext[i],2);
    btATU[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("8 - ATU ZM-2",50,0);
  tft.drawString(conf.ATUZM2enabled==1?"YES":"NO",230,40);
}

void displayTPA2016()
{
  const char comptext[4][4]={"OFF","1:2","1:4","1:8"};
  // botones setting
  int backcolor=TFT_WHITE;
  for (byte i=0;i<5;i++) if (btTPAact[i]==1)
    {
    if (i==0) backcolor=conf.TPA2016enabled==1?TFT_YELLOW:TFT_WHITE;
    else if (i==1) backcolor=conf.TPA2016Compvalue>0?TFT_YELLOW:TFT_WHITE;
    else backcolor=TFT_WHITE;
    btTPA[i].initButtonUL(&tft,50,35*i+30,160,30,2,backcolor,TFT_BLACK,btTPAtext[i],2);
    btTPA[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("9 - TPA2016 Settings",50,0);
  tft.drawString(conf.TPA2016enabled==1?"YES":"NO",230,40);
  tft.drawString(comptext[conf.TPA2016Compvalue],230,75);
  tft.drawNumber(conf.TPA2016Compmaxgain,230,110);
  tft.drawNumber(conf.TPA2016Gain,230,145);
}

void displayCONNS()
{
  for (byte i=0;i<5;i++) if (btConact[i]==1)
    {
    btCON[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btContext[i],2);
    btCON[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("11 - Connections Settings",50,0);
  tft.drawString(connmodetext[conf.connMode],230,40);
  tft.drawString(serial2modetext[conf.serial2Mode],230,75);
}

void displayMenuTEMP()
{
  for (byte i=0;i<5;i++) if (btTempact[i]==1)
    {
    btTEMP[i].initButtonUL(&tft,50,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btTemptext[i],2);
    btTEMP[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("12 - Temp. Settings",50,0);
  tft.setTextSize(1);
  for (byte i=0;i<3;i++)
    for (byte j=0;j<8;j++)
      tft.drawNumber(conf.probecode[conf.nprobe[i]][j],230+(15*j),40+(35*i));
  tft.setTextSize(2);
}

void displayMenuPORTS()
{
  for (byte i=0;i<5;i++) if (btPortsact[i]==1)
    {
    int backcolor;
    if (i==0) backcolor=conf.tcpenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==1) backcolor=conf.udpenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==2) backcolor=conf.wsenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==3) backcolor=conf.webenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==4) backcolor=conf.webenable==0?TFT_WHITE:TFT_YELLOW;
    btPORTS[i].initButtonUL(&tft,50,35*i+30,160,30,2,backcolor,TFT_BLACK,btPortstext[i],2);
    btPORTS[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("13 - Ports Settings",50,0);
  tft.drawNumber(conf.tcpPort,230,40);
  tft.drawNumber(conf.udpPortSmeter,230,75);
  tft.drawNumber(conf.wsPort,230,110);
  tft.drawNumber(conf.webPort,230,145);
  tft.drawNumber(conf.udpPortFreq,230,180);
}

void displayMenuNav()
{
  for (byte j=0;j<5;j++)
    {
    for (byte i=0;i<4;i++) if (btMenuNavact[(j*4)+i]==1)
      {
      btMenuNav[(j*4)+i].initButtonUL(&tft,50+80*i,45*j,70,40,2,i+j==0?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btMenuNavtext[(j*4)+i],2);
      btMenuNav[(j*4)+i].drawButton();
      }
    }
}


void updateDisplay(byte alldata) {
  //s2("updateDisplay\n");
  boolean localdebug=false;
  clearTFT();
  if (tftpage==0)   // Main page
    {
    if (alldata==1) 
      {
      displayMain();    // 10 botones principales
      displayNav();     // botones navegación "< H > "
      displayFlot();    // botones "Ent","Scan-","Scan+","Lock",">VFO""
      displayStatus();  // time, status
      if (conf.framemode==0)      // analog meters
        {
        if (inTx==0) displaySmeter(1);
        displayIFS(0);
        displayATT(0);
        displayWiFiSt();
        displayRstBt();
        }
      else if (conf.framemode==1)   // digital meters
        {
        if (inTx==0) displaybarSmeter(40,186,0,90,69);
        displayIFS(1);
        displayATT(1);
        displayWiFiSt();
        displayRstBt();
        }
      else if (conf.framemode==2) { displaySpectrum(); }  // spectrum
      else if (conf.framemode==3) { displayFreqs(); }  // frequencies
      }
    displayFreq(0,1,1,1);   // frecuencia
    displayStatus();
    }
  else if (tftpage==1) { displayUSERSet(); displayNav(); }   // Setup User
  else if (tftpage==2) { displaySetRad();  displayNav(); }   // Setup radio
  else if (tftpage==3) { displayCWSet(); displayNav();   }   // CW setting
  else if (tftpage==4) { displayNet(); displayNav();     }   // WiFi
  else if (tftpage==5) { displayCal(); displayNav();     }   // Calibration
  else if (tftpage==6) { displayKEYERSet(); displayNav(); }   // analog KEYER setting
  else if (tftpage==7) { displayMemMan(); displayNav();  }   // Mem manage
  else if (tftpage==8) { displayATU(); displayNav();     }   // ATU
  else if (tftpage==9) { displayTPA2016(); displayNav(); }   // Audio amplifier TPA2016
  else if (tftpage==10) { displaysetupSmeter(); displayNav(); }   // S-Meter
  else if (tftpage==11) { displayCONNS(); displayNav();  }   // Connections
  else if (tftpage==12) { displayMenuTEMP(); displayNav();  }   // Temperatures
  else if (tftpage==13) { displayMenuPORTS(); displayNav();  }   // Ports
  else if (tftpage==21) {  }   // select AP
  else if (tftpage==22)    // Mem display and select
    {
    Serial2.println("tftpage=22");
    }
  else if (tftpage==23)    // test ports
    {
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("23 - Test Ports",0,0);
    
    tft.drawString("ADSA",0,40);
    
    tft.drawString("ADSB",0,80);
    }
  else if (tftpage==24) {  
    displayMenuNav();  
    }
}

void setLOCK(byte value)
{
  if (value==2) 
    {
    strcpy(btFlottext[3],"Rem.");
    value=1;
    }
  else
    strcpy(btFlottext[3],"Lock");
  keylock=value; 
  sendwsData(tcpkeylock); 
  displayFlot();
}

void displayYN(byte en0, byte en1, byte en2)
{
  // botones Yes/Cancel
  btYNact[0]=en0; btYNact[1]=en1; btYNact[2]=en2;
  for (byte i=0;i<3;i++)
    if (btYNact[i]==1)
      {
      btYN[i].initButtonUL(&tft,100*i+170,260,90,40,2,TFT_WHITE,TFT_BLACK,btYNtext[i],2);
      btYN[i].drawButton();
      }
}

void displayKey(byte mode) // 0: all keys UP, 1: all keys LW, 2:alfa only UP, 3:alfa only LW, 4:numeric only 
{ 
  if ((mode<=1) || (mode==4))
    for (byte i=0;i<10;i++)
      {
      btKey[i].initButtonUL(&tft,41*(i%10),45+(41*int(i/10)),40,40,2,TFT_WHITE,TFT_BLACK,btKeytextU[i],2);
      btKey[i].drawButton();
      }
  if ((mode==0) || (mode==2))
    for (byte i=10;i<50;i++)
      {
      btKey[i].initButtonUL(&tft,41*(i%10),45+(41*int(i/10)),40,40,2,TFT_WHITE,TFT_BLACK,btKeytextU[i],2);
      btKey[i].drawButton();
      }
  if ((mode==1) || (mode==2))
    for (byte i=10;i<50;i++)
      {
      btKey[i].initButtonUL(&tft,41*(i%10),45+(41*int(i/10)),40,40,2,TFT_WHITE,TFT_BLACK,btKeytextL[i],2);
      btKey[i].drawButton();
      }
  displayYN(1,1,1);
}

int checkYN()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
    {
    if (tft.getRotation()==1) { y=tft.height()-y;}
    else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
    for (byte i=0;i<3;i++)
      if (btYN[i].contains(x,y)) { delay(100); return i;  } 
    }
  return -1;
}


long getNumberTFT(long valini, byte len, char *tunits)
{
  clearTFT();
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(tunits,200,0);
  displayKey(4);
  strcpy(auxtft,itoa(valini,buff,10));
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(auxtft,0,0);
  uint16_t x, y;
  while (true) 
    {
    if (tft.getTouch(&x, &y))
      {
      if (tft.getRotation()==1) { y=tft.height()-y;}
      else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
      for (byte i=0; i<10; i++)    // check KeyN buttons
        {
        if (btKey[i].contains(x,y)) 
          {
          delay(100);
          if (strlen(auxtft)<len)
            {
            strcat(auxtft, itoa(i,buff,10));
            tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.drawString(auxtft,0,0);
            }
          }
        }
      int result=checkYN();
      if (result==0) { return atol(auxtft);   }   // OK
      else if (result==1) { return -1; }     // Cancel
      else if (result==2)                    // Backspace
        { 
        if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0; 
        tft.drawString("                    ",0,0);  
        tft.drawString(auxtft,0,0); }     
      }
    }
}

int getCharTFT(char *valini, byte tam)
{
  byte modekeyer=0;
  clearTFT();
  displayKey(modekeyer);
  uint16_t x, y;
  strcpy(auxtft,valini);
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(auxtft,0,0);
  while (true) 
    {
    if (tft.getTouch(&x, &y))
      {
      if (tft.getRotation()==1) { y=tft.height()-y;}
      else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
      for (byte i=0; i<50; i++)    // check KeyA buttons
        {
        if (btKey[i].contains(x,y)) 
          {
          if (i==49)    // cambio Lower/Upper 
            {
            if (modekeyer==0) modekeyer=1; else modekeyer=0;
            displayKey(modekeyer);
            }
          else
            {
            if (strlen(auxtft)<tam)
              {
              if (modekeyer==0)
                strcat(auxtft, btKeytextU[i]);
              else
                strcat(auxtft, btKeytextL[i]);
              tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
              tft.drawString(auxtft,0,0);
              }
            }
          delay(100);
          }
        }
      int result=checkYN();
      if (result==0) { return strlen(auxtft);   }        // OK
      else if (result==1) { return -1; }  // Cancel
      else if (result==2)                 // Backspace
        { 
        if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0; 
        tft.drawString("                    ",0,0);  
        tft.drawString(auxtft,0,0); }     
      }
    }
}

void tftErrormsg(char *texto1, char *texto2, char *texto3)
{
  clearTFT();
  tft.setTextSize(4); tft.setTextColor(TFT_RED);
  tft.drawString(texto1,30,90);
  tft.setTextSize(2); tft.setTextColor(TFT_YELLOW);
  tft.drawString(texto2,0,170);
  tft.setTextSize(2); tft.setTextColor(TFT_YELLOW);
  tft.drawString(texto3,0,200);
  delay(2000);
}

void startScan(byte dir)
{
  if ((conf.actualBand==99) && (conf.scanallf==0))
    {
    tftErrormsg("OUT OF BAND","Modify parameter","Scan mode");
    tftpage=2;
    updateDisplay(1);
    return;
    }
  setSCAN(scanF==dir?0:dir);
  lastframemode=conf.framemode;
  conf.framemode=0;
}

void checkFlotButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5; i++)    // check float buttons
    { 
    if (btFlot[i].contains(x,y)) 
      {
      if (i==3) { setLOCK(keylock==0?1:0); delay(50); }    // lock key
      if (keylock==1) return; 
      if (i==0)     // ent frequency
        {
        setSCAN(0);
        long auxL=getNumberTFT(conf.frequency/1000,5,"Khz");  // retorna con OK o Cancel
        if (auxL!=-1) { setFrequency(auxL*1000); }
        updateDisplay(1);
        }
      else if (i==1) { startScan(1); }     // scan down
      else if (i==2) { startScan(2); }     // scan up
      else if (i==4)        // VFO to Mem 
        { 
        if (conf.memMode==0)   // VFO mode
          { 
          conf.lastmempos=mempos; 
          saveVFOtoMem();
          updateDisplay(1); 
          }
        else               // Mem mode
          { 
          mempos=conf.lastmempos; 
          setMEMtoVFO(mempos); 
          }          
        saveconf();
        }     
      }
    }
}

void checkFreqButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<9; i++)    // check step buttons
    { 
    if (btFreq[i].contains(x,y)) 
      { 
      setSTEP(i);
      if (i<=5) conf.frequency=conf.frequency/1000 *1000;  
      else if (i==6) conf.frequency=conf.frequency/100 *100;  
      else if (i==7) conf.frequency=conf.frequency/10 *10;  
      displayFreq(0,1,1,1); 
      } 
    }
}

void checkVFOButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<2;i++)    // check buttons VFO
    {
    if (btVFO[i].contains(x,y)) 
      {
      setVFO(conf.vfoActive==VFO_A?VFO_B:VFO_A);
      updateDisplay(1);
      return;
      }
    }
}


 char byteToChar(byte srcByte) { 
  return srcByte < 10?0x30 + srcByte:'A' + srcByte - 10; 
  }

//returns true if the button is pressed
//int btnDown(void) { return digitalRead(FBUTTON) == HIGH?0:1; }
int getBtnStatus() { 
  return digitalRead(FBUTTON) == HIGH?0:1; 
  }

int enc_prev_state = 3;

byte enc_state (void) {
//  return (digitalRead(ENC_A)==1?1:0) + (digitalRead(ENC_B)==1?2:0);
    return (analogRead(ENC_A) > 500 ? 1 : 0) + (analogRead(ENC_B) > 500 ? 2: 0);
}

int enc_read(void) {
  int result = 0; 
  byte newState;
  int enc_speed = 0;
  unsigned long start_at = millis();
  while (millis() - start_at < 10) { // check if the previous state was stable
//  while (millis() - start_at < 50) { // check if the previous state was stable
    newState = enc_state(); // Get current state  
    if (newState != enc_prev_state)
      delay (1);
    if (enc_state() != newState || newState == enc_prev_state)
      continue; 
    //these transitions point to the encoder being rotated anti-clockwise
    if ((enc_prev_state == 0 && newState == 2) || 
      (enc_prev_state == 2 && newState == 3) || 
      (enc_prev_state == 3 && newState == 1) || 
      (enc_prev_state == 1 && newState == 0)){
        result--;
      }
    //these transitions point o the enccoder being rotated clockwise
    if ((enc_prev_state == 0 && newState == 1) || 
      (enc_prev_state == 1 && newState == 3) || 
      (enc_prev_state == 3 && newState == 2) || 
      (enc_prev_state == 2 && newState == 0)){
        result++;
      }
    enc_prev_state = newState; // Record state for next pulse interpretation
    enc_speed++;
    delay(1);
  }
  return(result);
}

uint32_t getValByKnob(int valueType, int targetValue, int minValue, int maxValue, int incStep, const char* Title, int Sensitivity)
{
  int knob;
  int moveDetectStep=0;
  int antvalue=targetValue;
  if (targetValue<minValue) targetValue=minValue;
  if (targetValue>maxValue) targetValue=maxValue;
  TFT_eSPI_Button btaux;          // buttons aux
  clearTFT();
  tft.setTextSize(3); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(Title,55,20);
  displayYN(1,1,0);
  tft.setTextSize(3);  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if ((valueType!=0) && (valueType!=5) && (valueType!=19))
    {
    btaux.initButtonUL(&tft,265,5,100,40,2,TFT_BLACK,TFT_WHITE,itoa(targetValue,buff,10),3);
    btaux.drawButton();
    }
  if (valueType==2)      // IFS
    { 
    displayIFS(1); 
    if (conf.framemode==0) 
      {
      displaySmeter(1);
      }
    else if (conf.framemode==1) displaybarSmeter(40,166,0,90,87); 
    else if (conf.framemode==2) displaySpectrum();
    else if (conf.framemode==3) displayFreqs();
    }
  else if ((valueType==3) || (valueType==4))       // ajustar C1/C2
    {
    displaygauge(3,targetValue,1,minValue,maxValue,180,12);
    displayneedle(targetValue,160,190,140,minValue,maxValue,180);
    }
  else if (valueType==5)        // squelch
    {
    displaygauge(5,targetValue,1,minValue,maxValue,180,15);
    displayneedle(targetValue,160,190,140,minValue,maxValue,180);
    }
  else if (valueType==6)      // ATT
    { 
    displayATT(1); 
    if (conf.framemode==0) 
      {
      displaySmeter(1);
      }
    else if (conf.framemode==1) 
      {   
      displaybarSmeter(40,186,0,90,69);
      }
    else if (conf.framemode==2) displaySpectrum();
    else if (conf.framemode==3) displayFreqs();
    }
  else if ((valueType>=11) && (valueType<=18))        // analog KEYER setting
    {
    tft.setTextSize(2); tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    if ((valueType==11)||(valueType==15)) tft.drawString("Press DOT to check value",0,70); 
    else if ((valueType<=12)||(valueType<=16)) tft.drawString("Press DASH to check value",0,70); 
    else if ((valueType<=13)||(valueType<=17)) tft.drawString("Press BOTH to check value",0,70); 
    else if ((valueType<=14)||(valueType<=18)) tft.drawString("Press ST to check value",0,70); 
    tft.drawString("Value keyer:",0,100); 
    tft.drawString("Suggested value:",0,130); 
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
  else if (valueType==19) 
    {
    btaux.initButtonUL(&tft,215,5,100,40,2,TFT_BLACK,TFT_WHITE,timezonetext[targetValue],3);
    btaux.drawButton();
    }
  while(!btnDown())
    {
    knob = enc_read();
    if (knob != 0)
      {
      moveDetectStep += (knob > 0 ? 1 : -1);
      if (moveDetectStep < (Sensitivity * -1)) {
        if (targetValue > minValue) targetValue -= incStep;
        moveDetectStep = 0;
        }
      else if (moveDetectStep > Sensitivity) {
        if (targetValue < maxValue) targetValue += incStep;
        moveDetectStep = 0;
        }
      if ((valueType!=0) && (valueType!=5))
        {
        btaux.initButtonUL(&tft,220,5,100,40,2,TFT_BLACK,TFT_WHITE,itoa(targetValue,buff,10),3);
        btaux.drawButton();
        }
      if (valueType==2)   // IFS
        {
        conf.ifShiftValue=targetValue; isIFShift=conf.ifShiftValue==0?0:1;
        displayIFS(1);
        setFrequency(conf.frequency);
        SetCarrierFreq();
        if (conf.framemode==3) displayFreqs();
        }
      else if (valueType==3)        // ajustar C1
        {
        cap1.write(targetValue);             // tell servo to go to position in variable '0'
        displayneedle(targetValue,160,190,140,minValue,maxValue,180);
        }
      else if (valueType==4)        // ajustar C2
        {
        cap2.write(targetValue);             // tell servo to go to position in variable '0'
        displayneedle(targetValue,160,190,140,minValue,maxValue,180);
        }
      else if (valueType==5)        // squelch
        {
        if ((targetValue>=minValue) && (targetValue<=maxValue))
          displayneedle(targetValue,160,190,140,minValue,maxValue,180);
        }
      if (valueType==6)   // ATT
        {
        conf.attLevel=targetValue;
        displayATT(1);
        setFrequency(conf.frequency);
        SetCarrierFreq();
        if (conf.framemode==3) displayFreqs();
        }
      else if (valueType==7)      // Max gain TPA2016
        { 
        setTPAMaxGain(targetValue);
        }
      else if (valueType==8)      // Gain TPA2016
        { 
        setTPAGain(targetValue);
        }
      }  
    if (valueType==2)   // IFS
      {
      if (conf.framemode==0) 
        {
        displaySmeter(1);
        }
      else if (conf.framemode==1) 
        {
        displaySmeter(1);
        }
      else if (conf.framemode==3) 
        {   
        displaybarSmeter(40,186,0,90,69);
        }
      }
    else if (valueType==6)   // ATT
      {
      if (conf.framemode==0) 
        {
        displaySmeter(1);
        }
      else if (conf.framemode==1) 
        {
        displaySmeter(1);
        }
      else if (conf.framemode==3) 
        {
        displaybarSmeter(40,166,0,90,87); 
        }
      }
    else if ((valueType>=11) && (valueType<=18))        // analog KEYER setting
      {
      tft.setTextSize(2); tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.drawString("     ",160,100); 
      int valaux=(analogRead(ANALOG_KEYER)/10)*10;
      tft.drawNumber(valaux,160,100); 
      tft.setTextSize(3);
      tft.drawString("     ",200,130); 
      if (valueType<=14) tft.drawNumber(valaux-200,200,130);
      else tft.drawNumber(valaux+200,200,130);
      tft.setTextSize(2); tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }
    else if (valueType==19) 
      {
      btaux.initButtonUL(&tft,215,5,100,40,2,TFT_BLACK,TFT_WHITE,timezonetext[targetValue],3);
      btaux.drawButton();
      }
    int auxres=checkYN();
    if (auxres==0) 
      return targetValue;
    else if (auxres==1) 
      return antvalue;
    Check_Cat(0);  //To prevent disconnections
    }  
  s2("targetValue:"); s2(targetValue);s2(crlf);
  return targetValue;
}

void checkSmeterButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<2;i++)    // check buttons function
    {
    if (btSmeter[i].contains(x,y)) 
      {
      if (i==0)
        {
        conf.scanmode=conf.scanmode<2?conf.scanmode+1:0;
        displaySmeter(0);
        }
      else if (i==1)
        {
        conf.squelchval=getValByKnob(5,conf.squelchval,0,90,6,"Squelch",1);
        updateDisplay(1);
        }
      saveconf();
      }
    }
}

void setUSB_ubitx(byte value) 
  {
  if (value == conf.isUSB)  return;
  conf.isUSB = value; 
  if (conf.vfoActive==VFO_A) conf.isUSBA=conf.isUSB; else conf.isUSBB=conf.isUSB;
  sendwsData(tcp_is_USB); 
  btMainact[4]=conf.isUSB==1?1:0; 
  strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
  displayMain(); 
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1); 
}

void setUSB_faradio(byte value) 
  {
  if (value == conf.isUSB)  return;
  conf.isUSB = value; 
  if (conf.vfoActive==VFO_A) conf.isUSBA=conf.isUSB; else conf.isUSBB=conf.isUSB;
  sendwsData(tcp_is_USB); 
  btMainact[4]=conf.isUSB==1?1:0; 
  strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
  displayMain(); 
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1); 
}

void setUSB(byte value) {
#ifdef UBITX_RADIO
  setUSB_ubitx(value);
#endif
#ifdef FA_RADIO
  setUSB_faradio(value);
#endif
}

void saveFREQ()     // save freq values
{
  vfoActiveAct=conf.vfoActive;      // VFO 
  isUSBAct=conf.isUSB;       // mode
  cwModeAct=conf.cwMode;      // CW mode
  ritOnAct=conf.ritOn;       // RIT
  splitOnAct=conf.splitOn;     // SPL
  frequencyAct=conf.frequency;  // frequency
  ritTxFrequencyAct=conf.ritTxFrequency;  // frequency TX Rit
  splitOnAct=conf.splitOn;      // isUSB TX split mode
  cwModeAct=conf.cwMode;     // CW mode TX split
  ftxsplAct=conf.frequencyB;           // frequency TX Split
}

void checkMainButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<10; i++)    // check buttons function
    {
    if (btMain[i].contains(x,y)) 
      {
      if (i==0)   // ATU
        { 
        if (inTx==1) 
          { 
          txTFT=false; 
          //stopTx(); 
          }  // RX
        else 
          { 
           txTFT=true;
           startTx(TX_CW,1); 
           cwKeydown();
           acopla();
           delay_background(conf.ATUdelay*1000,9,1);
           cwKeyUp();
           stopTx();
           txTFT=false;
           }  // TX
        strcpy(btMaintext[0],inTx==0?"ATU":"TX");
        updateDisplay(1);
        }  
      else if (i==1)    // V/M
        { 
        conf.memMode=1;
        firstmem=1;
        tftpage=22;
        saveFREQ();
        clearTFT();
        }
      else if (i==2)    //Prior Band
        { 
        strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
        setRIT(0); 
        setNextHamBandFreq(conf.frequency,-1);
        displayFreq(1,1,1,1);
        } 
      else if (i==3)    //Next Band
        { 
        strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
        setRIT(0);
        setNextHamBandFreq(conf.frequency, 1); 
        displayFreq(1,1,1,1);
        }  
      else if (i==4)   // LSB-USB 
        { 
        setUSB(conf.isUSB==0?1:0);
        }
      else if (i==5)   // CW 
        {
        setCW(conf.cwMode>0?0:1);
        setFrequency(conf.frequency);
        }
      else if (i==6) { 
        setRIT(conf.ritOn==0?1:0); 
        }   // RIT
      else if (i==7) { 
        setSPLIT(conf.splitOn==0?1:0); 
        }   // SPL
      else if (i==8)    // IFS
        {
        conf.ifShiftValue = getValByKnob(2, conf.ifShiftValue, -2000, 2000, 100, "IF Shift Hz", 2);
        isIFShift=conf.ifShiftValue!=0?1:0; 
        btMainact[i]=conf.ifShiftValue!=0?1:0;
        setIFS(conf.ifShiftValue,1);
        }
        
      else if (i==9)    // ATT
        {
        conf.attLevel = getValByKnob(6, conf.attLevel, 0, 250, 10, "ATT Level", 1);
        btMainact[i]=conf.attLevel==0?0:1;
        setATT(conf.attLevel,1);
        }
        
      if (i>0) saveconf();
      }
    } // for
}


//////////////////  Touch ///////////////////////
void checkSetButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
  {
   if (btSet[i].contains(x,y)) 
     {
     if (i==0) 
       { conf.lang=conf.lang==0?1:0; }           // language
     else if (i==1)                              // CallSign
       { 
       int auxI=getCharTFT(conf.CallSign,10); 
       if (auxI !=-1) { strcpy(conf.CallSign,auxtft);  }
       }   
     else if (i==2)           // Latitude
       {  
       long auxL=getNumberTFT(conf.latitud,10,"");  
       if (auxL!=-1) { conf.latitud=auxL; }
       }                         
     else if (i==3)               // Longitude
       {  
       long auxL=getNumberTFT(conf.longitud,10,"");  
       if (auxL!=-1) { conf.longitud=auxL; }
       }                         // Latitude
     else if (i==4) 
       {  
       conf.timezone=getValByKnob(19, conf.timezone, 0, 38, 1, "Time Zone", 1);
       }                         // 
     saveconf(); 
     updateDisplay(1);
     }
  }
}

void checkSetRadButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
  {
   if (btSetRad[i].contains(x,y)) 
     {
     if (i==0) { conf.TXall=conf.TXall==0?1:0; }    // TX range
     else if (i==1) { conf.autoMode=conf.autoMode==0?1:0; }    // SSB auto                         
     else if (i==2) { conf.scanallf=conf.scanallf==0?1:0; }   // scan mode
     else if (i==3) { if (conf.scanmode<2) conf.scanmode++; else conf.scanmode=0; }    // scan mode                      
     else if (i==4) { conf.scandelay=getValByKnob(1,conf.scandelay,1,60,1,"Resume del. (s)",2); }  // resume delay                
     saveconf(); 
     updateDisplay(1);
     }
  }
}

void checkNetButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
    if (btNet[i].contains(x,y)) 
      {
      if (i==0)      { conf.autoWiFi=conf.autoWiFi==0?1:0;  }   // AutoConnect
      //else if (i==1) { selectapTFT(); }   // SSID
      else if (i==2) { if (getCharTFT(conf.passSTA,10) !=-1) strcpy(conf.passSTA ,auxtft); }  // Password
      else if (i==3) { if (conf.wifimode<3) conf.wifimode++; else conf.wifimode=0; }        // WiFi Mode
      else if (i==4) { conf.staticIP=conf.staticIP==0?1:0; }                                //  Static IP
      saveconf();  
      if (i!=1) updateDisplay(1);
      }
    }
}

void checkCalButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btCAL[i].contains(x,y)) 
       {
       if (i==0) { 
        setupFreq(); 
        }   // CAL
       else if (i==1) { 
        setupBFO();  
        }   // BFO
       else if (i==2)   // dirección I2C del módulo si5351
         {  
          long auxL=getNumberTFT(conf.SI5351BX_ADDR,3,"");  // retorna con OK o Cancel
          if (auxL!=-1) { conf.SI5351BX_ADDR=auxL; }
         }      // 
       else if (i==3) {  }      // 
       else if (i==4) {   // reset fábrica
        initConf(); 
        saveconf(); 
        ESP.restart(); 
        }      // 
       updateDisplay(1);
       }
    }
}

void checkCONButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btCON[i].contains(x,y)) 
       {
       if (i==0)      { }   // Conn Mode
       if (i==0) { 
        setupconnmode(); 
        }   // Conn Mode
       else if (i==1) { 
        setupserial2mode();  
        }   // Serial2 mode
       else if (i==2) {  }      // 
       else if (i==3) {  }      // 
       else if (i==4) {  }      // 
       updateDisplay(1);
       }
    }
}

void checkNavButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)     // navigation buttons
    {
    if (btNav[i].contains(x,y)) 
      {
      delay(100);
      if (i==0)         // Home, "H"
        { 
        if (tftpage==0) { /*setFrame();*/ }
        else tftpage=0;
        }               // main page
      else if (i==1)       // flecha izquierda "<"  
        { 
        tftpage=tftpage>0?tftpage-1:MAX_PAGES; // priot page
        }     
      else if (i==2)        // flecha derecha ">"
        { 
        tftpage=tftpage<MAX_PAGES?tftpage+1:0;  // nest page
        }
      else if (i==3) { tftpage=24; }
      else if (i==4) {  }
      updateDisplay(1);
      }
   }
}

void checkStaButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)   // buttons status line
    {
     if (btSta[i].contains(x,y)) 
       {
       if (i==0) { setWiFi(); }            // WiFi
       else if (i==1) {     }
       else if (i==2) {     }
       else if (i==3) {     }
       else if (i==4) { ESP.restart(); }    // reset
       updateDisplay(1);
       }
    }
}

void checkCWButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btCW[i].contains(x,y)) 
       {
       if (i==0) {conf.cwKeyType=conf.cwKeyType==2?0:conf.cwKeyType+1; }   // Key type
       else if (i==1) 
         { 
         int wpm=1200/conf.cwSpeed;
         wpm=getValByKnob(1, wpm, 3, 50, 1, "WPM", 3);
         conf.cwSpeed = 1200/wpm;
         }   
       else if (i==2) { conf.sideTone=getValByKnob(1, conf.sideTone, 100, 2000, 10, "Tone", 2); }    
       else if (i==3) { conf.cwDelayTime=getValByKnob(1, conf.cwDelayTime, 0, 120, 10, "Delay", 2); }      
       else if (i==4) { conf.delayBeforeCWStartTime=getValByKnob(1, conf.delayBeforeCWStartTime, 0, 250, 10, "Start time", 2); }
       saveconf(); 
       updateDisplay(1);
       }
    }
}

void checkKEYERButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<8;i++)
    {
    if (btKEYER[i].contains(x,y)) 
      {
      if (i==0) {conf.cwAdcDotFrom=getValByKnob(11, conf.cwAdcDotFrom, 100, 3000, 10, "DOT from", 2);}   
      else if (i==4) {conf.cwAdcDotTo=getValByKnob(15, conf.cwAdcDotTo, 100, 3000, 10, "DOT to", 2);}   
      else if (i==1) {conf.cwAdcDashFrom=getValByKnob(12, conf.cwAdcDashFrom, 100, 3000, 10, "DASH from", 2);}   
      else if (i==5) {conf.cwAdcDashTo=getValByKnob(16, conf.cwAdcDashTo, 100, 3000, 10, "DASH to", 2);}   
      else if (i==2) {conf.cwAdcBothFrom=getValByKnob(13, conf.cwAdcBothFrom, 100, 3000, 10, "BOTH from", 2);}  
      else if (i==6) {conf.cwAdcBothTo=getValByKnob(17, conf.cwAdcBothTo, 100, 3000, 10, "BOTH to", 2);}  
      else if (i==3) {conf.cwAdcSTFrom=getValByKnob(14, conf.cwAdcSTFrom, 100, 3000, 10, "ST from", 2);}  
      else if (i==7) {conf.cwAdcSTTo=getValByKnob(18, conf.cwAdcSTTo, 100, 3000, 10, "ST to", 2);}  
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkMemManButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btMemMan[i].contains(x,y)) 
       {
       if (i==0)      {  }   // 
       else if (i==1) {  }   // 
       else if (i==2) {  }      // 
       else if (i==3) {  }      // 
       else if (i==4) { 
        deleteMemo(); 
        }       //  Clear all memories
       updateDisplay(1);
       }
    }
}

void checkATUButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
  {
   if (btATU[i].contains(x,y)) 
     {
     if (i==0) { conf.ATUZM2enabled=conf.ATUZM2enabled==0?1:0; }         // 
     else if (i==1) { setupATU(1); }    // 
     else if (i==2) { setupATU(2); }    //                          
     else if (i==3) {  }    //                     
     else if (i==4) {  }    //          
     saveconf(); 
     updateDisplay(1);
     }
  }
}

void checkTPAButtons(uint16_t x, uint16_t y)
{
  tft.drawString(conf.TPA2016enabled==1?"YES":"NO",180,40);
  tft.drawString(conf.TPA2016Compvalue==0?"AGC OFF":itoa(conf.TPA2016Compvalue,buff,10),180,75);
  tft.drawNumber(conf.TPA2016Compmaxgain,180,110);
  for (byte i=0; i<5;i++)
    {
    if (btTPA[i].contains(x,y)) 
      {
      if (i==0)     // ON/OFF
        { 
        conf.TPA2016enabled=conf.TPA2016enabled==0?1:0; 
        //setTPAenable(conf.TPA2016enabled);
        }    
      if (i==1)     // compression value
        { 
        conf.TPA2016Compvalue=conf.TPA2016Compvalue<3?conf.TPA2016Compvalue+1:0; // 0-3 
        //setTPAComp(); 
        } 
      if (i==2)     // maxgain value
        { 
        conf.TPA2016Compmaxgain=getValByKnob(7, conf.TPA2016Compmaxgain, 0, 12, 1, "Max Gain", 2);// 0-12 
        //setTPAMaxGain(conf.TPA2016Compmaxgain);
        } 
      if (i==3)     // gain value
        { 
        conf.TPA2016Gain=getValByKnob(8,conf.TPA2016Gain,-28,30,1,"Gain", 2);// -28 a +30 
        s2("conf.TPA2016Gain:"); s2(conf.TPA2016Gain); s2(crlf);
        //setTPAGain(conf.TPA2016Gain);
        } 
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkSMEButtons(uint16_t x, uint16_t y)
{
  tft.drawString(conf.TPA2016enabled==1?"YES":"NO",180,40);
  tft.drawString(conf.TPA2016Compvalue==0?"AGC OFF":itoa(conf.TPA2016Compvalue,buff,10),180,75);
  tft.drawNumber(conf.TPA2016Compmaxgain,180,110);
  for (byte i=0; i<5;i++)
    {
    if (btSME[i].contains(x,y)) 
      {
      if (i==0) { conf.sMeterLevels[0]=getValByKnob(1, conf.sMeterLevels[0], 0, 20000, 100, "MIN.", 3); }    
      if (i==1) { conf.sMeterLevels[15]=getValByKnob(1, conf.sMeterLevels[15], 0, 20000, 100, "MAX.", 3); }    
      if (i==2) {  } 
      if (i==3) {  }    // 
      if (i==4) {  }    // 
      calcSmeterScale();
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void setTempProbe(byte n)
{
  conf.nprobe[n]=conf.nprobe[n]<nTemp-1?conf.nprobe[n]+1:0;
}


void checkTEMPButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btTEMP[i].contains(x,y)) 
       {
       if (i<=2) {
        setTempProbe(i); 
        }   // 
       else if (i==3) {  }      // 
       else if (i==4) {  }      // 
       updateDisplay(1);
       }
    }
}

void checkPORTSButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
    if (btPORTS[i].contains(x,y)) 
      {
      if (i==0)      { conf.tcpenable=conf.tcpenable==0?1:0; }   // 
      else if (i==1) { conf.udpenable=conf.udpenable==0?1:0; }      // 
      else if (i==2) { conf.wsenable=conf.wsenable==0?1:0; }      // 
      else if (i==3) { conf.webenable=conf.webenable==0?1:0; }      // 
      else if (i==4) {  }      // 
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void drawST(boolean flechas, boolean escok)     // barra navegación
{
  if (flechas)
    {
    btSel[0].initButtonUL(&tft,0,200,75,40,2,TFT_WHITE,TFT_BLACK,flecha[0],2);  btSel[0].drawButton();
    btSel[3].initButtonUL(&tft,245,200,75,40,2,TFT_WHITE,TFT_BLACK,flecha[3],2);  btSel[3].drawButton();
    }
  if (escok)
    {
    btSel[1].initButtonUL(&tft,80,200,75,40,2,TFT_WHITE,TFT_BLACK,"ESC",2);  btSel[1].drawButton();
    btSel[2].initButtonUL(&tft,165,200,75,40,2,TFT_WHITE,TFT_BLACK,"OK",2);  btSel[2].drawButton();
    }
}


void drawAP(byte nact)
{
  clearTFT();
  tft.setTextDatum(TL_DATUM);      // Set new datum
  for (int i=0; i<nAP; i++)
    if (WiFi.RSSI(i)>-90)
      {
      int y=i*20;
      WiFi.SSID(i).toCharArray(auxchar, 20);
      if (i==tftapactual) { tft.setTextColor(TFT_BLACK, TFT_WHITE);    }
      tft.drawNumber(i,0,y);
      tft.drawString(auxchar,20,y); 
      tft.drawNumber(WiFi.RSSI(i),240,y);
      tft.drawString("dbm",280,y); 
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }
  drawST(true,true);
}

void checkSelButtons(uint16_t x, uint16_t y)
{
  for (byte i=0;i<4;i++)  // botones Cancel,OK,N,Backspace
    { 
    if (btSel[i].contains(x,y))  
      { 
      if (i==0) { tftapactual--; if (tftapactual<0) tftapactual=nAP-1; drawAP(tftapactual);}
      else if (i==1) { tftpage=4; updateDisplay(1); }
      else if (i==2) { WiFi.SSID(tftapactual).toCharArray(conf.ssidSTA, 20); saveconf(); tftpage=4; updateDisplay(1); }
      else if (i==3) { tftapactual++; if (tftapactual>nAP-1) tftapactual=0; drawAP(tftapactual);}
      }  
    }
}

void checkMenuButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<20;i++)     // menu pages buttons
    {
    if (btMenuNav[i].contains(x,y)) 
      {
      delay(50);
      if ((i>=0) && (i<=19)) if (btMenuNavact[i]==1) { tftpage=i; }       
      updateDisplay(1);
      }
   }
}


void handletfttouch()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
    { 
    if (tft.getRotation()==1) { y=tft.height()-y;}
    else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
    if (tftpage==0)   // main page
      {
      if (scanF>0) { scanF=0; updateDisplay(1); return; }
      checkFlotButtons(x,y);
      if (keylock==1) return;
      checkFreqButtons(x,y);
      checkMainButtons(x,y);
      checkVFOButtons(x,y);
      checkSmeterButtons(x,y); 
      checkNavButtons(x,y); 
      checkStaButtons(x,y);
      }
    else if (tftpage==1) { checkSetButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // Setup page
    else if (tftpage==2) { checkSetRadButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); } // radio setting page
    else if (tftpage==3) { checkCWButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }     // CW setting page
    else if (tftpage==4) { checkNetButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // WiFi page
    else if (tftpage==5) { checkCalButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // Calibration page
    else if (tftpage==6) { checkKEYERButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // KEYER setting page
    else if (tftpage==7) { checkMemManButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); } // memories manager
    else if (tftpage==8) { checkATUButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // ATU page
    else if (tftpage==9) { checkTPAButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // TPA2016 page
    else if (tftpage==10) { checkSMEButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // S_METER page
    else if (tftpage==11) { checkCONButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // CONNS page
    else if (tftpage==12) { checkTEMPButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // TEMP page
    else if (tftpage==13) { checkPORTSButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // PORTS page
    else if (tftpage==21) { checkSelButtons(x,y); }    // Select AP
    else if (tftpage==22) 
      { 
      tftpage=0; 
      if (checkYN()==1)  // ESC
        {
        //restFREQ();
        }
      updateDisplay(1); 
      }    // 
    else if (tftpage==24) { checkMenuButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // 
    delay(20);
    } 
}

void clearMsg(int x,int y,int w, int h)
{
  tft.fillRect(x, y, w, h, TFT_BLACK);
}

void displayMsg(char* msg1,char* msg2,char* msg3,int x,int y,int w, int h)
{
  // Clear the screen areas
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(1);
  tft.fillRect(x, y, w, h, TFT_BLACK);
  tft.drawRect(x, y, w, h, TFT_WHITE);
  uint8_t td = tft.getTextDatum(); // Get current datum
  tft.setTextDatum(TL_DATUM);      // Set new datum
  tft.drawString(msg1, x+5, y+5, 2); // Message in font 2
  tft.drawString(msg2, x+5, y+25, 2); // Message in font 2
  tft.drawString(msg3, x+5, y+45, 2); // Message in font 2
  tft.setTextDatum(td); // Restore old datum
  displayNav();     // botones navegación "< H > "
}

void displaySWR(byte tam)
{
  if (tam==0)
    displaygauge(2,SWR,1,1,10,90,9);
  else
    displaygauge(2,SWR,1,1,10,180,9);
}

void displayFrame()
{
  if (conf.framemode==0) displaySmeter(1);
  else if (conf.framemode==1) 
      {   
      displaybarSmeter(40,186,0,90,69);
      }
  else if (conf.framemode==2) displaySpectrum();
  else if (conf.framemode==3) displayFreqs();
}

void displayMemList()   // tftpage=22
{
  tft.setTextSize(1); 
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("  n      Name             VFO L/U CW Rt Spl", 0, 74);
  tft.setTextSize(2); 
  for (int i=0; i<6; i++)
    {
    if (memlin==i)
      {
      btMemN[i].initButtonUL(&tft,0,82+20*i,30,20,2,TFT_WHITE,TFT_BLACK,itoa(mempos+i-memlin,buff,10),2);
      btMemN[i].drawButton();
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      }
    else
      {
      btMemN[i].initButtonUL(&tft,0,82+20*i,30,20,2,TFT_BLACK,TFT_WHITE,itoa(mempos+i-memlin,buff,10),2);
      btMemN[i].drawButton();
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
      }
    tft.drawString("                       ",35,92+20*i);    
    if (memo.act[mempos+i-memlin]==1)
      {
      tft.drawString(memo.descr[mempos+i-memlin],35,92+20*i);    
      tft.drawString(memo.vfoActive[mempos+i-memlin]==VFO_A?"A":"B",160,92+20*i);    
      tft.drawString(memo.isUSB[mempos+i-memlin]==0?"L":"U",180,92+20*i);    
      tft.drawString(memo.cwMode[mempos+i-memlin]==0?" ":"C",200,92+20*i);    
      tft.drawString(memo.ritOn[mempos+i-memlin]==0?" ":"R",220,92+20*i);    
      tft.drawString(memo.splitOn[mempos+i-memlin]==0?" ":"S",240,92+20*i);    
      }
    }
}

void clearLine2()
{
  printLine(0,"");
}
