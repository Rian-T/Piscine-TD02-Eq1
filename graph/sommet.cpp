#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <utility>
#include<unordered_map>
#include<unordered_set>
#include "sommet.h"

Sommet::Sommet(int id,double x,double y):m_id{id},m_x{x},m_y{y}
{

}

void Sommet::ajouterVoisin(const Sommet* a,std::vector<float> w){
    m_voisins.push_back({a,w});
}


void Sommet::afficher() const
{
    /** \brief Affiche les coordonnees du sommet
 *
 */
    std::cout<<m_id<<"   "<<m_x<<"  "<<m_y<<"  "<<std::endl;
}


void Sommet::afficherVoisins() const
{
    /** \brief Affiche les informations des voisins du sommet
 *
 */
    std::cout<<"voisins :"<<std::endl;
    for(auto v:m_voisins)
    {
        v.first->afficher();
        for(auto w:v.second)
        {
            std::cout<<w<<"   ";
        }
        std::cout<<std::endl;
    }
}


std::vector<const Sommet *> Sommet::getVoisins() const
{
    std::vector<const Sommet *> voisins;
    for (auto &elem : m_voisins)
    {
        voisins.push_back(elem.first);
    }
    return voisins;
}

Sommet::~Sommet()
{
    //dtor
}
