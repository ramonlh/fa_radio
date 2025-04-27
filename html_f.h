
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
    Serial2.println("No se pudo abrir el archivo");
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
  //Serial2.println("HTML procesado y guardado en 'msg'");
}

String generateUtcOptions(int selectedUtc) {
  String options = "";
  // Listado de las zonas horarias
  String utcOffsets[] = {
    "-12", "-11", "-10", "-9:30", "-9", "-8", "-7", "-6", "-5", "-4", "-3", "-2",
    "-1", "0", "+1", "+2", "+3", "+3:30", "+4", "+4:30", "+5", "+5:30", "+6", "+7", "+8",
    "+9", "+9:30", "+10", "+11", "+12", "+13", "+14"
    };
  for (int i = 0; i < 29; i++) {
    // Si la zona horaria coincide con la seleccionada, añade el atributo "selected"
    if (i == selectedUtc) {
      options += "<option value=\"" + String(i) + "\" selected>" + utcOffsets[i] + "</option>\n";
    } else {
      options += "<option value=\"" + String(i) + "\">" + utcOffsets[i] + "</option>\n";
    }
  }
  return options;
}

String generateResetOptions(int selectedReset) {
  String options = "";
  // Opciones disponibles: OFF (valor 0), 1, 24
  int resetOptions[] = {0, 1, 24};
  // Crear las opciones HTML
  for (int i = 0; i < 3; i++) {
    String optionValue = (resetOptions[i] == 0) ? "OFF" : String(resetOptions[i]);
    // Si la opción actual es la seleccionada, añadir el atributo "selected"
    if (resetOptions[i] == selectedReset) {
      options += "<option value=\"" + String(resetOptions[i]) + "\" selected>" + optionValue + "</option>\n";
    } else {
      options += "<option value=\"" + String(resetOptions[i]) + "\">" + optionValue + "</option>\n";
    }
  }
  return options;
}

String processordev(String var) {
  if (var == "CALLSIGN") return conf.CallSign;
  if (var == "CALIB") return String(conf.calibration);
  if (var == "USBcarrier") return String(conf.usbCarrier);
  if (var == "LAT") return String(conf.latitud, 6);
  if (var == "LON") return String(conf.longitud, 6);
  if (var == "LANG0") return conf.lang == 0 ? "selected" : "";
  if (var == "LANG1") return conf.lang == 1 ? "selected" : "";
  if (var == "LANG2") return conf.lang == 2 ? "selected" : "";
  if (var == "UTCOPTIONS") return generateUtcOptions(conf.timezone);
  if (var == "SMETER") return conf.Smeterenabled ? "checked" : "";
  if (var == "ROE") return conf.SWRenabled ? "checked" : "";
  if (var == "DS18") return conf.DS18B20enabled ? "checked" : "";
  if (var == "ATU") return conf.ATUZM2enabled ? "checked" : "";
  if (var == "ATUT") return String(conf.ATUdelay);
  if (var == "ADC") return String(conf.ATUIter);
  if (var == "ATUF") return String(conf.ATUFactor, 6);
  if (var == "ATUO") return String(conf.ATUOffset, 6);
  if (var == "ADDR") return String(conf.SI5351BX_ADDR);
  if (var == "SCAN0") return conf.scanallf == 0 ? "selected" : "";
  if (var == "TX0") return conf.TXall == 0 ? "selected" : "";
  if (var == "MODE0") return conf.scanmode == 0 ? "selected" : "";
  if (var == "DELAY") return String(conf.scandelay);
  if (var == "TALARM") return String(conf.TempAlarm);
  if (var == "TOFFTX") return String(conf.TempTxOff);
  if (var == "RESETHOURS") return generateResetOptions(conf.rstper);
  return String();
}

