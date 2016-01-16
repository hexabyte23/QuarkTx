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

         if(serialPort_->waitForReadyRead(3000))
         {
            qDebug() << "Anwsering... Check QuartTx device";

            QThread::sleep(2);

            if(output_.startsWith("Qu"))
            {
               qDebug() << "QuarkTx device detected";
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


#if 0
   QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();

   qDebug() << QObject::tr("Ports available: ") << serialPortInfoList.count() << endl;

   const QString blankString = QObject::tr("N/A");
   QString description;
   QString manufacturer;
   QString serialNumber;


   foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
   {
      description = serialPortInfo.description();
      manufacturer = serialPortInfo.manufacturer();
      serialNumber = serialPortInfo.serialNumber();
      qDebug() << endl
               << QObject::tr("Port: ") << serialPortInfo.portName() << endl
               << QObject::tr("Location: ") << serialPortInfo.systemLocation() << endl
               << QObject::tr("Description: ") << (!description.isEmpty() ? description : blankString) << endl
               << QObject::tr("Manufacturer: ") << (!manufacturer.isEmpty() ? manufacturer : blankString) << endl
               << QObject::tr("Serial number: ") << (!serialNumber.isEmpty() ? serialNumber : blankString) << endl
               << QObject::tr("Vendor Identifier: ") << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString) << endl
               << QObject::tr("Product Identifier: ") << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString) << endl
               << QObject::tr("Busy: ") << (serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) << endl;

      QSerialPort *port = new QSerialPort(serialPortInfo);
      if (port->open(QIODevice::ReadWrite))
      {
         qDebug() << "Baud rate:" << port->baudRate();
         qDebug() << "Data bits:" << port->dataBits();
         qDebug() << "Stop bits:" << port->stopBits();
         qDebug() << "Parity:" << port->parity();
         qDebug() << "Flow control:" << port->flowControl();
         qDebug() << "Read buffer size:" << port->readBufferSize();

         QByteArray out, in;

         out = "\nn\n";
         port->write(out);
         port->flush();
         port->waitForBytesWritten(1000);
         port->waitForReadyRead(10000);
         in = port->readAll();

         qDebug() << "-----------" << in;

         port->close();
      }
      else
      {
         qDebug() << "Unable to open port, error code" << port->error();
      }
      delete port;
   }
#endif

   return true;
}

bool RadioLink::searchForBlueToothLink()
{
   return false;
}

void RadioLink::init()
{
   searchForSerialLink();
   searchForBlueToothLink();
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

QString RadioLink::getNextLine() const
{
   QString ret;
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
   qDebug() << "data received: " << readData();
}

void RadioLink::serialPortHandleError(QSerialPort::SerialPortError error)
{
   if(error != QSerialPort::NoError)
      qWarning() << "cnx error: '" << error << "' " << serialPort_->errorString();
}
