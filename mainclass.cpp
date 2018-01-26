#include "mainclass.h"
#include "myprocess.h"
#include "registrymonitor.h"
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QFileSystemWatcher>

/**
 * @brief MainClass::MainClass
 * @param parent
 */
MainClass::MainClass(QObject *parent) : QObject(parent)
{
    switch(os) {
    case OS_WINDOWS:
        isWin = true;
        break;
    case OS_LINUX  :
        isLinux = true;
        break;
    case OS_MAC    :
        isMac = true;
        break;
    default        :
        qDebug() << "Unknown";
        return;
    }
    execPath = QCoreApplication::applicationFilePath();
    receiver->moveToThread(thread);
    thread->start();
    setStartup();
    runAnotherApp();
}

/**
 * Run another app by Qprocess
 * @brief MainClass::runAnotherApp
 */
void MainClass::runAnotherApp()
{
    MyProcess* myProcess = new MyProcess();
    myProcess->start();
}

/**
 * Set App 1 auto run when os startup
 * @brief MainClass::setStartup
 */
void MainClass::setStartup()
{
    if(isWin)
    {
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        settings.setValue("App1", execPath.replace('/', '\\'));
        RegistryMonitor *registerMonitor = new RegistryMonitor();
        connect(registerMonitor, SIGNAL(regChanged(QString)), receiver, SLOT(onRegChanged(QString)));
        registerMonitor->start();
        receiver->fileWatcher();
    }
    else if(isLinux)
    {
        QString content = "[Desktop Entry]\nType=Application\nExec=" + execPath + "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\nName[en_US]=App1\nName=App1\nComment[en_US]=run App1\nComment=run App1";
        QDir dir(homePath + "/.config/autostart");
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QString filename = homePath + "/.config/autostart/App1.desktop";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << content << endl;
        }
        receiver->fileWatcher();
    }
    else if (isMac)
    {
        qDebug() << "MAC";
        QString content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n<plist version=\"1.0\">\r\n<dict>\r\n    <key>Label</key>\r\n    <string>com.fpt.app1</string>\r\n    <key>ProgramArguments</key>\r\n    <array>\r\n        <string>" + execPath + "</string>\r\n     </array>\r\n    <key>KeepAlive</key>\r\n    <true/>\r\n    <key>RunAtLoad</key>\r\n        <true/>\r\n    <key>KeepAlive</key>\r\n        <true/>\r\n\r\n    <key>StandardErrorPath</key>\r\n        <string>/dev/null</string>\r\n    <key>StandardOutPath</key>\r\n        <string>/dev/null</string>\r\n</dict>\r\n</plist>";
        QDir dir(homePath + "/Library/LaunchAgents");
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QString filename = homePath + "/Library/LaunchAgents/com.fpt.app1.plist";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << content << endl;
        }
        QProcess p;
        p.start("launchctl load ~/Library/LaunchAgents/com.fpt.app1.plist");
        receiver->fileWatcher();
    }
}

