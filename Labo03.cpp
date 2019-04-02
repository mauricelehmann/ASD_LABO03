#include <iostream>
#include <utility>
#include <climits>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <ctime>
#include <chrono>

#include "countingSort.h"

using namespace std;
using namespace chrono;
using namespace asd1;

// Classe encapsulant un entier non signé. Elle ne met pas
// en oeuvre les opérateurs de comparaison tels que <, >, ...
class Incomparable {
public:
  unsigned i;
  Incomparable(unsigned _i = 0) : i(_i) { }
};

// Fonction utilisée pour afficher les Incomparables, et pour
// calculer la clé entière correspondant à un incomparable.
unsigned valeur(Incomparable inc) {
  return inc.i;
}

// Functor créant une paire d'entiers aléatoires à partir d'une distribution
template<typename T>
class RandomPairGenerator {
private:
  std::uniform_int_distribution<T>& dis;
  std::mt19937_64& gen;
public:
  RandomPairGenerator(std::uniform_int_distribution<T>& _dis,
                      std::mt19937_64& _gen): dis(_dis), gen(_gen)
  { }
  pair<T,T> operator() () const {
    return make_pair<T,T>( dis(gen), dis(gen) );
  }
  T max() const {
    return dis.max();
  }
};

// fonctions d'aide pour le tri de paires.
template<typename T>
string p2s(const pair<T,T>& p) {
  return to_string(p.first) + "," + to_string(p.second);
}

template<typename T>
T first(pair<T,T>& p) {
  return p.first;
}

