
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

void printhelp()
{
  s2("debug           (debug ON/OFF)\n");
  s2("a               (auto WiFi: yes/no)\n");
  s2("f               (check files)\n");
  s2("F               (reset factory)\n");
  s2("h               (help)\n");    
  s2("p,SSIDpass      (set passSTA)\n");
  s2("P,SSID-APpass   (set pass AP)\n");
  s2("r               (reset)\n");  
  s2("s,SSIDname      (set SSID STA)\n");
  s2("S,SSID-AP       (set SSID AP)\n");
  s2("t               (format SPIFFS)\n");
  s2("u               (show status)\n");
  s2("w,n             (wifimode:0,1,2,3=OFF,STA,AP,STA+AP)\n");
  s2("---------------------------------------\n");
}

String getHora() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  char buf[9];
  sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
  return String(buf);
}

