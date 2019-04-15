#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <utility>
#include<unordered_map>
#include<unordered_set>
#include "sommet.h"

Sommet::Sommet(std::string id,double x,double y):m_id{id},m_x{x},m_y{y}
{
}

void Sommet::ajouterVoisin(const Sommet* a,std::vector<float> w){
    m_voisins.push_back({a,w});
}

Sommet::~Sommet()
{
    //dtor
}
