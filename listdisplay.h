#ifndef LISTDISPLAY_H
#define LISTDISPLAY_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(MyTable)
QT_FORWARD_DECLARE_CLASS(MyTableItem)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(myProp)


class ListDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ListDisplay(myProp *prop, QWidget *parent = nullptr);
    QBoxLayout *getLayout() const;
    QString text() const;

private:
    myProp *prop;
    QLabel *TitleDisplay;
    QLabel *establishedTimeDisplay;
    QLineEdit *inputArea;
    QAbstractButton *addNew;
    MyTable *listWidget;
    QBoxLayout *myLayout;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *wathced, QEvent *event) override;

signals:

public slots:
    void appendItem();
    void eraseItem(MyTableItem *item);
};

#endif // LISTDISPLAY_H
