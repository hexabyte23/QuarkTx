#ifndef TX_COMMAND_H
#define TX_COMMAND_H

#include <QQmlContext>
#include <QVariantMap>
#include "RadioLink.h"


#define TX_CMD_LOAD_FROM_EEPROM     "a\r"
#define TX_CMD_TOGGLE_CALIBRATE     "c\r"
#define TX_CMD_DUMP_ALL             "d\r"
#define TX_CMD_DUMP_MODEL           "d m\r"
#define TX_CMD_DUMP_EEPROM          "d e\r"
#define TX_CMD_DUMP_SENSOR          "d s\r"
#define TX_CMD_DUMP_RCL             "d l\r"
#define TX_CMD_FREE_MEMORY          "f\r"
#define TX_CMD_HELP                 "h\r"

class TxCommand : public QObject
{
   Q_OBJECT

   RadioLink *radioLinkRef_;
   QStringList sensorCache_;
   bool refreshSensorCache_;

   void fillSensorCache();

public:
   TxCommand();
   virtual ~TxCommand();

   void init(RadioLink *radioLinkRef);
   bool sendCommand(const QString &cmd);
   QString getNextLine();

   // QML
   Q_INVOKABLE QString getFreeMemoryStr();

   Q_INVOKABLE QVariantMap getSensorData(int sensorID);
   Q_INVOKABLE void setSensorData(int sensorID, QVariantMap data);

};

#endif // TX_COMMAND_H
