#include "lab32.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include "Rec.h"
#include <QRect>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lab32 w;
    w.resize(800, 800);
    
    w.show();
    return a.exec();
}

//вставка рекурсией по клеточкам 