String processorbands(String var) {
  if (var.length() < 4 || var[0] != 'B') return String();

  String bandStr = var.substring(1, var.length() - 1); // Ej: "160", "80", ...
  char tipo = var[var.length() - 1]; // 'D' o 'H'
  int bandIdx = -1;

  // Lista de bandas en orden
  String bands[] = { "160", "80", "60", "40", "30", "20", "17", "15", "12", "10" };

  for (int i = 0; i < 10; i++) {
    if (bands[i] == bandStr) {
      bandIdx = i;
      break;
      }
    }
  if (bandIdx >= 0) {
    int subIdx = (tipo == 'D') ? 0 : (tipo == 'H') ? 1 : -1;
    if (subIdx != -1) {
      return String(conf.hamBandRange[bandIdx][subIdx]);
      }
    }
  return String();
}

String processorsmeterdata(String var) {
  if (var.startsWith("V")) {
    int idx = var.substring(1).toInt(); // extrae el número después de la 'V'
    if (idx >= 0 && idx < 16) {
      return String(conf.sMeterLevels[idx]);
    }
  }
  return String();
}

String processorscwdata(String var) {
  if (var == "CW_SPEED") return String(conf.cwSpeed);
  if (var == "CW_DELAY") return String(conf.cwTimeout);
  if (var == "PDE") return String(conf.cwAdcSTFrom);
  if (var == "PA") return String(conf.cwAdcSTTo);
  if (var == "RDE") return String(conf.cwAdcDotFrom);
  if (var == "RA") return String(conf.cwAdcDotTo);
  if (var == "ADE") return String(conf.cwAdcDashFrom);
  if (var == "AA") return String(conf.cwAdcDashTo);
  if (var == "SE") return String(conf.cwAdcBothFrom);
  if (var == "SA") return String(conf.cwAdcBothTo);
  return String();
}

// Genera las filas HTML de memorias
String generateMemoryRows() {
  String html;
  for (int i = 0; i < 10; i++) {
    html += "<tr><td>" + String(i + 1) + "</td>";

    html += "<td><input type='checkbox' name='" + String(i * 13 + 0) + "' value='1'";
    if (memo.act[i]) html += " checked";
    html += "></td>";
    html += "<td><input type='text' name='" + String(i * 13 + 1) + "' value='" + String(memo.descr[i]) + "' maxlength='9' size='9'></td>";
    html += "<td><input type='text' name='" + String(i * 13 + 2) + "' value='" + String(memo.frequency[i]) + "' maxlength='9' size='9'></td>";
    html += "<td><select name='" + String(i * 13 + 3) + "'>";
    html += "<option value='0'" + String(memo.isUSB[i] == 0 ? " selected" : "") + ">LSB</option>";
    html += "<option value='1'" + String(memo.isUSB[i] == 1 ? " selected" : "") + ">USB</option></select></td>";
    html += "<td><select name='" + String(i * 13 + 4) + "'>";
    html += "<option value='0'" + String(memo.cwMode[i] == 0 ? " selected" : "") + ">None</option>";
    html += "<option value='1'" + String(memo.cwMode[i] == 1 ? " selected" : "") + ">CW</option></select></td>";
    html += "<td><select name='" + String(i * 13 + 5) + "'>";
    html += "<option value='0'" + String(memo.vfoActive[i] == 0 ? " selected" : "") + ">VFO A</option>";
    html += "<option value='1'" + String(memo.vfoActive[i] == 1 ? " selected" : "") + ">VFO B</option></select></td>";
    html += "<td><input type='checkbox' name='" + String(i * 13 + 6) + "' value='1'";
    if (memo.ritOn[i]) html += " checked";
    html += "></td>";
    html += "<td><input type='text' name='" + String(i * 13 + 7) + "' value='" + String(memo.ritTxFrequency[i]) + "' maxlength='9' size='9'></td>";
    html += "<td><input type='checkbox' name='" + String(i * 13 + 8) + "' value='1'";
    if (memo.splitOn[i]) html += " checked";
    html += "></td>";
    html += "<td><input type='text' name='" + String(i * 13 + 9) + "' value='" + String(memo.ftxspl[i]) + "' maxlength='9' size='9'></td>";
    html += "<td><select name='" + String(i * 13 + 10) + "'>";
    html += "<option value='0'" + String(memo.isUSB[i] == 0 ? " selected" : "") + ">LSB</option>";
    html += "<option value='1'" + String(memo.isUSB[i] == 1 ? " selected" : "") + ">USB</option></select></td>";
    html += "<td><select name='" + String(i * 13 + 11) + "'>";
    html += "<option value='0'" + String(memo.cwModespl[i] == 0 ? " selected" : "") + ">None</option>";
    html += "<option value='1'" + String(memo.cwModespl[i] == 1 ? " selected" : "") + ">CW</option></select></td>";
    html += "</tr>\n";
    }
  return html;
}

