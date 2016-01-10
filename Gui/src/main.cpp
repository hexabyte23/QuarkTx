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


#include "src/main.h"
#include "src/global.h"
#include "BatteryModel.h"
#include "MainModel.h"


Q_LOGGING_CATEGORY(QT_QUARKTX, "qt.quarktx.main")
static MainModel g_model;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char* time = QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str();

    switch (type)
    {
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

