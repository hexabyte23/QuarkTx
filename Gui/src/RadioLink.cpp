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
   foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
   {
      serialPort_ = new QSerialPort(serialPortInfo, this);
      connect(serialPort_, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
      connect(serialPort_, SIGNAL(error(QSerialPort::SerialPortError)), this,
              SLOT(serialPortHandleError(QSerialPort::SerialPortError)));

      if(serialPort_->open(QIODevice::ReadWrite))
      {
         qDebug() << "Open cnx: " << serialPortInfo.portName() << " waiting for anwser...";

         if(serialPort_->waitForReadyRead(2000))
         {
            //qDebug() << "Anwsering... Check QuartTx device";

            //QThread::sleep(2);

            QString line = getNextLine();
            if(line.startsWith("Quark Tx v"))
            {
               txVersionStr_ = line.mid(sizeof("Quark Tx v")-1);
               qDebug() << "Quark Tx device detected " << txVersionStr_;
               foundCnx = true;
               serialPortInfo_ = serialPortInfo;
               break;
            }
            else
            {
               qWarning() << "this cnx is no a QuartTx device";
            }
         }
         else
            qDebug() << "Cnx is not anwsering since 3 sec";

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
      qWarning() << "No Quart Tx device found";
      return false;
   }

   isTxConnected_ = true;
   transportStatusStr = "serial";
   cnxStatusStr_ = serialPortInfo_.portName();

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

void RadioLink::writeData(const QByteArray &data)
{
   if(serialPort_ == NULL)
   {
      qWarning() << "Cnx with Quark Tx device not established";
      return;
   }

   input_ = data;
   serialPort_->write(data);
}

const QByteArray RadioLink::readData()
{
   QByteArray ret;

   if(serialPort_ == NULL)
   {
      qWarning() << "Cnx with Quark Tx device not established";
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

bool RadioLink::txDisconnect()
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
      qWarning() << "Cnx with Quark Tx device not established";
      return ret;
   }

   int i = 0;
   while(output_[i] != '\n')
   {
      char c = output_[i];
      if(c == '\r')
         continue;

      ret.append(c);
      output_.remove(0,1);

      if(output_.isEmpty())
         serialPort_->waitForReadyRead(-1);
   }

   return ret;
}

bool RadioLink::sendCommand(const QString &cmd)
{
   writeData(cmd.toLatin1());
   return true;
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
      qWarning() << "Cnx with Quark Tx device not established";
      return;
   }

   if(error != QSerialPort::NoError)
      qWarning() << "Cnx error: '" << error << "' " << serialPort_->errorString();
}