String processornet(String var) {
  if (var == "auto_conn") return conf.autoWiFi ? "checked" : "";
  if (var == "modo_0") return conf.wifimode == 0 ? "selected" : "";
  if (var == "modo_1") return conf.wifimode == 1 ? "selected" : "";
  if (var == "modo_2") return conf.wifimode == 2 ? "selected" : "";
  if (var == "modo_3") return conf.wifimode == 3 ? "selected" : "";
  if (var == "ssid_sta") return conf.ssidSTA;
  if (var == "password_sta") return conf.passSTA;
  if (var == "ssid_ap") return conf.ssidAP;
  if (var == "password_ap") return conf.passAP;
  if (var == "canal_0") return conf.canalAP == 1 ? "selected" : "";
  if (var == "canal_1") return conf.canalAP == 2 ? "selected" : "";
  if (var == "canal_2") return conf.canalAP == 3 ? "selected" : "";
  if (var == "canal_3") return conf.canalAP == 4 ? "selected" : "";
  if (var == "canal_4") return conf.canalAP == 5 ? "selected" : "";
  if (var == "canal_5") return conf.canalAP == 6 ? "selected" : "";
  if (var == "canal_6") return conf.canalAP == 7 ? "selected" : "";
  if (var == "canal_7") return conf.canalAP == 8 ? "selected" : "";
  if (var == "canal_8") return conf.canalAP == 9 ? "selected" : "";
  if (var == "canal_9") return conf.canalAP == 10 ? "selected" : "";
  if (var == "canal_10") return conf.canalAP == 11 ? "selected" : "";
  if (var == "canal_11") return conf.canalAP == 12 ? "selected" : "";
  if (var == "canal_12") return conf.canalAP == 13 ? "selected" : "";
  //if (var == "mac") return conf.EEmask;
  if (var == "static_ip") return conf.staticIP ? "checked" : "";
  //if (var == "dir_ip") return conf.EEip;
  if (var == "ip0") return String(conf.EEip[0]);
  if (var == "ip1") return String(conf.EEip[1]);
  if (var == "ip2") return String(conf.EEip[2]);
  if (var == "ip3") return String(conf.EEip[3]);
  if (var == "mk0") return String(conf.EEmask[0]);
  if (var == "mk1") return String(conf.EEmask[1]);
  if (var == "mk2") return String(conf.EEmask[2]);
  if (var == "mk3") return String(conf.EEmask[3]);
  if (var == "gw0") return String(conf.EEgw[0]);
  if (var == "gw1") return String(conf.EEgw[1]);
  if (var == "gw2") return String(conf.EEgw[2]);
  if (var == "gw3") return String(conf.EEgw[3]);
  if (var == "dns0") return String(conf.EEdns[0]);
  if (var == "dns1") return String(conf.EEdns[1]);
  if (var == "dns2") return String(conf.EEdns[2]);
  if (var == "dns3") return String(conf.EEdns[3]);
  if (var == "public_ip") return conf.myippub;

  // Agrega más sustituciones según necesites
   return String();
}

