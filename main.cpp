#include <iostream>
#include "graphe.h"
#include "plot/ploting.h"
#include <vector>
#include "plot/svgfile.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include "menu/menu.h"

int main()
{
    std::string nomFichier,weightFile;
    int orientation;
    nomFichier="files/"+choixFichier()+".txt";
    weightFile="files/"+choixWeight()+".txt";
    orientation=choixOrient();
    graphe g(nomFichier,weightFile,orientation);
    size_t nbP=g.getNbWeight();
    //int choix;
    //do
    //{
        //choix=choixAction();
        //switch(choix)
        //{
        //case 1:
            {
                std::vector<std::vector<bool>> prim;
                std::vector<std::vector<float>> primPoidsTot;
                for(size_t i=0;i<nbP;i++)
                {
                    std::vector<float>primPoids;
                    prim.push_back(g.fairePrim(i));
                    for(size_t j=0;j<nbP;j++)
                    {
                        primPoids.push_back(g.faireSomme(prim[i],j));
                    }
                    primPoidsTot.push_back(primPoids);
                }
                //afficher les grpahes de Prim;
                //break;
            }
        //case 2:
            {
                std::vector<int> choixP;
                choixP=choixPoids(nbP,orientation);
                using namespace std::chrono;
                Svgfile svgout;
                high_resolution_clock::time_point start_point = high_resolution_clock::now();
                int s;
                if(nbP==2)
                    s=choixP[0]+choixP[1];
                else
                    s=choixP[0]+choixP[1]+choixP[2];
                if(s==0)
                    g.search_sol2();
                else
                    g.search_sol();
                std::cout << "Graphe charge !" << std::endl;
                high_resolution_clock::time_point end_point = high_resolution_clock::now();
                duration<double> time_elpased = duration_cast<duration<double>>(end_point - start_point);
                std::cout << std::endl << "Time for generatating all connected subgraph elapsed : " << time_elpased.count() << " s" << std::endl;
                std::cout << "Affichage de la frontiere de pareto : " << std::endl;
                auto pareto = g.fairePareto(choixP,orientation);
                end_point = high_resolution_clock::now();
                time_elpased = duration_cast<duration<double>>(end_point - start_point);
                std::cout << std::endl << "Time of computation elapsed : " << time_elpased.count() << " s" << std::endl;
                std::cout << std::endl << "NUMBER OF PARETO OPTIMUM : " << pareto.first.size() << std::endl;
                fflush(stdin);
                if(nbP==2)
                {
                    plotPareto2D(pareto.first);
                    printPareto3D(pareto.first,pareto.second,svgout);
                }
                else
                {
                    plotPareto3D(pareto.first,pareto.second);
                    printPareto3D(pareto.first,pareto.second,svgout);
                }
                //break;
            }
        //}
    //}while(choix!=2);
    return 0;
}

