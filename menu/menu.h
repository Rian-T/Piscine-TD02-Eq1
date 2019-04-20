#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>

std::string choixFichier();
std::string choixWeight();
int choixOrient();
int choixAction();
std::vector<int> choixPoids(size_t tailleP,int ori);


#endif // MENU_H_INCLUDED
