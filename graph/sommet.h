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
        ///constructeur qui reçoit en params les données du sommet
        Sommet(std::string,double,double);
        void ajouterVoisin(const Sommet*, std::vector<float> w);
        std::string getId() const { return m_id; }

        std::vector<const Sommet *> getVoisins() const;

        ~Sommet();

    protected:

    private:
        /// Voisinage : liste d'adjacence
        std::list< std::pair<const Sommet*,std::vector<float>> > m_voisins;

        /// Données spécifiques du sommet
        std::string m_id; // Identifiant
        double m_x, m_y; // Position

};

#endif // SOMMET_H