String processorsnet(String var) {
  if (var == "IPPUB") return conf.hostmyip;
  if (var == "FTPCHK") return conf.ftpenable ? "checked" : "";
  if (var == "TCPCHK") return conf.tcpenable ? "checked" : "";
  if (var == "UDPCHK") return conf.udpenable ? "checked" : "";
  if (var == "WSCHK") return conf.wsenable ? "checked" : "";
  if (var == "WEBCHK") return conf.webenable ? "checked" : "";
  if (var == "MQCHK") return conf.staticIP ? "checked" : "";
  if (var == "MQSVR") return conf.mqttserver;
  if (var == "MQP1") return conf.mqttpath[0];
  if (var == "MQP2") return conf.mqttpath[1];
  if (var == "MQP3") return conf.mqttpath[2];
  if (var == "MQP4") return conf.mqttpath[3];
  if (var == "MQP5") return conf.mqttpath[4];
  if (var == "MQP6") return conf.mqttpath[5];

  // Agrega más sustituciones según necesites
   return String();
}

void indexHTML() {
  leerYProcesarHTML("/index.html");
  serversend200();
}

String addrToString(const uint8_t* addr) {
  String s = "";
  for (int i = 0; i < 8; i++) {
    if (addr[i] < 16) s += "0"; // Para que siempre sea 2 dígitos
    s += String(addr[i], HEX);
  }
  return s;
}

String generarSelectSondas(int num) {
  String opciones = "";  // Empezamos sin opciones

  // Primero agregamos las opciones de las sondas reales (empezamos desde i = 1)
  for (int i = 0; i < maxTemp; i++) {
    String codigo = "";
    for (int j = 0; j < 8; j++) {
      codigo += String(addr1Wire[i][j], HEX);
    }
    codigo.toUpperCase();

    // Agregar la opción para la sonda con su código
    opciones += "<option value=\"" + String(i) + "\"";

    // Seleccionar automáticamente si es el que está en conf.probecode[num]
    bool esSeleccionado = true;
    for (int j = 0; j < 8; j++) {
      if (addr1Wire[i][j] != conf.probecode[num][j]) {
        esSeleccionado = false;
        break;
      }
    }
    if (esSeleccionado) {
      opciones += " selected";  // Marcar como seleccionado si coincide
    }

    opciones += ">" + codigo + "</option>";
  }

  // Finalmente, agregamos la opción "Sin sonda" al final
  opciones += "<option value=\"0\">Sin sonda</option>";

  return opciones;
}

void setuptempHTML() {
  leerYProcesarHTML("/stemp.html");
  // los codigos leidos al iniciar se guardan en addr1Wire[1..8][8]
  // los codigos de sondas se guardan en probecode[1.8][8]  msg.replace("%S0%", addrToString(addr1Wire[0]));
  for (int i = 0; i < 3; i++) {  // Tenemos 3 select: 0, 1, 2
    String placeholder = "%S" + String(i) + "%";
    msg.replace(placeholder, generarSelectSondas(i));
    }
  serversend200();
}

void handleGuardarTemp() {
  for (int i = 0; i < 3; i++) {  // SOLO hay 3 selects: 0, 1, 2
    String key = String(i);       // "0", "1", "2"
    if (server.hasArg(key)) {
      int index = server.arg(key).toInt();
      if (index >= 0 && index < maxTemp) {
        memcpy(conf.probecode[i], addr1Wire[index], 8); // Copiar los 8 bytes
      }
    }
  }
  saveconf();  // función que salva a memoria flash / EEPROM
  server.send(200, "text/html", "<html><body><h2>Datos guardados</h2><a href=\"/sio\">Volver</a></body></html>");
}

