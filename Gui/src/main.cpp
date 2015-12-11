#include "src/main.h"
#include "src/global.h"
#include "batterymodel.h"
#include "mainmodel.h"
#include <QSerialPortInfo>

Q_LOGGING_CATEGORY(QT_QUARKTX, "qt.quarktx.main")
static MainModel g_model;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char* time = QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str();
    switch (type) {
    case QtInfoMsg:
        fprintf(stderr, "%s [I] %s (%s:%u, %s)\n", time, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtDebugMsg:
        fprintf(stderr, "%s [D] %s (%s:%u, %s)\n", time, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s [W] %s (%s:%u, %s)\n", time, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s [C] %s (%s:%u, %s)\n", time, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s [F] %s (%s:%u, %s)\n", time, localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

void Init(const QApplication &app)
{
    qInstallMessageHandler(myMessageOutput);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.osrc.main=true"));

    //app.setFont(QFont("courrier"));
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    qCInfo(QT_QUARKTX) << "Starting " APP_NAME << APP_VERSION;


    QTextStream out(stdout);
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();

    out << QObject::tr("Total number of ports available: ") << serialPortInfoList.count() << endl;

    const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;

    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        out << endl
            << QObject::tr("Port: ") << serialPortInfo.portName() << endl
            << QObject::tr("Location: ") << serialPortInfo.systemLocation() << endl
            << QObject::tr("Description: ") << (!description.isEmpty() ? description : blankString) << endl
            << QObject::tr("Manufacturer: ") << (!manufacturer.isEmpty() ? manufacturer : blankString) << endl
            << QObject::tr("Serial number: ") << (!serialNumber.isEmpty() ? serialNumber : blankString) << endl
            << QObject::tr("Vendor Identifier: ") << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString) << endl
            << QObject::tr("Product Identifier: ") << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString) << endl
            << QObject::tr("Busy: ") << (serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) << endl;
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Init(app);

    QTranslator qtTranslator;
    QString filename = "QuarkTx_" + QLocale::system().name() + ".qm";
    QString path = ":/";

    // For updating OSRC_xx_XX.qm use menu
    // Tools/Extern/linguist/update & lrelease

    if(qtTranslator.load(filename, path) == false)
    {
        QString msg = "'" + path + filename + "' not found";
        qCDebug(QT_QUARKTX) << msg;
    }
    else
        app.installTranslator(&qtTranslator);

    QQmlApplicationEngine engine;
    g_model.Init(engine.rootContext());
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    int ret = app.exec();

    qCInfo(QT_QUARKTX) << "Ending " APP_NAME << APP_VERSION;

    return ret;
}

