#ifndef QICQSERVER_H
#define QICQSERVER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QDialog>

#include "users.h"

#define SEARCH_PORT 55555

class QICQServer : public QDialog
{
    Q_OBJECT

public:
    QICQServer(QString name,QString email,QWidget *parent = 0);
    QString GetDestAddr();
    int GetDestPort();
    void LANSearch();
    void InfoFriend(User frd);
    void TalkToMe(User frd,int newport);
    void OkToTalk(User frd);
    void RefuseToTalk(User frd);

private slots:
    void ReadDataGram();

signals:
    void usersUpdated(QList<User>& users);
    void AskToTalk(User frd);

private:
    void search();
    QUdpSocket udpSocket;

    QString name;
    QString email;
};

#endif // QICQSERVER_H
