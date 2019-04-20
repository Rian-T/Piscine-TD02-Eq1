#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>
#include "graphe.h"
#include "sommet.h"
#include <functional>
#include <queue>


graphe::graphe(std::string nomFichier, std::string weightFile, int ori)
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
    int id;
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
        m_sommets.push_back(new Sommet(id,x,y));
    }
    int taille;
    ifs >> taille;
    m_edge_matrix = (int**) malloc(sizeof(int**)*ordre);
    for(int i=0;i<ordre;i++)
        m_edge_matrix[i] = (int*) malloc(sizeof(int*)*ordre);
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");
    int id_voisin;
    //lecture des aretes
    int id_edge;
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
        m_edges.push_back(new Edge(id_edge,m_sommets[id],m_sommets[id_voisin],tmp_weight));
        m_edge_matrix[id][id_voisin] = id_edge;
        m_edge_matrix[id_voisin][id] = id_edge;
        //ajouter chaque extrémité à la liste des voisins de l'autre (graphe non orienté)
        m_sommets[id]->ajouterVoisin(m_sommets[id_voisin],tmp_weight);
        if(ori==2)
            (m_sommets[id_voisin])->ajouterVoisin(m_sommets[id],tmp_weight);//remove si graphe orienté
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
    std::cout << m_sol_admissible.size() << std::endl;
}

//void graphe::DFS(std::vector<bool> &aretes_local)
//{
//    std::vector<const Sommet *> marked;
//    std::stack<const Sommet *> pile;
//    std::vector<const Sommet*> voisins;
//    const Sommet* sommet_actuelle = m_sommets[0];
//    pile.push(sommet_actuelle);
//    marked.push_back(sommet_actuelle);
//
//    std::vector<int> arete_select;
//    for (size_t i = 0 ; i <aretes_local.size() ; i++)
//    {
//        if (aretes_local[i] == 1)
//        arete_select.push_back(i);
//    }
//
//    do
//    {
//        sommet_actuelle = pile.top();
//        pile.pop();
//        voisins = sommet_actuelle->getVoisins();
//        for (size_t a = 0 ; a < arete_select.size() ; a++)
//        {
//            if ((aretes_local[a] == 1)) ///si arête utilisé dans le graphe
//            {
//                for (size_t i = 0 ; i < voisins.size() ; i ++)
//                {
//                    ///si les sommets appartiennent à l'arête
//                    if ((m_edges[arete_select[a]]->getStart() == sommet_actuelle || m_edges[a]->getSecond() == sommet_actuelle) && (m_edges[arete_select[a]]->getStart() == voisins[i] || m_edges[arete_select[a]]->getSecond() == voisins[i]))
//                    {
//                        bool used = false;
//                        for (size_t j = 0 ; j < marked.size() ; j ++) ///vérifie si le sommet d'arrivé est déjà marqué ou découvert
//                        {
//                            if (marked[j] == voisins[i]) ///le sommet d'arrivé est marqué ou découvert
//                                used = true;
//                        }
//                        if (!used) ///le sommet n'est ni découvert ni marqué
//                        {
//                            marked.push_back(voisins[i]);
//                            pile.push(voisins[i]);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    while (!pile.empty());
//
//    if (marked.size() == m_sommets.size())
//            m_sol_admissible.push_back(aretes_local);
//}


