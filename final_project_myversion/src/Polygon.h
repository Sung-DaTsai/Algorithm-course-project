#ifndef POLYGON_H
#define POLYGON_H
#include <vector>

//#include "Node.h"
#include "Reclist.h"
using namespace std;

class Polygon: public Reclist//, public Node
{
public:
    Polygon(const vector<int>&, bool);
    ~Polygon();
    //Node* loop;
    //Node* rings;
    //Polygon* next;
    //Polygon Merge(Polygon*, Polygon*);
    //Polygon Clip(Polygon*, Polygon*);
    //void Find_intersection(Polygon, Polygon);

};

#endif // POLYGON_H
