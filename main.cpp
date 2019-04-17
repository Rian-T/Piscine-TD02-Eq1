#include <iostream>
#include "graphe.h"
#include "plot/ploting.h"

int main()
{
    graphe g("files/broadway.txt","files/broadway_weights_0.txt");
    std::cout << "Graphe charge !" << std::endl;
    std::cout << "Affichage de la frontiere de pareto : " << std::endl;
    return 0;
}
