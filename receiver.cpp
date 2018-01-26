#include "receiver.h"
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QThread>

Receiver::Receiver(QObject *parent) : QObject(parent)
{

}

void Receiver::fileWatcher()
{

#if (defined (Q_OS_LINUX))
    path = homePath + "/.config/autostart/App1.desktop";
#elif (defined (Q_OS_MAC))
    path = homePath + "/Library/LaunchAgents/com.fpt.app1.plist";
    qDebug() << path <<"aaaaaaaaaaaa";
#endif
    watcher->addPath(path);
    QThread *fileWatcherThread = new QThread();
    watcher->moveToThread(fileWatcherThread);
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
    fileWatcherThread->start();
    qDebug() << __func__ << watcher->files() <<watcher->directories();
}

void Receiver::onRegChanged(QString message)
{
    qDebug() << message;
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(settings.value("App1").isNull())
    {
        settings.setValue("App1", QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
}

void Receiver::onFileChanged(QString name)
{
    qDebug() << name;
    if(!watcher->files().count())
    {
        qDebug() << "deleted";
#if (defined (Q_OS_LINUX))
        path = homePath + "/.config/autostart/App1.desktop";
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
        watcher->addPath(path);
#elif (defined (Q_OS_MAC))
        path = homePath + "/Library/LaunchAgents/com.fpt.app1.plist";
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
        watcher->addPath(path);
#endif

    }
}
