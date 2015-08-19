#include "LineEditButton.h"
#include <QToolButton>
#include <QStyle>

LineEditButton::LineEditButton(QWidget *parent): QLineEdit(parent)
{
 clearButton = new QToolButton(this);
 QPixmap pixmap("./images/delete.png");
 clearButton->setIcon(QIcon(pixmap));
 clearButton->setIconSize(pixmap.size()/3);
 clearButton->setCursor(Qt::ArrowCursor);
 clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
 clearButton->hide();
 connect(clearButton, SIGNAL(clicked()), this, SLOT(clear())); //clear is a slot of QLineEdit which clears the contents of the LineEdit
 connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateCloseButton(const QString&)));
 //returns the width of the frame using a specific metric
 int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
 //this sets the right-alignment, arg replaces %1 by the number indicated in brackets | padd more than size of frame ==> button is not overwritten
 setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth));
 QSize msz = minimumSizeHint(); //minimum size of the widget
 setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void LineEditButton::resizeEvent(QResizeEvent *)
{
 QSize sz = clearButton->sizeHint();
 int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
 clearButton->move(rect().right() - frameWidth - sz.width(),(rect().bottom() + 1 - sz.height())/2);
}

void LineEditButton::updateCloseButton(const QString& text)
{
 clearButton->setVisible(!text.isEmpty());
}
