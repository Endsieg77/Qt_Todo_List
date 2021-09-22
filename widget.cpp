#include "widget.h"
#include "ui_widget.h"
#include "titlebar.h"
#include "listdisplay.h"
#include "mytable.h"
#include "mydialog.h"
#include <QListWidget>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QToolButton>
#include <QToolButton>
#include <QtWidgets>
#include <QLineEdit>

auto readString = [] (QFile &stream, int _len) -> QString {
    QString res = "";
    for (auto i = 0; i < _len; ++i)
    {
        char _c;
        stream.read((char *)&_c, sizeof(char));
        res += _c;
    }
    return res;
};

Widget::Widget(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , ui(new Ui::Widget)
{
    readData();
    mainSetup();
    constructList();

    ui->setupUi(this);
}

Widget::~Widget()
{
    auto saveData = QFile("./save.dat");

    if (!dataMap.empty())
    {        
        saveData.open(QIODevice::WriteOnly);
        if (saveData.isOpen())
        {
            QDataStream os (&saveData);
            for (auto i = dataMap.begin(); i != dataMap.end(); ++i)
            {
                auto currentValue = i.value();

                // Serialize title:
                auto str = currentValue.scheduleTitle;
                auto strLen = str.length();

                //qDebug() << str << strLen;

                os.writeRawData((char *)&strLen, sizeof(int));
                os.writeRawData(str.toLatin1().data(), sizeof(char) * strLen);

                // Serialize time:
                auto time = currentValue.establishedAt.toTime_t();

                //qDebug() << timestr << timestrLen;

                os.writeRawData((char *)&time, sizeof(time_t));

                // Serialize list:
                auto list = currentValue.heuteMachenWirDie;
                auto listLen = list.length();

                os.writeRawData((char *)&listLen, sizeof(int));
                for (auto j: list)
                {
                    auto itemLen = j.length();
                    os.writeRawData((char *)&itemLen, sizeof(int));
                    os.writeRawData(j.toLatin1().data(), sizeof(char) * itemLen);
                }
            }
        }
    } else
    {
        saveData.open(QIODevice::WriteOnly);
        saveData.resize(0);
    }
    saveData.close();
    delete ui;
}

void Widget::readData()
{
    auto getSaved = QFile("./save.dat");
    getSaved.open(QIODevice::ReadOnly);

    if (getSaved.isOpen())
    {
        while (!getSaved.atEnd())
        {
            // Get unserialized title:
            int strLen = 0;

            getSaved.read((char *)&strLen, sizeof(int));

            QString str = readString(getSaved, strLen);

            // Get time:
            time_t iTime;
            getSaved.read((char *)&iTime, sizeof(time_t));

            QDateTime time = QDateTime::fromTime_t(iTime);

            // Get list:
            int listLen = 0;
            myProp prop (str, this);

            getSaved.read((char *)&listLen, sizeof(int));

            for (int i = 0; i < listLen; ++i)
            {
                int itemLen = 0;

                getSaved.read((char *)&itemLen, sizeof(int));

                prop.heuteMachenWirDie.push_back(readString(getSaved, itemLen));
            }

            prop.establishedAt = QDateTime(time);

            dataMap.insert(str, prop);
        }
    }
    getSaved.close();
}


void Widget::mainSetup()
{
    // close all subwidgets after the main widget being destroyed
//    setAttribute(Qt::WA_DeleteOnClose);

    setObjectName("Application");
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(QApplication::desktop()->width() * 0.57, QApplication::desktop()->height() * 0.80);
    title = new TitleBar("Pigeon Daily", this);

    leftSideBar = new QWidget(this);
    leftSideBar->setObjectName("leftSideBar");
    leftSideBar->setFixedWidth(
        reinterpret_cast
            <QWidget *>
        (leftSideBar->parent())->width() * 0.34);
    leftSideBar->setFixedHeight(
        reinterpret_cast
            <QWidget *>
        (leftSideBar->parent())->height() * 0.85);

    list = new MyTable(leftSideBar);
    list->setObjectName("list");

    // initialize layout of input area:
    inputArea = new QLineEdit(leftSideBar);
    inputArea->setObjectName("inputArea");
    inputArea->setPlaceholderText("Your Pigeon Plan Here");
    inputArea->setFixedWidth(leftSideBar->width() * 0.65);
    inputArea->setMaxLength(20);
    inputArea->installEventFilter(this);

    addNew = new QToolButton(leftSideBar);
    addNew->setToolTip("Add New Schedule...");
    addNew->setIcon(QPixmap(":/images/addNew.png"));
    addNew->setObjectName("addNew");
    addNew->installEventFilter(this);

    inputAreaLayout = new QHBoxLayout;
    inputAreaLayout->addWidget(inputArea);
    inputAreaLayout->addWidget(addNew);

    // establish connection:
    connect(addNew, &QToolButton::clicked, this, &Widget::createNewScheduleItem);
    connect(this, &Widget::onEnterPressed, this, &Widget::createNewScheduleItem);

    // initialize left sidebar layout:
    leftSideBarLayout = new QVBoxLayout(leftSideBar);
    leftSideBarLayout->setObjectName("leftSideBarLayout");
    leftSideBarLayout->addLayout(inputAreaLayout);
    leftSideBarLayout->addWidget(list);
    leftSideBarLayout->setAlignment(Qt::AlignLeft);
    leftSideBarLayout->setStretch(0, 3);
    leftSideBarLayout->setStretch(1, 7);

    // initialize right sidebar layout:
    stuffOnTheRightSide = new QVBoxLayout;
    altEqualsTo = new QLabel(this);
    altEqualsTo->setObjectName("altText");
    altEqualsTo->setText("Um... What shall I pigeon?");
    stuffOnTheRightSide->addWidget(altEqualsTo);
    stuffOnTheRightSide->setAlignment(Qt::AlignCenter);

    // initialize the main part layout of the widget (except title bar):
    mainLayout = new QHBoxLayout;
    mainLayout->setObjectName("mainLayout");
    mainLayout->addWidget(leftSideBar);
    mainLayout->addLayout(stuffOnTheRightSide);
    mainLayout->setAlignment(Qt::AlignLeft);

    // initialize the overall layout
    widgetLayout = new QVBoxLayout(this);
    widgetLayout->setObjectName("widgetLayout");
    widgetLayout->addWidget(title);
    widgetLayout->addLayout(mainLayout);
    widgetLayout->setAlignment(Qt::AlignTop);

    connect(list, &MyTable::itemClicked, this, &Widget::shiftStuffOnTheRightSide);

}

void Widget::constructList()
{
    for(auto i: dataMap)
    {
        connect(list->appendItem(i.scheduleTitle), &MyTableItem::broadcastMyDeath,
                this, &Widget::eraseItem);
    }

//    list->settleScrollArea();
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (addNew == watched)
    {
        auto thisBtn = static_cast<QToolButton *>(watched);
        if(QEvent::Enter == event->type())
        {
            thisBtn->setIcon(QPixmap(":/images/" + thisBtn->objectName() + "-hover.png"));
        } else if(QEvent::Leave == event->type())
        {
            thisBtn->setIcon(QPixmap(":/images/" + thisBtn->objectName() + ".png"));
        }
    }
    else if (inputArea == watched)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)
            {
                onEnterPressed();
                return 1;
            }
        }
    }

    return 0;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// slots defined here:
void Widget::createNewScheduleItem()
{
    if (auto contentStr = inputArea->text(); contentStr != "" && !dataMap.keys().contains(contentStr))
    {
        inputArea->clear();

        connect(list->appendItem(contentStr), &MyTableItem::broadcastMyDeath,
                this, &Widget::eraseItem);

        dataMap.insert(contentStr, myProp(contentStr, this));
    } else if (contentStr != "")
    {
        instantDialog = new MyDialog("Repeated Schedule Name!\nPlease remove one, or otherwise!", "Attention: " + contentStr);
        instantDialog->show();
    }
}

void Widget::eraseItem(MyTableItem *item)
{
    if (disp && disp->text() == item->text())
    {
        stuffOnTheRightSide->removeWidget(disp);
        delete disp;
        disp = nullptr;
        altEqualsTo->show();
    }

    dataMap.erase(dataMap.find(item->text()));
}

void Widget::shiftStuffOnTheRightSide(MyTableItem *item)
{
    if (disp)
    {
        stuffOnTheRightSide->removeWidget(disp);
        delete disp;
        disp = nullptr;
    }

    disp = new ListDisplay(&dataMap[item->text()], this);
    stuffOnTheRightSide->addWidget(disp);

    if (!altEqualsTo->isHidden())
    {
        altEqualsTo->hide();
    }
}