void setupDevHTML() {
  leerYProcesarHTML("/disp.html");
  // Reemplaza los marcadores %XXX% por los valores actuales
  int startIdx;
  while ((startIdx = msg.indexOf('%')) != -1) {
    int endIdx = msg.indexOf('%', startIdx + 1);
    if (endIdx == -1) break;

    String var = msg.substring(startIdx + 1, endIdx);
    String value = processordev(var);
    msg.replace('%' + var + '%', value);
    }
  serversend200();
}
void setupBandasHTML() {
  leerYProcesarHTML("/bandas.html");
  // Reemplaza los marcadores %XXX% por los valores actuales
  int startIdx;
  while ((startIdx = msg.indexOf('%')) != -1) {
    int endIdx = msg.indexOf('%', startIdx + 1);
    if (endIdx == -1) break;

    String var = msg.substring(startIdx + 1, endIdx);
    String value = processorbands(var);
    msg.replace('%' + var + '%', value);
    }
  serversend200();
}
void setupMemoHTML() {
  leerYProcesarHTML("/memo.html");
  msg.replace("<!--@@MEMORY_ROWS@@-->", generateMemoryRows()); 
  serversend200();
}
void setupCWHTML() {
  leerYProcesarHTML("/cw.html");
  // Reemplaza los marcadores %XXX% por los valores actuales
  int startIdx;
  while ((startIdx = msg.indexOf('%')) != -1) {
    int endIdx = msg.indexOf('%', startIdx + 1);
    if (endIdx == -1) break;
    String var = msg.substring(startIdx + 1, endIdx);
    String value = processorscwdata(var);
    msg.replace('%' + var + '%', value);
    }
  serversend200();
}

void setupSmeterHTML() {
  leerYProcesarHTML("/smeter.html");
  // Reemplaza los marcadores %XXX% por los valores actuales
  int startIdx;
  while ((startIdx = msg.indexOf('%')) != -1) {
    int endIdx = msg.indexOf('%', startIdx + 1);
    if (endIdx == -1) break;
    String var = msg.substring(startIdx + 1, endIdx);
    String value = processorsmeterdata(var);
    msg.replace('%' + var + '%', value);
    }
  serversend200();
}

void setupNetHTML() {
  leerYProcesarHTML("/red.html");
  // Reemplaza los marcadores %XXX% por los valores actuales
  int startIdx;
  while ((startIdx = msg.indexOf('%')) != -1) {
    int endIdx = msg.indexOf('%', startIdx + 1);
    if (endIdx == -1) break;
    String var = msg.substring(startIdx + 1, endIdx);
    String value = processornet(var);
    msg.replace('%' + var + '%', value);
    }
  serversend200();
}

void setupsNetHTML() {
  leerYProcesarHTML("/sred.html");
  // Reemplaza los marcadores %XXX% por los valores actuales
  int startIdx;
  while ((startIdx = msg.indexOf('%')) != -1) {
    int endIdx = msg.indexOf('%', startIdx + 1);
    if (endIdx == -1) break;
    String var = msg.substring(startIdx + 1, endIdx);
    String value = processorsnet(var);
    msg.replace('%' + var + '%', value);
    }
  serversend200();
}
void setupEstadoHTML() {
  leerYProcesarHTML("/estado.html");
  serversend200();
}
void setupSegHTML() {
  leerYProcesarHTML("/seg.html");
  serversend200();
}
void filesHTML() {
  leerYProcesarHTML("/files.html");
  serversend200();
}
void actualHTML() {
  leerYProcesarHTML("/actual.html");
  serversend200();
}

