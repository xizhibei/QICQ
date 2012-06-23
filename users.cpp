#include "users.h"

Users::Users()
{
}

bool Users::Add(User user)
{
    if(!Exist(user))
    {
        data.append(user);
        return true;
    }
    return false;
}

bool Users::Exist(User user)
{
    int size = data.size();
    for( int i = 0;i < size;i++ )
    {
        if( data[i].mail == user.mail )
        {
            data[i].addr = user.addr;//update info
            data[i].hostname = user.hostname;
            data[i].name = user.name;
            data[i].port = user.port;
            return true;
        }
    }
    return false;
}

void Users::Save(QString filename)
{
    QFile file( filename );
    if ( file.open( QFile::WriteOnly ) ) {
        QTextStream stream( &file );

        int size = data.size();
        for( int i = 0;i < size;i++ )
        {
            stream << data[i].addr
                   << data[i].hostname
                   << data[i].name
                   << data[i].mail
                   << data[i].port
                   << "\n";
        }
        file.close();
    }

}

void Users::Load(QString filename)
{
    QFile file( filename );
    if ( file.open( QFile::ReadOnly ) ) {
        QTextStream stream( &file );

        User tmp;
        while( !stream.atEnd() )
        {
            stream >> tmp.addr
                   >> tmp.hostname
                   >> tmp.name
                   >> tmp.mail
                   >> tmp.port;

            data.append(tmp);
        }
        file.close();
    }
}
