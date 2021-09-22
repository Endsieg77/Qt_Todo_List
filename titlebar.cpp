#include "titlebar.h"
#include <QtWidgets>
#include <QDesktopWidget>

TitleBar::TitleBar(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    parent
    ? setFixedSize(parent->width() * 0.97, 78)
    : setFixedSize(QApplication::desktop()->width() * 0.355, 78);

    setAttribute(Qt::WA_StyledBackground);

    titleIcon = new QLabel(this);
    titleIcon->setObjectName("TitleIcon");
    titleIcon->setFixedSize(QSize(55, 55));
    titleIcon->setScaledContents(true);
    titleIcon->setPixmap(QPixmap(":/images/titlebar/titleicon.png"));

    windowTitle = new QLabel(this);
    windowTitle->setFixedWidth(parent
                               ? parent->width() * 0.73
                               : QApplication::desktop()->width() * 0.31);
    windowTitle->setObjectName("WindowTitle");
    windowTitle->setText(title);
    windowTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    close = new QToolButton(this);
    close->setObjectName("titlebar/close");
    close->installEventFilter(this);
    close->setIcon(QPixmap(":/images/titlebar/close.png"));
    close->setToolTip("Close");

    minimum = new QToolButton(this);
    minimum->setObjectName("titlebar/minimum");
    minimum->installEventFilter(this);
    minimum->setIcon(QPixmap(":/images/titlebar/minimum.png"));
    minimum->setToolTip("Minimum");

    titleBarLayout = new QHBoxLayout(this);
    titleBarLayout->setMargin(2);
    titleBarLayout->addWidget(titleIcon);
    titleBarLayout->addWidget(windowTitle);
    titleBarLayout->addWidget(minimum);
    titleBarLayout->addWidget(close);
    //titleBarLayout->setAlignment(Qt::AlignJustify);

    titleConnectionsBuilder(parent);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        last_mouse_position_ = event->globalPos();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton))
        return;
    auto ptrParent = static_cast<QWidget *>(parent());
    if(ptrParent)
    {
        const QPoint parent_pos = ptrParent->pos() + event->globalPos() - last_mouse_position_;
        ptrParent->move(parent_pos);
        last_mouse_position_ = event->globalPos();
    }
    else {
        const QPoint position = pos() + event->globalPos() - last_mouse_position_;
        move(position);
        last_mouse_position_ = event->globalPos();
    }
}

void TitleBar::titleConnectionsBuilder(QWidget *widget)
{
    if(!widget)
    {
        connect(minimum, &QToolButton::clicked, this, &QWidget::showMinimized);
        connect(close, &QToolButton::clicked, this, &QWidget::close);
        return;
    }
    connect(minimum, &QToolButton::clicked, widget, &QWidget::showMinimized);
    connect(close, &QToolButton::clicked, widget, &QWidget::close);
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    if(close   == watched ||
       minimum == watched)
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
    return 0;
}

void TitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
