#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( &login,SIGNAL( dialogResponse( QString,QString ) ),this,SLOT( beginMain( QString,QString ) ) );
    connect( &login,SIGNAL( rejected() ),this,SLOT( close() ) );

    connect( ui->searchpushButton,SIGNAL( clicked() ),this,SLOT( LANSearch()) );
    connect( ui->listView,SIGNAL( doubleClicked( QModelIndex ) ),this,SLOT( UserListClicked( QModelIndex ) ) );
    //connect( ui->listView,SIGNAL( clicked( QModelIndex ) ),this,SLOT( UserListClicked( QModelIndex ) ) );


    server = NULL;

    login.show();
    usersStandardItemModel = new QStandardItemModel(this);
}

MainWindow::~MainWindow()
{
    if(!server)
        delete server;

    int size = mbs.size();
    for(int i = 0;i < size;i++)
        delete mbs[i];

    delete usersStandardItemModel;
    delete ui;
}

void MainWindow::beginMain(QString name, QString email)
{
    this->show();
    qDebug() << name << email;
    myname = name;
    mymail = email;
    server = new QICQServer(name,email);

    connect( server,SIGNAL( usersUpdated(QList<User>&) ),this,SLOT( ShowUsers(QList<User>&) ) );
    connect( server,SIGNAL( AskToTalk(User) ),this,SLOT( ShowAsks(User) ) );
}

void MainWindow::ShowAsks(User frd)
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Someone want to talk to you",
                                  frd.name + " want to talk to you, agree?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        server->OkToTalk(frd);

        MessageBox* mb = new MessageBox();

        int port = mb->BindPort();
        users.data[ 0 ].port = port;
        frd.mid = mbs.size();

        users.Add(frd);//update

        mb->Init(users.data[ 0 ],frd);
        mbs.append(mb);
        mb->show();
    }
    else if (reply == QMessageBox::No)
    {
        server->RefuseToTalk(frd);
    }
}

void MainWindow::ShowUsers(QList<User> &users)
{
    int size = users.size();

    for(int i = 0;i < size;i++)
    {
        qDebug() << users[i].name
                 << users[i].addr
                 << users[i].hostname
                 << users[i].mail
                 << users[i].port;
        if( this->users.Add(users[i]) && mymail != users[i].mail )//user not in list, we should send our info to him
        {
            qDebug() << "Info friend!!!";
            server->InfoFriend( users[i] );
        }
    }

    usersStandardItemModel->clear();

    size = this->users.data.size();

    for(int i = 0;i < size;i++)
    {
        QStandardItem* item = new QStandardItem(this->users.data[i].name);
        item->setEditable(false);
        //        if( i % 2 == 1 )
        //        {
        //            QLinearGradient   linearGrad(QPointF(0,0),QPointF(200,300));
        //            linearGrad.setColorAt(0,Qt::darkGreen);
        //            linearGrad.setColorAt(1,Qt::yellow);
        //            QBrush brush(linearGrad);
        //            item->setBackground(brush);
        //        }

        usersStandardItemModel->appendRow(item);
    }
    ui->listView->setModel(usersStandardItemModel);
}

void MainWindow::LANSearch()
{
    server->LANSearch();
}

void MainWindow::UserListClicked(QModelIndex idx)
{
    //qDebug() << idx.data().toString();
    //qDebug() << idx.row();
    if( idx.row() == 0)
        return;

    MessageBox* mb = new MessageBox();
    qDebug() << "Start talking to " << idx.data().toString();

    int port = mb->BindPort();

    users.data[ idx.row() ].port = port;
    users.data[ idx.row() ].mid = mbs.size();

    mb->Init( users.data[ 0 ],users.data[ idx.row() ] );

    mbs.append(mb);

    qDebug() << "Send request to ask for talking";
    server->TalkToMe( users.data[ idx.row() ],port );
}
