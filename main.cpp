#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    // setlocale(LC_ALL, "Ru");
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Мосты");


    MainWindow w;
    w.show();
    return a.exec();
}
