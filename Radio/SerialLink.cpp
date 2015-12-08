#include "SerialLink.h"

int serialWrite(char c, FILE *f)
{
    Serial.write(c);
    return 0;
}

SerialLink::SerialLink()
: cmd_(NULL)
{
  clearSerialBuffer();
}

bool SerialLink::setup(Command *cmd)
{
  Serial.begin(SERIAL_SPEED);

  // reroute printf() output to serial
  stdout = stderr = fdevopen(serialWrite, NULL);

  printf("Quark Tx v%s\nBooting..\n", VERSION);
    
  cmd_ = cmd;
    
  printf("Serial\t\tOK\n");
  return true;
}

void SerialLink::clearSerialBuffer() 
{
  idxBuffer_ = 0;
  memset((void*)idxBuffer_,0,sizeof(idxBuffer_));
}

void SerialLink::idle()
{
  if(Serial.available() <= 0)
    return;
    
  //debug("[d] enter serial\n");
    
  while (Serial.available())
  {
    char c = (char)Serial.read();
    //debug("[d] '%d'",c);
    if (c == '\n')
    {
      serialBuffer_[idxBuffer_] = 0;
      serialBuffer_[idxBuffer_+1] = 0;
      
      if(cmd_ != NULL)
      {
        //debug(" %d Sent\n", idxBuffer_);
        cmd_->onNewCommand(serialBuffer_);
        idxBuffer_ = 0;
      }
    }
    else
    {
      //debug(" %d %c\n", idxBuffer_, c);
      serialBuffer_[idxBuffer_] = c;
      
      if(idxBuffer_ < MAX_SERIAL_INPUT_BUFFER)
        idxBuffer_++;
      else
      {
        printf("[e] Command string '%s' too long\n", serialBuffer_);
        idxBuffer_ = 0;
        break;
      }
    }
  }

  //debug("[d] exit serial\n");
}


