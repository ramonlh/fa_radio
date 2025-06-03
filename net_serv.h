
#pragma once

void initWS();

#include <FTPServer.h>
#include <WebServer.h>
#include <Update.h>
#include <NTPClient.h>                // Local

#ifdef FTPSERVER
  FTPServer ftpSrv(SPIFFS);
#endif

void initFTP() { 
  #ifdef FTPSERVER
  ftpSrv.begin("admin","admin"); 
  #endif
  }  

#ifdef NTPCLIENT
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");
#endif

#ifdef UDPSERVER
  WiFiUDP udpsmeter, udpfreq;
#endif

void initupdateserver()
{
  server.on("/firm", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  server.on("/update", HTTP_POST, []() {
           server.sendHeader("Connection", "close");
           server.send(HTTP_CODE_OK, "text/plain", (Update.hasError())?"FAIL":"OK");
           ESP.restart();
           }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial2.setDebugOutput(true);
      s2("Update: "); s2(upload.filename.c_str());
      clearTFT();
      tft.setTextSize(2);
      tft.drawString("Updating firmware...",0,20);
      tft.drawString("Do not turn off the radio",0,40);
      if (!Update.begin()) { //start with max available size
        Update.printError(Serial2);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial2);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        { s2("Update Success: Rebooting..."); s2(upload.totalSize); s2(crlf); }
      } else {
        Update.printError(Serial);
      }
      Serial2.setDebugOutput(false);
    }
  });
}

void initWebserver() { 
  server.begin(); 
  } 

void initUDPS()
{
  /*
  if(udp.listen(udpPort)) {
    s2("UDP Listening on IP: ");  s2(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      //printhora();
      s2("UDP Packet Type: ");
      s2(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
      s2(", From: "); s2(packet.remoteIP());
      s2(":"); s2(packet.remotePort());
      s2(", To: ");  s2(packet.localIP());
      s2(":");  s2(packet.localPort());
      s2(", Length: "); s2(packet.length());
      s2(", Data: "); Serial2.write(packet.data(), packet.length());
      S2(crlf);
      //reply to the client
      packet.printf("Got %u bytes of data", packet.length());
      });
    }
    */
}

int checkInternet()
{
  msg=vacio;
  printP("/");
  HTTPClient http;
  http.begin("www.google.com", 80, msg);
  http.setConnectTimeout(2000);
  int httpCode=http.GET();
  http.end();
  msg=vacio;
  return httpCode;
}

void initTime() {
  ///// Time
  timeClient.begin();
  timeClient.setTimeOffset(timezoneval[conf.timezone]);
  if(timeClient.update()==1)
    { countfaulttime=0; setTime(timeClient.getEpochTime()); }
  else 
    { s2("timeclient.update:"); s2(timeClient.update()); }
}

int getMyIP()
{
  msg=vacio;
  printP("/");
  HTTPClient http;
  http.begin(conf.hostmyip, 80, msg);
//  http.setConnectTimeout(conf.timeoutNTP);
  http.setConnectTimeout(1000);
  int httpCode=http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) { msg=http.getString(); msg.toCharArray(conf.myippub, msg.length());  } }
  http.end();
  msg=vacio;
  return httpCode;
}

int checkMyIP()
{
  char auxip[16];
  strcpy(auxip, conf.myippub);
  int auxR=getMyIP();    // busca actual y la guarda en conf.txt
  if (strcmp(conf.myippub, auxip) != 0) // son diferentes
    {
    saveconf();
    }
  return auxR;
}

void initTCPS() { 
  //tcpserver.begin(conf.tcpPort); 
  }

void initNetServices()
{
  if ((conf.wifimode>0))  // 
    {
    s2("Servicios Red");s2(crlf);
    s2(F("  FTP server"));
    if (conf.ftpenable)
      {
      initFTP(); s2(F(" iniciado en puerto ")); s2(conf.ftpPort); s2(F(" (admin/admin)\n"));  
      }
    else
      s2(" disabled\n");

    s2("  WEB server "); 
    if (conf.webenable)
      {  
      initHTML();
      initWebserver(); s2(" iniciado en puerto ");  s2(conf.webPort); s2(crlf);
      }
    else
      s2(" disabled\n");

    s2("  TCP server "); 
    if (conf.tcpenable)
      {
      initTCPS(); s2("iniciado en puerto "); s2(conf.tcpPort); s2(crlf);
      }
    else
      s2("disabled\n");

    s2("  WS server "); 
    if (conf.wsenable)
      {
      initWS(); s2("iniciado en puerto "); s2(conf.wsPort);  s2(crlf);
      }
    else
      s2("disabled\n");

    if (conf.udpenable) { 
      initUDPS(); 
      s2("  UDP server ");
      s2("S iniciado en puerto "); s2(conf.udpPortSmeter); 
      s2(", F iniciado en puerto \n"); s2(conf.udpPortFreq); }
    else
      s2("disabled\n");
    }

  if ((conf.wifimode==1) || (conf.wifimode==3))  // STA o AP+STA
    {
    int auxI=checkInternet();  
    internetON=(auxI==HTTP_CODE_OK); 
    s2("  Internet "); 
    if (internetON)
      {
      s2(" OK\n");
      initTime();       s2("  initTime");s2(crlf);
      int auxR = checkMyIP();   
      s2("  IP publica "); 
      if (auxR == HTTP_CODE_OK) {
        s2(conf.myippub); s2(crlf);
        }
      else {
        s2(" ERROR\n");
        }
      }
    else {
      s2(" ERROR\n");
      }
    }
}

