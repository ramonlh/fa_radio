
#pragma once

const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

#ifdef WEBSERVER
  WebServer server(webportdefault);
#endif


void serversend200() { 
  server.send(HTTP_CODE_OK, "text/html", msg); 
  msg=vacio; 
  }

void htmlNotFound()
{
  msg=vacio;
  //printP(c(HTTP11), b);
  //printP(c(t404), b);
  server.send(404, "text/plain", msg);
  msg=vacio;
}

// Datos dinámicos simulados
String getDataRows() {
  String data = "";

  data += "<tr><td class='td_f'>Frecuencia</td><td>14.074 MHz</td><td>USB</td></tr>";
  data += "<tr><td class='td_f'>Modo</td><td>FT8</td><td>Activo</td></tr>";
  data += "<tr><td class='td_f'>Potencia</td><td>10W</td><td>Normal</td></tr>";

  return data;
}

// Estado o tiempo simulado
String getStateTime2() {
  return "Estado: OK - Hora: 12:34:56";
}

String frecuencia = "14.074 MHz";
String modo = "FT8";
String potencia = "10W";

String oldFrecuencia, oldModo, oldPotencia;
unsigned long lastUpdate = 0;


void leerYProcesarHTML(const char* ruta) {
  File archivo = SPIFFS.open(ruta);
  if (!archivo || archivo.isDirectory()) {
    Serial.println("❌ No se pudo abrir el archivo");
    return;
  }

  String html = "";
  while (archivo.available()) {
    html += (char)archivo.read();
  }
  archivo.close();

  // Reemplazo de marcadores
  html.replace("<%DATA_ROWS%>", getDataRows());
  html.replace("<%STATE_TIME%>", getStateTime2());

  msg = html;  // Guardamos el HTML procesado en msg
  Serial.println("✅ HTML procesado y guardado en 'msg'");
}


void indexHTML() {

  leerYProcesarHTML("/index.html");
  serversend200();
/*
  msg=vacio;
  if (server.method()==HTTP_POST) return; 
  writeHeader(false,true);
  byte auxI=server.arg(0).toInt();
  writeMenu(1, auxI);
  printP(menor, table,b);
  printP("class", ig, tpanel, mayor,tr);
   
  /////////////  CONTENIDO   ///////////
  printColspan(3);
  printP(c(uBitx), td_f, tr_f);

  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(0);  printP(comilla,mayor);
  HtmlGetStateData(0);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(1);  printP(comilla,mayor);
  HtmlGetStateData(1);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(2);  printP(comilla,mayor);
  HtmlGetStateData(2);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(3);  printP(comilla,mayor);
  HtmlGetStateData(3);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(4);  printP(comilla,mayor);
  HtmlGetStateData(4);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(5);  printP(comilla,mayor);
  HtmlGetStateData(5);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letral);
  printI(6);  printP(comilla,mayor);
  HtmlGetStateData(6);
  printP(tr_f);
  
  printP(menor,letrat,letrar,b);  printP(c(tid),ig,comilla,letraf);
  printI(7);  printP(comilla,mayor);
  HtmlGetStateData(7);
  printP(tr_f);
 */
  /**for (byte i=0;i<MAX_BANDS;i++)
    {
    printP(tr,td,"Band ",conf.hamBandName[i],b);
    printP(letram,td_f,td); 
    printI(conf.freqbyband[i][0]); printP(td_f, td);
    printI(conf.freqbyband[i][1]); printP(td_f, tr_f);
    }**/
  // final
  /*
  printP(menor,letrat,letrar,b,c(tid));
  printP(ig,comilla,letrat,letrat,comilla,mayor);
  HtmlGetStateTime();
  printP(tr_f, menor, barra, table, mayor);
  printP(c(body_f), menor, barra,thtml, mayor);
  */
}

void sendOther(const char *otherURL, int param)
{
  msg=vacio;
  //printP(c(HTTP11)," ");
  //printP(c(t303)," ");
  //printP(c(seeother),crlf);
  //printP(c(location),dp,, " ",otherURL);
  if (param>=0) { 
    //printP(paramn);
    //printI(param);
    }
  printP(crlf);
  printP(crlf);
  server.sendContent(msg);
  msg=vacio;
}

void systemHTML()
{
  msg =vacio;
  if (server.method()==HTTP_GET)
  {
    for (int i=0; i<server.args(); i++)
      {
      if (server.argName(i).compareTo(PSTR("si")) == 0)   {
        nAPact=server.arg(i-1).toInt();
        WiFi.SSID(nAPact).toCharArray(conf.ssidSTA, 20);
        saveconf();
        sendOther("sne",-1); return;
        }
      }
    //sendOther(panelhtm, 0);
  }
}

void initHTML()
{
  server.onNotFound (htmlNotFound);
  //initupdateserver();
  server.on("/", indexHTML);
  server.on("/p", indexHTML);
  server.on("/sy", systemHTML);
  /**if (!checkfiles()) { server.on("/", filesHTML); return;  }*/
  //server.on("/f", filesHTML);

  //server.on("/", handleRoot);
  //server.on("/cw", setupCWHTML);
  //server.on("/sd", setupDevHTML);
  //server.on("/sm", setupMemHTML);
  //server.on("/sne", setupNetHTML);
  //server.on("/sc", scanapHTML);
  //server.on("/sns", setupNetServHTML);

  
  /*
  server.on("/l0", handleStateData0);       // data
  server.on("/l1", handleStateData1);       // data
  server.on("/l2", handleStateData2);       // data
  server.on("/l3", handleStateData3);       // data
  server.on("/l4", handleStateData4);       // data
  server.on("/l5", handleStateData5);       // data
  server.on("/l6", handleStateData6);       // data
  server.on("/l7", handleStateData7);       // data
  
  server.on("/f0", handleStateDataf0);       // data fast
  server.on("/f1", handleStateDataf1);       // data fast
  server.on("/f2", handleStateDataf2);       // data fast
  server.on("/f3", handleStateDataf3);       // data fast
  server.on("/f4", handleStateDataf4);       // data fast
  server.on("/f5", handleStateDataf5);       // data fast
  server.on("/f6", handleStateDataf6);       // data fast
  server.on("/f7", handleStateDataf7);       // data fast
  server.on("/tt", handleStateTime);         // Pie
  */
}

