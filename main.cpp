#include <iostream>
#include "graphe.h"
#include "plot/ploting.h"
#include <vector>
#include "plot/svgfile.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include "menu/menu.h"
/*
int main()
{


    Svgfile svgout;
    ///Pour affichage Svg : a toujours déclaré
    double ecart_x;
    double ecart_y;
    std::string a = "files/broadway.txt";
    std::string aa = "files/broadway_weights_0.txt";
    graphe g(a,aa);
    ///Pour affichage Svg : a écrire après avoir déclarer le graphe
    g.InitialisationDonneeAffichageSvg(ecart_x,ecart_y);

    g.search_sol();
    g.dessinerGrapheOrg(svgout,ecart_x);
    /// il faut que les conteneurs sois remplie pour le fonctionner ces 2 derniers méthodes :
    g.dessinerGraphesPareto(svgout,ecart_x,ecart_y,f);
    g.dessinerGraphesPrim(svgout,ecart_x,ecart_y,t,f);
    return 0;
}*/


int main()
{
//    using namespace std::chrono;
//    Svgfile svgout;
//    high_resolution_clock::time_point start_point = high_resolution_clock::now();
//
//
//    graphe g("files/broadway.txt","files/broadway_weights_0.txt");
//    g.search_sol();
//    g.test();
//    std::cout << "Graphe charge !" << std::endl;
//    high_resolution_clock::time_point end_point = high_resolution_clock::now();
//    duration<double> time_elpased = duration_cast<duration<double>>(end_point - start_point);
//    std::cout << std::endl << "Time for generatating all connected subgraph elapsed : " << time_elpased.count() << " s" << std::endl;
//    std::cout << "Affichage de la frontiere de pareto : " << std::endl;
//    auto pareto = g.fairePareto({0,1,2});
//    end_point = high_resolution_clock::now();
//    time_elpased = duration_cast<duration<double>>(end_point - start_point);
//    std::cout << std::endl << "Time of computation elapsed : " << time_elpased.count() << " s" << std::endl;
//    std::cout << std::endl << "NUMBER OF PARETO OPTIMUM : " << pareto.first.size() << std::endl;
//    plotPareto3D(pareto.first,pareto.second);
//    printPareto3D(pareto.first,pareto.second,svgout);
//
//    return 0;
//}
//
    std::string nomFichier,weightFile;
    int orientation;
        double ecart_x=100,ecart_y=100;
    std::string tmp = choixFichier();
    nomFichier="files/"+tmp+".txt";
    weightFile="files/"+tmp+"_weights_"+choixWeight()+".txt";
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
                    plotPareto2D(pareto.first,pareto.second);
                    printPareto2D(pareto.first,pareto.second,svgout);
                }
                else
                {
                    plotPareto3D(pareto.first,pareto.second);
                    printPareto3D(pareto.first,pareto.second,svgout);
                }
                g.InitialisationDonneeAffichageSvg(ecart_x,ecart_y);
                g.dessinerGrapheOrg(svgout,ecart_x);
                #warning TODO (Anyone#9#): Implementer svg pour prim et pareto
                //g.dessinerGraphesPareto(svgout,ecart_x,ecart_y,pareto.first);
                //break;
            }
        //}
    //}while(choix!=2);
    return 0;
}

