#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>

class MailSender
{
public:
    MailSender(QString host);
    void Send(QString rcv,QString snd,QString usr,QString pwd,QString subject,QString content);
    void communication(QString msg);
private:
    QTcpSocket tcpSocket;
};

#endif // MAILSENDER_H
