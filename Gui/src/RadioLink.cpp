#include "RadioLink.h"
#include <QThread>

RadioLink::RadioLink()
   : serialPort_(NULL), serialPortInfo_(NULL)
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
      delete serialPort_;
      serialPort_ = NULL;
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
               txVersion_ = line.mid(sizeof("Quark Tx v")-1);
               qDebug() << "QuarkTx device detected " << txVersion_;
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
         serialPort_->close();
         disconnect(serialPort_, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead()));
         disconnect(serialPort_, SIGNAL(error(QSerialPort::SerialPortError)), this,
                    SLOT(serialPortHandleError(QSerialPort::SerialPortError)));

         delete serialPort_;
         serialPort_ = NULL;
      }
      else
      {
         qWarning() << "Fail to open: " << serialPortInfo.portName();
      }
   }

   if(!foundCnx)
   {
      qWarning() << "No QuartTx device found";
      return false;
   }
#endif


   QString l2 = getNextLine();
   QString l3 = getNextLine();


   return true;
}

bool RadioLink::searchForBlueToothLink()
{
   return false;
}

void RadioLink::init()
{
   if(!searchForSerialLink())
   {
      if(!searchForBlueToothLink())
      {
         qWarning() << "No device found";
      }
   }
}

void RadioLink::writeData(const QByteArray &data)
{
   input_ = data;
   serialPort_->write(data);
}

const QByteArray RadioLink::readData()
{
   QByteArray recNow = serialPort_->readAll();
   output_ += recNow;
   return recNow;
}


// QML

QString RadioLink::getNextLine()
{
   QString ret;

   int i = 0;
   while(output_[i] != '\n')
   {
      ret.append((char)output_[i]);
      output_.remove(0,1);

      if(output_.isEmpty())
         serialPort_->waitForReadyRead(-1);
   }

   return ret;
}

bool RadioLink::sendCommand(const QString &cmd)
{
   QByteArray cmdData;
   cmdData.fromStdString(cmd.toStdString());
   writeData(cmdData);
   return true;
}

void RadioLink::serialPortReadyRead()
{
   qDebug() << "> " << readData();
}

void RadioLink::serialPortHandleError(QSerialPort::SerialPortError error)
{
   if(error != QSerialPort::NoError)
      qWarning() << "Cnx error: '" << error << "' " << serialPort_->errorString();
}
