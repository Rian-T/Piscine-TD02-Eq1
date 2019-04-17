#ifndef EDGE_H
#define EDGE_H
#include "sommet.h"

class Edge
{
    public:
        Edge(std::string, Sommet*, Sommet*, std::vector<float>);
        virtual ~Edge();

        std::string getId() const { return m_id; }            //GETTERS AND SETTERS
        const Sommet* getStart() { return m_start; }
        const Sommet* getSecond() { return m_end; }
        std::vector<float> getWeight() { return m_weight;}
        ///pour BFS
        float getWeight(int &pos) { return m_weight[pos];}
        void  setNewFlot(int &pos, float val) {m_weight[pos] = val;}

    protected:

    private:
        const std::string m_id;
        const Sommet* m_start;
        const Sommet* m_end;
        std::vector<float> m_weight;
};

#endif // EDGE_H