float graphe::max_flot(std::vector<bool> &aretes_local, int posP)
{
    std::vector<int> chemin (m_sommets.size(), 0);
    graphe g = *this; /// graphe résiduel qui copie le graphe complet
    float flot_max = 0;
    int i = 0;

    while (g.BFS(aretes_local,chemin,posP))
    {
        g.BFS(aretes_local,chemin,posP);
        float flot_min = INT_MAX;
        std::vector<int> arete_nb;
        int sommet_nb = m_sommets.size() - 1; /// on part du puit
        while (sommet_nb != 0) /// fin quand le chemin inverse arrive à la source
        {
            for (size_t i = 0 ; i < m_edges.size() ; i++)
            {
                if ((m_edges[i]->getStart() == m_sommets[sommet_nb] || m_edges[i]->getSecond() == m_sommets[sommet_nb]) && (m_edges[i]->getStart() == m_sommets[chemin[sommet_nb]] || m_edges[i]->getSecond() == m_sommets[chemin[sommet_nb]]))
                {
                    flot_min = std::min (flot_min, m_edges[i]->getWeight(posP));
                    arete_nb.push_back(i);
                }
            }
            sommet_nb = chemin[sommet_nb];
        }
        std::cout << std::endl;
        sommet_nb = m_sommets.size() - 1; /// on part du puit
        if(flot_min<INT_MAX){
            while (sommet_nb != 0) /// fin quand le chemin inverse arrive à la source
            {
                for(size_t i = 0 ; i < arete_nb.size(); ++i)
                {
                    ///si sens directe (sachant qu'on est en chemin inverse)
                    if ( (m_edges[arete_nb[i]]->getSecond() == m_sommets[sommet_nb]) && (m_edges[arete_nb[i]]->getStart() == m_sommets[chemin[sommet_nb]]))
                        m_edges[arete_nb[i]]->setNewFlot(posP, m_edges[arete_nb[i]]->getWeight(posP) - flot_min); /// on le descend à 0
                    ///si sens indirecte
                    if ( (m_edges[arete_nb[i]]->getStart() == m_sommets[sommet_nb]) && (m_edges[arete_nb[i]]->getSecond() == m_sommets[chemin[sommet_nb]]))
                         m_edges[arete_nb[i]]->setNewFlot(posP, m_edges[arete_nb[i]]->getWeight(posP) + flot_min); /// on l'augmente
                }
                sommet_nb = chemin[sommet_nb];
            }
            std::cout << std::endl;
            std::cout << std::endl;
            flot_max += flot_min;
        }
        i++;
    }
    //std::cout << "max = " << flot_max << std::endl;
    std::cout<<flot_max<<std::endl;
    return flot_max;
}

bool graphe::BFS(std::vector<bool> &aretes_local, std::vector<int> &chemin, int &posP)
{
    std::vector<const Sommet *> marked;
    std::queue<const Sommet *> file;
    std::vector<const Sommet*> voisins;
    const Sommet* sommet_actuelle = m_sommets[0];
    file.push(sommet_actuelle);
    marked.push_back(sommet_actuelle);
    do
    {
        sommet_actuelle = file.front();
        file.pop();
        voisins = sommet_actuelle->getVoisins();
        for (size_t a = 0 ; a < m_edges.size() ; ++a)
        {
            if ((aretes_local[a] == 1)) ///si arête utilisé dans le graphe
            {
                for (size_t i = 0 ; i < voisins.size() ; i ++)
                {
                    ///si les sommets appartiennent à l'arête
                    if ((m_edges[a]->getStart() == sommet_actuelle || m_edges[a]->getSecond() == sommet_actuelle) && (m_edges[a]->getStart() == voisins[i] || m_edges[a]->getSecond() == voisins[i]))
                    {
                        bool used = false;
                        for (size_t j = 0 ; j < marked.size() ; j ++) ///vérifie si le sommet d'arrivé est déjà marqué ou découvert
                        {
                            if (marked[j] == voisins[i]) ///le sommet d'arrivé est marqué ou découvert
                                used = true;
                        }
                        ///vérifier les flots à partir de là (si le flot de l'arete ou voisin ? n'est pas max mettre dans file)
                        if ((!used) && (m_edges[a]->getWeight(posP) != 0)) ///le sommet n'est ni découvert ni marqué et l'arete est ni saturé
                        {
                            chemin [voisins[i]->getId()] = sommet_actuelle->getId();
                            marked.push_back(voisins[i]);
                            file.push(voisins[i]);
                        }
                    }
                }
            }
        }
    }
    while (!file.empty());
    if (marked[marked.size()-1]->getId() == m_sommets[m_sommets.size() - 1]->getId())
        return true;
    else
        return false;
}


