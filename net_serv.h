
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
  s2("hostmyip:"); s2(conf.hostmyip);
  http.begin("www.google.com", 80, msg);
  http.setConnectTimeout(2000);
  s2("checkInternet host: www.google.com:"); s2(80); s2(msg); s2("=");
  int httpCode=http.GET();
  s2(" "); s2(httpCode);
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
    //if (conf.iftttenabled) ifttttrigger(conucochar, conf.iftttkey, conf.aliasdevice, "NewIP", conf.myippub);
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
    s2("IP services");s2(crlf);
    if (conf.ftpenable)
      {
      initFTP(); s2(F("  FTP server started, port ")); s2(conf.ftpPort); s2(F(" admin/admin"));  
      }
    else
      s2("  FTP server disabled");
    s2(crlf);

    if (conf.webenable)
      {  
      initHTML();  s2("  HTML server started"); 
      initWebserver(); s2("  Web server started, port "); s2(conf.webPort);
      }
    else
      s2("  WEB server disabled");
    s2(crlf);

    if (conf.tcpenable)
      {
      initTCPS(); s2("  TCP server started, port "); s2(conf.tcpPort); 
      }
    else
      s2("  TCP server disabled");
    s2(crlf);

    s2("conf.wsenable:"); s2(conf.wsenable);
    if (conf.wsenable)
      {
      initWS(); s2("  WS server started, port "); s2(conf.wsPort); 
      }
    else
      s2("  WS server disabled");
    s2(crlf);

    if (conf.udpenable) { 
      initUDPS(); 
      s2("  UDP-S service started, port "); s2(conf.udpPortSmeter); 
      s2("  UDP-F service started, port "); s2(conf.udpPortFreq); }
    else
      s2("  UDP server disabled");
    s2(crlf);
    }

  if ((conf.wifimode==1) || (conf.wifimode==3))  // STA o AP+STA
    {
    int auxI=checkInternet();  s2("  Check Internet: ");s2(auxI==HTTP_CODE_OK?"OK":"ERROR");s2(crlf);
    internetON=(auxI==HTTP_CODE_OK); 
    if (internetON)
      {
      s2("Internet services");s2(crlf);
      initTime();       s2("  initTime");s2(crlf);
      int auxR=checkMyIP();   
         s2("  MyIP server: "); s2(conf.hostmyip);
         s2(auxR==HTTP_CODE_OK?" OK ":" ERROR "); s2(conf.myippub); s2(crlf);
      }
    }
}

