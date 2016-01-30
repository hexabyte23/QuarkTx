/*
Command.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Betrand WILMOT.  All rights reserved.

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

#include "RadioLink.h"
#include <QThread>

RadioLink::RadioLink()
   :
     isTxConnected_(false),
     transportStatusStr("No transport"),
     cnxStatusStr_("Not connected"),
     txVersionStr_("NA"),
     serialPort_(NULL),
     serialPortInfo_(NULL)
{
}

RadioLink::~RadioLink()
{
   closeSerialLink();
   closeBlueToothLink();
}

void RadioLink::closeSerialLink()
{
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)

   if(serialPort_)
   {
      if(serialPort_->isOpen())
         serialPort_->close();
      disconnect(serialPort_, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
      disconnect(serialPort_, SIGNAL(error(QSerialPort::SerialPortError)), this,
                 SLOT(serialPortHandleError(QSerialPort::SerialPortError)));

      delete serialPort_;
      serialPort_ = NULL;

      QSerialPortInfo i;
      serialPortInfo_ = i;
   }
#endif
}

void RadioLink::closeBlueToothLink()
{
}

bool RadioLink::searchForSerialLink()
{
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)

   bool foundCnx = false;
   QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
   for (int i = 0; i < serialPortInfoList.size(); i++)
   {
      const QSerialPortInfo &serialPortInfo = serialPortInfoList[i];
      serialPort_ = new QSerialPort(serialPortInfo, this);
      connect(serialPort_, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
      connect(serialPort_, SIGNAL(error(QSerialPort::SerialPortError)), this,
              SLOT(serialPortHandleError(QSerialPort::SerialPortError)));

      qDebug() << "Try open cnx: " << serialPortInfo.portName();

      if(serialPort_->open(QIODevice::ReadWrite))
      {
         qDebug() << "Waiting for anwser...";

         if(serialPort_->waitForReadyRead(QUARKTX_MIN_WAIT_DEVICE))
         {
            qDebug() << "Anwsering... Check if QuarkTx device";

            //QThread::sleep(2);

            QString line = getNextLine();
            if(line.startsWith("Quark Tx v"))
            {
               txVersionStr_ = line.mid(sizeof("Quark Tx v")-1);
               qDebug() << "QuarkTx device detected " << txVersionStr_;
               foundCnx = true;
               serialPortInfo_ = serialPortInfo;
               break;
            }
            else
            {
               qWarning() << "Not a QuartTx device";
            }
         }
         else
            qDebug() << "Device not anwsering since 3 sec";

         qDebug() << "Close cnx: " << serialPortInfo.portName();
         closeSerialLink();
      }
      else
      {
         qWarning() << "Fail to open: " << serialPortInfo.portName();
      }
   }

   if(!foundCnx)
   {
      qWarning() << "No QuarkTx device found on serial link";
      return false;
   }

   isTxConnected_ = true;
   transportStatusStr = "Serial link";
   cnxStatusStr_ = serialPortInfo_.portName();

   output_.clear();

   return true;
#else
   return false;
#endif
}

bool RadioLink::searchForBlueToothLink()
{
   return false;
}

void RadioLink::init()
{
}

bool RadioLink::writeData(const QByteArray &data)
{
   if(serialPort_ == NULL)
   {
      qWarning() << "Cnx not established";
      return false;
   }

   input_ = data;
   if(serialPort_->write(data) != -1)
   {
      qDebug() << "< " << data;
      return true;
   }

   return false;
}

const QByteArray RadioLink::readData()
{
   QByteArray ret;

   if(serialPort_ == NULL)
   {
      qWarning() << "Cnx not established";
      return ret;
   }

   ret = serialPort_->readAll();
   output_ += ret;

   return ret;
}


// QML

bool RadioLink::findTxAndConnect()
{ 
   if(!searchForSerialLink())
   {
      if(!searchForBlueToothLink())
      {
         qWarning() << "No device found";
         return false;
      }
   }

   return true;
}

void RadioLink::txDisconnect()
{
   closeSerialLink();
   closeBlueToothLink();

   isTxConnected_ = false;
   transportStatusStr = "No transport";
   cnxStatusStr_ = "Not connected";
   txVersionStr_ = "NA";

   qDebug() << "Disconnect Tx";
}

QString RadioLink::getNextLine()
{
   QString ret;

   if(serialPort_ == NULL)
   {
      qWarning() << "Cnx not established";
      return ret;
   }

   while(1)
   {
      if(output_.isEmpty())
         serialPort_->waitForReadyRead(-1);

      char c = output_[0];
      output_.remove(0,1);

      if(c == '\r')
         continue;
      else if(c == '\n')
         break;

      ret.append(c);
   }

   return ret;
}

bool RadioLink::sendCommand(const QString &cmd)
{
   output_.clear();
   return writeData(cmd.toLatin1());
}

// Callbacks

void RadioLink::serialPortReadyRead()
{
   qDebug() << "> " << readData();
}

void RadioLink::serialPortHandleError(QSerialPort::SerialPortError error)
{
   if(serialPort_ == NULL)
   {
      qWarning() << "Cnx not established";
      return;
   }

   if(error != QSerialPort::NoError)
   {
      cnxStatusStr_ = serialPort_->errorString();
      transportStatusStr = "Error";
      qWarning() << "Cnx error: '" << error << "' " << cnxStatusStr_;
   }
}
