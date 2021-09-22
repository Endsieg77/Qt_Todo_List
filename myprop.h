#ifndef MYPROP_H
#define MYPROP_H

#include <QObject>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(ListDisplay)
QT_FORWARD_DECLARE_CLASS(MyTableItem)

struct myProp
{
public:
    myProp(const QString &title = "", QWidget *parent = nullptr);
    void appendItem(const QString &item);
    void eraseItem(MyTableItem *item);
    QWidget *parent;
    QStringList heuteMachenWirDie;
    QDateTime establishedAt;
    QString scheduleTitle;
};

#endif // MYPROP_H
