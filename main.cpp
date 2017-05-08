#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <qthread.h>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString current_folder = QDir::currentPath();
    QPixmap pixmap("/Users/macbookpro/Documents/C++Projects/BinaryFolderSort/splash_screen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    QThread::sleep(3);

    MainWindow w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
