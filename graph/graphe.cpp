#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <thread>
#include <queue>
#include "graphe.h"
#include "sommet.h"
#include "edge.h"
#include <functional>
#include <queue>


void scoring(graphe& g,std::vector<std::vector<float>>& tot_object,std::vector<std::vector<bool>> m_sol_admissible,std::vector<int>& choix_pond,int ori,int debut, int fin);

graphe::graphe(std::string nomFichier, std::string weightFile, int ori) : m_ori(ori), m_nomFichier(nomFichier), m_weightFile(weightFile)
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
            throw std::runtime_error("Probleme lecture donn�es sommet");
        ifs>>x;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture donn�es sommet");
        ifs>>y;
        if(ifs.fail())
            throw std::runtime_error("Probleme lecture donn�es sommet");
        m_sommets.push_back(new Sommet(id,x,y));
    }
    int taille;
    ifs >> taille;
    m_edge_matrix = (int**) malloc(sizeof(int**)*ordre);
    for(int i=0; i<ordre; i++)
        m_edge_matrix[i] = (int*) malloc(sizeof(int*)*ordre);
    for(int i=0; i<ordre; i++)
        for(int j=0; j<ordre; j++)
            m_edge_matrix[i][j] =-1;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");
    int id_voisin;
    //lecture des aretes
    int id_edge;
    int nb_weight;
    ifs2 >> nb_weight;
    for (int i=0; i<taille; ++i)
    {
        //lecture des ids des deux extr�mit�s
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
        //ajouter chaque extr�mit� � la liste des voisins de l'autre (graphe non orient�)
        m_sommets[id]->ajouterVoisin(m_sommets[id_voisin],tmp_weight);
        if(ori==1){
            (m_sommets[id_voisin])->ajouterVoisin(m_sommets[id],tmp_weight);//remove si graphe orient�
             m_edge_matrix[id_voisin][id] = id_edge;
        }
    }
    this->setOrdre(ordre);
    this->setTaille(taille);
}


