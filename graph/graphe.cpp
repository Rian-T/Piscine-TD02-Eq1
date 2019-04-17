#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>
#include "graphe.h"


graphe::graphe(std::string nomFichier, std::string weightFile)
{
    std::ifstream ifs{nomFichier};
    std::ifstream ifs2(weightFile);
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    if (!ifs2)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + weightFile );
    int ordre,ordre2;
    ifs >> ordre;
    ifs2 >> ordre2;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");
    std::string id;
    double x,y;
    //lecture des sommets
    for (int i=0; i<ordre; ++i)
    {
        ifs>>id;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture données sommet");
        ifs>>x;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture données sommet");
        ifs>>y;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture données sommet");
        m_sommets.insert({id,new Sommet{id,x,y}});
    }
    int taille;
    ifs >> taille;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");
    std::string id_voisin;
    //lecture des aretes
    std::string id_edge;
    int nb_weight;
    ifs2 >> nb_weight;
    for (int i=0; i<taille; ++i)
    {
        //lecture des ids des deux extrémités
        ifs>>id_edge;
        ifs2>>id_edge;
        ifs>>id;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture arete sommet 1");
        ifs>>id_voisin;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture arete sommet 2");
        std::vector<float> tmp_weight;
        for(int i=0; i<nb_weight; i++)
        {
            float tmp;
            ifs2>>tmp;
            tmp_weight.push_back(tmp);
        }
        m_edges.insert({id_edge,new Edge(id_edge,m_sommets.find(id)->second,m_sommets.find(id_voisin)->second,tmp_weight)});
        //ajouter chaque extrémité à la liste des voisins de l'autre (graphe non orienté)
        (m_sommets.find(id))->second->ajouterVoisin((m_sommets.find(id_voisin))->second,tmp_weight);
        (m_sommets.find(id_voisin))->second->ajouterVoisin((m_sommets.find(id))->second,tmp_weight);//remove si graphe orienté
    }
}

void graphe::search_sol()
{
    std::vector<bool > aretes; ///aretes affichés et non affichés
    for (size_t i = 0 ; i < m_edges.size() ; i++)
    {
        if ( i < (m_edges.size() -(m_sommets.size()-1)) )
            aretes.push_back(false);
        else
            aretes.push_back(true);
    }
    for (size_t i = 0 ; i <= (m_edges.size()-(m_sommets.size()-1)) ; i++ ) /// 0 = edg - som + 1 nombre de 0 ; final = 0 nombre de 0
    {
        do
        {
            DFS(aretes);
        }
        while(std::next_permutation(aretes.begin(), aretes.end()));
        aretes.erase(aretes.begin() + 0);
        aretes.push_back(true);
    }
}

void graphe::DFS(std::vector<bool> &aretes_local)
{
    std::vector<const Sommet *> marked;
    std::stack<const Sommet *> pile;
    std::vector<const Sommet*> voisins;
    const Sommet* sommet_actuelle = m_sommets.find("0")->second;
    pile.push(sommet_actuelle);
    marked.push_back(sommet_actuelle);
    do
    {
        sommet_actuelle = pile.top();
        pile.pop();
        voisins = sommet_actuelle->getVoisins();
        for (auto &elem : m_edges)
        {
            if ((aretes_local[ (m_edges.size()-1) - std::stoi (elem.first)] == 1)) ///si arête utilisé dans le graphe
            {
                for (size_t i = 0 ; i < voisins.size() ; i ++)
                {
                    ///si les sommets appartiennent à l'arête
                    if ((elem.second->getStart() == sommet_actuelle || elem.second->getSecond() == sommet_actuelle) && (elem.second->getStart() == voisins[i] || elem.second->getSecond() == voisins[i]))
                    {
                        bool used = false;
                        for (size_t j = 0 ; j < marked.size() ; j ++) ///vérifie si le sommet d'arrivé est déjà marqué ou découvert
                        {
                            if (marked[j] == voisins[i]) ///le sommet d'arrivé est marqué ou découvert
                                used = true;
                        }
                        if (!used) ///le sommet n'est ni découvert ni marqué
                        {
                            marked.push_back(voisins[i]);
                            pile.push(voisins[i]);
                        }
                    }
                }
            }
        }
    }
    while (!pile.empty());

    if (marked.size() == m_sommets.size())
        m_sol_admissible.push_back(aretes_local);
}

