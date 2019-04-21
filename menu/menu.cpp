#include <iostream>
#include "menu.h"

std::string choixFichier()
{
    std::string nomFichier;
    std::cout<<"Bienvenue sur notre programme d'optimisation multi-objectif"<<std::endl
             <<std::endl
             <<"Quel graphe voulez vous chargez ?"<<std::endl;
    fflush(stdin);
    std::cin>>nomFichier;
    std::cout<<std::endl;
    return nomFichier;
}

std::string choixWeight()
{
    std::string weightFile;
    std::cout<<"Quel fichier de poids voulez vous chargez ?"<<std::endl;
    fflush(stdin);
    std::cin>>weightFile;
    std::cout<<std::endl;
    return weightFile;
}
int choixOrient()
{
    int ori;
    do
    {
        std::cout<<"Votre graphe est :"<<std::endl
                <<"1. non-oriente"<<std::endl
                <<"2. oriente"<<std::endl;
        fflush(stdin);
        std::cin>>ori;
    }while((ori<1)||(ori>2));
    return ori;
}


int choixAction()
{
    int choix;

    do
    {
        std::cout<<std::endl<<"Voulez vous :"<<std::endl
                <<"1. Trouver les arbres couvrants mono-objectif"<<std::endl
                <<"2. Trouver les graphes partiels multi-objectif"<<std::endl
                <<"3. Quitter"<<std::endl;
        fflush(stdin);
        std::cin>>choix;
    }while((choix<1)||(choix>3));
    return choix;
}

std::vector<int> choixPoids(size_t tailleP,int ori)
{
    std::vector<int> poids;
    int p;
    for(size_t i=0;i<tailleP;i++)
    {
        do
        {
            std::cout<<std::endl<<"Le poids "<<i+1<<" est un :"<<std::endl
                    <<"1. cout"<<std::endl
                    <<"2. distance"<<std::endl;
            if(ori==2)
            {
                std::cout<<"3. flot"<<std::endl;
            }
            std::cout<<std::endl;
            fflush(stdin);
            std::cin>>p;
        }while((p<1)||(p>ori+1));
        poids.push_back(p-1);
    }
    return poids;
}
