
#pragma once

#include "FS.h"
#include "SPIFFS.h"

int initSPIFSS(boolean testfiles, boolean format) {
  if (!SPIFFS.begin(format))
    return 1;
  if (testfiles) {
    fs::File dir = SPIFFS.open("/");
    fs::File file=dir.openNextFile();
    s2("Ficheros:\n");
    while(file) 
      {
      s2("  "); s2(file.name()); s2(" "); s2(file.size());s2(crlf); 
      file.close();
      file=dir.openNextFile(); 
      }
  }
  return 0;
}

boolean checkfile(char* namefile) {  
  if (!SPIFFS.exists(namefile)) { 
    s2(namefile); s2(" no existe\n"); 
    return false; 
    }  
  return true; 
}

boolean checkfiles() {
  boolean auxB=true;
  //auxB=auxB && checkfile(fileconf); 
  auxB=auxB && checkfile(filememo); 
  auxB=auxB && checkfile(filecommon); 
  auxB=auxB && checkfile(filespanish); 
  auxB=auxB && 
  checkfile(fileenglish); 
  filesok = auxB;
  return auxB;
}

int readmemo()
{
  int count=0;
  fs::File auxfile=SPIFFS.open(filememo,"r");
  if (auxfile)
    {
    for (count=0;count<sizeof(memo);count++)*(buffmemo+count)=auxfile.read();
    auxfile.close();
    }
  else 
    {
    s2("ERROR OPENNING FILE "); s2(filememo); s2(crlf);
    }
  return count;
}

void savememo()
{
  fs::File auxfile=SPIFFS.open(filememo, "w+");
  if (auxfile) 
    { 
    int byteswriten=auxfile.write(buffmemo,sizeof(memo)); 
    if (byteswriten!=sizeof(memo)) s2("ERROR AL GUARDAR FICHERO memo");
    auxfile.close();  
    }
  else
    {
    s2("ERROR OPENNING FILE MEMO");
    }
}