float graphe::max_flot(std::vector<bool> &aretes_local, int &posP)
{
    std::unordered_map<const Sommet*, const Sommet*> chemin;
    graphe g = *this; /// graphe résiduel qui copie le graphe complet
    float flot_max = 0;
    while (g.BFS(aretes_local,chemin,posP))
    {
        float flot_min = INT_MAX;
        const Sommet * sommet_actuelle;
        sommet_actuelle = m_sommets.find(std::to_string(m_sommets.size()-1))->second; /// on part du puit

        while (sommet_actuelle != m_sommets.find("0")->second) /// fin quand le chemin inverse arrive à la source
        {
            for (auto &elem : g.m_edges)
            {
                if ((elem.second->getStart() == sommet_actuelle || elem.second->getSecond() == sommet_actuelle) && (elem.second->getStart() == chemin.find(sommet_actuelle)->second || elem.second->getSecond() == chemin.find(sommet_actuelle)->second))
                    flot_min = std::min ( flot_min, elem.second->getWeight(posP));
            }
            sommet_actuelle = chemin.find(sommet_actuelle)->second;
        }
        sommet_actuelle = m_sommets.find(std::to_string(m_sommets.size()-1))->second; /// on part du puit
        while (sommet_actuelle != m_sommets.find("0")->second) /// fin quand le chemin inverse arrive à la source
        {
            for (auto &elem : m_edges)
            {///si directe : (sachant qu'on est en chemin inverse)
                if ((elem.second->getSecond() == sommet_actuelle) && (elem.second->getStart() == chemin.find(sommet_actuelle)->second))
                    elem.second->setNewFlot(posP,elem.second->getWeight(posP) - flot_min);
             ///si inverse : (sachant qu'on est en chemin inverse)
                if ((elem.second->getStart() == sommet_actuelle) && (elem.second->getSecond() == chemin.find(sommet_actuelle)->second))
                    elem.second->setNewFlot(posP, elem.second->getWeight(posP) +flot_min);
            }
            sommet_actuelle = chemin.find(sommet_actuelle)->second;
        }
        chemin.clear();
        flot_max += flot_min;
    }
    return flot_max;
    std::cout << "max = " << flot_max << std::endl;
}
bool graphe::BFS(std::vector<bool> &aretes_local, std::unordered_map<const Sommet *,const Sommet *> &chemin, int &posP)
{
    std::vector<const Sommet *> marked;
    std::queue<const Sommet *> file;
    std::vector<const Sommet*> voisins;
    const Sommet* sommet_actuelle = m_sommets.find("0")->second;
    file.push(sommet_actuelle);
    marked.push_back(sommet_actuelle);
    do
    {
        sommet_actuelle = file.front();
        file.pop();
        voisins = sommet_actuelle->getVoisins();
        for (auto &elem : m_edges)
        {
            if ((aretes_local[(m_edges.size()-1) - std::stoi (elem.first)] == 1)) ///si arête utilisé dans le graphe
            {
                for (size_t i = 0 ; i < voisins.size() ; i ++)
                {
                    ///si les sommets appartiennent à l'arête
                    if ((elem.second->getStart() == sommet_actuelle || elem.second->getSecond() == sommet_actuelle) && (elem.second->getStart() == voisins[i] || elem.second->getSecond() == voisins[i]))
                    {
                        bool used = false;
                        for (size_t j = 0 ; j < marked.size() ; j ++) ///vérifie si le sommet d'arrivé est déjà marqué ou découvert
                        {
                            if (marked[j] == voisins[i]) ///le sommet d'arrivé est marqué ou découvert
                                used = true;
                        }
                        ///vérifier les flots à partir de là (si le flot de l'arete ou voisin ? n'est pas max mettre dans file)
                        if ((!used) && (elem.second->getWeight(posP) != 0)) ///le sommet n'est ni découvert ni marqué et l'arete est ni saturé
                        {
                            chemin.insert({voisins[i],sommet_actuelle});
                            marked.push_back(voisins[i]);
                            file.push(voisins[i]);
                        }
                    }
                }
            }
        }
    }
    while (!file.empty());
    ///----------------
    /*
    for (auto &elem : chemin)
    {
        std::cout << elem.first->getId() << " " << elem.second->getId() << std::endl;
    }
    */
    ///----------------
    if (marked[marked.size()-1]->getId() == m_sommets.find(std::to_string(m_sommets.size()-1))->second->getId())
        return true;
    else
        return false;
}
graphe::~graphe()
{
    //dtor
}
