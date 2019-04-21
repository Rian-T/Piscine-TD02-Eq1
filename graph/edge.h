#ifndef EDGE_H
#define EDGE_H
#include "sommet.h"

/** \brief \class Edge
    Arete reliant des sommets dans un arbre
    Elle possede un ensemble de poids et 2 sommets
 */
class Edge
{
    public:
        Edge(int, Sommet*, Sommet*, std::vector<float>);
        virtual ~Edge();

        int getId() const { return m_id; }            //GETTERS AND SETTERS
        Sommet* getStart() { return m_start; }
        Sommet* getSecond() { return m_end; }
        std::vector<float> getWeight() { return m_weight;}
        void afficher() const;
        ///pour BFS
        float getWeight(int &pos) { return m_weight[pos];}
        void  setNewFlot(int &pos, float val) {m_weight[pos] = val;}

    protected:

    private:
        const int m_id;
        Sommet* m_start;
        Sommet* m_end;
        std::vector<float> m_weight;
};

#endif // EDGE_H
