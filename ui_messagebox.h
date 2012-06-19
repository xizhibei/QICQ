/********************************************************************************
** Form generated from reading UI file 'messagebox.ui'
**
** Created: Tue Jun 19 21:32:23 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOX_H
#define UI_MESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageBox
{
public:
    QTextEdit *msgtextEdit;
    QListView *listView;
    QPushButton *sendpushButton;

    void setupUi(QWidget *MessageBox)
    {
        if (MessageBox->objectName().isEmpty())
            MessageBox->setObjectName(QString::fromUtf8("MessageBox"));
        MessageBox->resize(503, 497);
        msgtextEdit = new QTextEdit(MessageBox);
        msgtextEdit->setObjectName(QString::fromUtf8("msgtextEdit"));
        msgtextEdit->setGeometry(QRect(30, 330, 441, 111));
        listView = new QListView(MessageBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(30, 20, 441, 291));
        sendpushButton = new QPushButton(MessageBox);
        sendpushButton->setObjectName(QString::fromUtf8("sendpushButton"));
        sendpushButton->setGeometry(QRect(380, 460, 75, 23));

        retranslateUi(MessageBox);

        QMetaObject::connectSlotsByName(MessageBox);
    } // setupUi

    void retranslateUi(QWidget *MessageBox)
    {
        MessageBox->setWindowTitle(QApplication::translate("MessageBox", "Form", 0, QApplication::UnicodeUTF8));
        sendpushButton->setText(QApplication::translate("MessageBox", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MessageBox: public Ui_MessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOX_H
