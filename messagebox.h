#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QStringList>
#include <QWidget>
#include <QStandardItemModel>
#include <QKeySequence>
#include <QMessageBox>
#include <QHostInfo>

#include "users.h"
#include "mailsender.h"

#define TALK_PORT 45454
#define TTL 3000

namespace Ui {
class MessageBox;
}

class MessageBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageBox(QWidget *parent = 0);
    ~MessageBox();
    void closeEvent(QCloseEvent *e);
    void Send(QString msg);
    void TalkToMe();

    void Init(User self,User frd);
    int BindPort();
    void UpdateFrdPort(int port);
private slots:
    void ReadDataGram();
    void SendAgain();
    void SendMsg();
    void ReciveMsg(QStringList& msgs);
    void SendMail(QHostInfo hostaddr);

signals:
    void MsgRecived(QStringList& msgs);
    void MsgBoxClosed(int mid);
private:
    void SendACK(int ack_num);
private:
    QUdpSocket udpSocket;
    User self;
    User frd;
//    QString addr;
//    int port;
//    QString username;
//    QString friendname;

    int serial;//ÏûÏ¢±àºÅ
    QTimer timer;
    int resend_times;

    QStringList msgCache;

    Ui::MessageBox *ui;

    QStandardItemModel* msgStandardItemModel;

    QHostInfo* hostinfo;
};

#endif // MESSAGEBOX_H
