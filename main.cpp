#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <qthread.h>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Définition du fichier de ressources
    QFile File(":/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    //Ajout du fichier de ressources à l'application
    a.setStyleSheet(StyleSheet);

    //Splash screen
    QPixmap pixmap("/Users/macbookpro/Documents/C++Projects/BinaryFolderSort/splash_screen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    //On simule un 3 secondes
    QThread::sleep(3);

    MainWindow w;
    w.show();

    //Fermeture du splash screen
    splash.finish(&w);

    return a.exec();
}
