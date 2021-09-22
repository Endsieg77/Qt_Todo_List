#include "widget.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

    Widget w;
    QFile qss(":/style.qss");
    qss.open(QFile::ReadOnly);
    if(qss.isOpen())
    {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
    w.show();
    return a.exec();
}
