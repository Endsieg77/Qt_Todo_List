#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)

class MyDialog : public QWidget
{
    Q_OBJECT
public:
    explicit MyDialog(const QString &text = "", const QString &title = "Attention", QWidget *parent = nullptr);

private:
    TitleBar *windowTitle;
    QLabel *textLabel;
    QAbstractButton *confirm;

signals:

public slots:
};

#endif // MYDIALOG_H
