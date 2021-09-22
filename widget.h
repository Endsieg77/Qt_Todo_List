#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "myprop.h"

QT_FORWARD_DECLARE_CLASS(MyTable)
QT_FORWARD_DECLARE_CLASS(MyTableItem)
QT_FORWARD_DECLARE_CLASS(QStackedLayout)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(ListDisplay)
QT_FORWARD_DECLARE_CLASS(MyDialog)


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void readData();
    void mainSetup();
    void constructList();

private:
    Ui::Widget *ui;
    TitleBar *title;
    QMap<QString, myProp> dataMap;
    QWidget *leftSideBar;
    QLineEdit *inputArea;
    QAbstractButton *addNew;
    MyTable *list;
    QBoxLayout *widgetLayout;
    QBoxLayout *mainLayout;
    QBoxLayout *leftSideBarLayout;
    QBoxLayout *inputAreaLayout;
    QBoxLayout *stuffOnTheRightSide;
    QLabel *altEqualsTo;
    ListDisplay *disp = nullptr;
    MyDialog *instantDialog = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *wathced, QEvent *event) override;

public slots:
    void createNewScheduleItem();
    void shiftStuffOnTheRightSide(MyTableItem *item);
    void eraseItem(MyTableItem *item);

signals:
    void onEnterPressed();
};
#endif // WIDGET_H
