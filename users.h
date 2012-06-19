#ifndef USERS_H
#define USERS_H

#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>

struct User
{
    User()
    {
        mid = -1;
    }

    int mid;
    int port;
    QString addr;
    QString hostname;
    QString name;
    QString mail;
};

class Users
{
public:
    Users();
    bool Add(User user);
    bool Exist(User user);
    void Save(QString filename);
    void Load(QString filename);
public:
    QList<User> data;
};

#endif // USERS_H