void graphe::DFS(std::vector<bool> &aretes_local)
{
    std::vector<bool> marked(m_sommets.size(),false);
    std::stack<int> pile;
    std::vector<const Sommet*> voisins;
    Sommet* sommet_actuelle = m_sommets[0];
    pile.push(0);
    do
    {
        sommet_actuelle = m_sommets[pile.top()];
        const int id = sommet_actuelle->getId();
        pile.pop();
        voisins = sommet_actuelle->getVoisins();
        const int nb_voisins = voisins.size();
        if(!marked[id]){
                marked[id] = true;
        }
        for(unsigned i = nb_voisins; i--;){
                const int id_voisin= voisins[i]->getId();
                if(aretes_local[m_edge_matrix[id][id_voisin]])
                        if(!marked[id_voisin])
                            pile.push(id_voisin);

        }
    }
    while (!pile.empty());
    if (  std::count(marked.begin(),marked.end(),true) == (int)m_sommets.size())
            m_sol_admissible.push_back(aretes_local);
}

/*
float graphe::max_flot(std::vector<bool> &aretes_local, int posP)
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
        if(flot_min<INT_MAX){
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
            flot_max += flot_min;
        }
        chemin.clear();
    }
    //std::cout << "max = " << flot_max << std::endl;
    return flot_max;
}*/
/*
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

    for (auto &elem : chemin)
    {
        std::cout << elem.first->getId() << " " << elem.second->getId() << std::endl;
    }

    if (marked[marked.size()-1]->getId() == m_sommets.find(std::to_string(m_sommets.size()-1))->second->getId())
        return true;
    else
        return false;
}
 */
void graphe::afficher() const{
    std::cout<<"graphe : "<<std::endl;
    std::cout<<"ordre : "<<m_sommets.size()<<std::endl;
    for(size_t i=0;i<m_sommets.size();i++)
    {
        std::cout<<"sommet : ";
        m_sommets[i]->afficher();
        m_sommets[i]->afficherVoisins();
    }
    std::cout<<std::endl<<std::endl;
    std::cout<<"taille : "<<m_edges.size()<<std::endl;
    for(size_t i=0;i<m_edges.size();i++)
    {
        std::cout<<"arretes : ";
        m_edges[i]->afficher();
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
        Edge* best = nullptr;
        for(size_t i=0;i<m_edges.size();i++)
        {
            const Sommet* st=m_edges[i]->getStart();
            const Sommet* se=m_edges[i]->getSecond();
            if(((marque[st->getId()]==1)&&(marque[se->getId()]==0))||((marque[st->getId()]==0)&&(marque[se->getId()]==1)))
            {
                if(minPoids>m_edges[i]->getWeight()[poids])
                {
                    minPoids=m_edges[i]->getWeight()[poids];
                    best=m_edges[i];
                }
            }
        }
        if(marque[best->getStart()->getId()]==0)
        {
            marque[best->getStart()->getId()]=1;
        }
        if(marque[best->getSecond()->getId()]==0)
        {
            marque[best->getSecond()->getId()]=1;
        }
        nbAjout++;
        prim[best->getId()]=1;
    }
    while (nbAjout<m_sommets.size()-1);
    return prim;
}

float graphe::faireSomme(std::vector<bool> sol_admi,int poids)
{
    float somme=0;
    for(size_t i=0;i<m_edges.size();i++)
    {
        if(sol_admi[i]==1)
        {
            somme=somme+m_edges[i]->getWeight()[poids];
        }
    }
    return somme;
}

