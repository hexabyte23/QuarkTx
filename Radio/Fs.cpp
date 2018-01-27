/*
Mesure.h - QuarkTx
Copyright (c) 2015-2018 Thierry & Bertrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SerialLink.h"
#include "Fs.h"

#ifdef QUARKTX_TEENSY

//
// Implemente Lua Basis file functions
//

extern "C" 
{
  
#define QUARKTX_MAX_OPEN_FILE 10

File fileHandleTab[QUARKTX_MAX_OPEN_FILE];
int openHandleIdx = 0;

int _open(const char *filename, int flags, mode_t mode)
{  
  if(openHandleIdx < QUARKTX_MAX_OPEN_FILE)
  {
    fileHandleTab[openHandleIdx++] = SD.open(filename, mode);
    return openHandleIdx;
  }
  else
    return -1;
}

int _link(const char *oldpath, const char *newpath)
{
  return 0;
}

int _unlink(const char *pathname)
{
  return 0;
}

}

// Classname functions

bool QuarkTxFileSystem::setup()
{
  return SD.begin(QUARKTX_SPI_CS1_PIN);
}

int QuarkTxFileSystem::open(const char *pathname)
{
  return _open(pathname, 0, FILE_READ);
}

const char* QuarkTxFileSystem::read(int handle)
{
  File dataFile = fileHandleTab[handle];
  uint32_t s = dataFile.size()+1;

  STDOUT << "siz" << s << endl;

  char* data = new char[s];
  memset((void*)data, 0, s);

  if(dataFile) 
  {
    STDOUT << "data available" << endl;
    dataFile.read(data, s-1);
    
    dataFile.close();
  }

  return data;
}

void printDirectory(File dir, int numTabs) 
{
   while(true) 
   {  
     File entry =  dir.openNextFile();
     if(! entry)
       break;
    
     for(uint8_t i=0; i < numTabs; i++)
       STDOUT << '\t';
 
     STDOUT << entry.name();
     
     if(entry.isDirectory()) 
     {
       STDOUT << "/" << endl;
       printDirectory(entry, numTabs+1);
     } 
     else
     {
        for(uint8_t i=0; i < (12-strlen(entry.name())); i++)
           STDOUT << " ";
      
         STDOUT << "\t\t" << entry.size() << endl;
     }
     
     entry.close();
   }
}

void QuarkTxFileSystem::dumpDirectory(const char* path)
{
  STDOUT << "Path '" << path << "'" << endl;
  
  printDirectory(SD.open(path), 0);
}

#endif

