
#pragma once

#include "FS.h"
#include "FFat.h"

void initFFat(boolean testfiles, boolean format) {
  s2("FFat ");
  if (FFat.begin(format)) { s2("OK\n"); } else  { s2("ERROR\n"); }
    size_t total = FFat.totalBytes();
  size_t used = FFat.usedBytes();
  Serial2.printf("FFat total: %u bytes, usados: %u bytes, libres: %u bytes\n",
                total, used, total - used);
  if (testfiles) {
    File dir=FFat.open("/");
    File file=dir.openNextFile();
    while(file) 
      {
      s2(file.name()); s2(": "); s2(file.size());s2("\n"); 
      file.close();
      file=dir.openNextFile(); 
      }
  }
}

boolean checkfile(char* namefile) {  
  if (!FFat.exists(namefile)) { 
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
  fs::File auxfile=FFat.open(filememo,"r");
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
  fs::File auxfile=FFat.open(filememo, "w+");
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