float graphe::faireDjikstra(std::vector<bool> sol_admi,int poids)
{
    float tot=0;
    for(size_t i=0;i<m_sommets.size();i++)
    {
        float sous_tot=0;
        std::vector<float>dist(m_sommets.size(),INT_MAX);//distance max
        int temp;
        std::priority_queue<std::pair<float,int>,std::vector<std::pair<float,int>>> pq;//poids et sommet
        pq.push(std::make_pair(0,i));
        dist[i]=0;
        while(pq.size())
        {
            temp=pq.top().second;
            pq.pop();
            for(auto v:m_sommets[temp]->getVoisinsList())
            {
                for(size_t j=0;j<m_edges.size();j++)
                {
                    if(((m_edges[j]->getStart()->getId()==temp)&&(m_edges[j]->getSecond()->getId()==v.first->getId()))||((m_edges[j]->getSecond()->getId()==temp)&&(m_edges[j]->getStart()->getId()==v.first->getId())))
                    {
                        if(sol_admi[j]==1)
                        {
                            if(dist[v.first->getId()]>dist[temp]+v.second[poids])
                            {
                                dist[v.first->getId()]=dist[temp]+v.second[poids];
                                pq.push(std::make_pair(dist[v.first->getId()],v.first->getId()));
                            }
                        }
                    }
                }
            }
        }
        for(size_t j=0;j<dist.size();j++)
        {
            if(dist[j]<INT_MAX)
                sous_tot=sous_tot+dist[j];
        }
        tot=tot+sous_tot;
    }
    return tot;
}

float graphe::faireDjikstra(std::vector<bool> sol_admi,int poids,Sommet* dep, Sommet* arriv)
{
    float tot=0;
    std::vector<float>dist(m_sommets.size(),INT_MAX);//distance max
    int temp;
    std::priority_queue<std::pair<float,int>,std::vector<std::pair<float,int>>> pq;//poids et sommet
    pq.push(std::make_pair(0,dep->getId()));
    dist[dep->getId()]=0;
    while(pq.size())
    {
        temp=pq.top().second;
        pq.pop();
        for(auto v:m_sommets[temp]->getVoisinsList())
        {
            for(size_t j=0;j<m_edges.size();j++)
            {
                if(((m_edges[j]->getStart()->getId()==temp)&&(m_edges[j]->getSecond()->getId()==v.first->getId()))||((m_edges[j]->getSecond()->getId()==temp)&&(m_edges[j]->getStart()->getId()==v.first->getId())))
                {
                    if(sol_admi[j]==1)
                    {
                        if(dist[v.first->getId()]>dist[temp]+v.second[poids])
                        {
                            dist[v.first->getId()]=dist[temp]+v.second[poids];
                            pq.push(std::make_pair(dist[v.first->getId()],v.first->getId()));
                        }
                    }
                }
            }
        }
    }
    tot=dist[arriv->getId()];
    std::cout<<tot<<" ";
    return tot;
}

std::pair<std::vector<std::vector<float>>,std::vector<std::vector<float>>> graphe::fairePareto(std::vector<int> choix_pond,int ori)
{
    std::vector<std::vector<float>> tot_object;
    std::vector<std::vector<float>> tot_object_pareto;
    std::vector<std::vector<float>> tot_object_rest;
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
                if(ori==1)
                    objectif.push_back(faireDjikstra(m_sol_admissible[i],j/*,m_sommets[0],m_sommets[m_sommets.size()-1]*/));
                    else
                        objectif.push_back(faireDjikstra(m_sol_admissible[i],j,m_sommets[0],m_sommets[m_sommets.size()-1]));
                break;
            case 2:
                objectif.push_back(max_flot(m_sol_admissible[i],j));
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
        else
        {
            tot_object_rest.push_back(tot_object[i]);
            //std::cout << " TOT REST : " << tot_object[i][0] << " " <<tot_object[i][1]<< " " <<tot_object[i][2] << std::endl;
        }
    }
    return std::make_pair(tot_object_pareto,tot_object_rest);
}

int graphe::getNbWeight()
{
    return m_edges[0]->getWeight().size();
}


graphe::~graphe()
{
    //dtor
}

