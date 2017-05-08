#ifndef FICHIERMODEL_H
#define FICHIERMODEL_H

#include "QString"

class FichierModel : public QAbstractTableModel {
   QList<Fichier> m_data;
public:
   FichierModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
   int rowCount(const QModelIndex &) const override { return m_data.count(); }
   int columnCount(const QModelIndex &) const override { return 3; }
   QVariant data(const QModelIndex &index, int role) const override {
      if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
      const auto & fichier = m_data[index.row()];
      switch (index.column()) {
      case 0: return QString::fromStdString(fichier.GetNomFichier());
      case 1: return QString::fromStdString(fichier.GetDossier());
      case 2: return "";
      default: return {};
      };
   }
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
      if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
      switch (section) {
      case 0: return "Fichier";
      case 1: return "Dossier";
      case 2: return "R";
      default: return {};
      }
   }
   void append(const Fichier & fichier) {
      beginInsertRows({}, m_data.count(), m_data.count());
      m_data.append(fichier);
      endInsertRows();
   }
};


#endif // FICHIERMODEL_H
