#include <QTranslator>

#include "mainwindow.h"
#include <QApplication>
#include <paintwidget.h>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;

    translator.load(QLocale(), QLatin1String("mandala"), QLatin1String("_"), QLatin1String(":/"));
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
