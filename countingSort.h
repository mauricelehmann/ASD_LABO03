/*
 -----------------------------------------------------------------------------------
 Laboratoire : ASD_03
 Fichier     : CountingSort.h
 Auteur(s)   : Maurice Lehmann, Nicolas Hungerbühler, Florian Schaufelberger
 Date        : 02.04.2019

 But         : Déclarations & définitions des tris suivants :
             - Tri comptage générique
             - Radix Sort ( avec des unsigned int ), utilisant le tri comptage 4 fois

 Compilateur : MinGW-g++ 6.3.0
 -----------------------------------------------------------------------------------
*/
#ifndef CountingSort_h
#define CountingSort_h

#include <vector>
#include <algorithm>
#include <iostream>
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
}

#endif /* CountingSort_h */
