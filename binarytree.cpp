#include "binarytree.h"

// Insertion
void BinaryTreeFichier::Inserer(Fichier f)
{
   Noeud *pere = NULL;
   actuel = racine;

   // Chercher l'emplacement du noeud en partant de la racine
   while(!Vide(actuel) && f != actuel->fichier)
   {
      pere = actuel;

      //Aller dans le sous-arbre de droite ou gauche selon le cas
      if (f >= actuel->fichier) actuel = actuel->droite;
      else if (f < actuel->fichier) actuel = actuel->gauche;
   }

   // Si la donnee existe, returner sans l'inserer
   if(!Vide(actuel)) return;

   // Si pere == NULL l'arbre est vide, le noeud sera la racine
   if(Vide(pere)) racine = new Noeud(f);
   // Si donnee < noeud pere, insertion a gauche
   else if(f < pere->fichier) pere->gauche = new Noeud(f);
   // Si donnee < noeud pere, insertion a droite
   else if(f >= pere->fichier) pere->droite = new Noeud(f);
}

// Insertion trier par fichier
void BinaryTreeFichier::InsererFichier(Fichier f)
{
   Noeud *pere = NULL;
   actuel = racine;

   // Chercher l'emplacement du noeud en partant de la racine
   while(!Vide(actuel) && f != actuel->fichier)
   {
      pere = actuel;

      //Aller dans le sous-arbre de droite ou gauche selon le cas
      if (f.GetNomFichier() > actuel->fichier.GetNomFichier()) actuel = actuel->droite;
      else if (f.GetNomFichier() < actuel->fichier.GetNomFichier()) actuel = actuel->gauche;
   }

   // Si la donnee existe, returner sans l'inserer
   if(!Vide(actuel)) return;

   // Si pere == NULL l'arbre est vide, le noeud sera la racine
   if(Vide(pere)) racine = new Noeud(f);
   // Si donnee < noeud pere, insertion a gauche
   else if(f.GetNomFichier() < pere->fichier.GetNomFichier()) pere->gauche = new Noeud(f);
   // Si donnee < noeud pere, insertion a droite
   else if(f.GetNomFichier() > pere->fichier.GetNomFichier()) pere->droite = new Noeud(f);

   size++;
}

// Effacer
void BinaryTreeFichier::Effacer(Fichier f)
{
   Noeud *pere = NULL;
   Noeud *noeud;
   Fichier aux;

   actuel = racine;
   while(!Vide(actuel))
   {
      //La donnee a ete trouve
      // Donc la donnee est dans le noeud actuel
      if(f == actuel->fichier)
      {
         // La valeur est dans le noeud actuel, qui est une feuille
         if(EstFeuille(actuel))
         {
            if(pere) // S'il a un pere n'est pas la racine: annuler son pointeur reference
               if(pere->droite == actuel)
               {
                   pere->droite = NULL;
               }
               else
               {
                   if(pere->gauche == actuel)
                   {
                       pere->gauche = NULL;
                   }
               }
            delete actuel; 	// Effacer le noeud
            size--;
            actuel = NULL;
            return;
         }
         else  // La valeur est dans le noeud actuel, mais n'est pas une feuille
         {
            pere = actuel;
            // Chercher le noeud plus a gauche de la branche droite
            if(actuel->droite)
            {
               noeud = actuel->droite;
               while(noeud->gauche)
               {
                  pere = noeud;
                  noeud = noeud->gauche;
               }
            }
            // Chercher le noeud plus a droite de la branche gauche
            else
            {
               noeud = actuel->gauche;
               while(noeud->droite)
               {
                  pere = noeud;
                  noeud = noeud->droite;
               }
            }
            // Echanger ses valeurs
            aux = actuel->fichier;
            actuel->fichier = noeud->fichier;
            noeud->fichier = aux;
            actuel = noeud;
         }
      }
      else
       { // On n'a pas trouve la donnee, continuer la recherche
         pere = actuel;
         if(f > actuel->fichier) actuel = actuel->droite;
         else if(f < actuel->fichier) actuel = actuel->gauche;
      }
   }
}

// Chercher un fichier par son nom
void BinaryTreeFichier::RechercherFichier(string nom, vector<Fichier> * vec, bool firstOnly)
{
    vec->clear();
    vector<Fichier> results = vector<Fichier>();
    vector<Fichier> *r = &results;
    actuel = racine;
    string f1=nom;

    while(!Vide(actuel) && nom != "")
    {
        string f2 = actuel->fichier.GetNomFichier();
        if (f2.compare(0, f1.length(), f1) == 0)
        {
            vec->push_back(actuel->fichier);
            if(firstOnly)
            {
                break;
            }
            else
            {
                actuel = actuel->droite;
                continue;
            }
        }
        else if(f1 >= f2) actuel = actuel->droite;
        else if(f1 < f2) actuel = actuel->gauche;
    }
}

// Calculer la longueur du noeud qui contient la donnee
int BinaryTreeFichier::Hauteur(Fichier dat)
{
   int hauteurN = 0;
   actuel = racine;


   while(!Vide(actuel)) {
      if(dat == actuel->fichier) return hauteurN; //retourner la hauteur lorsque la donnees est trouvee
      else {
         hauteurN++; // Incrementer la  hauteur en descendant de niveau
         if(dat > actuel->fichier) actuel = actuel->droite;
         else if(dat < actuel->fichier) actuel = actuel->gauche;
      }
   }
   return -1; //Retourner -1 si l'arbre est vide
}

// Comptage de noeuds
int BinaryTreeFichier::NbNoeuds()
{
   compteur = 0;
   auxCompteur(racine);
   return compteur;
}

// Comptage de noeuds. Parcours PreFixe
void BinaryTreeFichier::auxCompteur(Noeud *noeud)
{
   compteur++;

   if(noeud->gauche) auxCompteur(noeud->gauche);
   if(noeud->droite) auxCompteur(noeud->droite);
}

// Longueur de l'arbre: longueur du noeud plus longue
int BinaryTreeFichier::HauteurArbre()
{
   hauteur = 0;
   auxHauteur(racine, 0);
   return hauteur;
}

// Calcul de la longueur en postfixe
// actualiser la longueur uniquement en feuilles de longueuer > a la longueur max actuelle
void BinaryTreeFichier::auxHauteur(Noeud *noeud, int a)
{
   //
   if(noeud->gauche) auxHauteur(noeud->gauche, a+1);
   if(noeud->droite) auxHauteur(noeud->droite, a+1);
   //
   //
   if(EstFeuille(noeud) && a > hauteur) hauteur = a;
}

void BinaryTreeFichier::FillVector(vector<Fichier> &vec, Noeud *noeud, bool r)
{
    return;
}

void Affiche(int &d)
{
   cout << d << ",";
}
