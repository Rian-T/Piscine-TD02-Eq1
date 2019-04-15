#include <fstream>
#include <iostream>
#include "graphe.h"

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
        //(m_sommets.find(id_voisin))->second->ajouterVoisin((m_sommets.find(id))->second);//remove si graphe orienté
    }
}


graphe::~graphe()
{
    //dtor
}
