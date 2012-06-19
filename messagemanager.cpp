#include "messagemanager.h"

MessageManager::MessageManager(QString addr, int port,QWidget *parent)
    :QDialog(parent)
{
    this->port = port;
    this->addr = addr;

    udpSocket.bind(port);
    QObject::connect( &udpSocket, SIGNAL( readyRead() ),this,SLOT( ReadDataGram() ) );

    serial = 0;
    timer.setInterval(TTL);
    QObject::connect( &timer, SIGNAL( timeout() ),this,SLOT( SendAgain() ) );
}

void MessageManager::Send(QString msg)
{
    QString tmp;
    msg += QString( "SERIAL" ) + tmp.sprintf("%3d", msgCache.size());
    QByteArray data = msg.toUtf8();
    udpSocket.writeDatagram( data.data(),data.size(),QHostAddress( addr ),port );

    msgCache.append(msg);
    timer.stop();
    timer.start();
}

void MessageManager::SendAgain()
{
    int size = msgCache.size();
    for(int i = serial;i < size;i++)
    {
        QByteArray data = msgCache[i].toUtf8();
        udpSocket.writeDatagram( data.data(),data.size(),QHostAddress( addr ),port );
    }
    timer.stop();
    timer.start();
}

void MessageManager::ReadDataGram()
{
    int send_ack_num = -1;
    QStringList msgs;
    bool msg_recived = false;
    while( udpSocket.hasPendingDatagrams() )
    {
        QByteArray data;
        data.resize( udpSocket.pendingDatagramSize() );
        udpSocket.readDatagram( data.data(),data.size() );
        if( data.startsWith( "ACK" ) )
        {
            data.remove(0,3);
            int ack = data.toInt();
            if( ack + 1 == msgCache.size() )
            {
                msgCache.clear();
                serial = 0;
            }else
                serial = ack;
        }
        else if( data.startsWith( "SERIAL" ) )
        {
            data.remove(0,6);
            QString tmp = data[0] + data[1] + data[2];
            send_ack_num = tmp.toInt();
            data.remove(0,3);

            msgs.append(data);
            msg_recived = true;
        }
    }
    if( send_ack_num != -1 )
        SendACK(send_ack_num);
    if( msg_recived )
        emit MsgRecived(msgs);
}

void MessageManager::SendACK(int ack_num)
{
    QByteArray data = (QString("ACK") + QString::number(ack_num)).toUtf8();
    udpSocket.writeDatagram( data.data(),data.size(),QHostAddress( addr ),port );
}
