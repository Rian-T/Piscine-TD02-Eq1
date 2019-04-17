#include <iostream>
#include "p_func.h"
#include <vector>
#include "graphe.h"
#include <queue>
#include <functional>
#include <utility>
#include <fstream>


bool greaterPair(std::pair<float,Sommet*>& a,std::pair<float,Sommet*>& b){
    if(a.first > b.first)
        return true;
    else
        return false;
}

/*std::vector<const Sommet*> Sommet::getVoisins
{

}*/
