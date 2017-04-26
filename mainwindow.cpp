#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <sys/types.h>
#include <sys/stat.h>
#include "dirent.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "QDir"
#include <vector>
#include <cstring>
#include <string.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnChooseFolder, SIGNAL (released()), this, SLOT (btnChooseFolder_OnClick()));
    connect(ui->btnIndexFiles, SIGNAL (released()), this, SLOT (btnIndexFiles_OnClick()));
    connect(ui->btnSearch, SIGNAL (released()), this, SLOT (btnSearchFiles_OnClick();));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnChooseFolder_OnClick()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->txtFolder->setText(dir);

    if(dir != "")
    {
        ui->btnIndexFiles->setEnabled(true);
    }
    else
    {
        ui->btnIndexFiles->setEnabled(false);
    }

}

int getdir (string dir, vector<Fichier> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        string name(dirp->d_name);
        if (name != "." && name != "..") {
            Fichier fichier(dir+"/", name);
            files.push_back(fichier);
            if (dirp->d_type == DT_DIR) {
                getdir(fichier.GetPath(),files);
            }
        }
    }
    closedir(dp);
    return 0;
}

void MainWindow::btnIndexFiles_OnClick()
{
    QString fileName = ui->txtFolder->toPlainText();
    string path = fileName.toStdString();
    vector<Fichier> files = vector<Fichier>();
    BinaryTreeFichier *tree = this->bin;
    QStandardItemModel *model = new QStandardItemModel(1, 1, this);

    time_t before = time(0);

  // remplissage d'un vecteur contenant les fichiers du dossier et sous-dossiers.
  getdir(path, files);

  int cpt=-1;
  int size = files.size();
  for(Fichier f : files)
  {
      tree->Inserer(f);
      ui->progressBar->setValue(cpt++/size*100);
  }
  ui->progressBar->setValue(100);

  time_t after = time(0);

  ui->lblNbIndexesFiles->setText(cpt + " indexed files!");
}

void MainWindow::btnSearchFiles_OnClick()
{
      vector<Fichier> files = vector<Fichier>();
      BinaryTreeFichier* tree = this->bin;
      string nom_fichier = ui->txtSearchFile->toPlainText().toStdString();

        Fichier f = tree->Rechercher(nom_fichier);

        QStandardItem *item = new QStandardItem(QString::fromStdString(f.GetNomFichier()));
        QStandardItemModel *model = new QStandardItemModel(1, 1, this);
        model->setItem(1, 0, item);
}
