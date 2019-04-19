#include <iostream>
#include "graphe.h"
#include "plot/ploting.h"
#include <vector>
#include "plot/svgfile.h"
#include <ctime>
#include <ratio>
#include <chrono>

int main()
{
    using namespace std::chrono;
    Svgfile svgout;
    float som1;
    high_resolution_clock::time_point start_point = high_resolution_clock::now();
    graphe g("files/manhattan.txt","files/manhattan_weights_0.txt");
    g.search_sol();
    std::cout << "Graphe charge !" << std::endl;
    std::cout << "Affichage de la frontiere de pareto : " << std::endl;
    auto pareto = g.fairePareto({0,1});
    high_resolution_clock::time_point end_point = high_resolution_clock::now();
    duration<double> time_elpased = duration_cast<duration<double>>(end_point - start_point);
    std::cout << std::endl << "Time of computation elapsed : " << time_elpased.count() << " s" << std::endl;
    std::cout << std::endl << "PARETO :" << pareto.first.size() << std::endl;
    plotPareto2D(pareto.first);
    //printPareto3D(pareto.first,pareto.second,svgout);
    return 0;
}
