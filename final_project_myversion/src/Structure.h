#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <vector>
#include <iostream>

//#include "Node.h"
//#include "Reclist.h"
#include "Polygon.h"
using namespace std;

class Structure: public Polygon
{
public:
    Structure(const vector<int> &, bool);
    ~Structure();
    vector<Polygon *> newPolygon;
    vector<int> remaining;
    void Initialize_polygon(const vector<int> &, bool);
    Structure Find_intersection_clip(Structure &, Structure &);
    Structure Find_intersection_merge(Structure &, Structure &);
    bool inorout_left(const vector <Node*>&, int, int, double);
    bool inorout_bottom(const vector <Node*>&, int, int, double);
    bool inorout_merge(const vector <Node*>&, int, int, double, double); //in: return true; out: return false
    //bool hole_equal_clip(const vector <Node*>, const vector <Node*>);
    //bool inorout(const vector <Node*> nodes, int x, int y);//for clipping
    void Initialize_nodeorhole_after_merge(const vector< vector < int > > &, Structure&, vector<int> &, const vector <Node*>&);
    void Initialize_nodeorhole_after_clip(const vector< vector < int > > &, Structure&, unsigned, bool&, bool&);
    int split_H(Structure &, ostream &);
    int split_V(Structure &, ostream &);
    //void split_O(Structure &, ostream &);
private:
    //Polygon* head_P;

};

#endif // STRUCTURE_H

