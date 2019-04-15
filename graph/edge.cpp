#include "edge.h"
#include "sommet.h"

Edge::Edge(std::string id, Sommet* a, Sommet*b,std::vector<float> weight): m_id(id), m_start(a), m_end(b), m_weight(weight)
{
    //ctor
}

Edge::~Edge()
{
    //dtor
}
