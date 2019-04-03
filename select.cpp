/*
 -----------------------------------------------------------------------------------
 Laboratoire : ASD_03
 Fichier     : select.cpp
 Auteur(s)   : Maurice Lehmann, Nicolas Hungerbühler, Florian Schaufelberger
 Date        : 02.04.2019

 But         : Déclarations & définitions du tri Selection Sort
               Tests & affichage du tri Selection Sort ( Created by Olivier Cuisenaire on 10.04.15.)
               Copyright (c) 2015 Olivier Cuisenaire. All rights reserved.

 Compilateur : MinGW-g++ 6.3.0
 -----------------------------------------------------------------------------------
*/
#include <iostream>
#include <utility>
#include <climits>
#include <vector>
#include <string>

using namespace std;

// display
//
// Affiche les valeur entre begin et end (non inclus)
// en entourant de [] les valeurs pointées par it1 et
// it2 (it1<=it2) pour souligner les valeurs échangées
// par le tri par sélection
//
// NE RIEN MODIFIER A CETTE FONCTION

template < typename RandomAccessIterator >
void display( const RandomAccessIterator begin,
             const RandomAccessIterator it1,
             const RandomAccessIterator it2,
             const RandomAccessIterator end )
{
    for(auto it = begin; it<it1; ++it) cout << " " << *it << " ";
    if(it1<end) cout << "[" << *it1 << "]";
    for(auto it = it1+1; it<it2; ++it) cout << " " << *it << " ";
    if(it2<end && it1!=it2) cout << "[" << *it2 << "]";
    for(auto it = it2+1; it<end; ++it) cout << " " << *it << " ";
    cout << endl;
}

// selectionSort
//
// Effectue le tri par sélection des éléments entre begin
// et end (non inclus). Doit appeler display() après chaque
// échange.
//
// A COMPLETER

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
        display(begin,itI,iMin,end);
    }
}

// main
//
// Programme testant la mise en oeuvre de selectionSort
// appelle cette fonction avec une string, un tableau
// C d'entiers et un std::vector<double>
//
// NE RIEN MODIFIER A CETTE FONCTION

int main(int argc, const char * argv[]) {

    // std::string

    string s("EXEMPLE_DE_TRI_SELECTION");
    display(s.begin(),s.end(),s.end(),s.end());

    selectionSort(s.begin(),s.end());

    display(s.begin(),s.end(),s.end(),s.end());

    // C array

    int array[] = { 7, 3, 6, 1, 9, 2, 0, 10, 12, -3 };
    cout << endl;
    display(array,array+10,array+10,array+10);

    selectionSort(array,array+10);

    display(array,array+10,array+10,array+10);

    // std::vector

    vector<double> vd { 0.1, 1.2, 3.5, 1.8, 0.4, 10.2, -0.4, 5.8, 6.9, 12.5, 24.3, 0.6, 12.2, 4.5, 3.1415 };
    cout << endl;
    display(vd.begin(),vd.end(),vd.end(),vd.end());

    selectionSort(vd.begin(),vd.end());

    display(vd.begin(),vd.end(),vd.end(),vd.end());

    return 0;

}
