#include <iostream>
#include "graphe.h"

int main()
{
    std::vector<bool> prim;
    std::vector<bool> prim2;
    std::vector<bool> djik(5,1);
    float som1;
    //float som2;
    graphe g("files/broadway.txt","files/broadway_weights_0.txt");
    //g.afficher();
    /*prim=g.fairePrim(0);
    for(size_t i=0;i<prim.size();++i)
    {
        std::cout<<prim[i];
    }
    std::cout<<std::endl;
    prim2=g.fairePrim(1);
    for(size_t j=0;j<prim2.size();++j)
    {
        std::cout<<prim2[j];
    }
    som1=g.faireSomme(prim,0);
    som2=g.faireSomme(prim2,1);
    std::cout<<std::endl<<som1<<std::endl<<som2;*/

    som1=g.faireDjikstra(djik,1);
    std::cout<<som1;
    return 0;
}
