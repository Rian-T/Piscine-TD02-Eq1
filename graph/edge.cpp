#include "edge.h"
#include "sommet.h"
#include <iostream>

Edge::Edge(int id, Sommet* a, Sommet*b,std::vector<float> weight)
: m_id(id), m_start(a), m_end(b), m_weight(weight)
{
    //ctor
}


void Edge::afficher() const
{
    /** \brief Affiche les informations de l'arete
 *
 */
    std::cout<<m_id<<std::endl;
    m_start->afficher();
    m_end->afficher();
    for(auto w:m_weight)
    {
        std::cout<<w<<"   ";
    }
    std::cout<<std::endl;
}

Edge::~Edge()
{
    //dtor
}
