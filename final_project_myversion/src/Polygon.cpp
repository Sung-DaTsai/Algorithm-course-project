#include <iostream>
#include <vector>

#include "Polygon.h"
using namespace std;

Polygon::Polygon(const vector<int>& coordinate, bool sorc)
    : Reclist(coordinate, sorc)
{
    //loop = newNode[0];
}

Polygon::~Polygon(){
    //cout<<"Byebye"<<endl;
}


/*
Polygon Polygon::Clip(Polygon Polygon_A, Polygon Polygon_B)
{

}
*/
