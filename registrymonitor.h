#ifndef REGISTRYMONITOR_H
#define REGISTRYMONITOR_H

#include <QObject>
#include <QThread>
class RegistryMonitor : public QThread
{
    Q_OBJECT

public:
    RegistryMonitor();
signals:
    void regChanged(QString);
protected:
    void run();
    int exec();
public slots:
private:
};

#endif // REGISTRYMONITOR_H
