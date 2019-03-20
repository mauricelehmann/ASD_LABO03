//
//  CountingSort.h
//

#ifndef CountingSort_h
#define CountingSort_h

#include <vector>
#include <algorithm>
#include <type_traits>

namespace asd1 {

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
// a compléter
  }

  /**
   Tri par base d'entiers non signés sur 32 bits mis en oeuvre en appelant
   4 fois le tri comptage en triant successivement par groupe de 8 bits.

   https://en.wikipedia.org/wiki/Radix_sort

   @param v vecteur à trier, modifié par cette fonction
   */
  void RadixSort(std::vector<unsigned int>& v)
  {
// a compléter
  }
}

#endif /* CountingSort_h */
