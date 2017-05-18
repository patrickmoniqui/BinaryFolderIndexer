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
#include "QThread"
#include <QDesktopServices>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "QObject"

using namespace  std;
//
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnChooseFolder, SIGNAL (released()), this, SLOT (btnChooseFolder_OnClick()));
    connect(ui->btnSearch, SIGNAL (released()), this, SLOT (btnSearchFiles_OnClick()));
    connect(ui->tableResult, SIGNAL(cellClicked(int,int)), this, SLOT(TableResult_OnCellClicked(int,int)));

    //Mettre le logo de l'application en background d'un label
    QPixmap pix(":/splash_screen.png");
    ui->lblLogo->setPixmap(pix);
    ui->lblLogo->setScaledContents(true);
}

// Function qui rempli l'arbre binaire
void MainWindow::IndexFolder(string path)
{
    vector<Fichier> files = vector<Fichier>();
    BinaryTreeFichier *tree = this->bin;

    time_t start,end;
    time (&start);

    // remplissage d'un vecteur contenant les fichiers du dossier et sous-dossiers.
    getdir(path, &files);

    int cpt=-1;
    int size = files.size();
    for(Fichier f : files)
    {
        tree->Inserer(f);
        ui->progressBar->setValue(cpt++/size*100);
    }
    ui->progressBar->setValue(100);

    time (&end);
    float duration = difftime (end,start);

    stringstream stream;
    stream << fixed << setprecision(2) << duration;
    string length = stream.str();

    string str = std::to_string(size);

    QString qstr = QString::fromStdString(str.append(" indexed file in ").append(length).append(" secs."));
    ui->lblNbIndexesFiles->setText(qstr);
    ui->frameSearch->setVisible(true);
}

// Destruction de la fenêtre
MainWindow::~MainWindow()
{
    delete ui;
}

// Événement OnPressed du bouton 'Choose Folder'
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
        ui->lblNbIndexesFiles->setText("Indexing...");

        //Indexation des fichiers
        IndexFolder(dir.toStdString());

        this->vec->clear();

        ui->btnSearch->setEnabled(true);
    }
    else
    {
       ui->btnSearch->setEnabled(false);
    }
}

// Fonction qui rempli un vecteur contenant tous les fichiers et sous-fichiers à partir du nom de dossier
int MainWindow::getdir(string dir, vector<Fichier> *files)
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
            //nom du fichier tolower
            //std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            Fichier fichier(dir+"/", name);

            if (dirp->d_type == DT_DIR) {
                getdir(fichier.GetPath(),files);
            }
            else
            {
                // on insert seulement les fichiers
                files->push_back(fichier);
            }
        }
    }
    closedir(dp);
    return 0;
}

// Événement OnPressed du bouton Search
void MainWindow::btnSearchFiles_OnClick()
{
    QTableWidget* m_pTableWidget = ui->tableResult;
    QStringList m_TableHeader;
    FichierModel m_model;

    BinaryTreeFichier* tree = this->bin;
    string nom_fichier = ui->txtSearchFile->toPlainText().toStdString();

    tree->RechercherFichier(nom_fichier, this->vec, true);

    m_pTableWidget->setRowCount(this->vec->size());
    m_pTableWidget->setColumnCount(2);
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
        }

        //resize columns to fit content
        m_pTableWidget->resizeColumnsToContents();

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

//Événement OnCellClick de la table de résultat
void MainWindow::TableResult_OnCellClicked(int row, int col)
{
    // En cliquant sur l'item, il sera ouvert par le programme associé à l'extension (EX: Notepad avec les fichiers *.txt)
    QString path = ui->tableResult->item(row, 1)->text() + ui->tableResult->item(row, 0)->text();
    string command;
    QString OSVersion = QSysInfo::productType(); // Vérifie la version
    if(OSVersion == ("windows"))
    {

    }
    else if (OSVersion == ("macos") || OSVersion == "osx"){
        // Selon la documentation en ligne, Le open est la meilleur solution.
        command = "open '" + path.toStdString() + "'";
    }
    system(command.c_str());
}
