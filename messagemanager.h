#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QStringList>
#include <QDialog>

#define SELF_PORT 45454
#define TTL 5000

class MessageManager : public QDialog
{
    Q_OBJECT

public:
    MessageManager(QString addr,int port,QWidget *parent = 0);
    void Send(QString msg);
    QString Recive();
private slots:
    void ReadDataGram();
    void SendAgain();
signals:
    void MsgRecived(QStringList& msgs);
private:
    void SendACK(int ack_num);
private:
    QUdpSocket udpSocket;
    QString addr;
    int port;

    int serial;//ÏûÏ¢±àºÅ
    QTimer timer;

    QStringList msgCache;
};

#endif // MESSAGEMANAGER_H
