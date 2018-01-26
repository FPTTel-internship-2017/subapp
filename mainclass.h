#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QThread>
#include "receiver.h"
#include <QDir>
#include <QFile>
#include <QTextStream>

class MainClass : public QObject
{
    Q_OBJECT
    QThread *thread = new QThread();
    Receiver *receiver = new Receiver();
    bool isWin = false;
    bool isLinux = false;
    bool isMac = false;
    QString homePath = QDir::homePath();
    QString execPath;

    enum OperatingSytem {OS_WINDOWS, OS_UNIX, OS_LINUX, OS_MAC};

    #if (defined (Q_OS_WIN) || defined (Q_OS_WIN32) || defined (Q_OS_WIN64))
    OperatingSytem os = OS_WINDOWS;
    #elif (defined (Q_OS_LINUX))
    OperatingSytem os = OS_LINUX;
    #elif (defined (Q_OS_MAC))
    OperatingSytem os = OS_MAC;
    #endif
public:
    explicit MainClass(QObject *parent = nullptr);
    void runAnotherApp();
    void setStartup();
    void handelFileChange();
signals:

public slots:
    void onRegChanged(QString message);
};

#endif // MAINCLASS_H
