#include <fstream>
#include <iostream>
#include "graphe.h"
#include <functional>
#include <queue>

graphe::graphe(std::string nomFichier, std::string weightFile){
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
    for (int i=0; i<ordre; ++i){
        ifs>>id; if(ifs.fail()) throw std::runtime_error("Probleme lecture données sommet");
        ifs>>x; if(ifs.fail()) throw std::runtime_error("Probleme lecture données sommet");
        ifs>>y; if(ifs.fail()) throw std::runtime_error("Probleme lecture données sommet");
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
    for (int i=0; i<taille; ++i){
        //lecture des ids des deux extrémités
        ifs>>id_edge;
        ifs2>>id_edge;
        ifs>>id; if(ifs.fail()) throw std::runtime_error("Probleme lecture arete sommet 1");
        ifs>>id_voisin; if(ifs.fail()) throw std::runtime_error("Probleme lecture arete sommet 2");
        std::vector<float> tmp_weight;
        for(int i=0;i<nb_weight;i++){
            float tmp;
            ifs2>>tmp;
            tmp_weight.push_back(tmp);
        }
        m_edges.insert({id_edge,new Edge(id_edge,m_sommets.find(id)->second,m_sommets.find(id_voisin)->second,tmp_weight)});
        //ajouter chaque extrémité à la liste des voisins de l'autre (graphe non orienté)
        (m_sommets.find(id))->second->ajouterVoisin((m_sommets.find(id_voisin))->second,tmp_weight);
        //(m_sommets.find(id_voisin))->second->ajouterVoisin((m_sommets.find(id))->second,tmp_weight);//remove si graphe orienté
    }
}


void graphe::afficher() const{
    std::cout<<"graphe : "<<std::endl;
    std::cout<<"ordre : "<<m_sommets.size()<<std::endl;
    for(const auto& elem : m_sommets)
    {
        std::cout<<"sommet : ";
        elem.second->afficher();
        elem.second->afficherVoisins();
    }
    std::cout<<std::endl<<std::endl;
    std::cout<<"taille : "<<m_edges.size()<<std::endl;
    for(const auto& ed : m_edges)
    {
        std::cout<<"arretes : ";
        ed.second->afficher();
    }
}

std::vector<bool> graphe::fairePrim(int poids) const
{
    std::vector<bool> prim(m_edges.size(),0);
    std::vector<bool> marque(m_sommets.size(),0);
    marque[0]=1;
    size_t nbAjout=0;
    do
    {
        int minPoids=INT_MAX;
        Edge* best;
        for(auto e:m_edges)
        {
            const Sommet* st=e.second->getStart();
            const Sommet* se=e.second->getSecond();
            if(((marque[stoi(st->getId())]==1)&&(marque[stoi(se->getId())]==0))||((marque[stoi(st->getId())]==0)&&(marque[stoi(se->getId())]==1)))
            {
                if(minPoids>e.second->getWeight()[poids])
                {
                    minPoids=e.second->getWeight()[poids];
                    best=e.second;
                }
            }
        }
        if(marque[stoi(best->getStart()->getId())]==0)
        {
            marque[stoi(best->getStart()->getId())]=1;
        }
        if(marque[stoi(best->getSecond()->getId())]==0)
        {
            marque[stoi(best->getSecond()->getId())]=1;
        }
        nbAjout++;
        prim[m_edges.size()-1-stoi(best->getId())]=1;
    }
    while (nbAjout<m_sommets.size()-1);
    return prim;
}

float graphe::faireSomme(std::vector<bool> sol_admi,int poids)
{
    float somme=0;
    for(auto edg:m_edges)
    {
        if(sol_admi[m_edges.size()-1-stoi(edg.first)]==1)
        {
            somme=somme+edg.second->getWeight()[poids];
        }
    }
    return somme;
}

float graphe::faireDjikstra(std::vector<bool> sol_admi,int poids)
{
    float tot=0;
    for(auto s0: m_sommets)
    {
        float sous_tot=0;
        std::vector<float>dist(m_sommets.size(),INT_MAX);//distance max
        int temp;
        std::priority_queue<std::pair<float,int>,std::vector<std::pair<float,int>>/*,std::greater<std::pair<float,int>*/> pq;//poids et sommet
        pq.push(std::make_pair(0,stoi(s0.second->getId())));
        dist[stoi(s0.second->getId())]=0;
        while(pq.size())
        {
            temp=pq.top().second;
            pq.pop();
            for(auto v:m_sommets.find(std::to_string(temp))->second->getVoisins())
            {
                for(auto edg:m_edges)
                {
                    if(((std::stoi(edg.second->getStart()->getId())==temp)&&(std::stoi(edg.second->getSecond()->getId())==std::stoi(v.first->getId())))||((std::stoi(edg.second->getSecond()->getId())==temp)&&(std::stoi(edg.second->getStart()->getId())==std::stoi(v.first->getId()))))
                    {
                        if(sol_admi[stoi(edg.first)]==1)
                        {
                            if(dist[stoi(v.first->getId())]>dist[temp]+v.second[poids])
                            {
                                dist[stoi(v.first->getId())]=dist[temp]+v.second[poids];
                                pq.push(std::make_pair(dist[stoi(v.first->getId())],stoi(v.first->getId())));
                            }
                        }
                    }
                }
            }
        }
        for(size_t i=0;i<dist.size();i++)
        {
            sous_tot=sous_tot+dist[i];
        }
        tot=tot+sous_tot;
    }
    return tot;
}

std::vector<std::vector<float>> graphe::fairePareto(std::vector<int> choix_pond)
{
    std::vector<std::vector<float>> tot_object;
    std::vector<std::vector<float>> tot_object_pareto;
    for(size_t i=0;i<m_sol_admissible.size();i++)
    {
        std::vector<float> objectif;
        for(size_t j=0;j<choix_pond.size();j++)
        {
            switch(choix_pond[j])
            {
            case 0:
                objectif.push_back(faireSomme(m_sol_admissible[i],j));
                break;
            case 1:
                objectif.push_back(faireDjikstra(m_sol_admissible[i],j));
                break;
            case 2:
                //objectif.push_back(faireMaxflux(m_sol_admissible[i],j));
                break;
            }
        }
        tot_object.push_back(objectif);
    }
    std::vector<bool> marque(m_sol_admissible.size(),0);
    for(size_t i=0;i<tot_object.size();i++)
    {
        if(marque[i]==0)
        {
            for(size_t j=0;j<tot_object.size();j++)
            {
                if((marque[j]==0)&&(j!=i))
                {
                    unsigned int verif=0;
                    for(size_t k=0;k<choix_pond.size();k++)
                    {
                        switch(choix_pond[k])
                        {
                            case 2:
                                if(tot_object[i][k]>=tot_object[j][k])
                                {
                                    verif=verif+1;
                                }
                                break;
                            default:
                                if(tot_object[i][k]<=tot_object[j][k])
                                {
                                    verif=verif+1;
                                }
                                break;
                        }
                    }
                    if(verif==choix_pond.size())
                    {
                        marque[j]=1;
                    }
                }
            }
        }
    }
    for(size_t i=0;i<tot_object.size();i++)
    {
        if(marque[i]==0)
        {
            m_pareto_frontier.push_back(m_sol_admissible[i]);
            tot_object_pareto.push_back(tot_object[i]);
        }
    }
    return tot_object_pareto;
}


graphe::~graphe()
{
    //dtor
}