template<typename T>
T second(pair<T,T>& p) {
  return p.second;
}

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
template<typename T>
vector<T> creationVecteurValeurRandom(size_t& taille, size_t valeurMax) {
  vector<T> v(taille);
  for(int i = 0; i < taille; ++i) {
    v.at(i) = rand() % valeurMax + 1;
  }
  return v;
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

// quickSort
//
// Effectue le tri rapide des éléments entre begin
// et end (non inclus). Doit appeler selectPivot(...)
// pour le choix du pivot, et display() après chaque
// partition

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

//________________________________________
//
//Fonctions de tests des algorithmes de tris
//________________________________________

enum class TRIS{SELECTION_SORT, QUICK_SORT, COUNTING_SORT};

void textResultat(unsigned int borneMax, unsigned int taille, double resultat,  unsigned int nbrSimulations){
         cout << "Pour un vecteur de taille : " << taille << " de valeur entre 1 a "
              << borneMax << " [compris]. " << endl
              << "Nombres de simulations      : " << nbrSimulations << endl
              << "Temps moyen en nanosecondes : " << resultat << "[ns]" << endl
              << "Temps moyen en seconde      : " << resultat / 1000000000 << "[s] " << endl;
}
template<typename T>
double testsTri(size_t taille, size_t borneMax, const size_t nbrSimulations, TRIS tri ){
   double tempsTotal = 0;
   for(size_t i = 0; i < nbrSimulations; i++){
       vector<T> vValeurRandom = creationVecteurValeurRandom<T>(taille, borneMax);
      //prendre le moment de départ
       high_resolution_clock::time_point t1 = high_resolution_clock::now();
       //exécuter les opérations à chronométrer ici
       switch (tri) {
           case TRIS::QUICK_SORT: quickSort(vValeurRandom.begin(),vValeurRandom.end()); break;
           case TRIS::SELECTION_SORT: selectionSort(vValeurRandom.begin(),vValeurRandom.end()); break;
       }
       //prendre le moment d’arrivée
       high_resolution_clock::time_point t2 = high_resolution_clock::now();
       //calcul du temps, ici en nanosecondes
       double temps = (double) duration_cast<nanoseconds>(t2 - t1).count();
       tempsTotal += temps;
   }
   return tempsTotal / (double)nbrSimulations;
}

double testTriComptage(const size_t& tailleVecteur,const size_t& NBR_SIMULATIONS, const size_t& V_MAX){

    std::mt19937_64 gen(0);
    std::uniform_int_distribution<unsigned> alea (0,V_MAX);
    RandomPairGenerator<unsigned> rpg(alea,gen);

    using E = unsigned int;
    using Pairs = pair<E,E>;
    auto pair2string = p2s<E>;
    auto first_element = first<E>;
    auto second_element = second<E>;

    vector<Pairs> v(tailleVecteur), w(tailleVecteur);

    double tempsTotal = 0;

    for(size_t i = 0; i < NBR_SIMULATIONS; i++){
        generate(v.begin(), v.end(), rpg );
       //prendre le moment de départ
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        //exécuter les opérations à chronométrer ici
        CountingSort(v.begin(), v.end(), w.begin(), first_element, rpg.max());
        //prendre le moment d’arrivée
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        //calcul du temps, ici en nanosecondes
        double temps = (double) duration_cast<nanoseconds>(t2 - t1).count();
        tempsTotal += temps;
    }
    return tempsTotal;
}

double testRadix (size_t taille, size_t borneMax, const size_t nbrSimulations){
   double tempsTotal = 0;
   for(size_t i = 0; i < nbrSimulations; i++){
      vector<unsigned int> vValeurRandom = creationVecteurValeurRandom<unsigned int>(taille, borneMax);
      //prendre le moment de départ
       high_resolution_clock::time_point t1 = high_resolution_clock::now();
       //exécuter les opérations à chronométrer ici
       RadixSort(vValeurRandom);
       //prendre le moment d’arrivée
       high_resolution_clock::time_point t2 = high_resolution_clock::now();
       //calcul du temps, ici en nanosecondes
       double temps = (double) duration_cast<nanoseconds>(t2 - t1).count();
       tempsTotal += temps;
   }
   return tempsTotal / (double)nbrSimulations;
}

int main() {

   srand(time(NULL));
   const size_t NBR_SIMULATIONS = 30;
   const size_t VALEUR_MAX = 100;
   const size_t TAILLE_MAX = 1000000;
   double resultat = 0;

   cout << "#################\nTest tri selectif\n#################\n" ;

   // Tests tri séléction pour tailleVecteur = {10^m | m ∈ [1,2,3,4]
   // valeurs entre 1-100
   // Pour cette fonction le temps de calcul est trop long, on va aller jusqu'à 10^4.

   for(size_t tailleVecteur = 10; tailleVecteur <= 10000; tailleVecteur *= 10){
      resultat = testsTri<int>(tailleVecteur, VALEUR_MAX, NBR_SIMULATIONS,TRIS::SELECTION_SORT);
      textResultat(VALEUR_MAX, tailleVecteur, resultat, NBR_SIMULATIONS);
   }

  cout << "#################\nTest tri rapide\n#################\n" ;

   //test tri rapide pour tailleVecteur = {10^m | m ∈ [1,2,3,4,5,6]
   // valeurs entre 1-100.
   resultat = 0;
   for(size_t tailleVecteur = 10; tailleVecteur <= TAILLE_MAX; tailleVecteur *= 10){
      resultat = testsTri<int>(tailleVecteur, VALEUR_MAX, NBR_SIMULATIONS,TRIS::QUICK_SORT);
      textResultat(VALEUR_MAX, tailleVecteur, resultat, NBR_SIMULATIONS);
   }

   //test tri rapide pour tailleVecteur = 10^6
   // valeurs entre 1-k ou k = {k^m | k ∈ [1,2,3,4,5,6];
   resultat = 0;
   for(size_t borneMax = 10; borneMax  < TAILLE_MAX; borneMax *= 10){
      resultat = testsTri<int>(TAILLE_MAX, borneMax, NBR_SIMULATIONS,TRIS::QUICK_SORT);
      textResultat(borneMax, TAILLE_MAX, resultat, NBR_SIMULATIONS);
   }

   cout << "#################\nTest tri comptage\n#################\n" ;

   //test tri de comptage pour tailleVecteur = {10^m | m ∈ [1,2,3,4,5,6]
   // valeurs entre 1-100.
   size_t V_MAX = 100;
   for(size_t tailleVecteur = 10; tailleVecteur <= TAILLE_MAX; tailleVecteur *= 10){
       double tempsTotal = testTriComptage(tailleVecteur,NBR_SIMULATIONS,V_MAX);
       tempsTotal /= (double)NBR_SIMULATIONS;
       textResultat(VALEUR_MAX, tailleVecteur, tempsTotal,  NBR_SIMULATIONS);
   }

   //test tri de comptage pour tailleVecteur = 10^6
   // valeurs entre 1-k ou k = {k^m | k ∈ [1,2,3,4,5,6];
   for(size_t borneMax = 10; borneMax <= TAILLE_MAX; borneMax *= 10){
       double tempsTotal = testTriComptage(TAILLE_MAX,NBR_SIMULATIONS,borneMax);
       tempsTotal /= (double)NBR_SIMULATIONS;
       textResultat(borneMax, TAILLE_MAX, tempsTotal, NBR_SIMULATIONS);
   }

   cout << "#################\nTest tri radix\n#################\n" ;

   //test tri radix pour tailleVecteur = {10^m | m ∈ [1,2,3,4,5,6]
   // valeurs entre 1-100.
   resultat = 0;
   for(size_t tailleVecteur = 10; tailleVecteur <= TAILLE_MAX; tailleVecteur *= 10){
      resultat = testRadix(tailleVecteur, VALEUR_MAX, NBR_SIMULATIONS);
      textResultat(VALEUR_MAX, tailleVecteur, resultat, NBR_SIMULATIONS);
   }

   //test tri radix pour tailleVecteur = 10^6
   // valeurs entre 1-k ou k = {k^m | k ∈ [1,2,3,4,5,6];
   size_t TAILLE_MAX_RADIX = 1000000;
   resultat = 0;
   for(size_t borneMax = 10; borneMax  < TAILLE_MAX_RADIX; borneMax *= 10){
      resultat = testRadix(TAILLE_MAX_RADIX, borneMax, NBR_SIMULATIONS);
      textResultat(borneMax, TAILLE_MAX_RADIX, resultat, NBR_SIMULATIONS);
   }

   return EXIT_SUCCESS;
}
