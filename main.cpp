#include "main.h"
#include "ui_main.h"

main::main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main)
{
    ui->setupUi(this);
}

main::~main()
{
    delete ui;
}

/*#include <QtGui/QApplication>
#include "yload.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    yLoad w;
    w.show();

    return a.exec();
}*/