void handleGuardarConfig() {
  if (server.hasArg("0")) strncpy(conf.userDev, server.arg("0").c_str(), sizeof(conf.userDev));
  if (server.hasArg("1")) strncpy(conf.passDev, server.arg("1").c_str(), sizeof(conf.passDev));
  if (server.hasArg("2")) conf.latitud = server.arg("2").toFloat();
  if (server.hasArg("3")) conf.longitud = server.arg("3").toFloat();
  if (server.hasArg("5")) conf.lang = server.arg("5").toInt();
  if (server.hasArg("9")) conf.SI5351BX_ADDR = server.arg("9").toInt();
  if (server.hasArg("10")) conf.scanallf = server.arg("10").toInt();
  if (server.hasArg("11")) conf.TXall = server.arg("11").toInt();
  if (server.hasArg("12")) conf.ATUdelay = server.arg("12").toInt();
  if (server.hasArg("13")) conf.ATUFactor = server.arg("13").toInt();
  if (server.hasArg("14")) conf.ATUOffset = server.arg("14").toInt();
  if (server.hasArg("15")) conf.TempAlarm = server.arg("15").toInt();
  if (server.hasArg("16")) conf.TempTxOff = server.arg("16").toInt();
  if (server.hasArg("17")) conf.ATUIter = server.arg("17").toInt();
  if (server.hasArg("18")) conf.scanmode = server.arg("18").toInt();
  if (server.hasArg("19")) conf.scandelay = server.arg("19").toInt();
  // Checkboxes (presente = activado)
  conf.Smeterenabled = server.hasArg("20") ? 1 : 0;
  conf.SWRenabled = server.hasArg("21") ? 1 : 0;
  conf.DS18B20enabled = server.hasArg("22") ? 1 : 0;
  conf.ATUZM2enabled = server.hasArg("23") ? 1 : 0;
  if (server.hasArg("24")) conf.timezone = server.arg("24").toInt();
  if (server.hasArg("99")) conf.rstper = server.arg("99").toInt();
  // Aquí iría tu función que guarda todo en almacenamiento:
  saveconf();
  // Enviar respuesta al navegador
  server.send(200, "text/html", "<html><body><h2>Datos guardados</h2><a href=\"/sd\">Volver</a></body></html>");
}
/*
String processorsnet(String var) {
  if (var == "IPPUB") return conf.hostmyip;
  if (var == "FTPCHK") return conf.ftpenable ? "checked" : "";
  if (var == "TCPCHK") return conf.tcpenable ? "checked" : "";
  if (var == "UDPCHK") return conf.udpenable ? "checked" : "";
  if (var == "WSCHK") return conf.wsenable ? "checked" : "";
  if (var == "WEBCHK") return conf.webenable ? "checked" : "";
  if (var == "MQCHK") return conf.staticIP ? "checked" : "";
  if (var == "MQSVR") return conf.mqttserver;
  if (var == "MQP1") return conf.mqttpath[0];
  if (var == "MQP2") return conf.mqttpath[1];
  if (var == "MQP3") return conf.mqttpath[2];
  if (var == "MQP4") return conf.mqttpath[3];
  if (var == "MQP5") return conf.mqttpath[4];
  if (var == "MQP6") return conf.mqttpath[5];
}
*/
void handleGuardarNet() {
  if (server.hasArg("1")) strncpy(conf.passDev, server.arg("1").c_str(), sizeof(conf.passDev));
  if (server.hasArg("2")) conf.latitud = server.arg("2").toFloat();
  if (server.hasArg("3")) conf.longitud = server.arg("3").toFloat();
  if (server.hasArg("5")) conf.lang = server.arg("5").toInt();
  if (server.hasArg("9")) conf.SI5351BX_ADDR = server.arg("9").toInt();
  if (server.hasArg("10")) conf.scanallf = server.arg("10").toInt();
  if (server.hasArg("11")) conf.TXall = server.arg("11").toInt();
  if (server.hasArg("12")) conf.ATUdelay = server.arg("12").toInt();
  if (server.hasArg("13")) conf.ATUFactor = server.arg("13").toInt();
  if (server.hasArg("14")) conf.ATUOffset = server.arg("14").toInt();
  if (server.hasArg("15")) conf.TempAlarm = server.arg("15").toInt();
  if (server.hasArg("16")) conf.TempTxOff = server.arg("16").toInt();
  if (server.hasArg("17")) conf.ATUIter = server.arg("17").toInt();
  if (server.hasArg("18")) conf.scanmode = server.arg("18").toInt();
  if (server.hasArg("19")) conf.scandelay = server.arg("19").toInt();
  // Checkboxes (presente = activado)
  conf.Smeterenabled = server.hasArg("20") ? 1 : 0;
  conf.SWRenabled = server.hasArg("21") ? 1 : 0;
  conf.DS18B20enabled = server.hasArg("22") ? 1 : 0;
  conf.ATUZM2enabled = server.hasArg("23") ? 1 : 0;
  if (server.hasArg("24")) conf.timezone = server.arg("24").toInt();
  if (server.hasArg("99")) conf.rstper = server.arg("99").toInt();
  // Aquí iría tu función que guarda todo en almacenamiento:
  saveconf();
  // Enviar respuesta al navegador
  server.send(200, "text/html", "<html><body><h2>Datos guardados</h2><a href=\"/sne\">Volver</a></body></html>");
}

