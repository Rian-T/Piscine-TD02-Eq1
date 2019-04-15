#ifndef GRAPHE_H
#define GRAPHE_H
#include <string>
#include <unordered_map>
#include "sommet.h"
#include "edge.h"

class graphe
{
    public:
        ///constructeur qui charge le graphe en mémoire
        //format du fichier ordre/liste des sommets/taille/liste des arêtes
        graphe(std::string, std::string weightFile);
        ~graphe();

    private:
        /// Le réseau est constitué d'une collection de sommets
        std::unordered_map<std::string, Sommet*> m_sommets;//stockée dans une map (clé=id du sommet, valeur= pointeur sur le sommet)
        std::unordered_map<std::string, Edge* > m_edges;
        std::vector<std::vector<bool>> m_sol_admissible;
        std::vector<std::vector<bool>> m_pareto_frontier;
};

#endif // GRAPHE_H
