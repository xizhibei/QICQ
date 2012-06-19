#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect( ui->buttonBox,SIGNAL( accepted() ),this,SLOT( sendValue() ) );
}

Login::~Login()
{
    delete ui;
}

void Login::sendValue()
{
    if(ui->namelineEdit->text() == ""
            ||ui->emaillineEdit->text() == "")
    {
        ui->msglabel->setText(tr("<span style=color:#ff0000;>²»ÄÜÎª¿Õ</span>"));
        this->show();
        return;
    }
    emit dialogResponse( ui->namelineEdit->text(),ui->emaillineEdit->text() );
}
