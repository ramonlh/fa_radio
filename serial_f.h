

#pragma once

char cinput;
String sinput="";

void execcomdebug() {
  char command=cinput;
  String param;
  param=sinput.substring(sinput.indexOf(",")+1);
  boolean done=false;
  if (command=='a') { conf.autoWiFi=conf.autoWiFi==0?1:0; done=true; }
  else if( command=='f') {
    File dir=SPIFFS.open("/");
    File file=dir.openNextFile();
    while(file) { s2(file.name()); s2(" "); s2(file.size()); s2(crlf); file=dir.openNextFile(); }
    s2("files ");
    s2(checkfiles()?"OK":"ERROR"); s2(crlf);} 
  //else if (command=='F') { initFab(); }
  //else if (command=='h') { printhelp(); } 
  else if (command=='p') { param.toCharArray(conf.passSTA,20); done=true; }
  else if (command=='P') { param.toCharArray(conf.passAP,20); done=true; }
  else if (command=='r') { ESP.restart(); }
  else if (command=='s') { param.toCharArray(conf.ssidSTA,20); done=true; }
  else if (command=='S') { param.toCharArray(conf.ssidAP,20); done=true; }
  else if (command=='t') { SPIFFS.format(); }
  //else if (command=='u') { printstatus(); } 
  else if (command=='w') { if ((param.toInt()>=0) &&(param.toInt()<=3)) { conf.wifimode=param.toInt(); done=true; }}
  //else { s2("Command not found"); s2(crlf); printhelp(); }
  //if (done) { saveconf(); s2("Done"); s2(crlf); printstatus(); }
}


void handleSerial()
{
  if (Serial2.available()) 
    {
    boolean ejec=false;
    char tChar = Serial2.read(); 
    if (tChar=='\r')          // si es #13, CR
      {
      char tCharTrash=Serial2.read();       // leemos #10 y ejecutamos
      ejec=true;
      }
    else if (tChar=='\n') 
      { ejec=true; }    // si es #10, hemos terminado y ejecutamos
    else                    // si no es #10, lo añadimos
      {
      if (cinput=='\0') { cinput=tChar; }   // nuevo comando
      else { sinput=sinput+tChar; }         // añadir al parámetro
      } 
    if (ejec)
      {
      if (conf.serial2Mode==0)    // modo debug
        execcomdebug();
      else
        {
        //if ((conf.connMode==1) || (conf.connMode==2) || (conf.connMode==3))  // modo IP or Serial2-IP, mod Manager
         // handleRecDataIP(cinput, sinput);  
        }
      sinput=""; cinput='\0';  
      }
    }
}
