#include "messagebox.h"
#include "ui_messagebox.h"

MessageBox::MessageBox( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);

}

void MessageBox::closeEvent(QCloseEvent *e)
{
    emit MsgBoxClosed( frd.mid );
}

int MessageBox::BindPort()
{
    int port = TALK_PORT;
    while( !udpSocket.bind( port ) )
        port++;
    qDebug() << "Bind talk port :" << port;
    return port;
}

void MessageBox::UpdateFrdPort(int port)
{
    frd.port = port;
}

void MessageBox::Init(User self, User frd)
{
    this->self = self;
    this->frd = frd;

    connect( &udpSocket, SIGNAL( readyRead() ),this,SLOT( ReadDataGram() ) );

    serial = 0;
    resend_times = 0;
    timer.setInterval(TTL);
    connect( &timer, SIGNAL( timeout() ),this,SLOT( SendAgain() ) );

    connect( ui->sendpushButton,SIGNAL( clicked() ),this,SLOT( SendMsg() ) );

    ui->sendpushButton->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Enter) );

    connect( this,SIGNAL(MsgRecived(QStringList&)),this,SLOT( ReciveMsg(QStringList&) ) );

    msgStandardItemModel = new QStandardItemModel(this);
    ui->listView->setModel( msgStandardItemModel );

    ui->frdlabel->setText( "You are talking to " + frd.name + " <" + frd.mail + ">"  );

    hostinfo = new QHostInfo();
}

MessageBox::~MessageBox()
{
    timer.stop();
    delete hostinfo;
    delete msgStandardItemModel;
    delete ui;
}

void MessageBox::Send(QString msg)
{
    QString tmp;
    MsgBody mb(msg,serial++);

    msg = tmp.sprintf("SERIAL%d\r\n", mb.serial) + mb.msg;
    QByteArray data = msg.toUtf8();
    udpSocket.writeDatagram( data.data(),data.size(),QHostAddress( frd.addr ),frd.port );

    qDebug() << "Send msg :" << msg;
    msgCache.append(mb);
    timer.stop();
    timer.start();
}

void MessageBox::SendMail(QHostInfo hostaddr)
{
    qDebug() << "DNS lookup finish: ";
    QList<QHostAddress> adds = hostaddr.addresses();
    if( adds.empty() )
        qDebug() << hostaddr.errorString();
    else
        for(int i = 0;i < adds.size();i++)
            qDebug() << adds[ i ].toString();

    MailSender ms( adds[ 0 ].toString() );
    QString content = "You have unreaded message from " + self.name + "\r\n";

    for(int i = 0;i < msgCache.size();i++)
        content += msgCache[i].msg + "\r\n";

    ms.Send(frd.mail,"hitjisuanji09@163.com","hitjisuanji09","123456","You have unreaded message!",content);

    msgCache.clear();
    timer.stop();
    resend_times = 0;
}

void MessageBox::SendAgain()
{
    int size = msgCache.size();
    for(int i = 0;i < size;i++)
    {
        QString tmp;
        QByteArray data = ( tmp.sprintf("SERIAL%d\r\n", msgCache[i].serial) + msgCache[i].msg ).toUtf8();
        udpSocket.writeDatagram( data.data(),data.size(),QHostAddress( frd.addr ),frd.port );
        qDebug() << "Send msg again:" << msgCache[i].msg;

        resend_times++;
    }
    if( resend_times > 5 )
    {
        qDebug() << "Seem that user offline";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "SendEmail?",
                                      "Wann to send an email to him/her?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            hostinfo->lookupHost("smtp.163.com",this,SLOT( SendMail(QHostInfo)) );
        }
        else if (reply == QMessageBox::No)
        {
        }
    }
    timer.stop();
    timer.start();
}

