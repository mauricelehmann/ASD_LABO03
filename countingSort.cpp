//
//  main.cpp
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "CountingSort.h"

using namespace std;
using namespace asd1;

const size_t V_SIZE = 25;
const size_t V_MAX = 5;

// Affiche le contenu d'un vecteur, en délégant l'affichage des
// éléments à une fonction passée en paramètre
template<typename Container, typename Fn>
void afficher(const string& m, const Container& v, Fn afficher_element) {
  cout << m << ": ";
  for(auto a : v) {
    cout << afficher_element(a) << " ";
  }
  cout << endl;
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

// Fonction principale

#define TEST_NR 5

int main() {

  std::mt19937_64 gen(0);
  std::uniform_int_distribution<unsigned> alea (0,V_MAX);
  RandomPairGenerator<unsigned> rpg(alea,gen);

#if TEST_NR > 0
  {
    cout << "\n** Test 1 - Tri d'incomparables **\n";

    vector<Incomparable> v(V_SIZE), w(V_SIZE);
    generate(v.begin(), v.end(), [&](){ return alea(gen); } );
    afficher("Avant tri",v,valeur);

    CountingSort(v.begin(),v.end(), w.begin(), valeur, alea.max());
    afficher("Apres tri",w,valeur);
  }
#endif

#if TEST_NR > 1
  using E = unsigned int;
  using Pairs = pair<E,E>;
  auto pair2string = p2s<E>;
  auto first_element = first<E>;
  auto second_element = second<E>;

  {
    cout << "\n** Test 2 - Tri de paires par element **\n";

    vector<Pairs> v(V_SIZE), w(V_SIZE);
    generate(v.begin(), v.end(), rpg );
    afficher("Avant tri", v, pair2string);

    CountingSort(v.begin(), v.end(), w.begin(), first_element, rpg.max());
    afficher("Apres tri par premier element", w, first_element);

    CountingSort(v.begin(), v.end(), w.begin(), second_element, rpg.max());
    afficher("Apres tri par second element", w, second_element);
  }
#endif

#if TEST_NR > 2
  {
    cout << "\n** Test 3 - Test de stabilite **\n";

    vector<Pairs> v(V_SIZE), w(V_SIZE);
    generate(v.begin(), v.end(), rpg );
    afficher("Avant tri", v, pair2string);

    CountingSort(v.begin(), v.end(), w.begin(), second_element, rpg.max() );
    CountingSort(w.begin(), w.end(), v.begin(), first_element, rpg.max() );
    afficher("Apres tris second puis premier", v, pair2string);
  }
#endif

#if TEST_NR > 3
  {
    cout << "\n** Test 4 - Tri d'incomparables sans max_key **\n";
    vector<Incomparable> v(V_SIZE), w(V_SIZE);
    generate(v.begin(), v.end(), [&](){ return alea(gen); } );
    afficher("Avant tri",v,valeur);

    CountingSort(v.begin(), v.end(), w.begin(), valeur);
    afficher("Apres tri", w, valeur);
  }
#endif

#if TEST_NR > 4
  {
    std::uniform_int_distribution<unsigned> alea2 (0,numeric_limits<unsigned>::max());

    cout << "\n** Test 5 - Radix sort **\n";
    vector<unsigned> v(100000);
    generate(v.begin(), v.end(), [&](){ return alea2(gen); } );
    RadixSort(v);
    cout << "is_sorted(): " << boolalpha << is_sorted(v.begin(),v.end()) << endl;
  }
#endif

  return 0;
}
