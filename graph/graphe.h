#ifndef GRAPHE_H
#define GRAPHE_H
#include <string>
#include <unordered_map>
#include "sommet.h"
#include "edge.h"
#include "../plot/svgfile.h"

class graphe
{
    public:
        ///constructeur qui charge le graphe en mémoire
        //format du fichier ordre/liste des sommets/taille/liste des arêtes
        graphe(std::string, std::string weightFile,int);

        void search_sol();
        void DFS(std::vector<bool> &arete_local);

        float max_flot(std::vector<bool> &aretes_local, int posP);
        bool BFS(std::vector<bool> &arete_local, std::unordered_map<const Sommet* ,const Sommet*> & chemin, int &posP);
        ~graphe();
        bool BFS(std::vector<bool> &aretes_local, std::vector<int> &chemin, int &posP);
        void afficher() const;
        void setOrdre(int o){m_ordre = o;}
        void setTaille(int t){m_taille = t;}
        int getNbWeight();
        std::string getFile()const {return m_nomFichier;}
        std::string getWeightFile()const {return m_weightFile;}
        std::vector<Sommet*> getSommet()const {return m_sommets;}
        float faireDjikstra(std::vector<bool>&,int poids);
        float faireDjikstra(std::vector<bool>& sol_admi,int poids,Sommet* dep, Sommet* arriv);
        std::vector<bool> fairePrim(int) const;
        float faireSomme(std::vector<bool>&,int);
        std::pair<std::vector<std::vector<float>>,std::vector<std::vector<float>>> fairePareto(std::vector<int>& choix_pond, int ori);

        //Svgfile
        void InitialisationDonneeAffichageSvg(double &ecart_x, double &ecart_y);
        void dessinerGrapheOrg(Svgfile &svgout, double &ecart_x);
        void dessinerGraphe(Svgfile &svgout, std::vector<bool> &aretes, double ecart_x,double ecart_y);
        void dessinerGraphesPareto(Svgfile &svgout, double &ecart_x, double &ecart_y,std::vector<std::vector<float>> &valtot );
        void dessinerGraphesPrim(Svgfile &svgout,double &ecart_x, double &ecart_u, std::vector<std::vector<bool>> &arbres, std::vector<std::vector<float>> &couts);
    private:
        /// Le réseau est constitué d'une collection de sommets
        std::vector<Sommet*> m_sommets;//stockée dans une map (clé=id du sommet, valeur= pointeur sur le sommet)
        std::vector<Edge* > m_edges;
        int** m_edge_matrix;
        const int m_ori;
        int m_ordre;
        int m_taille;
        std::vector<std::vector<bool>> m_sol_admissible;
        std::vector<std::vector<bool>> m_pareto_frontier;
        std::string m_nomFichier,m_weightFile;
};

#endif // GRAPHE_H
