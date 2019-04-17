#ifndef SOMMET_H
#define SOMMET_H
#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <unordered_set>

class Sommet
{
    public:
        ///constructeur qui re�oit en params les donn�es du sommet
        Sommet(std::string,double,double);
        std::string getId() const { return m_id; }
        void ajouterVoisin(const Sommet*, std::vector<float> w);
        void afficher() const;
        std::list<std::pair<const Sommet*,std::vector<float>>> getVoisins(){return m_voisins;}
        void afficherVoisins() const;
        ~Sommet();

    protected:

    private:
        /// Voisinage : liste d'adjacence
        std::list<std::pair<const Sommet*,std::vector<float>>> m_voisins;

        /// Donn�es sp�cifiques du sommet
        std::string m_id; // Identifiant
        double m_x, m_y; // Position


};

#endif // SOMMET_H
