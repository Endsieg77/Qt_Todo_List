#ifndef MYTABLE_H
#define MYTABLE_H

#include <QWidget>
#include <QDebug>

QT_FORWARD_DECLARE_CLASS(MyTableItem)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QScrollArea)

class MyTable: public QWidget
{
    Q_OBJECT
public:
    MyTable(QWidget *parent = nullptr, const QString &customizabelIconName = "delete");
    QBoxLayout *getLayout() const;
    MyTableItem *appendItem(const QString &name);
    MyTableItem *currentItem() const;
    void setNullptrItemSelected();
    QString getCustomizableIconName() const;

private:
    QString customizableIconName;
    QBoxLayout *listView;
    QScrollArea *scrollArea;
    MyTableItem *itemSelected = nullptr;

signals:
    void itemClicked(MyTableItem *item);

private slots:
    void changeSelectedItem(MyTableItem *item);
};

class MyTableItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyTableItem(MyTable *parent = nullptr, const QString &name = "");
    QString text() const;
    void setSelected();
    void setUnselected();

private:
    QLabel *thumbnail;
    QLabel *info;
    QAbstractButton *delBtn;
    MyTable *parent;

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *wathced, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(MyTableItem *me);
    void broadcastMyDeath(MyTableItem *me);

public slots:
    void onDelBtnClicked(bool checked);
};

#endif // MYTABLE_H
