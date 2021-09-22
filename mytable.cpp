#include "mytable.h"
#include <QBoxLayout>
#include <QtWidgets>
#include <QScrollArea>

MyTable::MyTable(QWidget *parent, const QString &customizabelIconName)
    : QWidget(parent)
    , customizableIconName(customizabelIconName)
    , listView(new QVBoxLayout(this))
{
    setStyleSheet("border-radius: 1rem;");
    setFixedWidth(parent->width());
    scrollArea = new QScrollArea;
    qDebug() << scrollArea->pos();
    scrollArea->setFixedSize(parent->width(), parent->height() * 0.7);
    qDebug() << parent->height() * 0.7;
    scrollArea->setWidgetResizable(false);
    scrollArea->setWidget(this);

    listView->setMargin(0);
    listView->setSpacing(0);
    listView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

QBoxLayout *MyTable::getLayout() const
{ return listView; }

MyTableItem *MyTable::currentItem() const
{ return itemSelected; }

void MyTable::setNullptrItemSelected()
{ itemSelected = nullptr; }

QString MyTable::getCustomizableIconName() const
{ return customizableIconName; }

MyTableItem * MyTable::appendItem(const QString &name)
{
    auto item = new MyTableItem(this, name);
    listView->addWidget(item);
    connect(this, &MyTable::itemClicked, this, &MyTable::changeSelectedItem);
    setMinimumHeight(height() + item->height());
    qDebug() << this->height();

    return item;
}

void MyTable::changeSelectedItem(MyTableItem *item)
{
    if (itemSelected)
        itemSelected->setUnselected();

    (itemSelected = item)->setSelected();
}

MyTableItem::MyTableItem(MyTable *parent, const QString &name)
    : QWidget(parent)
    , parent(parent)
{
    installEventFilter(this);
    connect(this, &MyTableItem::clicked, this->parent, &MyTable::itemClicked);

    setFixedWidth(parent->width());
    setFixedHeight(qobject_cast<QWidget *>(parent->parent())->height() * 0.1);
    thumbnail = new QLabel(this);
    info = new QLabel(this);
    delBtn = new QToolButton(this);

    thumbnail->setPixmap(QPixmap(":/images/list.png").scaledToHeight(height() * 0.35));
    thumbnail->setFixedWidth(width() * 0.05);

    info->setFixedWidth(width() * 0.72);
    info->setText(name);

    delBtn->setObjectName(parent->getCustomizableIconName());
    delBtn->setIcon(QPixmap(QString(":/images/%1.png").arg(parent->getCustomizableIconName())));
    delBtn->installEventFilter(this);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(thumbnail);
    layout->addWidget(info);
    layout->addWidget(delBtn);
    layout->setAlignment(Qt::AlignLeft);
    parent->getLayout()->addWidget(this);

    connect(delBtn, &QToolButton::clicked, this, &MyTableItem::onDelBtnClicked);
}

QString MyTableItem::text() const
{ return info->text(); }

void MyTableItem::setSelected()
{
    thumbnail->setPixmap(QPixmap(":/images/list-hover.png").scaledToHeight(height() * 0.35));
    delBtn->setIcon(QPixmap(QString(":/images/%1-hover.png").arg(parent->getCustomizableIconName())));
    setStyleSheet("background-color: rgb(12, 4, 124);"
                  "color: rgb(227, 231, 231);");
}

void MyTableItem::setUnselected()
{
    thumbnail->setPixmap(QPixmap(":/images/list.png").scaledToHeight(height() * 0.35));
    delBtn->setIcon(QPixmap(QString(":/images/%1.png").arg(parent->getCustomizableIconName())));
    setStyleSheet("background-color: none;"
                  "border: none;");
}

void MyTableItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool MyTableItem::eventFilter(QObject *watched, QEvent *event)
{
    if (this == watched)
    {
        if (QEvent::Enter == event->type())
        {
            thumbnail->setPixmap(QPixmap(":/images/list-hover.png").scaledToHeight(height() * 0.35));
            delBtn->setIcon(QPixmap(QString(":/images/%1-hover.png").arg(parent->getCustomizableIconName())));
            setStyleSheet("background-color: rgb(12, 4, 124);"
                          "color: rgb(227, 231, 231);"
                          "border-radius: 1rem;");
        } else if (QEvent::Leave == event->type() && this != parent->currentItem())
        {
            thumbnail->setPixmap(QPixmap(":/images/list.png").scaledToHeight(height() * 0.35));
            delBtn->setIcon(QPixmap(QString(":/images/%1.png").arg(parent->getCustomizableIconName())));
            setStyleSheet("background-color: none;"
                          "border: none;");
        }
    }

    return 0;
}

void MyTableItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }
}

// slots:

void MyTableItem::onDelBtnClicked(bool checked)
{
    parent->getLayout()->removeWidget(this);
    if (this == parent->currentItem())
        parent->setNullptrItemSelected();

    emit broadcastMyDeath(this);
    delete this;
}
