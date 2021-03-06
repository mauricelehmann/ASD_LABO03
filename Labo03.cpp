/*
 -----------------------------------------------------------------------------------
 Laboratoire : ASD_03
 Fichier     : labo03.cpp
 Auteur(s)   : Maurice Lehmann, Nicolas Hungerbühler, Florian Schaufelberger
 Date        : 02.04.2019

 But         : Mesurer la durée en temps des algorithmes de tris implémentés dans le cadre du labo ADS03
               Les tris sont : Quick Sort , Selection Sort, Counting Sort & Radix Sort
               Les mesures sont basées sur une moyenne de 30 simulations.
               Chaque simulation utilise un vecteur à trier, rempli de valeurs aléatoires.
               On test deux cas de figure :
               - Vecteurs avec n entiers positif aléatoires entre 1 et 100 (inclus), avec n = {10^m | m ∈ [1,2, … ,6]}
               - Vecteurs de taille n = 100'000 entiers positifs aléatoire entre 1 et k (inclus), avec k = {10^m | m ∈ [1,2, … ,6]}

 Compilateur : MinGW-g++ 6.3.0
 -----------------------------------------------------------------------------------
*/

#include <iostream>
#include <utility>
#include <climits>
#include <vector>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <type_traits>
#include <algorithm>

using namespace std;
using namespace chrono;

  /**
   Tri comptage générique

   https://en.wikipedia.org/wiki/Counting_sort

   @param first  [first,last] est la plage d'éléments d'un tableau à trier.
   @param last   [first,last] est la plage d'éléments d'un tableau à trier.
   @param output début du tableau où écrire le résultat. Doit être différent
                 du tableau d'entrée
   @param key fonction prenant un élément en entrée et retourant sa position
              dans le tableau de comptage
   @param max_key valeur maximale pouvant être retournée par key(...). Si -1,
                  la fonction calcule cette valeur en parcourant le tableau une
                  fois de plus
   */
  template<typename RandomAccessIterator, typename Fn>
  void CountingSort(RandomAccessIterator first,
                  RandomAccessIterator last,
                  RandomAccessIterator output,
                  Fn key,
                  size_t max_key = -1)
  {
    //La valeur maximume de l'input n'a pas été définie, on doit le chercher
    if(max_key == -1){
        max_key = 0;
        for(auto i = first ; i != last ; i++){
            if(key(*i) > max_key ){
                max_key = key(*i);
            }
        }
    }
    //On initialise un tableau de comptage, de taille égal à max_key
    //Toute les éléments sont initialisées à zéro
    std::vector<std::size_t> countingTab(max_key+1,0);
    //On compte les éléments du vecteur à trier
    for(auto i = first ; i != last ; i++ ){
        ++countingTab.at(key(*i));
    }
    //On additione chaques éléments avec le précedent
    for(auto i = countingTab.begin() + 1; i != countingTab.end(); i++){
        *i += *(i-1);
    }

    //shift tout les éléments de 1 à droite
    std::rotate(countingTab.begin(),countingTab.end()-1,countingTab.end());
    //le premier élément doit être à zéro
    countingTab.front() = 0;

    //Pour chaque éléments du tableau à trier:
    size_t j;
    for(auto i = first; i != last ; i++){
        //j est la position dans le nouveau tableau (ici output) de l'élément *i
        j = countingTab.at(key(*i));
        *(output + j) = *i ;
        ++countingTab.at(key(*i));
    }
  }

  /**
   Tri par base d'entiers non signés sur 32 bits mis en oeuvre en appelant
   4 fois le tri comptage en triant successivement par groupe de 8 bits.

   https://en.wikipedia.org/wiki/Radix_sort

   @param v vecteur à trier, modifié par cette fonction
   */
  void RadixSort(std::vector<unsigned int>& v)
  {
      std::vector<unsigned int> w(v.size());
      const size_t MASK_SIZE = 0xff; //masque sur 8 bits
      const size_t MAX_KEY = 0b11111111; //valeur maximale pour 8 bits
      //On "trie comptage" 4 fois sur les groupes de 8 bits.
      //La fonction lambda s'occupe de selectioner la bonne range de bits de l'unsigned int
      for(size_t i = 0 ; i < 4 ; i++){
        CountingSort(v.begin(),v.end(),w.begin(),[i,MASK_SIZE](const unsigned& val ){ return ((val >> (i * 8)) & MASK_SIZE);},MAX_KEY);
        v = w;
     }
  }

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

void textResultat(unsigned int borneMax, unsigned int taille, double resultat,  unsigned int nbrSimulations){

    //nanoseconds in seconds
    const size_t nsPerS = 1000000000;
    cout << "Pour un vecteur de taille : " << taille << " de valeur entre 1 a "
      << borneMax << " [compris]. " << endl
      << "Nombres de simulations      : " << nbrSimulations << endl
      << "Temps moyen en nanosecondes : " << resultat << "[ns]" << endl
      << "Temps moyen en seconde      : " << resultat / nsPerS << "[s] " << endl;
}

template<typename T, typename Fn>
double testsTri(size_t taille, size_t borneMax, const size_t nbrSimulations, Fn tri ){
   double tempsTotal = 0;
   for(size_t i = 0; i < nbrSimulations; i++){
       vector<T> vValeurRandom = creationVecteurValeurRandom<T>(taille, borneMax);
      //prendre le moment de départ
       high_resolution_clock::time_point t1 = high_resolution_clock::now();
       //exécuter les opérations à chronométrer ici
       tri(vValeurRandom.begin(),vValeurRandom.end());
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

   #define vectorIntIt vector<int>::iterator
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
      resultat = testsTri<int>(tailleVecteur, VALEUR_MAX, NBR_SIMULATIONS,selectionSort<vectorIntIt>);
      textResultat(VALEUR_MAX, tailleVecteur, resultat, NBR_SIMULATIONS);
   }

  cout << "#################\nTest tri rapide\n#################\n" ;

   //test tri rapide pour tailleVecteur = {10^m | m ∈ [1,2,3,4,5,6]
   // valeurs entre 1-100.
   resultat = 0;
   for(size_t tailleVecteur = 10; tailleVecteur <= TAILLE_MAX; tailleVecteur *= 10){
      resultat = testsTri<int>(tailleVecteur, VALEUR_MAX, NBR_SIMULATIONS,quickSort<vectorIntIt>);
      textResultat(VALEUR_MAX, tailleVecteur, resultat, NBR_SIMULATIONS);
   }

   //test tri rapide pour tailleVecteur = 10^6
   // valeurs entre 1-k ou k = {k^m | k ∈ [1,2,3,4,5,6];
   resultat = 0;
   for(size_t borneMax = 10; borneMax  < TAILLE_MAX; borneMax *= 10){
      resultat = testsTri<int>(TAILLE_MAX, borneMax, NBR_SIMULATIONS,quickSort<vectorIntIt>);
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