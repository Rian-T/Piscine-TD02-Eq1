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
    Svgfile svgout;
    svgout.addGrid();
    svgout.addLine(500,0,500,800,"black");
    svgout.addLine(1300,0,1300,800,"black");

    std::string a = "files/broadway.txt";
    std::string aa = "files/broadway_weights_0.txt";
    /*
    std::string a = "files/cubetown.txt";
    std::string aa = "files/cubetown_weights_0.txt";
    /*
    std::string a = "files/triville.txt";
    std::string aa = "files/triville_weights_0.txt";
    */
    /*
    std::string a = "files/manhattan.txt";
    std::string aa = "files/manhattan_weights_0.txt";
*/

    graphe g(a,aa);
    g.search_sol();
    //std::pair< std::vector<std::vector<float>>, std::vector<std::vector<float>>> p_pareto;
    //p_pareto = g.fairePareto({0,0});
    //printPareto3D(p_pareto.first,p_pareto.second,svgout);
    g.dessiner(svgout);
    //g.dessinerGrapheOrg(svgout);
    //g.dessinerGraphePrim(svgout,1);
    return 0;
}

/*
int main()
{
    using namespace std::chrono;
    Svgfile svgout;
    high_resolution_clock::time_point start_point = high_resolution_clock::now();


    graphe g("files/broadway.txt","files/broadway_weights_0.txt");
    g.search_sol();
    g.test();
    std::cout << "Graphe charge !" << std::endl;
    high_resolution_clock::time_point end_point = high_resolution_clock::now();
    duration<double> time_elpased = duration_cast<duration<double>>(end_point - start_point);
    std::cout << std::endl << "Time for generatating all connected subgraph elapsed : " << time_elpased.count() << " s" << std::endl;
    std::cout << "Affichage de la frontiere de pareto : " << std::endl;
    auto pareto = g.fairePareto({0,1,2});
    end_point = high_resolution_clock::now();
    time_elpased = duration_cast<duration<double>>(end_point - start_point);
    std::cout << std::endl << "Time of computation elapsed : " << time_elpased.count() << " s" << std::endl;
    std::cout << std::endl << "NUMBER OF PARETO OPTIMUM : " << pareto.first.size() << std::endl;
    plotPareto3D(pareto.first,pareto.second);
    printPareto3D(pareto.first,pareto.second,svgout);

    return 0;
}
*/