void handleGuardarsNet() {
  if (server.hasArg("0")) strncpy(conf.hostmyip, server.arg("0").c_str(), sizeof(conf.hostmyip));
  conf.ftpenable = server.hasArg("1") ? 1 : 0;
  conf.tcpenable = server.hasArg("2") ? 1 : 0;
  conf.udpenable = server.hasArg("3") ? 1 : 0;
  conf.wsenable = server.hasArg("4") ? 1 : 0;
  conf.webenable = server.hasArg("5") ? 1 : 0;
  conf.mqttenabled = server.hasArg("6") ? 1 : 0;
  if (server.hasArg("7"))  strncpy(conf.mqttserver, server.arg("7").c_str(), sizeof(conf.mqttserver));
  if (server.hasArg("8")) strncpy(conf.mqttpath[0], server.arg("8").c_str(), sizeof(conf.mqttpath[0]));
  if (server.hasArg("9")) strncpy(conf.mqttpath[1], server.arg("9").c_str(), sizeof(conf.mqttpath[1]));
  if (server.hasArg("10")) strncpy(conf.mqttpath[2], server.arg("10").c_str(), sizeof(conf.mqttpath[2]));
  if (server.hasArg("11")) strncpy(conf.mqttpath[3], server.arg("11").c_str(), sizeof(conf.mqttpath[3]));
  if (server.hasArg("12")) strncpy(conf.mqttpath[4], server.arg("12").c_str(), sizeof(conf.mqttpath[4]));
  if (server.hasArg("13")) strncpy(conf.mqttpath[5], server.arg("13").c_str(), sizeof(conf.mqttpath[5]));
  saveconf();
  // Enviar respuesta al navegador
  server.send(200, "text/html", "<html><body><h2>Datos guardados</h2><a href=\"/sns\">Volver</a></body></html>");
}

void handleGuardarBandas() {
  for (int i = 0; i < 10; i++) {
    int idx0 = i * 2;
    int idx1 = idx0 + 1;
    if (server.hasArg(String(idx0))) {
      conf.hamBandRange[i][0] = server.arg(String(idx0)).toInt();
      }
    if (server.hasArg(String(idx1))) {
      conf.hamBandRange[i][1] = server.arg(String(idx1)).toInt();
      }
  }
  saveconf();
  server.send(200, "text/html","<html><body><h2>Datos guardados</h2><a href=\"/sb\">Volver</a></body></html>");
}

void handleGuardarCW() {

  if (server.hasArg("1")) conf.cwSpeed = server.arg("1").toInt();
  if (server.hasArg("2")) conf.cwTimeout = server.arg("2").toInt();
  if (server.hasArg("3")) conf.cwAdcSTFrom = server.arg("3").toInt();
  if (server.hasArg("4")) conf.cwAdcSTTo = server.arg("4").toInt();
  if (server.hasArg("5")) conf.cwAdcDotFrom = server.arg("5").toInt();
  if (server.hasArg("6")) conf.cwAdcDotTo = server.arg("6").toInt();
  if (server.hasArg("7")) conf.cwAdcDashFrom = server.arg("7").toInt();
  if (server.hasArg("8")) conf.cwAdcDashTo = server.arg("8").toInt();
  if (server.hasArg("9")) conf.cwAdcBothFrom = server.arg("9").toInt();
  if (server.hasArg("10")) conf.cwAdcBothTo = server.arg("10").toInt();
  saveconf();
  server.send(200, "text/html", 
    "<html><body><h2>Datos guardados</h2><a href=\"/cw\">Volver</a></body></html>");
}

