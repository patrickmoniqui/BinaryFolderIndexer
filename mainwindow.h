#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "binarytree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    BinaryTreeFichier b = BinaryTreeFichier();
    BinaryTreeFichier *bin = &b;

    ~MainWindow();

    private slots:
    void btnChooseFolder_OnClick();
    void btnIndexFiles_OnClick();
    void btnSearchFiles_OnClick();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
