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

void Sommet::afficher() const
{
    std::cout<<m_id<<"   "<<m_x<<"  "<<m_y<<"  "<<std::endl;
}

void Sommet::afficherVoisins() const
{
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

/*
std::vector<const Sommet *> Sommet::getVoisins() const
{
    std::vector<const Sommet *> voisins;
    for (auto &elem : m_voisins)
    {
        voisins.push_back(elem.first);
    }
    return voisins;
}

int Sommet::connexe(int nb,)
{
    int nb_sommet=1;
    std::stack<const Sommet*>pile;
    std::vector<const Sommet*>marque(nb,0);
    const Sommet* top;
    pile.emplace(this);
    marque[m_id]=1;
    while(pile.size()!=0)
    {
        top=pile.top();
        pile.pop();
        for(auto v:top->m_voisins)
        {
            if(marque[v->first->getId()]==0)
            {
                marque[v->first->getId()]==0
                pile.emplace(v);
                nb_sommet++;
            }
        }
    }
    return nb_sommet;
}*/

Sommet::~Sommet()
{
    //dtor
}
