#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

class Receiver : public QObject
{
    Q_OBJECT
    QFileSystemWatcher* watcher = new QFileSystemWatcher();
    QString path;
    QString homePath = QDir::homePath();
    QString execPath = QCoreApplication::applicationFilePath();
public:
    explicit Receiver(QObject *parent = nullptr);
    void fileWatcher();
signals:

public slots:
    void onRegChanged(QString message);
    void onFileChanged(QString name);
};

#endif // RECEIVER_H