void handleGuardarSmeter() {
  for (int i = 0; i < 16; i++) {
    String key = String(i);
    if (server.hasArg(key)) {
      conf.sMeterLevels[i] = server.arg(key).toInt();
      }
    }
  // Aquí iría tu función que guarda todo en almacenamiento:
  saveconf();
  // Enviar respuesta al navegador
  server.send(200, "text/html", "<html><body><h2>Datos guardados</h2><a href=\"/sme\">Volver</a></body></html>");
}

void handleGuardarMemo() {
  for (int i = 0; i < maxMem; i++) {
    String act = server.arg("act" + String(i));
    if (act == "on") {
      memo.act[i] = 1;
    } else {
      memo.act[i] = 0;
    }
    // Actualizar la descripción
    String descr = server.arg("descr" + String(i));
    descr.toCharArray(memo.descr[i], 20);
    // Actualizar la frecuencia
    String freq = server.arg("freq" + String(i));
    if (freq.length() > 0) {
      memo.frequency[i] = freq.toInt();
    }
    // Actualizar el modo (LSB o USB)
    String mode = server.arg("mode" + String(i));
 
    if (mode == "LSB") { memo.isUSB[i] = 0; } else { memo.isUSB[i] = 1; }
    // Actualizar CW mode
    String cwMode = server.arg("cwMode" + String(i));
    if (cwMode == "CW") {
      memo.cwMode[i] = 1;
    } else {
      memo.cwMode[i] = 0;
    }
    // Actualizar otras variables si es necesario...
  }
  // Aquí iría tu función que guarda todo en almacenamiento:
  savememo();
  // Enviar respuesta al navegador
  server.send(200, "text/html", "<html><body> <h2>Datos guardados</h2> <a href=\"/sm\">Volver</a> </body></html>");
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
  server.on("/sio", HTTP_GET, setuptempHTML);
  server.on("/sio", HTTP_POST, handleGuardarTemp);
  server.on("/sd", HTTP_GET, setupDevHTML);         // <- Para mostrar la página
  server.on("/sd", HTTP_POST, handleGuardarConfig); // <- Para guardar datos
  server.on("/sb", HTTP_GET, setupBandasHTML);
  server.on("/sb", HTTP_POST, handleGuardarBandas);
  server.on("/sm", HTTP_GET, setupMemoHTML);
  server.on("/sm", HTTP_POST, handleGuardarMemo);
  server.on("/cw", HTTP_GET, setupCWHTML);
  server.on("/cw", HTTP_POST, handleGuardarCW);
  server.on("/sme", HTTP_GET, setupSmeterHTML);
  server.on("/sme", HTTP_POST, handleGuardarSmeter);
  server.on("/sne", HTTP_GET, setupNetHTML);
  server.on("/sne", HTTP_POST, handleGuardarNet);
  server.on("/sns", HTTP_GET, setupsNetHTML);
  server.on("/sns", HTTP_POST, handleGuardarsNet);
  server.on("/es", setupEstadoHTML);
  server.on("/ss", setupSegHTML);
  server.on("/f", filesHTML);
  server.on("/firm", actualHTML);
  // Maneja la ruta para servir el archivo styles.css
  server.on("/styles.css", HTTP_GET, []() {
    File file = SPIFFS.open("/styles.css", "r");
    if (file) {
      server.streamFile(file, "text/css");
      file.close();
    } else {
      server.send(404, "text/plain", "Archivo no encontrado");
    }
  });
  // Maneja la ruta para servir el archivo script.js
  server.on("/script.js", HTTP_GET, []() {
    File file = SPIFFS.open("/script.js", "r");
    if (file) {
      server.streamFile(file, "application/javascript");
      file.close();
    } else {
      server.send(404, "text/plain", "Archivo no encontrado");
    }
  });
/**if (!checkfiles()) { server.on("/", filesHTML); return;  }*/
  //server.on("/sc", scanapHTML);
  
  
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

