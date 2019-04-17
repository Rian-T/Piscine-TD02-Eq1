#include <iostream>
#include "graphe.h"

#include <vector>
int main()
{
    graphe g("files/broadway.txt","files/broadway_weights_0.txt");
    //graphe g("files/cubetown.txt","files/cubetown_weights_0.txt");
    //graphe g("files/triville.txt","files/triville_weights_0.txt");
    //graphe g("files/manhattan.txt","files/manhattan_weights_0.txt");

    //g.search_sol();
    std::vector<bool> o = {1,1,1,1,1};
    std::unordered_map<const Sommet*,const Sommet* > c;
    int pos = 1;
    std::cout << g.max_flot(o,pos) << std::endl;

    return 0;
}
