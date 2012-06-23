#include "mailsender.h"

MailSender::MailSender(QString host)
{
    //QHostInfo::lookupHost( host,this,SLOT( ConnectToHost(QHostInfo) ) );
    tcpSocket.connectToHost( host ,25);
    if( !tcpSocket.waitForConnected(-1) )
        qDebug() << tcpSocket.errorString();
}

void MailSender::Send(QString rcv, QString snd, QString usr, QString pwd, QString subject, QString content)
{
    communication( "HELO world\r\n" );
    communication( "AUTH LOGIN\r\n" );
    communication( QString( "%1\r\n" ).arg( QString( usr.toUtf8().toBase64() ) ) );
    communication( QString( "%1\r\n").arg( QString( pwd.toUtf8().toBase64() ) ) );
    communication( QString( "MAIL FROM: <%1>\r\n" ).arg( snd ) );
    communication( QString( "RCPT TO: <%1>\r\n" ).arg( rcv ) );
    communication( "DATA\r\n" );
    communication( QString("From: %1\r\nTo: %2\r\nSubject: %3\r\n\r\n%4\r\n.\r\n").arg( snd,rcv,subject,content ) );
    communication( "quit\r\n" );

    qDebug() << "send email ok.";
}

void MailSender::communication(QString msg)
{
    qDebug() << msg;

    tcpSocket.write(msg.toUtf8());
    tcpSocket.flush();

    char data[1024];
    if (tcpSocket.waitForReadyRead(-1) == true)
    {
        memset(data, '\0', sizeof(data));
        tcpSocket.readLine(data, 1024);
        qDebug() << data;
    }
}
