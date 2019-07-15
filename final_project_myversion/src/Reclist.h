#ifndef RECLIST_H
#define RECLIST_H
#include <vector>

#include "Node.h"
using namespace std;

class Reclist: public Node
{
public:
    Reclist(const vector<int>&, bool);
    ~Reclist();
    void Initialize_node_subject(const vector<int>&);
    void Initialize_node_clip(const vector<int>&);
    bool isloop;      //if it's loop(counterclockwise): true; holes(clockwise): false
    vector<Node *> newNode;
    vector< vector<Node*> > newHole;  //newHole.resize(no_of_rows, vector<Node*>(no_of_cols, initial_value));
    //Node* head;
    int boxl, boxr, boxt, boxb;
    bool notice = false;
    int notice_1;  //only use when notice is true
    int notice_2;  //only use when notice is true
private:

};

#endif // RECLIST_H
