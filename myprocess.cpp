#include "myprocess.h"
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

MyProcess::MyProcess(QObject *parent) : QObject(parent)
{
    program = QCoreApplication::applicationDirPath() + "/fpt_sescureplayer/bin/java";
    args << "-jar" << QCoreApplication::applicationDirPath() + "/fpt_sescureplayer/bin/fpt-secure-player.jar";
    processApp1 = new QProcess(this);
    processApp1->setProcessChannelMode(QProcess::MergedChannels);
    processApp1->setProgram(program);
    processApp1->setArguments(args);
    connect(processApp1, SIGNAL(finished(int,QProcess::ExitStatus)),this, SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(processApp1, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
}

/**
 * SLOT is called when process exit, when it is called, it will restart process
 * @brief MyProcess::onFinished
 * @param code
 * @param status
 */
void MyProcess::onFinished(int code, QProcess::ExitStatus status)
{
    qDebug() << __func__ << code << status;
    start();
}

/**
 * SLOT is called when Process ready read
 * @brief MyProcess::onReadyRead
 */
void MyProcess::onReadyRead()
{
    qDebug() << processApp1->readAll();
}

/**
 * @brief MyProcess::start
 */
void MyProcess::start()
{
    processApp1->start();
    if (processApp1->waitForStarted()) {
        if (!processApp1->waitForReadyRead(5000)) {
            qDebug() << "waitForReadyRead() [false] : CODE: " << QVariant(processApp1->error()).toString() << " | ERROR STRING: " << processApp1->errorString();
        }
        qDebug() << processApp1->readAll();
        qDebug() << processApp1->processId();
        if(!processApp1->waitForFinished(-1)){
            qDebug() << "waitForFinished() [false] : CODE: " << QVariant(processApp1->error()).toString() << " | ERROR STRING: " << processApp1->errorString();
        }
    }
    else{
        qDebug() << "waitForStarted() [false] : CODE: " << QVariant(processApp1->error()).toString() << " | ERROR STRING: " << processApp1->errorString();
    }

}

QProcess *MyProcess::getProcessApp1() const
{
    return processApp1;
}

void MyProcess::setProcessApp1(QProcess *value)
{
    processApp1 = value;
}

QStringList MyProcess::getArgs() const
{
    return args;
}

void MyProcess::setArgs(const QStringList &value)
{
    args = value;
}

QString MyProcess::getProgram() const
{
    return program;
}

void MyProcess::setProgram(const QString &value)
{
    program = value;
}
