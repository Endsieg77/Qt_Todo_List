#include "myprop.h"
#include "listdisplay.h"
#include "mytableitem.h"
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QToolButton>

myProp::myProp(const QString &title, QWidget *parent)
    : scheduleTitle(title)
    , parent(parent)
    , establishedAt(QDateTime::currentDateTime())
{}

void myProp::appendItem(const QString &item)
{
    heuteMachenWirDie.push_back(item);
}

void myProp::eraseItem(MyTableItem *item)
{
    heuteMachenWirDie.removeOne(item->text());
}

