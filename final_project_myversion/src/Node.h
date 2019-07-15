#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
using namespace std;

class Node
{
public:
    Node();
    Node(int, int, bool);
    ~Node();
    int x;        //coordinate x
    int y;        //coordinate y
    bool repeat_appear = false;
    int repeat_point;
    bool Inters;  //return true if the point is an intersection point
    bool Used = false;    //check whether the intersection point has been used, used for multiple polygons
    bool clip_into_subject; //check if it's entry point then true; exit point: false; if it's clip operation then from true point walk next_2
    Node* next_1; //for intersection point: point to subject polygon; for others: form a polygon itself
    Node* next_2 = nullptr; //for intersection point: point to clip polygon; for others: points to intersection point; otherwise it's NULL
    //Node* next_3 = nullptr; //only for subject polygon: if there are holes then use this pointer to represent connection between polygon and holes
    void setX(int);
    void setY(int);
    bool for_connection = false; //only for building connection in clipping
    int for_connection_num = -1;  //only for building connection in clipping
};





#endif // NODE_H

