#include "qicqserver.h"

QICQServer::QICQServer(QString name,QString email,QWidget *parent )
    :QDialog( parent )
{
    if( !udpSocket.bind( SEARCH_PORT ) )
        qDebug() << "PORT bind error!!!";
    connect( &udpSocket,SIGNAL( readyRead() ),this,SLOT( ReadDataGram() ) );

    this->name = name;
    this->email = email;

    LANSearch();
}

void QICQServer::LANSearch()
{
    QByteArray data = QString( "xizhibeiqicq\r\nHOST%1\r\nNAME%2\r\nMAIL%3\r\n" )
            .arg( QHostInfo::localHostName(),name,email ).toUtf8();

    udpSocket.writeDatagram( data,QHostAddress::Broadcast,SEARCH_PORT );
}

void QICQServer::TalkToMe(User frd, int newport)
{
    QByteArray data = QString( "xizhibeiqicq\r\nHOST%1\r\nNAME%2\r\nMAIL%3\r\nPORT%4\r\nTKTM" )
            .arg( QHostInfo::localHostName())
            .arg( name )
            .arg( email )
            .arg( newport )
            .toUtf8();

    udpSocket.writeDatagram( data,QHostAddress( frd.addr ),SEARCH_PORT );
}

void QICQServer::OkToTalk(User frd)
{
    QByteArray data = QString( "Ok to talk" ).toUtf8();

    udpSocket.writeDatagram( data,QHostAddress( frd.addr ),frd.port );
}

void QICQServer::RefuseToTalk(User frd)
{
    QByteArray data = QString( "Refuse to talk" ).toUtf8();

    udpSocket.writeDatagram( data,QHostAddress( frd.addr ),frd.port );
}

void QICQServer::ReadDataGram()
{
    bool users_update = false;
    QHostAddress host;
    quint16 port;
    QByteArray data;
    QList<User> users;
    while( udpSocket.hasPendingDatagrams() )
    {
        data.resize( udpSocket.pendingDatagramSize() );
        udpSocket.readDatagram( data.data(),data.size(),&host,&port );

        qDebug() << "Server get data: "<< data;

        User tmp;

        if( data.startsWith("xizhibeiqicq\r\n") )
        {
            data.remove(0,14);

            tmp.addr = host.toString();
            tmp.port = port;

            int size = data.size();
            for(int i = 0;i < size;i++)
            {
                if( data[i] == 'H' && data[i + 1] == 'O' && data[i + 2] == 'S' && data[i + 3] == 'T' )
                {
                    i += 4;
                    while( i < size && data[i] != '\r' && data[i + 1] != '\n' )
                    {
                        tmp.hostname += data[i];
                        i++;
                    }
                    //i += 2;
                }
                else if( data[i] == 'N' && data[i + 1] == 'A' && data[i + 2] == 'M' && data[i + 3] == 'E' )
                {
                    i += 4;
                    while( i < size && data[i] != '\r' && data[i + 1] != '\n' )
                    {
                        tmp.name += data[i];
                        i++;
                    }
                    //i += 2;
                }
                else if( data[i] == 'M' && data[i + 1] == 'A' && data[i + 2] == 'I' && data[i + 3] == 'L' )
                {
                    i += 4;
                    while( i < size && data[i] != '\r' && data[i + 1] != '\n' )
                    {
                        tmp.mail += data[i];
                        i++;
                    }
                    //i += 2;
                }
                else if( data[i] == 'P' && data[i + 1] == 'O' && data[i + 2] == 'R' && data[i + 3] == 'T' )
                {
                    i += 4;
                    QString tmp_port = "";
                    while( i < size && data[i] != '\r' && data[i + 1] != '\n' )
                    {
                        tmp_port += data[i];
                        i++;
                    }
                    tmp.port = tmp_port.toInt();
                    qDebug() << "New port: " << tmp.port;
                    //i += 2;
                }
                else if( data[i] == 'T' && data[i + 1] == 'K' && data[i + 2] == 'T' && data[i + 3] == 'M' )
                {
                    emit AskToTalk(tmp);
                }
            }

            //            size = users.size();
            //            bool exist = false;
            //            for( int i = 0;i < size;i++ )
            //            {
            //                if( tmp.mail == users[i].mail )
            //                    exist = true;
            //            }
            //            if(!exist)
            //            {

            users.append(tmp);
            users_update = true;
            //            }
        }
    }


    if( users_update )
    {
        qDebug() << "User update!";
        emit usersUpdated(users);
    }
}
//tell friend my infomation
void QICQServer::InfoFriend(User frd)
{
    QByteArray data = QString( "xizhibeiqicq\r\nHOST%1\r\nNAME%2\r\nMAIL%3\r\n" )
            .arg( QHostInfo::localHostName(),name,email).toUtf8();

    udpSocket.writeDatagram( data,QHostAddress( frd.addr ),SEARCH_PORT );
}
