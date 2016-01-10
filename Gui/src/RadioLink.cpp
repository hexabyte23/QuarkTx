#include "RadioLink.h"

RadioLink::RadioLink()
{
}

RadioLink::~RadioLink()
{
}

void RadioLink::setup()
{
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
}

QString RadioLink::getNextLine() const
{
    QString ret;
    return ret;
}

