
#pragma once

void initSerial(long baud) { 
  Serial.begin(baud); 
  Serial.flush(); 
  }

void initSerial2(long baud) { 
  Serial2.begin(baud, SERIAL_8N2, RXD2, TXD2); 
  Serial2.flush(); 
  }

template <typename T>
void s2(T valor) {
  Serial2.print(valor);
  }

void printP(PGM_P texto1) { 
  char c;  
  while ((c = pgm_read_byte(texto1++))) msg += c; 
  }

// The generic routine to display one line on the LCD 
void printLine(unsigned char linenmbr, const char *c) {
  //tft.drawString(c,0,20*linenmbr);
}


void printLineF(char linenmbr, const __FlashStringHelper *c)
{
  int i;
  char tmpBuff[21];
  PGM_P p = reinterpret_cast<PGM_P>(c);  
  for (i = 0; i < 21; i++){
    unsigned char fChar = pgm_read_byte(p++);
    tmpBuff[i] = fChar;
    if (fChar == 0)
      break;
  }
  printLine(linenmbr, tmpBuff);
}

