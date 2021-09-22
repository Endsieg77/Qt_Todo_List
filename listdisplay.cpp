#include "widget.h"
#include "listdisplay.h"
#include "myprop.h"
#include "mytable.h"
#include <QLabel>
#include <QTableWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QPainter>
#include <QLineEdit>
#include <QScrollArea>

ListDisplay::ListDisplay(myProp *prop, QWidget *parent)
    : prop(prop)
    , QWidget(parent)
{
    setFixedHeight(parent->height() * 0.85);
    setFixedWidth(parent->width() * 0.61);

    TitleDisplay = new QLabel(this);
    establishedTimeDisplay = new QLabel(this);
    inputArea = new QLineEdit(this);
    addNew = new QToolButton(this);
    listWidget = new MyTable(this, "completion");

    for (auto i : prop->heuteMachenWirDie)
    {
        connect(listWidget->appendItem(i), &MyTableItem::broadcastMyDeath,
                this, &ListDisplay::eraseItem);
    }

    TitleDisplay->setObjectName("ScheduleTitle");
    TitleDisplay->setText(prop->scheduleTitle);
    TitleDisplay->setAlignment(Qt::AlignRight);

    establishedTimeDisplay->setObjectName("EstablishedTime");
    establishedTimeDisplay->setText(prop->establishedAt.toString());
    establishedTimeDisplay->setAlignment(Qt::AlignRight);

    inputArea->setPlaceholderText("Was wünschen Sie heute machen?");
    inputArea->setFixedWidth(width() * 0.7);
    inputArea->setMaxLength(20);

    addNew->setObjectName("append");
    addNew->setToolTip("add a new plan in this schedule!");
    addNew->setIcon(QPixmap(":/images/append.png"));
    addNew->installEventFilter(this);
    connect(addNew, &QToolButton::clicked, this, &ListDisplay::appendItem);

    auto inputAreaLayout = new QHBoxLayout;
    inputAreaLayout->addWidget(inputArea);
    inputAreaLayout->addWidget(addNew);
    inputAreaLayout->setMargin(0);
    inputAreaLayout->setSpacing(0);

    myLayout = new QVBoxLayout(this);
    myLayout->addWidget(TitleDisplay);
    myLayout->addWidget(establishedTimeDisplay);
    myLayout->addLayout(inputAreaLayout);
    myLayout->addWidget(listWidget);
    myLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
//    myLayout->setSpacing(0);
    myLayout->setMargin(15);
}

QBoxLayout *ListDisplay::getLayout() const
{ return myLayout; }

QString ListDisplay::text() const
{ return TitleDisplay->text(); }

bool ListDisplay::eventFilter(QObject *watched, QEvent *event)
{
    if (addNew == watched)
    {
        auto thisBtn = static_cast<QToolButton *>(watched);
        if (QEvent::Enter == event->type())
        {
            thisBtn->setIcon(QPixmap(":/images/" + thisBtn->objectName() + "-hover.png"));
        } else if (QEvent::Leave == event->type())
        {
            thisBtn->setIcon(QPixmap(":/images/" + thisBtn->objectName() + ".png"));
        }
    }

    return 0;
}

void ListDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// slots:

void ListDisplay::appendItem()
{
    if (auto contentStr = inputArea->text(); contentStr != "")
    {
        inputArea->clear();
        listWidget->appendItem(contentStr);
        prop->appendItem(contentStr);
    }
}

void ListDisplay::eraseItem(MyTableItem *item)
{
    prop->eraseItem(item);
}
