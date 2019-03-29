//
//  select.cpp
//
//  Created by Olivier Cuisenaire on 10.04.15.
//  Copyright (c) 2015 Olivier Cuisenaire. All rights reserved.
//

#include <iostream>
#include <utility>
#include <climits>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

// selectPivot(begin,end)
//
// choisit un pivot pour le tri rapide parmi le tableau
// entre begin et end (non inclus). Calcule la médiane
// entre le premier, le dernier et l'élément central.
// retourne un iterateur du même type que begin et end
// pointant sur la valeur choisie.
//
// NE RIEN MODIFIER DANS CETTE FONCTION

template < typename RandomAccessIterator >
RandomAccessIterator selectPivot( const RandomAccessIterator begin,
                                 const RandomAccessIterator end )
{
    RandomAccessIterator p1 = begin;
    RandomAccessIterator p2 = begin + ( end-begin ) / 2;
    RandomAccessIterator p3 = end-1;

    if(*p1 < *p2) {
        if( *p2 < *p3 ) return p2;
        else if(*p1 < *p3) return p3;
        return p1;
    } else {
        if( *p1 < *p3 ) return p1;
        else if(*p2 < *p3) return p3;
        return p2;
    }
}

//template < typename T>
//vector<T> creationVecteurValeurRandom(const T& TAILLE, T valeurMax) {
//  vector<T> v(TAILLE);
//  for(int i = 0; i < TAILLE; ++i) {
//    v.at(i) = rand() % valeurMax + 1;
//  }
//  return v;
//}


vector<int> creationVecteurValeurRandom(const int& TAILLE, int valeurMax) {
  vector<int> v(TAILLE);
  for(int i = 0; i < TAILLE; ++i) {
    v.at(i) = rand() % valeurMax + 1;
  }
  return v;
}


void textResultat(unsigned int borneMax, unsigned int taille, double resultat,  unsigned int nbrSimulations){
         cout << "Pour un vecteur de taille : " << taille << " de valeur entre 1 a "
              << borneMax << " [compris]. " << endl
              << "Nombres de simulations      : " << nbrSimulations << endl 
              << "Temps moyen en nanosecondes : " << resultat << "[ns]" << endl
              << "Temps moyen en seconde      : " << resultat / 1000000000 << "[s] " << endl;
}


// ________________________________________

// selectionSort
//
// Effectue le tri par sélection des éléments entre begin
// et end (non inclus). Doit appeler display() après chaque
// échange.
template < typename RandomAccessIterator >
void selectionSort( RandomAccessIterator begin,
                   RandomAccessIterator end )
{
    for(auto itI = begin; itI != end-1 ; itI++ ){
        auto iMin = itI;
        for(auto itJ = itI+1; itJ != end ; itJ++){
            if(*itJ < *iMin){
                iMin = itJ;
            }
        }
        swap(*itI,*iMin);
    }
}


double testSelection (unsigned int taille, const unsigned int borneMax, const unsigned int nbrSimulations){
   double tempsTotal = 0;
   for(size_t i = 0; i < nbrSimulations; i++){
      vector<int> vValeurRandom = creationVecteurValeurRandom(taille, borneMax);
      //prendre le moment de départ
       high_resolution_clock::time_point t1 = high_resolution_clock::now();
       //exécuter les opérations à chronométrer ici
       selectionSort(vValeurRandom.begin(), vValeurRandom.end());
       //prendre le moment d’arrivée
       high_resolution_clock::time_point t2 = high_resolution_clock::now();
       //calcul du temps, ici en nanosecondes
       double temps = (double) duration_cast<nanoseconds>(t2 - t1).count();
       tempsTotal += temps;
   }
   return tempsTotal / (double)nbrSimulations;
}


// _____________________________________

