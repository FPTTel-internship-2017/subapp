#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QObject>
#include <QProcess>
class MyProcess : public QObject
{
    Q_OBJECT
public:
    explicit MyProcess(QObject *parent = nullptr);

    QString getProgram() const;
    void setProgram(const QString &value);

    QStringList getArgs() const;
    void setArgs(const QStringList &value);

    QProcess *getProcessApp1() const;
    void setProcessApp1(QProcess *value);

    void start();

signals:

public slots:
    void onFinished(int code, QProcess::ExitStatus status);

    void onReadyRead();

private:
    QString program;
    QStringList args;
    QProcess *processApp1;
};

#endif // MYPROCESS_H