void graphe::search_sol()
{
    /** \brief Recherche toute les solutions admissibles: les graphes partiels couvrants connexes
 */
    const int k = m_taille;
    const int n = m_ordre;
    const int nb_arete = k-(n-1);
    std::vector<bool > aretes(nb_arete,false); ///aretes affich�s et non affich�s
    for (int i = k-(n-1) ; i < k ; ++i)
    {
            aretes.push_back(true);
    }
    for (int i = 0 ; i <= nb_arete ; ++i ) /// 0 = edg - som + 1 nombre de 0 ; final = 0 nombre de 0
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


void graphe::search_sol2()
{
    /** \brief Recherche toutes les solutions admissibles: Les arbres couvrants
 */
    std::vector<bool > aretes; ///aretes affich�s et non affich�s
    const size_t tarete= m_edges.size();
    const size_t tsommet= m_sommets.size();
    for (size_t i = 0 ; i < tarete; i++)
    {
        if ( i < (tarete -(tsommet-1)) )
            aretes.push_back(false);
        else
            aretes.push_back(true);
    }
    do
    {
        DFS(aretes);
    }
    while(std::next_permutation(aretes.begin(), aretes.end()));
}


float graphe::max_flot(std::vector<bool> &aretes_local, int posP)
{
    /** \brief  Calcul le flot maximal du graphe pond�r�, connexe et orient�
 * \param   std::vector<bool> &aretes_local     Ensemble d'ar�tes exprim�s en bool d'un graphe partiel ou arbre
 * \param   int posP    Indique quel pond�ration a utilis�
 * \return un float qui repr�sente le flot maximal
 */
    std::vector<int> chemin (m_ordre, 0);
    graphe g(this->getFile(),this->getWeightFile(),2);
    float flot_max = 0;

    while (g.BFS(aretes_local,chemin,posP))
    {
        float flot_min = INT_MAX;
        std::vector<int> arete_nb;
        int sommet_nb = g.m_ordre - 1; /// on part du puit
        while (sommet_nb != 0) /// fin quand le chemin inverse arrive � la source
        {
            for (int i = 0 ; i < g.m_taille ; i++)
            {
                if ((g.m_edges[i]->getStart() == g.m_sommets[sommet_nb] || g.m_edges[i]->getSecond() == g.m_sommets[sommet_nb]) && (g.m_edges[i]->getStart() == g.m_sommets[chemin[sommet_nb]] || g.m_edges[i]->getSecond() == g.m_sommets[chemin[sommet_nb]]))
                {
                    flot_min = std::min (flot_min, g.m_edges[i]->getWeight(posP));
                    arete_nb.push_back(i);
                }
            }
            sommet_nb = chemin[sommet_nb];
        }
        sommet_nb = g.m_ordre - 1; /// on part du puit
        if(flot_min<INT_MAX)
        {
            while (sommet_nb != 0) /// fin quand le chemin inverse arrive � la source
            {
                for(size_t i = 0 ; i < arete_nb.size(); ++i)
                {
                    ///si sens directe (sachant qu'on est en chemin inverse)
                    if ( (g.m_edges[arete_nb[i]]->getSecond() == g.m_sommets[sommet_nb]) && (g.m_edges[arete_nb[i]]->getStart() == g.m_sommets[chemin[sommet_nb]]))
                        g.m_edges[arete_nb[i]]->setNewFlot(posP, g.m_edges[arete_nb[i]]->getWeight(posP) - flot_min); /// on le descend � 0
                    ///si sens indirecte
                    if ( (g.m_edges[arete_nb[i]]->getStart() == g.m_sommets[sommet_nb]) && (g.m_edges[arete_nb[i]]->getSecond() == g.m_sommets[chemin[sommet_nb]]))
                        g.m_edges[arete_nb[i]]->setNewFlot(posP, g.m_edges[arete_nb[i]]->getWeight(posP) + flot_min); /// on l'augmente
                }
                sommet_nb = chemin[sommet_nb];
            }
            flot_max += flot_min;
        }
    }
    return flot_max;
}


bool graphe::BFS(std::vector<bool> &aretes_local, std::vector<int> &chemin, int &posP)
{
    /** \brief Parcours en largeur du graphe : permet de savoir s'il existe un chemin vers la destination fix�e
 * \param   std::vector<bool> &aretes_local     Ensemble d'ar�tes exprim�s en bool d'un graphe partiel ou arbre
 * \param   std::vector<int> &chemin            Conteneur contenant les indices des sommets pr�cesseurs des sommets dont leurs indices sont les positions dans le conteneur
 * \param   int &posP       Indique quel pond�ration a utilis�
 * \return un bool qui indique si il existe un chemin permettant d'arriver � la destination fix�e
 */
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
        for (int a = 0 ; a < m_taille ; ++a)
        {
            if ((aretes_local[a] == 1)) ///si ar�te utilis� dans le graphe
            {
                for (size_t i = 0 ; i < voisins.size() ; i ++)
                {
                    ///si les sommets appartiennent � l'ar�te
                    if ((m_edges[a]->getStart() == sommet_actuelle || m_edges[a]->getSecond() == sommet_actuelle) && (m_edges[a]->getStart() == voisins[i] || m_edges[a]->getSecond() == voisins[i]))
                    {
                        bool used = false;
                        for (size_t j = 0 ; j < marked.size() ; j ++) ///v�rifie si le sommet d'arriv� est d�j� marqu� ou d�couvert
                        {
                            if (marked[j] == voisins[i]) ///le sommet d'arriv� est marqu� ou d�couvert
                                used = true;
                        }
                        ///v�rifier les flots � partir de l� (si le flot de l'arete ou voisin ? n'est pas max mettre dans file)
                        if ((!used) && (m_edges[a]->getWeight(posP) != 0)) ///le sommet n'est ni d�couvert ni marqu� et l'arete est ni satur�
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

    if (marked[marked.size()-1]->getId() == m_sommets[m_ordre - 1]->getId())
        return true;
    else
        return false;
}

void graphe::DFS(std::vector<bool> &aretes_local)
{
    /** \brief Parcours en profondeur du graphe : Permet de savoir si une solution est connexe
    * \param   std::vector<bool> &aretes_local     Ensemble d'ar�tes exprim�s en bool d'un graphe partiel ou arbre
    */
    std::vector<bool> marked(m_ordre,false);
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
        if(!marked[id])
        {
            marked[id] = true;
        }
        for(unsigned i = nb_voisins; i--;)
        {
            const int id_voisin= voisins[i]->getId();
            if(aretes_local[m_edge_matrix[id][id_voisin]])
                if(!marked[id_voisin])
                    pile.push(id_voisin);

        }
    }
    while (!pile.empty());
    if (  std::count(marked.begin(),marked.end(),true) == (int)m_ordre)
        m_sol_admissible.push_back(aretes_local);
}


void graphe::afficher() const
{
    /** \brief Affiche les informations du graphe
    *   \details On parcourt tous les sommets et toutes les aretes du graphe et on demande d'afficher leurs informations correspondantes
    *
    */
    std::cout<<"graphe : "<<std::endl;
    std::cout<<"ordre : "<<m_ordre<<std::endl;
    for(int i=0; i<m_ordre; i++)
    {
        std::cout<<"sommet : ";
        m_sommets[i]->afficher();
        m_sommets[i]->afficherVoisins();
    }
    std::cout<<std::endl<<std::endl;
    std::cout<<"taille : "<<m_taille<<std::endl;
    for(int i=0; i<m_taille; i++)
    {
        std::cout<<"arretes : ";
        m_edges[i]->afficher();
    }
}


std::vector<bool> graphe::fairePrim(int poids) const
{
    /** \brief Trouve l'arbre couvrant de poids minimum
    * \details On determine l'arbre couvrant de poids minimum selon une ponderation grace � l'algorithme de Prim
    * \param  poids    Numero de la ponderation sur laquelle appliquer l'algorithme
    * \return Un vecteur de bool�en repr�sente l'arbre couvrant de poids minimum. Chaque case correspond a une arete
    *
    */
    std::vector<bool> prim(m_taille,0);
    std::vector<bool> marque(m_ordre,0);
    marque[0]=1;
    int nbAjout=0;
    do
    {
        int minPoids=INT_MAX;
        Edge* best = nullptr;
        for(int i=0; i<m_taille; i++)
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
    while (nbAjout<m_ordre-1);
    return prim;
}


float graphe::faireSomme(std::vector<bool>& sol_admi,int poids)
{
    /** \brief Calcule la somme des poids des aretes d'un graphe
    * \details On somme tous les poids de meme type
    * \param sol_admi  Vecteur de bool�en repr�sentant un graphe
    * \param poids    Numero de la ponderation sur laquelle appliquer l'algorithme
    * \return Un float repr�sentant la somme des aretes selon une pond�ration
    *
    */
    float somme=0;
    for(int i=0; i<m_taille; i++)
    {
        if(sol_admi[i]==1)
        {
            somme=somme+m_edges[i]->getWeight()[poids];
        }
    }
    return somme;
}


float graphe::faireDjikstra(std::vector<bool>& sol_admi,int poids)
{
    /** \brief Calcul la somme des plus courts chemins entre tous les points
    * \details On applique sur chaque sommet l'algorithme de Djikstra et
    *          on somme les plus courts chemins entre ce sommet et tous les autres
    *          Pour appliquer l'algorithme de Djikstra on utilise une priority queue
    * \param sol_admi  Vecteur de bool�en repr�sentant un graphe
    * \param poids    Numero de la ponderation sur laquelle appliquer l'algorithme
    * \return Un float repr�sentant la somme des plus courts chemins entre tous les points
    *
    */
    float tot=0;
    for(int i=0; i<m_ordre; i++)
    {
        float sous_tot=0;
        std::vector<float>dist(m_ordre,INT_MAX);//distance max
        int temp;
        std::priority_queue<std::pair<float,int>,std::vector<std::pair<float,int>>> pq;//poids et sommet
        pq.push(std::make_pair(0,i));
        dist[i]=0;
        while(!pq.empty())
        {
            temp=pq.top().second;
            pq.pop();
            auto voisins = m_sommets[temp]->getVoisinsList();
            for(auto v : voisins)
            {
                const int v_id = v.first->getId();
                if(m_edge_matrix[temp][v_id]!=-1 && sol_admi[m_edge_matrix[temp][v_id]])

                {
                        if(dist[v_id]>dist[temp]+v.second[poids])
                        {
                            dist[v_id]=dist[temp]+v.second[poids];
                            pq.push(std::make_pair(dist[v_id],v_id));
                        }
                }
            }
        }
        const size_t tdist=dist.size();
        for(size_t j=0;j<tdist;j++)
        {
            if(dist[j]<INT_MAX){
                sous_tot=sous_tot+dist[j];
            }
        }
        tot=tot+sous_tot;
    }
    return tot;
}


float graphe::faireDjikstra(std::vector<bool>& sol_admi,int poids,Sommet* dep, Sommet* arriv)
{
    /** \brief Calcul le plus court chemin entre 2 points
    * \details On applique l'algorithme de Djikstra pour obtenir le plus court chemin
    *          entre un point de depart et un point d'arrivee. On utilise une priority queue pour appliquer Djikstra
    * \param sol_admi  Vecteur de bool�en repr�sentant un graphe
    * \param poids    Numero de la ponderation sur laquelle appliquer l'algorithme
    * \param dep   Sommet de depart
    * \param arriv     Sommet d'arriv�e
    * \return Un float repr�sentant la somme du plus court chemin
    *
    */
    float tot=0;
    std::vector<float>dist(m_ordre,INT_MAX);//distance max
    int temp;
    std::priority_queue<std::pair<float,int>,std::vector<std::pair<float,int>>> pq;//poids et sommet
    pq.push(std::make_pair(0,dep->getId()));
    dist[dep->getId()]=0;
    while(!pq.empty())
    {
        temp=pq.top().second;
        pq.pop();
        for(auto v:m_sommets[temp]->getVoisinsList())
        {
            for(int j=0; j<m_taille; j++)
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
    return tot;
}


void scoring(graphe& g,std::vector<std::vector<float>>& tot_object,std::vector<std::vector<bool>> m_sol_admissible,std::vector<int>& choix_pond,int ori,int debut, int fin){
    /** \brief Calcule pour chaque graphe partiel admissible ses differentes composantes
 *  \details Pour chaque graphe partiel admissible, on applique selon le type de ponderation un calcul specifique.
 *           On obtient 2 ou 3 donnees pour chaque solution, que l'on va pouvoir compar�
 *           avec les autres pour trouver la frontiere de Pareto.
 * \param g     Le graphe initial
 * \param tot_object    Un vecteur de vecteur de float repr�sentant l'ensemble des groupes de donnees de chaque solution
 * \param m_sol_admissible      Un vecteur de vecteur de booleen repr�sentant l'ensemble des graphes partiel qui sont des solutions possibles
 * \param choix_pond    Un vecteur d'entier qui contient l'ensemble des types des ponderations qui peuvent etre, des couts, des distances ou des capacit�s
 * \param ori   Entier designant si le graphe est orient� ou non
 * \param debut Numero de test
 * \param fin   Numero de test
 *
 */
    std::cout << "DEBUT : " << debut << " FIN : " << fin << std::endl;
    for(int i=debut; i<fin; i++){
        std::vector<float> objectif;
        for(size_t j=0; j<choix_pond.size(); j++)
            {
                switch(choix_pond[j])
                {
                case 0:
                    objectif.push_back(g.faireSomme(m_sol_admissible[i],j));
                    break;
                case 1:
                    if(ori==1)
                        objectif.push_back(g.faireDjikstra(m_sol_admissible[i],j/*,m_sommets[0],m_sommets[m_ordre-1]*/));
                        else
                            objectif.push_back(g.faireDjikstra(m_sol_admissible[i],j,g.getSommet()[0],g.getSommet()[g.getSommet().size()-1]));
                    break;
                case 2:
                    objectif.push_back(g.max_flot(m_sol_admissible[i],j));
                    break;
                }
            }
            tot_object[i] = objectif;
    }
}


std::pair<std::vector<std::vector<float>>,std::vector<std::vector<float>>> graphe::fairePareto(std::vector<int>& choix_pond,int ori)
{

    /** \brief Methode pour trouver les optimums de pareto, utilise le multithreading selon les capacite de l'ordinateur pour optimiser
 *
 */
    std::vector<std::vector<float>> tot_object(m_sol_admissible.size());
    std::vector<std::vector<float>> tot_object_pareto;
    std::vector<std::vector<float>> tot_object_rest;
    std::cout << "NUMBER OF THREAD : " << std::thread::hardware_concurrency() << std::endl;
    if(std::thread::hardware_concurrency() <= 2){
        std::thread t1(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,0,m_sol_admissible.size()/2);
        std::thread t2(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,m_sol_admissible.size()/2,m_sol_admissible.size());
        t1.join();
        t2.join();
    }
    else if(std::thread::hardware_concurrency() <= 4){
        std::thread t1(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,0,m_sol_admissible.size()/4);
        std::thread t2(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,m_sol_admissible.size()/4,m_sol_admissible.size()/2);
        std::thread t3(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,m_sol_admissible.size()/2,(m_sol_admissible.size()*3)/4);
        std::thread t4(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*3)/4,m_sol_admissible.size());
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    else if(std::thread::hardware_concurrency() <= 8){
        std::thread t1(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,0,m_sol_admissible.size()/8);
        std::thread t2(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,m_sol_admissible.size()/8,(m_sol_admissible.size()*2)/8);
        std::thread t3(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*2)/8,(m_sol_admissible.size()*3)/8);
        std::thread t4(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*3)/8,(m_sol_admissible.size()*4)/8);
        std::thread t5(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*4)/8,(m_sol_admissible.size()*5)/8);
        std::thread t6(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*5)/8,(m_sol_admissible.size()*6)/8);
        std::thread t7(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*6)/8,(m_sol_admissible.size()*7)/8);
        std::thread t8(scoring,std::ref(*this),std::ref(tot_object),m_sol_admissible,std::ref(choix_pond),ori,(m_sol_admissible.size()*7)/8,m_sol_admissible.size());
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
    }
    std::vector<bool> marque(m_sol_admissible.size(),0);
    const size_t tsoladmi=m_sol_admissible.size();
    const size_t tchoixpond=choix_pond.size();
    for(size_t i=0;i<tsoladmi;i++)
    {
        if(marque[i]==0)
        {
            for(size_t j=0;j<tsoladmi;j++)
            {
                if((marque[j]==0)&&(j!=i))
                {
                    unsigned int verif=0;
                    for(size_t k=0;k<tchoixpond;k++)
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
                    if(verif==tchoixpond)
                    {
                        marque[j]=1;
                    }
                }
            }
        }
    }
    for(size_t i=0;i<tsoladmi;i++)
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


void graphe::dessinerGraphe(Svgfile &svgout, std::vector<bool> &arete, double ecart_x, double ecart_y, int ori)
{
    /** \brief Dessine un graphe/arbre sur le document .svg
 * \param   Svgfile &svgout     Affiche les graphes sur le fichier .svg
 * \param   std::vector<bool> &arete        Ensemble d'ar�tes exprim�s en bool d'un graphe partiel ou arbre
 * \param   double &ecart_x     Distance entre le sommet aillant la plus petite valeur x et celui avec la plus grande valeur x
 * \param   double &ecart_y     Distance entre le sommet aillant la plus petite valeur y et celui avec la plus grande valeur y
 * \param   int ori             Indique si le graphe est orient� ou non
 */
    double origine_x = ecart_x;
    double origine_y = ecart_y;
    double image_size= 50;
    double decaler = 25;
    size_t ordre = m_ordre;
    size_t taille = m_taille;
    size_t val_max = std::max(m_ordre,m_taille);

    for (size_t i = val_max; i!=0; --i)
    {
        if ( i <= ordre)
            svgout.addImage("images/village.png", image_size,image_size,m_sommets[i-1]->getX()-decaler+origine_x,m_sommets[i-1]->getY()-decaler+origine_y);
        if ( i <= taille)
        {
            if (arete[i-1] == 1 )
            {
                int s1_x = m_edges[i-1]->getStart()->getX()+origine_x;
                int s1_y = m_edges[i-1]->getStart()->getY()+origine_y;
                int s2_x = m_edges[i-1]->getSecond()->getX()+origine_x;
                int s2_y = m_edges[i-1]->getSecond()->getY()+origine_y;
                svgout.addLine(s1_x,s1_y,s2_x,s2_y,"LightSalmon");
                if(ori==2)
                {
                    svgout.addDisk(s1_x+(s2_x-s1_x)*3/4,s1_y+(s2_y-s1_y)*3/4,5,"LightSalmon");
                }
                int x_min = std::min(s1_x, s2_x);
                int y_min = std::min(s1_y, s2_y);
                std::vector<float> poids = m_edges[i-1]->getWeight();

                std::string ponderation = "(";
                for (size_t j = 0 ; j < poids.size() ; ++j)
                {
                    std::string tmp = std::to_string(poids[j]);
                    tmp = tmp.substr(0,1);
                    ponderation += tmp;
                    if (j != poids.size() - 1 )
                        ponderation += ";";
                }
                ponderation += ")";
                svgout.addText(x_min + std::abs(s1_x - s2_x)/2 - 20, y_min + std::abs(s1_y - s2_y)/2 - 2, ponderation, "red");
            }
        }
    }
}


 void graphe::InitialisationDonneeAffichageSvg(double &ecart_x, double &ecart_y)
{
    /** \brief Calcul la distance entre le sommet aillant la plus petite valeur x et celui avec la plus grande valeur x et �galement pour y
 * \param   double &ecart_x     Distance entre le sommet aillant la plus petite valeur x et celui avec la plus grande valeur x
 * \param   double &ecart_y     Distance entre le sommet aillant la plus petite valeur y et celui avec la plus grande valeur y
 */
    double ordre = m_ordre;
    double x_min = 100000;
    double x_max = 0;
    double y_min = 100000;
    double y_max = 0;
    for (size_t i = ordre; i!= 0 ; --i)
    {
        x_min = std::min(x_min,m_sommets[i-1]->getX());
        x_max = std::max(x_max,m_sommets[i-1]->getX());
        y_min = std::min(y_min,m_sommets[i-1]->getY());
        y_max = std::max(y_max,m_sommets[i-1]->getY());
    }
    ecart_x = (x_max - x_min); ///distance du graphe complet en x
    std::cout << ecart_x << std::endl;
    ecart_y = (y_max - y_min); ///distance du graphe complet en y
}


void graphe::dessinerGrapheOrg(Svgfile &svgout, double &ecart_x,int ori)
{
    /** \brief  Affiche le graphe du fichier sur le document .svg
 * \param   Svgfile &svgout     Affiche les graphes sur le fichier .svg
 * \param   double &ecart_x     Distance entre le sommet aillant la plus petite valeur x et celui avec la plus grande valeur x
 * \param   int ori             Indique si le graphe est orient� ou non
 */
    std::vector<bool> org(m_taille, 1);
    ///graphe d'origine
    svgout.addText(890,800+50, "Graphe initial :");
    dessinerGraphe(svgout,org,((svgout.getWidth()/3)*2)-300 - ecart_x/2 - m_sommets[0]->getX(),800,ori);
}

int graphe::getNbWeight()
{
    return m_edges[0]->getWeight().size();
}


void graphe::dessinerGraphesPareto(Svgfile &svgout,double &ecart_x, double &ecart_y,std::vector<std::vector<float>> &valtot,int ori)
{
    /** \brief  Affiche chaque solution de la fronti�re pareto sur le document .svg
 * \param   Svgfile &svgout     Affiche les graphes sur le fichier .svg
 * \param   double &ecart_x     Distance entre le sommet aillant la plus petite valeur x et celui avec la plus grande valeur x
 * \param   double &ecart_y     Distance entre le sommet aillant la plus petite valeur y et celui avec la plus grande valeur y
 * \param   std::vector<std::vector<float>> &valtot     Conteneur qui contient pour chaque solution de la fronti�re pareto : les valeurs des objectifs
 * \param   int ori             Indique si le graphe est orient� ou non
 */
    int qte = m_pareto_frontier.size();
    int diviseur = 3;
    int qte_rst = 0;
    int cpt_x = 1;
    svgout.addLine(100,800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,1800,800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,"black");
    svgout.addText(825,800+(200+ecart_y)*svgout.getCptY()+50+1.5*ecart_y, "Solutions sur la Frontiere de Pareto :");

    if (qte >= 3)
    {
        while (qte%diviseur != 0) /// si pas divisible
        {
            qte--;
            ++qte_rst;
        }
        ///affichage
        for (int i = 0 ; i < qte ; ++i)
        {
            std::string msg = "(";
            for (size_t j = 0 ; j <  valtot[i].size(); ++j)
            {
                std::string tmp = std::to_string(valtot[i][j]);
                tmp = tmp.substr(0,5);
                msg += tmp;
                if (j != valtot[i].size() - 1)
                    msg+= ";";
            }
            msg += ")";
            svgout.addText(((svgout.getWidth()/3)*cpt_x)-300 - ecart_x/2 + m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY() + 70+1.5*ecart_y, msg, "black");
            dessinerGraphe(svgout,m_pareto_frontier[i],((svgout.getWidth()/3)*cpt_x)-300 - ecart_x/2 - m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,ori);
            if (cpt_x == 3)
            {
                svgout.incremCptY();
                cpt_x = 0;
            }
            cpt_x++;
        }
    }
    else
        qte_rst = qte;
    cpt_x = 1;
    if (qte_rst== 1)
    {
        if (qte == 1) /// cas il existe qu'un graphe
            qte = 0;
        std::string msg = "(";
        for (size_t j = 0 ; j <  valtot[qte].size(); ++j)
        {
            std::string tmp = std::to_string(valtot[qte][j]);

            tmp = tmp.substr(0,5);
            msg += tmp;
            if (j != valtot[qte].size() - 1)
                msg+= ";";
        }
        msg += ")";
        svgout.addText((svgout.getWidth()/2) - ecart_x/2 + m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY() + 70+1.5*ecart_y, msg, "black");
        dessinerGraphe(svgout,m_pareto_frontier[qte],(svgout.getWidth()/2) - ecart_x/2 - m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,ori);
        svgout.incremCptY(); ///pour les affichages suivants, s'il y en a
    }
    if (qte_rst == 2)
    {
        int empl;
        if (qte == 2) /// cas o� il existe 2 graphes
            empl = 0;
        else          /// cas o� il reste encore 2 graphes
            empl = qte;
        for (int i = 0; i < 2; ++i)
        {
            std::string msg = "(";
            for (size_t j = 0 ; j <  valtot[i].size(); ++j)
            {
                std::string tmp = std::to_string(valtot[i][j]);
                tmp = tmp.substr(0,5);
                msg += tmp;
                if (j != valtot[i].size() - 1)
                    msg+= ";";
            }
            msg += ")";
            svgout.addText(((svgout.getWidth()/3)*cpt_x) - ecart_x/2 + m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY() + 70+1.5*ecart_y, msg, "black");
            dessinerGraphe(svgout,m_pareto_frontier[empl+i],((svgout.getWidth()/3)*cpt_x) - ecart_x/2 - m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y, ori);
            ++cpt_x;
        }
        svgout.incremCptY(); ///pour les affichages suivants, s'il y en a
    }

}


void graphe::dessinerGraphesPrim(Svgfile &svgout, double &ecart_x, double &ecart_y, std::vector<std::vector<bool>> &arbres, std::vector<std::vector<float>> &couts, int ori)
{
    /** \brief  Affiche chaque arbre couvrant de poids minimal de chaque pond�ration du fichier et du graphe choisi sur le document .svg
 * \param   Svgfile &svgout     Affiche les graphes sur le fichier .svg
 * \param   double &ecart_x     Distance entre le sommet aillant la plus petite valeur x et celui avec la plus grande valeur x
 * \param   double &ecart_y     Distance entre le sommet aillant la plus petite valeur y et celui avec la plus grande valeur y
 * \param   std::vector<std::vector<bool>> &arbres     Conteneur qui contient pour chaque arbre : tout les ar�tes exprim�s en bool, qui permet la r�alisation d'un arbre
 * \param   std::vector<std::vector<float>> &couts     Conteneur qui contient pour chaque arbre : la somme des couts pour chaque pond�ration du fichier de poids
 * \param   int ori             Indique si le graphe est orient� ou non
 */
    int qte = arbres.size(); ///nombre d'arbres � afficher
    int diviseur = 3;
    int qte_rst = 0;
    int cpt_x = 1;
    svgout.addLine(100,800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,1800,800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,"black");
    svgout.addText(870,800+(200+ecart_y)*svgout.getCptY()+50+1.5*ecart_y, "Arbres issu de Prim :");
    if (qte >= 3)
    {
        while (qte%diviseur != 0) /// si pas divisible
        {
            qte--;
            ++qte_rst;
        }
        for (int i = 0 ; i < qte ; ++i)
        {
            std::string msg = "(";
            for (size_t j = 0 ; j <  couts[i].size(); ++j)
            {
                std::string tmp = std::to_string(couts[i][j]);
                tmp = tmp.substr(0,5);
                msg += tmp;
                if (j != couts[i].size() - 1)
                    msg+= ";";
            }
            msg += ")";
            svgout.addText(((svgout.getWidth()/3)*cpt_x)-300 - ecart_x/2 + m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY() + 70+1.5*ecart_y, msg, "black");
            dessinerGraphe(svgout,arbres[i],((svgout.getWidth()/3)*cpt_x)-300 - ecart_x/2 - m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,ori);
            if (cpt_x == 3)
            {
                svgout.incremCptY();
                cpt_x = 0;
            }
            cpt_x++;
        }
    }
    else
        qte_rst = qte; ///si 1 : il existe un graphe, si 2 : il existe 2 graphes
    cpt_x = 1;
    if (qte_rst== 1)
    {
        if (qte == 1) ///dans le cas o� il n'existe qu'un graphe
            qte = 0;
        std::string msg = "(";
        for (size_t j = 0 ; j <  couts[qte].size(); ++j)
        {
            std::string tmp = std::to_string(couts[qte][j]);
            tmp = tmp.substr(0,5);
            msg += tmp;
            if (j != couts[qte].size() - 1)
                msg+= ";";
        }
        msg += ")";
        svgout.addText((svgout.getWidth()/2) - ecart_x/2 + m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY() + 70+1.5*ecart_y, msg, "black");
        dessinerGraphe(svgout,arbres[qte],(svgout.getWidth()/2) - ecart_x/2 - m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,ori);
        svgout.incremCptY(); ///pour les affichages suivants, s'il y en a
    }
    if (qte_rst == 2)
    {
        int empl;
        if (qte == 2) /// cas o� il existe 2 graphes
            empl = 0;
        else          /// cas o� il reste encore 2 graphes
            empl = qte;
        for (int i = 0; i < 2; ++i)
        {
            std::string msg = "(";
            for (size_t j = 0 ; j <  couts[i].size(); ++j)
            {
                std::string tmp = std::to_string(couts[i][j]);
                tmp = tmp.substr(0,5);
                msg += tmp;
                if (j != couts[i].size() - 1)
                    msg+= ";";
            }
            msg += ")";
            svgout.addText(((svgout.getWidth()/3)*cpt_x) - ecart_x/2 +  m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY() + 70+1.5*ecart_y, msg, "black");
            dessinerGraphe(svgout,arbres[empl+i],((svgout.getWidth()/3)*cpt_x) - ecart_x/2 - m_sommets[0]->getX(), 800+(200+ecart_y)*svgout.getCptY()+1.5*ecart_y,ori);
            ++cpt_x;
        }
        svgout.incremCptY(); ///pour les affichages suivants, s'il y en a
    }
}
graphe::~graphe()
{
    //dtor
}