template < typename RandomAccessIterator >
void quickSort( RandomAccessIterator begin,
               RandomAccessIterator end )
{
    if((end-1) <= begin){
        return;
    }
    //On choisi le pivot
    auto pivot = selectPivot(begin,end);
    swap(*pivot,*(end-1));
    auto i = begin;
    pivot = (end - 1);
    auto j = pivot - 1;

    while(true){
        while(*i < *pivot){
            i++;
        }
        while(j > i && *pivot < *j ){
            j--;
        }
        if(i >= j){
            break;
        }
        swap(*i,*j);
        ++i;
        --j;
    }
        swap(*i,*pivot);
        quickSort(begin,i);
        quickSort(i+1,end);
}



double testRapide (unsigned int taille, const unsigned int borneMax, const unsigned int nbrSimulations){
   double tempsTotal = 0;
   for(size_t i = 0; i < nbrSimulations; i++){
      vector<int> vValeurRandom = creationVecteurValeurRandom(taille, borneMax);
      //prendre le moment de départ
       high_resolution_clock::time_point t1 = high_resolution_clock::now();
       //exécuter les opérations à chronométrer ici
       quickSort(vValeurRandom.begin(), vValeurRandom.end());
       //prendre le moment d’arrivée
       high_resolution_clock::time_point t2 = high_resolution_clock::now();
       //calcul du temps, ici en nanosecondes
       double temps = (double) duration_cast<nanoseconds>(t2 - t1).count();
       tempsTotal += temps;
   }
   return tempsTotal / (double)nbrSimulations;
}



// ________________________________________________





int main(int argc, const char * argv[]) {
   srand(time(NULL));
   
   int NBR_SIMULATIONS = 30;
   int BORNE_VALEUR_MAX_1 = 100;
   
   
      cout << "#################" << endl <<
            "Test tri selectif" << endl <<
            "#################" << endl << endl;
       
      
   //test tri séléction pour tailleVecteur = {10^m | m E [1,2,3,4]
   // valeurs entre 1-100
   // Pour cette fonction le temps de calcul est trop long, on va aller jusqu'à 10^4.
   double resultat = 0;
   for(unsigned int tailleVecteur = 10; tailleVecteur <= 10000; tailleVecteur *= 10){
      resultat = testSelection(tailleVecteur, BORNE_VALEUR_MAX_1, NBR_SIMULATIONS); 
      textResultat(BORNE_VALEUR_MAX_1, tailleVecteur, resultat, NBR_SIMULATIONS);
   }
        
   //test tri séléction pour tailleVecteur = 10^3
   // valeurs entre 1 - k ou k = {k^m | k E [1,2,3,4,5,6];
   resultat = 0;
   unsigned int tailleSelection = 1000;
   for(unsigned int borneMax = 10; borneMax  < 1000000; borneMax *= 10){
      resultat = testSelection(tailleSelection, borneMax, NBR_SIMULATIONS); 
      textResultat(borneMax, tailleSelection, resultat, NBR_SIMULATIONS);
   }
   
   cout << endl << "###############" << endl <<
         "Test tri rapide" << endl <<
         "###############" << endl << endl;
   
   //test tri repide pour tailleVecteur = {10^m | m E [1,2,3,4,5,6]
   // valeurs entre 1-100
   // Pour cette fonction le temps de calcul est trop long, on va aller jusqu'à 10^4.
   resultat = 0;
   for(unsigned int tailleVecteur = 10; tailleVecteur <= 1000000; tailleVecteur *= 10){
      resultat = testRapide(tailleVecteur, BORNE_VALEUR_MAX_1, NBR_SIMULATIONS); 
      textResultat(BORNE_VALEUR_MAX_1, tailleVecteur, resultat, NBR_SIMULATIONS);
   }
   
   //test tri rapide pour tailleVecteur = 10^6
   // valeurs entre 1-k ou k = {k^m | k E [1,2,3,4,5,6];
   resultat = 0;
   unsigned int tailleRapide = 1000000;
   for(unsigned int borneMax = 10; borneMax  < 1000000; borneMax *= 10){
      resultat = testRapide(tailleRapide, borneMax, NBR_SIMULATIONS); 
      textResultat(borneMax, tailleRapide, resultat, NBR_SIMULATIONS);
   }
   
      cout << endl << "#################" << endl <<
           "Test tri comptage" << endl <<
           "#################" << endl << endl;
   
   
   
   
   
   
   

    return 0;

}

