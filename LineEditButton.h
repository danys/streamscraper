#ifndef LINEEDITBUTTON_H
#define LINEEDITBUTTON_H

#include <QLineEdit>

class QToolButton;
class LineEditButton : public QLineEdit
{
 Q_OBJECT
 public:
 LineEditButton(QWidget *parent = 0);
 protected:
 void resizeEvent(QResizeEvent *);
 private slots:
 void updateCloseButton(const QString &text);
 private:
 QToolButton *clearButton;
};

#endif // LINEEDITBUTTON_H
