#include <iostream>
#include <vector>

#include "Node.h"
using namespace std;

Node::Node(){
}

Node::Node(int a, int b, bool c)
    :Inters(c),next_2(nullptr)
{
    setX(a);
    setY(b);

}

Node::~Node()
{
}

void Node::setX(int a)
{
    x = a;
}

void Node::setY(int b)
{
    y = b;
}
