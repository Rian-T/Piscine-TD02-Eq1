#include <iostream>
#include "graphe.h"
#include "plot/ploting.h"
#include <vector>
#include "plot/svgfile.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include "menu/menu.h"
<<<<<<< HEAD


>>>>>>> 902f19b4b0063c77c711b5b341e7ca531f815fc7

int main()
{
    std::string nomFichier,weightFile;
    int orientation;
        double ecart_x=200,ecart_y=300;
    std::string tmp = choixFichier();
    nomFichier="files/"+tmp+".txt";
    weightFile="files/"+tmp+"_weights_"+choixWeight()+".txt";
    orientation=choixOrient();
    graphe g(nomFichier,weightFile,orientation);
    size_t nbP=g.getNbWeight();
    int choix;
    do
    {
        choix=choixAction();
        Svgfile svgout;
        g.dessinerGrapheOrg(svgout,ecart_x,orientation);
        switch(choix)
        {
        case 1:
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
                g.dessinerGraphesPrim(svgout,ecart_x,ecart_y,prim,primPoidsTot,orientation);
                break;
            }
        case 2:
            {
                std::vector<int> choixP;
                choixP=choixPoids(nbP,orientation);
                using namespace std::chrono;
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
                    printPareto2D(pareto.first,pareto.second,svgout); //enregistre un png
                    plotPareto2D(pareto.first,pareto.second);           //affiche une courbe interactive
                }
                else
                {
                    printPareto3D(pareto.first,pareto.second,svgout);   //enregistre un gif
                    plotPareto3D(pareto.first,pareto.second);           //affiche un nuage de point interactif
                }
                g.dessinerGraphesPareto(svgout,ecart_x,ecart_y,pareto.first,orientation);
                break;
            }
        }
    }while(choix!=3);
    return 0;
}