void MessageBox::ReadDataGram()
{
    int send_ack_num = -1;
    QStringList msgs;
    bool msg_recived = false;

    while( udpSocket.hasPendingDatagrams() )
    {
        QByteArray data;
        data.resize( udpSocket.pendingDatagramSize() );
        udpSocket.readDatagram( data.data(),data.size() );

        qDebug() << "Msgbox Recive data: " << data;

        if( data.startsWith( "ACK" ) )
        {
            data.remove(0,3);
            int ack = data.toInt();
            qDebug() << "Get ack: " << ack;
            if( ack == serial )
            {
                msgCache.clear();
                resend_times = 0;
                timer.stop();
            }
            else if( ack < serial )
            {
                for(int m = 0;m < msgCache.size();m++)
                {
                    if( msgCache[ m ].serial <= ack )
                    {
                        msgCache.removeAt( m );
                        m--;
                    }
                }
            }
        }
        else if( data.startsWith( "SERIAL" ) )
        {
            data.remove(0,6);
            int dsize = data.size();
            int d;
            QString tmp;
            for(d = 0;d < dsize && data[ d ] != '\r' ;d++)
            {
                tmp += data[ d ];
            }
//            QString tmp;
//            if(data[0] != ' ')
//                tmp += data[0];
//            if(data[1] != ' ')
//                tmp += data[1];
//            if(data[2] != ' ')
//                tmp += data[2];

            send_ack_num = tmp.toInt();

            qDebug() << "Get serial: " << send_ack_num;

            if( ack_msg_num >= send_ack_num )//收到重复数据
            {
                qDebug() << "Same msg again!";
                continue;
            }

            ack_msg_num = send_ack_num;//update ack_msg_num

            data.remove( 0,d + 2 );// remove \r\n

            msgs.append( data );
            msg_recived = true;
        }
        else if( data.startsWith( "Ok to talk" ) )
        {
            qDebug() << "Ok to talk";
            //frd.port = _port;
            this->show();
        }
        else if( data.startsWith( "Refuse to talk" ) )
        {
            qDebug() << "Refuse to talk";
            this->close();
        }
    }
    if( send_ack_num != -1 )
        SendACK( send_ack_num );

    if( msg_recived )
        emit MsgRecived( msgs );
}

void MessageBox::SendACK(int ack_num)
{
    QByteArray data = ( QString("ACK") + QString::number(ack_num) ).toUtf8();
    udpSocket.writeDatagram( data.data(),data.size(),QHostAddress( frd.addr ),frd.port );
}

void MessageBox::SendMsg()
{
    if( ui->msgtextEdit->toPlainText() != "")
    {
        Send( ui->msgtextEdit->toPlainText() );

        QStandardItem* item = new QStandardItem( self.name + " " + QDateTime::currentDateTime().toString("hh:mm:ss"));

        QLinearGradient linearGrad(QPointF(0,0),QPointF(200,300));
        linearGrad.setColorAt(0,Qt::darkGreen);
        linearGrad.setColorAt(1,Qt::yellow);
        QBrush brush(linearGrad);
        item->setBackground(brush);

        item->setEditable(false);
        msgStandardItemModel->appendRow(item);

        item = new QStandardItem( ui->msgtextEdit->toPlainText() );

        item->setEditable(false);
        msgStandardItemModel->appendRow(item);

        ui->msgtextEdit->setText("");
    }
}

void MessageBox::ReciveMsg(QStringList &msgs)
{
    int size = msgs.size();

    for(int i = 0;i < size;i++)
    {
        QStandardItem* item = new QStandardItem( frd.name + " " + QDateTime::currentDateTime().toString("hh:mm:ss") );

        QLinearGradient linearGrad(QPointF(0,0),QPointF(200,300));
        linearGrad.setColorAt( 0,Qt::white );
        linearGrad.setColorAt( 1,Qt::yellow );
        QBrush brush(linearGrad);
        item->setBackground(brush);

        msgStandardItemModel->appendRow(item);

        item = new QStandardItem( msgs[i] );

        msgStandardItemModel->appendRow(item);
    }
}
