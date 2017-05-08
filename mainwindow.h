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
    vector<Fichier> v = vector<Fichier>();
    vector<Fichier> *vec = &v;

    ~MainWindow();

    private slots:
    void btnChooseFolder_OnClick();
    void btnIndexFiles_OnClick();
    void btnSearchFiles_OnClick();
    void TableResult_OnCellClicked(int row,int col);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
