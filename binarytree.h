#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <iostream>

using namespace std;

class Fichier {
private:
    string dossier;
    string nom_fichier;
public:
    Fichier() {}

    Fichier(string d, string f) : dossier(d), nom_fichier(f) {}

    string GetPath() const
    {
        return dossier+nom_fichier;
    }

    string GetDossier()
    {
        return dossier;
    }

    string GetNomFichier()
    {
        return nom_fichier;
    }

    friend bool operator< (const Fichier & f1, const Fichier & f2)
    {
        if(f1.GetPath() < f2.GetPath())
            return true;
        else
            return false;
    }

    friend bool operator> (const Fichier & f1, const Fichier & f2)
    {
        if(f1.GetPath() > f2.GetPath())
            return true;
        else
            return false;
    }

    friend bool operator== (const Fichier& f1, const Fichier & f2)
    {
        if(f1.GetPath().compare(f2.GetPath()) == 0)
            return true;
        else
            return false;
    }

    friend bool operator!= (const Fichier & f1, const Fichier & f2)
    {
        if(f1.GetPath().compare(f2.GetPath()) != 0)
            return true;
        else
            return false;
    }
};

class BinaryTreeFichier
{

  private:

   class Noeud
   {
     public:
      // Constructeur:
      Noeud(Fichier f, Noeud *gau=NULL, Noeud *dro=NULL) :
        fichier(f), gauche(gau), droite(dro) {}
      // Membres:
      Fichier fichier;
      Noeud *gauche;
      Noeud *droite;
   };

   // Pointeurs de l'arbre racine et noeud actuel:
   Noeud *racine;
   Noeud *actuel;
   int compteur;
   int hauteur;

  public:
   BinaryTreeFichier() : racine(NULL), actuel(NULL) {}

   void Inserer(Fichier ficher);     	// Insertion

   void InsererFichier(Fichier ficher);     	// Insertion trier par fichier

   void Effacer(Fichier ficher);	// Supprimer une donnes de l'arbre

   Fichier Rechercher(string fichier);	// Rechercher

   bool Vide(Noeud *r) { return r==NULL; }    // Arbre vide

   bool EstFeuille(Noeud *r) { return !r->droite && !r->gauche; }    // Feuille

   int NbNoeuds();		// Comptage de noeuds

   int HauteurArbre();

   int Hauteur(Fichier);		// Calculer la longueur

   //Parcours InFixe de l'arbre
   void InFixe  (void (*func)(Fichier&), Noeud *noeud=NULL, bool r=true);

   //Parcours PreFixe de l'arbre
   void PreFixe (void (*func)(Fichier&), Noeud *noeud=NULL, bool r=true);

   //Parcours PostFixe de l'arbre
   void PostFixe(void (*func)(Fichier&), Noeud *noeud=NULL, bool r=true);

   void ParcoursEnLargeur();

  private:
   // Fonctions auxiliaires
   void auxCompteur(Noeud*);
   void auxHauteur(Noeud*, int);
};

#endif // BINARYTREE_H
