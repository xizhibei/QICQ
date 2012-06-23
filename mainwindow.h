#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QList>
#include <QStandardItemModel>

#include "login.h"
#include "qicqserver.h"
#include "users.h"
#include "messagebox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void beginMain(QString name,QString email);
    void ShowUsers(QList<User>& users);

    void LANSearch();
    void UserListClicked(QModelIndex idx);

    void ShowAsks(User frd);

    void DeleteMsgBox(int mid);
    
private:
    Ui::MainWindow *ui;
    Login login;
    QICQServer* server;

    Users users;
    QString myname;
    QString mymail;
    QList<MessageBox*> mbs;

    QStandardItemModel* usersStandardItemModel;
};

#endif // MAINWINDOW_H
