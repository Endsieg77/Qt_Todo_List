#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(Help)

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(const QString &title, QWidget *parent = nullptr);
    void titleConnectionsBuilder(QWidget *widget);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *wathced, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    QPoint last_mouse_position_;
    QLabel *titleIcon;
    QLabel *windowTitle;
    QAbstractButton *close;
    QAbstractButton *minimum;
    QBoxLayout *titleBarLayout;
};

#endif // TITLEBAR_H
