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
#include "FichierModel.h"
#include "QSortFilterProxyModel"
#include <thread>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frameSearch->setVisible(false);

    connect(ui->btnChooseFolder, SIGNAL (released()), this, SLOT (btnChooseFolder_OnClick()));
    connect(ui->btnIndexFiles, SIGNAL (released()), this, SLOT (btnIndexFiles_OnClick()));
    connect(ui->btnSearch, SIGNAL (released()), this, SLOT (btnSearchFiles_OnClick()));
    connect(ui->tableResult, SIGNAL(cellClicked(int,int)), this, SLOT(TableResult_OnCellClicked(int,int)));

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

            if (dirp->d_type == DT_DIR) {
                getdir(fichier.GetPath(),files);
            }
            else
            {
                // on insert seulement les fichiers
                files.push_back(fichier);
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

    time_t start,end;
    time (&start);

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

    time (&end);
    double duration = difftime (end,start);

    std:string str = std::to_string(cpt);
    QString qstr = QString::fromStdString(str.append(" indexed file in ").append(std::to_string(duration).append(" secs.")));
    ui->lblNbIndexesFiles->setText(qstr);
    ui->frameSearch->setVisible(true);
}

void MainWindow::btnSearchFiles_OnClick()
{
    QTableWidget* m_pTableWidget = ui->tableResult;
    QStringList m_TableHeader;
    FichierModel m_model;

    BinaryTreeFichier* tree = this->bin;
    string nom_fichier = ui->txtSearchFile->toPlainText().toStdString();


    tree->RechercherFichier(nom_fichier, this->vec, false);

    m_pTableWidget->setRowCount(this->vec->size());
    m_pTableWidget->setColumnCount(3);
    m_TableHeader<<"Fichier"<<"Dossier"<<"";
    m_pTableWidget->setHorizontalHeaderLabels(m_TableHeader);
    m_pTableWidget->verticalHeader()->setVisible(false);
    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableWidget->setShowGrid(false);

    if(this->vec->size() > 0)
    {
        for(std::vector<Fichier>::size_type i = 0; i != this->vec->size(); i++) {
            Fichier fich = this->vec->at(i);
            m_pTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(fich.GetNomFichier())));
            m_pTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(fich.GetDossier())));
            m_pTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString("")));
        }

        std:string str = std::to_string(this->vec->size());
        QString qstr = QString::fromStdString(str.append(" files found!"));
        ui->lblNoSearchFound->setText(qstr);
    }
    else
    {
        QString qstr = QString::fromStdString("Aucun fichier trouvé.");
        ui->lblNoSearchFound->setText(qstr);
    }
}


void MainWindow::TableResult_OnCellClicked(int row, int col)
{
    QTableWidgetItem * item = ui->tableResult->item(row, col);

    QString path = item->text();

    QDesktopServices::openUrl(path);
}
