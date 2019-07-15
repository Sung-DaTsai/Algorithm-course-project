#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>
#include <cmath>
using namespace std;

class Node
{
public:
    Node(int,int,int);
    ~Node();
    int x;
    int y;
    int flow;
    int last_flow;
    bool issource;
    int parent_num_oneside;
    int parent_num;
    vector < vector <int> > length_flow_capacity;
    vector < vector <int> > flow_network;
    static int num_source;
    static int num_sink;
    static int get_numsource();
    static int get_numsink();
    void construct_flow(vector <Node*> &, vector <Node*> &);
    bool optimal(vector <Node*> &, vector <Node*> &);
};
#endif // NODE_H
