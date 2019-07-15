#include <iostream>
#include <vector>
#include <algorithm>
#include "Structure.h"
#include <assert.h>
#include <cmath>
#include <time.h>
#include <cstdlib>
using namespace std;


Structure::Structure(const vector<int>& coordinate, bool sorc)
    : Polygon(coordinate, sorc)
{
    Initialize_polygon(coordinate, sorc);
    for(unsigned i=0;i<Reclist::newNode.size();i++)
        delete Reclist::newNode.at(i);
    vector<Node*>().swap(Reclist::newNode);
    for(unsigned i=0;i<Reclist::newHole.size();i++)
    {
        for(unsigned j=0;j<Reclist::newHole.at(i).size();j++)
            delete Reclist::newHole.at(i).at(j);
    }

    vector < vector < Node* > >().swap(Reclist::newHole);

}

Structure::~Structure(){
}

void Structure::Initialize_polygon(const vector<int>& coordinate, bool sorc)
{
    newPolygon.push_back(new Polygon(coordinate, sorc));
}


bool Structure::inorout_left(const vector <Node*> &nodes, int x, int temp_y, double ones) //in: return true; out: return false
{
    int intersect = 0;
    double y = static_cast<double>(temp_y) - ones *0.5;

    for(unsigned i=0;i<nodes.size();i++)
    {
        double current_y = nodes[i]->y;
        double next_y = nodes[i]->next_1->y;

        if((nodes[i]->x == nodes[i]->next_1->x)&&(nodes[i]->x < x)&&(((current_y < y)&&(next_y > y))||((current_y > y)&&(next_y < y))))
            ++intersect;
        if((nodes[i]->x == nodes[i]->next_1->x)&&(nodes[i]->x == x)&&(((current_y < y)&&(next_y > y))||((current_y > y)&&(next_y < y))))
        {
            int counter = 0;
            ++intersect;
            for(int q=0;q<nodes.size();q++)
            {
                if((nodes.at(q)->x == x)&&(nodes.at(q)->y == temp_y-ones))
                {
                    counter++;
                }
            }
            if(counter == 2)
                return true;
        }

    }
    if(intersect %2 == 0)
        return false;
    else
        return true;

}

bool Structure::inorout_bottom(const vector <Node*> &nodes, int temp_x, int y, double ones) //in: return true; out: return false
{
    int intersect = 0;
    double x = static_cast<double>(temp_x) - ones *0.5;

    for(unsigned i=0;i<nodes.size();i++)
    {
        double current_x = nodes[i]->x;
        double next_x = nodes[i]->next_1->x;
        if((nodes[i]->y == nodes[i]->next_1->y)&&(nodes[i]->y < y)&&(((current_x < x)&&(next_x > x))||((current_x > x)&&(next_x < x))))
            ++intersect;
        if((nodes[i]->y == nodes[i]->next_1->y)&&(nodes[i]->y == y)&&(((current_x < x)&&(next_x > x))||((current_x > x)&&(next_x < x))))
        {
            ++intersect;
            int counter = 0;
            for(int q=0;q<nodes.size();q++)
            {
                if((nodes.at(q)->x == temp_x-ones)&&(nodes.at(q)->y == y))
                {
                    counter++;
                }
            }
            if(counter == 2)
                return true;
        }
    }



    if(intersect %2 == 0)
        return false;
    else
        return true;

}


bool Structure::inorout_merge(const vector <Node*> &nodes, int temp_x, int temp_y, double xs, double ys) //in: return true; out: return false, int a=1-->double x/a=2-->double y
{
    int intersect = 0;
    double x = static_cast<double>(temp_x) - xs *0.5;
    double y = static_cast<double>(temp_y) - ys *0.5;
    for(unsigned i=0;i<nodes.size();i++)
    {
        double current_x = nodes[i]->x;
        double next_x = nodes[i]->next_1->x;
        if((nodes[i]->y == nodes[i]->next_1->y)&&(nodes[i]->y < y)&&(((current_x < x)&&(next_x > x))||((current_x > x)&&(next_x < x))))
            ++intersect;
    }

    if(intersect %2 == 0)
        return false;
    else
        return true;

}



Structure Structure::Find_intersection_clip(Structure& a, Structure& b) //a: subject polygon ; b: clip polygon
{
    vector < vector < Node* > > intersection_points(a.newPolygon.size());
    vector <Node*> clip = b.newPolygon.at(0)->newNode;
    vector < vector < Node* > > subject(a.newPolygon.size()); //row = a.newPolygon.size(); column = a.newPolygon[i]->newNode.size()
    for(unsigned i=0;i<a.newPolygon.size();i++)
    {
        if(a.newPolygon.at(i)->boxt < b.newPolygon.at(0)->boxb)
            continue;
        else if(a.newPolygon.at(i)->boxb > b.newPolygon.at(0)->boxt)
            continue;
        else if(a.newPolygon.at(i)->boxl > b.newPolygon.at(0)->boxr)
            continue;
        else if(a.newPolygon.at(i)->boxr < b.newPolygon.at(0)->boxl)
            continue;
        else if(a.newPolygon.at(i)->newNode.size()==0)
        {
            swap(a.newPolygon.at(i),a.newPolygon.at(a.newPolygon.size()-1));
            a.newPolygon.pop_back();
            subject.pop_back();
            continue;
        }


        int sum = a.newPolygon.at(i)->newNode.size() - 1;
        subject.at(i) = vector <Node*> (a.newPolygon.at(i)->newNode.size());

        subject.at(i) = a.newPolygon.at(i)->newNode;
        for(unsigned j=0;j<a.newPolygon.at(i)->newHole.size();j++)
        {
            for(unsigned k=0;k<a.newPolygon.at(i)->newHole.at(j).size();k++)
            {
                subject.at(i).push_back(a.newPolygon.at(i)->newHole.at(j).at(k));
                if(k!=0)
                {
                    subject.at(i).at(subject.at(i).size()-1)->for_connection = false;
                    subject.at(i).at(subject.at(i).size()-1)->for_connection_num = -1;
                }
                if(k==0)
                {
                    sum += a.newPolygon.at(i)->newHole.at(j).size();
                    subject.at(i).at(subject.at(i).size()-1)->for_connection = true;
                    subject.at(i).at(subject.at(i).size()-1)->for_connection_num = sum;
                }
            }
        }
        subject.at(i).at(0)->for_connection = true;
        subject.at(i).at(0)->for_connection_num = a.newPolygon.at(i)->newNode.size() - 1;
    }



    for(unsigned k=0;k<clip.size();k++)
    {
        if((clip[k]->x)==(clip[k]->next_1->x)) // this edge is a vertical edge
        {
            int temp_x = clip[k]->x;
            for(unsigned i=0;i<subject.size();i++)
            {
                for(unsigned j=0;j<subject[i].size();j++)
                {
                    int x_previous, y_previous;
                    if(subject[i][j]->for_connection_num<0)
                    {
                        x_previous = subject[i][j-1]->x;
                        y_previous = subject[i][j-1]->y;
                    }
                    else
                    {
                        x_previous = subject[i][subject.at(i).at(j)->for_connection_num]->x;
                        y_previous = subject[i][subject.at(i).at(j)->for_connection_num]->y;
                    }

                    if((subject[i][j]->y)==(subject[i][j]->next_1->y))    // horizontal edge
                    {
                        int temp_y = subject[i][j]->y;
                        if((clip[k]->y > temp_y)&&(clip[k]->next_1->y < temp_y))   // clip edge points downwards
                        {
                            // case 1
                            if((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x))     // subject edge points leftward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //downward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x > temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }

                            }
                            else if((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //downward & rightward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x < temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }

                            }
                            // case 2-1
                            else if(subject[i][j]->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->y < y_previous)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->y > y_previous)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y > temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->y < subject[i][j]->next_1->next_1->y)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->y > subject[i][j]->next_1->next_1->y)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y > temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }

                        }
                        else if((clip[k]->y < temp_y)&&(clip[k]->next_1->y > temp_y))     // clip edge points upwards
                        {
                            // case 1
                            if((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x))     // subject edge points leftward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;

                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x > temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }

                            }
                            else if((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & rightward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;

                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x < temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }

                            }
                            // case 2-1
                            else if(subject[i][j]->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->y < y_previous)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->y > y_previous)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y < temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->y < subject[i][j]->next_1->next_1->y)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->y > subject[i][j]->next_1->next_1->y)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y < temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }

                        }



                        else if((clip[k]->y) == temp_y)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-1
                            if(((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))||((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->y) > clip[k]->y)&&((inorout_left(subject[i], temp_x, temp_y + 1, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->y) < clip[k]->y)&&((inorout_left(subject[i], temp_x, temp_y - 1, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->x < temp_x)
                                        {
                                            if(compare->next_2->x < temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x < temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->x > temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x > temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }

                            }

                            // case 3-1
                            else if((subject[i][j]->x == temp_x)&&(y_previous > subject[i][j]->y)&&(clip[k]->y < clip[k]->next_1->y)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->x == temp_x)&&(y_previous < subject[i][j]->y)&&(clip[k]->y > clip[k]->next_1->y)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&((subject[i][j]->next_1->next_1->y < subject[i][j]->next_1->y)&&(clip[k]->next_1->y < clip[k]->y))&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&((subject[i][j]->next_1->next_1->y > subject[i][j]->next_1->y)&&(clip[k]->next_1->y > clip[k]->y))&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                        }
                        else if((clip[k]->next_1->y) == temp_y)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-2
                            if(((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))||((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->y) > clip[k]->y)&&((inorout_left(subject[i], temp_x, temp_y - 1, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->y) < clip[k]->y)&&((inorout_left(subject[i], temp_x, temp_y + 1, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_1->next_2 == nullptr)
                                    {
                                        clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->x < temp_x)
                                        {
                                            if(compare->next_2->x < temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x < temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->x > temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x > temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }
                            }
                            // case 3-2
                            else if((subject[i][j]->x == temp_x)&&(y_previous > subject[i][j]->y)&&(clip[k]->y > clip[k]->next_1->y)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->x == temp_x)&&(y_previous < subject[i][j]->y)&&(clip[k]->y < clip[k]->next_1->y)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&(subject[i][j]->next_1->next_1->y < subject[i][j]->next_1->y)&&(clip[k]->y < clip[k]->next_1->y)&&inorout_left(subject[i], temp_x, temp_y + 1, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&(subject[i][j]->next_1->next_1->y > subject[i][j]->next_1->y)&&(clip[k]->y > clip[k]->next_1->y)&&inorout_left(subject[i], temp_x, temp_y - 1, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                        }
                    }
                }
            }
        }

        else       //this edge is a horizontal edge
        {
            int temp_y = clip[k]->y;
            for(unsigned i=0;i<subject.size();i++)
            {
                for(unsigned j=0;j<subject[i].size();j++)
                {
                    int x_previous, y_previous;
                    if(subject[i][j]->for_connection_num<0)
                    {
                        x_previous = subject[i][j-1]->x;
                        y_previous = subject[i][j-1]->y;
                    }
                    else
                    {
                        x_previous = subject[i][subject.at(i).at(j)->for_connection_num]->x;
                        y_previous = subject[i][subject.at(i).at(j)->for_connection_num]->y;
                    }

                    if((subject[i][j]->x)==(subject[i][j]->next_1->x))    // vertical edge
                    {
                        int temp_x = subject[i][j]->x;
                        if((clip[k]->x > temp_x)&&(clip[k]->next_1->x < temp_x))   // clip edge points leftward
                        {
                            // case 1
                            if((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y))     // subject edge points downward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //downward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;

                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y > temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }
                            else if((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y < temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }
                            }
                            // case 2-1
                            else if(subject[i][j]->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->x < x_previous)&&inorout_bottom(subject[i], temp_x - 1, temp_y, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->x > x_previous)&&inorout_bottom(subject[i], temp_x + 1, temp_y, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;

                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x > temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->x < subject[i][j]->next_1->next_1->x)&&inorout_bottom(subject[i], temp_x-1, temp_y, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->x > subject[i][j]->next_1->next_1->x)&&inorout_bottom(subject[i], temp_x+1, temp_y, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x > temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }

                            }
                        }


                        else if((clip[k]->x < temp_x)&&(clip[k]->next_1->x > temp_x))     // clip edge points rightward
                        {
                            // case 1
                            if((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y))     // subject edge points downward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //rightward and downward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y > temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }

                            }
                            else if((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & rightward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y < temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }
                            }
                            // case 2-1
                            else if(subject[i][j]->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->x < x_previous)&&inorout_bottom(subject[i], temp_x - 1, temp_y, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->x > x_previous)&&inorout_bottom(subject[i], temp_x + 1, temp_y, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x < temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->x < subject[i][j]->next_1->next_1->x)&&inorout_bottom(subject[i], temp_x - 1, temp_y, -1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->x > subject[i][j]->next_1->next_1->x)&&inorout_bottom(subject[i], temp_x + 1, temp_y, 1))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x < temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                        }



                        else if((clip[k]->x) == temp_x)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-1
                            if(((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))||((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->x) > clip[k]->x)&&((inorout_bottom(subject[i], temp_x + 1, temp_y, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->x) < clip[k]->x)&&((inorout_bottom(subject[i], temp_x - 1, temp_y, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->y < temp_y)
                                        {
                                            if(compare->next_2->y < temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y < temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->y > temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y > temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }


                            }
                            // case 3-1
                            else if((subject[i][j]->y == temp_y)&&(x_previous > subject[i][j]->x)&&(clip[k]->x < clip[k]->next_1->x)&&inorout_bottom(subject[i], temp_x - 1, temp_y, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->y == temp_y)&&(x_previous < subject[i][j]->x)&&(clip[k]->x > clip[k]->next_1->x)&&inorout_bottom(subject[i], temp_x + 1, temp_y, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&((subject[i][j]->next_1->next_1->x < subject[i][j]->next_1->x)&&(clip[k]->next_1->x < clip[k]->x))&&inorout_bottom(subject[i], temp_x+1, temp_y, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&((subject[i][j]->next_1->next_1->x > subject[i][j]->next_1->x)&&(clip[k]->next_1->x > clip[k]->x))&&inorout_bottom(subject[i], temp_x-1, temp_y, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }


                        }
                        else if((clip[k]->next_1->x) == temp_x)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-2
                            if(((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))||((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->x) > clip[k]->x)&&((inorout_bottom(subject[i], temp_x-1, temp_y, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->x) < clip[k]->x)&&((inorout_bottom(subject[i], temp_x+1, temp_y, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_1->next_2 == nullptr)
                                    {
                                        clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->y < temp_y)
                                        {
                                            if(compare->next_2->y < temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y < temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->y > temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y > temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }

                            }
                            // case 3-2
                            else if((subject[i][j]->y == temp_y)&&(x_previous > subject[i][j]->x)&&(clip[k]->x > clip[k]->next_1->x)&&inorout_bottom(subject[i], temp_x-1, temp_y, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->y == temp_y)&&(x_previous < subject[i][j]->x)&&(clip[k]->x < clip[k]->next_1->x)&&inorout_bottom(subject[i], temp_x+1, temp_y, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&(subject[i][j]->next_1->next_1->x < subject[i][j]->next_1->x)&&(clip[k]->x < clip[k]->next_1->x)&&inorout_bottom(subject[i], temp_x+1, temp_y, 1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&(subject[i][j]->next_1->next_1->x > subject[i][j]->next_1->x)&&(clip[k]->x > clip[k]->next_1->x)&&inorout_bottom(subject[i], temp_x-1, temp_y, -1))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // no intersections, all the subject polygons should check with clip respectively
    const int const_newPolygon_size = a.newPolygon.size();
    int counting_Polygon = 0;
    int iii = 0;
    while(counting_Polygon < const_newPolygon_size)
    {
        counting_Polygon++;
        if(a.newPolygon.at(iii)->boxt < b.newPolygon.at(0)->boxb)
        {
            iii++;
            continue;
        }
        else if(a.newPolygon.at(iii)->boxb > b.newPolygon.at(0)->boxt)
        {
            iii++;
            continue;
        }
        else if(a.newPolygon.at(iii)->boxl > b.newPolygon.at(0)->boxr)
        {
            iii++;
            continue;
        }
        else if(a.newPolygon.at(iii)->boxr < b.newPolygon.at(0)->boxl)
        {
            iii++;
            continue;
        }
        if(intersection_points.at(iii).size()==0)
        {
            //clip special case 1
            int x_temp = clip.at(0)->x;
            int y_temp = clip.at(0)->y;
            double change_x = 0;
            double change_y = 0;
            if(x_temp == clip.at(0)->next_1->x)
            {
                if((inorout_bottom(subject.at(iii), x_temp + 1, (y_temp + clip.at(0)->next_1->y)/2, 1))&&(inorout_bottom(subject.at(iii), x_temp - 1, (y_temp + clip.at(0)->next_1->y)/2, -1)))
                {
                    int k = 0;
                    const int const_newHole_size = a.newPolygon.at(iii)->newHole.size();
                    int counting_Hole = 0;
                    while(counting_Hole < const_newHole_size)
                    {
                        counting_Hole++;
                        int hole_x = a.newPolygon.at(iii)->newHole.at(k).at(0)->x;
                        int hole_y = a.newPolygon.at(iii)->newHole.at(k).at(0)->y;
                        change_x = 0;
                        change_y = 0;
                        if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x-1, hole_y-1,-1,-1))
                        {
                            change_x = -1;
                            change_y = -1;
                        }
                        else if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x+1, hole_y+1,1,1))
                        {
                            change_x = 1;
                            change_y = 1;
                        }
                        else if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x-1, hole_y+1,-1,1))
                        {
                            change_x = -1;
                            change_y = 1;
                        }
                        else if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x+1, hole_y-1,1,-1))
                        {
                            change_x = 1;
                            change_y = -1;
                        }
                        if(inorout_merge(clip, hole_x+change_x, hole_y+change_y, change_x, change_y))
                        {
                            swap(a.newPolygon.at(iii)->newHole.at(k), a.newPolygon.at(iii)->newHole.at(a.newPolygon.at(iii)->newHole.size()-1));
                            a.newPolygon.at(iii)->newHole.pop_back();
                        }
                        else
                            k++;
                    }
                    a.newPolygon.at(iii)->newHole.push_back(clip);
                    iii++;
                    continue;
                }
            }
            else
            {
                if((inorout_left(subject.at(iii), (x_temp + clip.at(0)->next_1->x)/2, y_temp + 1, 1))&&(inorout_left(subject.at(iii), (x_temp + clip.at(0)->next_1->x)/2, y_temp - 1, -1)))
                {
                    int k = 0;
                    const int const_newHole_size = a.newPolygon.at(iii)->newHole.size();
                    int counting_Hole = 0;
                    while(counting_Hole < const_newHole_size)
                    {
                        counting_Hole++;
                        int hole_x = a.newPolygon.at(iii)->newHole.at(k).at(0)->x;
                        int hole_y = a.newPolygon.at(iii)->newHole.at(k).at(0)->y;
                        change_x = 0;
                        change_y = 0;
                        if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x-1, hole_y-1,-1,-1))
                        {
                            change_x = -1;
                            change_y = -1;
                        }
                        else if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x+1, hole_y+1,1,1))
                        {
                            change_x = 1;
                            change_y = 1;
                        }
                        else if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x-1, hole_y+1,-1,1))
                        {
                            change_x = -1;
                            change_y = 1;
                        }
                        else if(inorout_merge(a.newPolygon[iii]->newHole[k], hole_x+1, hole_y-1,1,-1))
                        {
                            change_x = 1;
                            change_y = -1;
                        }
                        if(inorout_merge(clip, hole_x+change_x, hole_y+change_y, change_x, change_y))
                        {
                            swap(a.newPolygon.at(iii)->newHole.at(k), a.newPolygon.at(iii)->newHole.at(a.newPolygon.at(iii)->newHole.size()-1));
                            a.newPolygon.at(iii)->newHole.pop_back();
                        }
                        else
                            k++;
                    }
                    a.newPolygon.at(iii)->newHole.push_back(clip);
                    iii++;
                    continue;
                }
            }
            //special case 2: no intersection & clip is bigger than subject
            x_temp = a.newPolygon[iii]->newNode[0]->x;
            y_temp = a.newPolygon[iii]->newNode[0]->y;
            change_x = 0;
            change_y = 0;
            if(inorout_merge(a.newPolygon[iii]->newNode, x_temp-1, y_temp-1,-1,-1))
            {
                change_x = -1;
                change_y = -1;
            }
            else if(inorout_merge(a.newPolygon[iii]->newNode, x_temp+1, y_temp+1,1,1))
            {
                change_x = 1;
                change_y = 1;
            }
            else if(inorout_merge(a.newPolygon[iii]->newNode, x_temp-1, y_temp+1,-1,1))
            {
                change_x = -1;
                change_y = 1;
            }
            else if(inorout_merge(a.newPolygon[iii]->newNode, x_temp+1, y_temp-1,1,-1))
            {
                change_x = 1;
                change_y = -1;
            }
            if(inorout_merge(clip, x_temp+change_x, y_temp+change_y, change_x, change_y))
            {
                swap(subject.at(iii), subject.at(subject.size()-1));
                swap(a.newPolygon.at(iii), a.newPolygon.at(a.newPolygon.size()-1));
                swap(intersection_points.at(iii),intersection_points.at(intersection_points.size()-1));
                vector<Node*>().swap(a.newPolygon.at(a.newPolygon.size()-1)->newNode);
                vector< vector <Node*> >().swap(a.newPolygon.at(a.newPolygon.size()-1)->newHole);
                a.newPolygon.pop_back();
                subject.pop_back();
                intersection_points.pop_back();
                continue;
            }
	          else
	              iii++;
        }
    }
    //Structure* ans = new Structure
    vector <int> ans; //only used to expand rows
    int great = 0;
    bool special_case = false;
    int counting_special = 0;
    for(int i=a.newPolygon.size()-1;i>=0;i--)
    {
        bool timeexceed = false;
        vector< vector <int> > answer;
        counting_special = 0;
        for(int j=0;j<intersection_points.at(i).size();j++)
        {
            for(int h=j+1;h<intersection_points.at(i).size();h++)
            {
                if((intersection_points.at(i).at(j)->x == intersection_points.at(i).at(h)->x)&&(intersection_points.at(i).at(j)->y == intersection_points.at(i).at(h)->y))
                {
                    intersection_points.at(i).at(j)->repeat_appear = true;
                    intersection_points.at(i).at(h)->repeat_appear = true;
                    intersection_points.at(i).at(j)->repeat_point = h;
                    intersection_points.at(i).at(h)->repeat_point = j;
                    counting_special++;
                }
            }
        }

        for(unsigned k=0;k<intersection_points.at(i).size();k++)
        {
            if((intersection_points[i][k]->Used == false)&&(intersection_points[i][k]->repeat_appear == false))
            {
                answer.push_back(ans);
                Node* temp = intersection_points[i][k];
                bool now_in_or_out = !temp->clip_into_subject;
                while((temp != intersection_points[i][k])||(temp->Used == false))
                {
                    if((temp->clip_into_subject == true)&&(now_in_or_out == false)&&(temp->repeat_appear == false))
                    {
                        now_in_or_out = true;
                        temp->Used = true;
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        temp = temp->next_2;  //first clip vertex or directly an exit intersection point
                        if(temp->Inters == false)  //first clip vertex
                        {
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                            while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                            {
                                temp = temp->next_1;
                                answer.at(answer.size()-1).push_back(temp->x);
                                answer.at(answer.size()-1).push_back(temp->y);
                            }
                            temp = temp->next_2;

                        }

                    }
                    else if((temp->clip_into_subject == false)&&(now_in_or_out == true)&&(temp->repeat_appear == false))
                    {
                        now_in_or_out = false;
                        temp->Used = true;
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        temp = temp->next_1;  //first clip vertex or directly an exit intersection point
                        if(temp->Inters == false)  //first clip vertex
                        {
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                            while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                            {
                                temp = temp->next_1;
                                answer.at(answer.size()-1).push_back(temp->x);
                                answer.at(answer.size()-1).push_back(temp->y);
                            }
                            temp = temp->next_2;
                        }
                    }
                    else if((now_in_or_out == false)&&(temp->repeat_appear == true))
                    {
                        temp->Used = true;
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        temp = intersection_points.at(i).at(temp->repeat_point);
                        temp->Used = true;
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        if(temp->clip_into_subject)
                            temp = intersection_points.at(i).at(temp->repeat_point);
                        now_in_or_out = false;

                        temp = temp->next_1;  //first clip vertex or directly an exit intersection point
                        if(temp->Inters == false)  //first clip vertex
                        {
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);

                            while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                            {
                                temp = temp->next_1;
                                answer.at(answer.size()-1).push_back(temp->x);
                                answer.at(answer.size()-1).push_back(temp->y);
                            }
                            temp = temp->next_2;
                        }
                    }
                    else if((now_in_or_out == true)&&(temp->repeat_appear == true))
                    {
                        temp->Used = true;
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        temp = intersection_points.at(i).at(temp->repeat_point);
                        temp->Used = true;
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        if(!temp->clip_into_subject)
                            temp = intersection_points.at(i).at(temp->repeat_point);
                        now_in_or_out = true;

                        temp = temp->next_2;  //first clip vertex or directly an exit intersection point
                        if(temp->Inters == false)  //first clip vertex
                        {
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                            while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                            {
                                temp = temp->next_1;
                                answer.at(answer.size()-1).push_back(temp->x);
                                answer.at(answer.size()-1).push_back(temp->y);
                            }
                            temp = temp->next_2;
                        }
                    }
                }
                answer.at(answer.size()-1).push_back(intersection_points[i][k]->x);
                answer.at(answer.size()-1).push_back(intersection_points[i][k]->y);
            }
        }


        if(answer.size()>0)
        {
            bool sub_intersect_change = false;
            bool must = false;
            Initialize_nodeorhole_after_clip(answer, a, i, sub_intersect_change, must);
            if(sub_intersect_change)
            {
                bool secure = false;
                for(int g=0;g<clip.size();g++)
                {
                    int hole_x = clip.at(g)->x;
                    int hole_y = clip.at(g)->y;
                    int change_x = 0;
                    int change_y = 0;
                    if(!inorout_merge(clip, hole_x-1, hole_y-1,-1,-1))
                    {
                        change_x = -1;
                        change_y = -1;
                    }
                    else if(!inorout_merge(clip, hole_x+1, hole_y+1,1,1))
                    {
                        change_x = 1;
                        change_y = 1;
                    }
                    else if(!inorout_merge(clip, hole_x-1, hole_y+1,-1,1))
                    {
                        change_x = -1;
                        change_y = 1;
                    }
                    else if(!inorout_merge(clip, hole_x+1, hole_y-1,1,-1))
                    {
                        change_x = 1;
                        change_y = -1;
                    }
                    if(!inorout_merge(a.newPolygon.at(i)->newNode, hole_x+change_x, hole_y+change_y, change_x, change_y))
                    {
                        secure = true;
                    }
                }
                if(secure||must)
                {
                    swap(subject.at(i), subject.at(subject.size()-1));
                    subject.pop_back();
                    swap(intersection_points.at(i), intersection_points.at(intersection_points.size()-1));
                    intersection_points.pop_back();
                    swap(a.newPolygon.at(i), a.newPolygon.at(a.newPolygon.size()-1));
                    vector < Node* > ().swap(a.newPolygon.at(a.newPolygon.size()-1)->newNode);
                    a.newPolygon.pop_back();
                }
            }
        }
	else if(counting_special == 4)
	{

	    bool sub_intersect_change = false;
        bool must = false;
        for(int p=0;p<intersection_points.at(i).size();p++)
	    {
            if((intersection_points.at(i).at(p)->Used == false)&&(intersection_points.at(i).at(p)->clip_into_subject == false))
            {
                answer.push_back(ans);
                Node* temp = intersection_points.at(i).at(p);

                while((temp != intersection_points.at(i).at(p))||(temp->Used == false))
                {
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);
		    if((temp->Inters))
		    {

		        intersection_points.at(i).at(temp->repeat_point)->Used = true;
		        if(temp->clip_into_subject)
		            temp = intersection_points.at(i).at(temp->repeat_point);
		        temp = temp->next_1;
		    }
		    else if((temp->next_2 == nullptr))
		    {

		        temp = temp->next_1;

		    }
		    else if((temp->next_2->Inters))
		    {

		        temp = temp->next_2;
		        if(temp->clip_into_subject)
		            temp = intersection_points.at(i).at(temp->repeat_point);
                    }
                }
                answer.at(answer.size()-1).push_back(intersection_points.at(i).at(p)->x);
                answer.at(answer.size()-1).push_back(intersection_points.at(i).at(p)->y);
            }
	    }
            Initialize_nodeorhole_after_clip(answer, a, i, sub_intersect_change, must);
        }

        if(i<=0)
            break;
    }

    vector < vector < Node* > > ().swap(subject);
    vector < Node* >().swap(clip);
    vector < vector < Node* > > ().swap(intersection_points);

    return a;

}



Structure Structure::Find_intersection_merge(Structure& a, Structure& b) //a: subject polygon ; b: clip polygon(still use clip to represent merge)
{
    if(b.newPolygon.at(0)->newHole.size()!=0)
    {
        a.newPolygon.push_back(b.newPolygon.at(0));
	      return a;
    }


    vector < vector < Node* > > intersection_points(a.newPolygon.size());
    vector <Node*> clip = b.newPolygon[0]->newNode;

    vector < vector < Node* > > subject(a.newPolygon.size()); //row = a.newPolygon.size(); column = a.newPolygon[i]->newNode.size()
    for(unsigned i=0;i<a.newPolygon.size();i++)
    {
        if(a.newPolygon.at(i)->boxt < b.newPolygon.at(0)->boxb)
            continue;
        else if(a.newPolygon.at(i)->boxb > b.newPolygon.at(0)->boxt)
            continue;
        else if(a.newPolygon.at(i)->boxl > b.newPolygon.at(0)->boxr)
            continue;
        else if(a.newPolygon.at(i)->boxr < b.newPolygon.at(0)->boxl)
            continue;
        else if(a.newPolygon.at(i)->newNode.size()==0)
        {
            swap(a.newPolygon.at(i),a.newPolygon.at(a.newPolygon.size()-1));
            a.newPolygon.pop_back();
            subject.pop_back();
            continue;
        }

        unsigned sum = a.newPolygon[i]->newNode.size() - 1;
        subject[i] = vector <Node*> (a.newPolygon[i]->newNode.size());
        subject[i] = a.newPolygon[i]->newNode;


        for(unsigned j=0;j<a.newPolygon[i]->newHole.size();j++)
        {
            for(unsigned k=0;k<a.newPolygon[i]->newHole[j].size();k++)
            {
                subject[i].push_back(a.newPolygon[i]->newHole[j][k]);
                if(k!=0)
                {
                    subject.at(i).at(subject.at(i).size()-1)->for_connection = false;
                    subject.at(i).at(subject.at(i).size()-1)->for_connection_num = -1;
                }
                if(k==0)
                {
                    sum += a.newPolygon[i]->newHole[j].size();
                    subject[i][subject[i].size()-1]->for_connection = true;
                    subject[i][subject[i].size()-1]->for_connection_num = sum;
                }
            }
        }
        subject[i][0]->for_connection = true;
        subject[i][0]->for_connection_num = a.newPolygon[i]->newNode.size() - 1;
    }



    for(unsigned i=0;i<subject.size();i++)
    {
        for(unsigned j=0;j<subject[i].size();j++)
        {
            subject[i][j]->next_2 = nullptr;
        }

    }

    for(unsigned k=0;k<clip.size();k++)
    {
        if((clip[k]->x)==(clip[k]->next_1->x)) // this edge is a vertical edge
        {
            int temp_x = clip[k]->x;
            for(unsigned i=0;i<subject.size();i++)
            {
                for(unsigned j=0;j<subject[i].size();j++)
                {
                    int x_previous, y_previous;
                    if(subject[i][j]->for_connection_num<0)
                    {
                        x_previous = subject[i][j-1]->x;
                        y_previous = subject[i][j-1]->y;
                    }
                    else
                    {
                        x_previous = subject[i][subject[i][j]->for_connection_num]->x;
                        y_previous = subject[i][subject[i][j]->for_connection_num]->y;
                    }

                    if((subject[i][j]->y)==(subject[i][j]->next_1->y))    // horizontal edge
                    {
                        int temp_y = subject[i][j]->y;
                        if((clip[k]->y > temp_y)&&(clip[k]->next_1->y < temp_y))   // clip edge points downwards
                        {
                            // case 1
                            if((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x))     // subject edge points leftward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //downward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x > temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }

                            }
                            else if((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //downward & rightward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x < temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }

                            }
                            // case 2-1
                            else if(subject[i][j]->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->y < y_previous)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->y > y_previous)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y > temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->y < subject[i][j]->next_1->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->y > subject[i][j]->next_1->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y > temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                        }
                        else if((clip[k]->y < temp_y)&&(clip[k]->next_1->y > temp_y))     // clip edge points upwards
                        {
                            // case 1
                            if((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x))     // subject edge points leftward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;

                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x > temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }

                            }
                            else if((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & rightward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;

                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->x < temp_x)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }

                            }
                            // case 2-1
                            else if(subject[i][j]->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->y < y_previous)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->y > y_previous)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y < temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->x == temp_x)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->y < subject[i][j]->next_1->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->y > subject[i][j]->next_1->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->y < temp_y)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }

                        }



                        else if((clip[k]->y) == temp_y)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-1
                            if(((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))||((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->y) > clip[k]->y)&&(!(inorout_left(subject[i], temp_x, temp_y + 1, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->y) < clip[k]->y)&&(!(inorout_left(subject[i], temp_x, temp_y - 1, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->x < temp_x)
                                        {
                                            if(compare->next_2->x < temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x < temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->x > temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x > temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }

                            }


                            // case 3-1
                            else if((subject[i][j]->x == temp_x)&&(y_previous > subject[i][j]->y)&&(clip[k]->y < clip[k]->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->x == temp_x)&&(y_previous < subject[i][j]->y)&&(clip[k]->y > clip[k]->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&((subject[i][j]->next_1->next_1->y < subject[i][j]->next_1->y)&&(clip[k]->next_1->y < clip[k]->y))&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&((subject[i][j]->next_1->next_1->y > subject[i][j]->next_1->y)&&(clip[k]->next_1->y > clip[k]->y))&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                        }
                        else if((clip[k]->next_1->y) == temp_y)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-2
                            if(((subject[i][j]->x < temp_x)&&(subject[i][j]->next_1->x > temp_x))||((subject[i][j]->x > temp_x)&&(subject[i][j]->next_1->x < temp_x)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->y) > clip[k]->y)&&(!(inorout_left(subject[i], temp_x, temp_y - 1, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->y) < clip[k]->y)&&(!(inorout_left(subject[i], temp_x, temp_y + 1, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_1->next_2 == nullptr)
                                    {
                                        clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->x < temp_x)
                                        {
                                            if(compare->next_2->x < temp_x)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->x < temp_x)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->x > temp_x)
                                            {
                                                compare = compare->next_2;
                                                ////cout<<"test"<<compare->x<<compare->y<<endl;
                                                ////cout<<"test2"<<compare->next_1->x<<compare->next_1->y<<endl;
                                                ////cout<<"test2"<<compare->next_1->next_1->x<<compare->next_1->next_1->y<<endl;
                                                while(compare->next_1->x > temp_x)
                                                {
                                                    compare = compare->next_1;
                                                    ////cout<<compare->x<<endl;
                                                    ////cout<<compare->y<<endl;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }
                            }
                            // case 3-2
                            else if((subject[i][j]->x == temp_x)&&(y_previous > subject[i][j]->y)&&(clip[k]->y > clip[k]->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->x == temp_x)&&(y_previous < subject[i][j]->y)&&(clip[k]->y < clip[k]->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&(subject[i][j]->next_1->next_1->y < subject[i][j]->next_1->y)&&(clip[k]->y < clip[k]->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y + 1, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->x == temp_x)&&(subject[i][j]->next_1->next_1->y > subject[i][j]->next_1->y)&&(clip[k]->y > clip[k]->next_1->y)&&(!inorout_left(subject[i], temp_x, temp_y - 1, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                        }
                    }
                }
            }
        }

        else       //this edge is a horizontal edge
        {
            int temp_y = clip[k]->y;
            for(unsigned i=0;i<subject.size();i++)
            {
                for(unsigned j=0;j<subject[i].size();j++)
                {
                    int x_previous, y_previous;
                    if(subject[i][j]->for_connection_num < 0)
                    {
                        x_previous = subject[i][j-1]->x;
                        y_previous = subject[i][j-1]->y;
                    }
                    else
                    {
                        x_previous = subject[i][subject.at(i).at(j)->for_connection_num]->x;
                        y_previous = subject[i][subject.at(i).at(j)->for_connection_num]->y;
                    }

                    if((subject[i][j]->x)==(subject[i][j]->next_1->x))    // vertical edge
                    {
                        int temp_x = subject[i][j]->x;
                        if((clip[k]->x > temp_x)&&(clip[k]->next_1->x < temp_x))   // clip edge points leftward
                        {
                            // case 1
                            if((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y))     // subject edge points downward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //downward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;

                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y > temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }


                            }
                            else if((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & leftward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x > temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y < temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }
                            }
                            // case 2-1
                            else if(subject[i][j]->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->x < x_previous)&&(!inorout_bottom(subject[i], temp_x - 1, temp_y, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->x > x_previous)&&(!inorout_bottom(subject[i], temp_x + 1, temp_y, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x > temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->x < subject[i][j]->next_1->next_1->x)&&(!inorout_bottom(subject[i], temp_x-1, temp_y, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->x > subject[i][j]->next_1->next_1->x)&&(!inorout_bottom(subject[i], temp_x+1, temp_y, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x > temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }

                            }
                        }


                        else if((clip[k]->x < temp_x)&&(clip[k]->next_1->x > temp_x))     // clip edge points rightward
                        {
                            // case 1
                            if((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y))     // subject edge points downward
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //rightward and downward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y > temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y > temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }

                            }
                            else if((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));   //upward & rightward
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    while(compare->next_2->x < temp_x)
                                    {
                                        compare = compare->next_2;
                                    }
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    if(compare->next_2->y < temp_y)
                                    {
                                        compare = compare->next_2;
                                        while(compare->next_1->y < temp_y)
                                        {
                                            compare = compare->next_1;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                        compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                    else
                                    {
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                }
                            }
                            // case 2-1
                            else if(subject[i][j]->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->x < x_previous)&&(!inorout_bottom(subject[i], temp_x - 1, temp_y, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->x > x_previous)&&(!inorout_bottom(subject[i], temp_x + 1, temp_y, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x < temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                            // case 2-2
                            else if(subject[i][j]->next_1->y == temp_y)
                            {
                                bool check = false;
                                if((subject[i][j]->next_1->x < subject[i][j]->next_1->next_1->x)&&(!inorout_bottom(subject[i], temp_x - 1, temp_y, -1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if((subject[i][j]->next_1->x > subject[i][j]->next_1->next_1->x)&&(!inorout_bottom(subject[i], temp_x + 1, temp_y, 1)))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        while(compare->next_2->x < temp_x)
                                        {
                                            compare = compare->next_2;
                                        }
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_1->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }
                                }
                            }
                        }



                        else if((clip[k]->x) == temp_x)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-1
                            if(((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))||((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->x) > clip[k]->x)&&(!(inorout_bottom(subject[i], temp_x + 1, temp_y, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->x) < clip[k]->x)&&(!(inorout_bottom(subject[i], temp_x - 1, temp_y, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_2 == nullptr)
                                    {
                                        clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->y < temp_y)
                                        {
                                            if(compare->next_2->y < temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y < temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->y > temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y > temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }


                            }
                            // case 3-1
                            else if((subject[i][j]->y == temp_y)&&(x_previous > subject[i][j]->x)&&(clip[k]->x < clip[k]->next_1->x)&&(!inorout_bottom(subject[i], temp_x - 1, temp_y, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->y == temp_y)&&(x_previous < subject[i][j]->x)&&(clip[k]->x > clip[k]->next_1->x)&&(!inorout_bottom(subject[i], temp_x + 1, temp_y, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&((subject[i][j]->next_1->next_1->x < subject[i][j]->next_1->x)&&(clip[k]->next_1->x < clip[k]->x))&&(!inorout_bottom(subject[i], temp_x+1, temp_y, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&((subject[i][j]->next_1->next_1->x > subject[i][j]->next_1->x)&&(clip[k]->next_1->x > clip[k]->x))&&(!inorout_bottom(subject[i], temp_x-1, temp_y, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_2 == nullptr)
                                {
                                    clip[k]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }


                        }
                        else if((clip[k]->next_1->x) == temp_x)
                        {
                            bool check_2 = false;
                            bool check_3 = false;
                            // case 4-2
                            if(((subject[i][j]->y < temp_y)&&(subject[i][j]->next_1->y > temp_y))||((subject[i][j]->y > temp_y)&&(subject[i][j]->next_1->y < temp_y)))
                            {
                                bool check = false;
                                if(((clip[k]->next_1->x) > clip[k]->x)&&(!(inorout_bottom(subject[i], temp_x-1, temp_y, -1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }
                                else if(((clip[k]->next_1->x) < clip[k]->x)&&(!(inorout_bottom(subject[i], temp_x+1, temp_y, 1))))
                                {
                                    intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                    intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = true;
                                    check = true;
                                }

                                if(check)
                                {
                                    if(clip[k]->next_1->next_2 == nullptr)
                                    {
                                        clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = clip[k]->next_1;
                                        intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                        compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    }

                                    if(subject[i][j]->next_2 == nullptr)
                                    {
                                        subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                        intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                    }
                                    else
                                    {
                                        Node* compare = subject[i][j];
                                        if(subject[i][j]->y < temp_y)
                                        {
                                            if(compare->next_2->y < temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y < temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }
                                        else
                                        {
                                            if(compare->next_2->y > temp_y)
                                            {
                                                compare = compare->next_2;
                                                while(compare->next_1->y > temp_y)
                                                {
                                                    compare = compare->next_1;
                                                }
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_1;
                                                compare->next_1 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                            else
                                            {
                                                intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                                compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                            }
                                        }

                                    }

                                }

                            }
                            // case 3-2
                            else if((subject[i][j]->y == temp_y)&&(x_previous > subject[i][j]->x)&&(clip[k]->x > clip[k]->next_1->x)&&(!inorout_bottom(subject[i], temp_x-1, temp_y, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->y == temp_y)&&(x_previous < subject[i][j]->x)&&(clip[k]->x < clip[k]->next_1->x)&&(!inorout_bottom(subject[i], temp_x+1, temp_y, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_2 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&(subject[i][j]->next_1->next_1->x < subject[i][j]->next_1->x)&&(clip[k]->x < clip[k]->next_1->x)&&(!inorout_bottom(subject[i], temp_x+1, temp_y, 1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }
                            else if((subject[i][j]->next_1->y == temp_y)&&(subject[i][j]->next_1->next_1->x > subject[i][j]->next_1->x)&&(clip[k]->x > clip[k]->next_1->x)&&(!inorout_bottom(subject[i], temp_x-1, temp_y, -1)))
                            {
                                intersection_points[i].push_back(new Node(temp_x, temp_y, true));
                                intersection_points[i][intersection_points[i].size()-1]->clip_into_subject = false;
                                check_3 = true;
                            }

                            if(check_2)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_2 == nullptr)
                                {
                                    subject[i][j]->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                            if(check_3)
                            {
                                if(clip[k]->next_1->next_2 == nullptr)
                                {
                                    clip[k]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = clip[k]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = clip[k]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_2 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }

                                if(subject[i][j]->next_1->next_2 == nullptr)
                                {
                                    subject[i][j]->next_1->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = subject[i][j]->next_1->next_1;
                                }
                                else
                                {
                                    Node* compare = subject[i][j]->next_1;
                                    intersection_points[i][intersection_points[i].size()-1]->next_1 = compare->next_2;
                                    compare->next_2 = intersection_points[i][intersection_points[i].size()-1];
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    bool add_merge = true;//see whether the merge need to add into the structure or not
    // no intersections, all the subject polygons should check with clip respectively
    const int const_newPolygon_size = a.newPolygon.size();
    int counting_Polygon = 0;
    int i = 0;
    vector<int> remains;
    while(counting_Polygon < const_newPolygon_size)
    {
        counting_Polygon++;
        if(a.newPolygon.at(i)->boxt < b.newPolygon.at(0)->boxb)
        {
            i++;
            continue;
        }
        else if(a.newPolygon.at(i)->boxb > b.newPolygon.at(0)->boxt)
        {
            i++;
            continue;
        }
        else if(a.newPolygon.at(i)->boxl > b.newPolygon.at(0)->boxr)
        {
            i++;
            continue;
        }
        else if(a.newPolygon.at(i)->boxr < b.newPolygon.at(0)->boxl)
        {
            i++;
            continue;
        }

        if(intersection_points.at(i).size()==0)
        {
            //merge special case 1
            int x_temp = clip.at(0)->x;
            int y_temp = clip.at(0)->y;
            double change_x = 0;
            double change_y = 0;
            if(inorout_merge(clip, x_temp-1, y_temp-1,-1,-1))
            {
                change_x = -1;
                change_y = -1;
            }
            else if(inorout_merge(clip, x_temp+1, y_temp+1,1,1))
            {
                change_x = 1;
                change_y = 1;
            }
            else if(inorout_merge(clip, x_temp-1, y_temp+1,-1,1))
            {
                change_x = -1;
                change_y = 1;
            }
            else if(inorout_merge(clip, x_temp+1, y_temp-1,1,-1))
            {
                change_x = 1;
                change_y = -1;
            }
            if(inorout_merge(subject.at(i), x_temp+change_x, y_temp+change_y,change_x,change_y))
            {
                add_merge = false;
                int k = 0;
                const int const_newHole_size = a.newPolygon.at(i)->newHole.size();
                int counting_Hole = 0;
                while(counting_Hole < const_newHole_size)
                {
                    counting_Hole++;
                    int hole_x = a.newPolygon.at(i)->newHole.at(k).at(0)->x;
                    int hole_y = a.newPolygon.at(i)->newHole.at(k).at(0)->y;
                    change_x = 0;
                    change_y = 0;
                    if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x-1, hole_y-1,-1,-1))
                    {
                        change_x = -1;
                        change_y = -1;
                    }
                    else if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x+1, hole_y+1,1,1))
                    {
                        change_x = 1;
                        change_y = 1;
                    }
                    else if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x-1, hole_y+1,-1,1))
                    {
                        change_x = -1;
                        change_y = 1;
                    }
                    else if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x+1, hole_y-1,1,-1))
                    {
                        change_x = 1;
                        change_y = -1;
                    }
                    if(inorout_merge(clip, hole_x+change_x, hole_y+change_y, change_x, change_y))
                    {
                        swap(a.newPolygon.at(i)->newHole.at(k), a.newPolygon.at(i)->newHole.at(a.newPolygon.at(i)->newHole.size()-1));
                        for(unsigned q=0;q<a.newPolygon.at(i)->newHole.at(a.newPolygon.at(i)->newHole.size()-1).size();q++)
                            delete a.newPolygon.at(i)->newHole.at(a.newPolygon.at(i)->newHole.size()-1).at(q);
                        a.newPolygon.at(i)->newHole.pop_back();
                    }
                    else
                        k++;
                }
            }
            else if(inorout_merge(a.newPolygon.at(i)->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y))
            {
                int k = 0;
                const int const_newHole_size = a.newPolygon.at(i)->newHole.size();
                int counting_Hole = 0;
                bool case2 = true;
                while(counting_Hole < const_newHole_size)
                {
                    counting_Hole++;
                    int hole_x = a.newPolygon.at(i)->newHole.at(k).at(0)->x;
                    int hole_y = a.newPolygon.at(i)->newHole.at(k).at(0)->y;
                    change_x = 0;
                    change_y = 0;
                    if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x-1, hole_y-1,-1,-1))
                    {
                        change_x = -1;
                        change_y = -1;
                    }
                    else if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x+1, hole_y+1,1,1))
                    {
                        change_x = 1;
                        change_y = 1;
                    }
                    else if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x-1, hole_y+1,-1,1))
                    {
                        change_x = -1;
                        change_y = 1;
                    }
                    else if(inorout_merge(a.newPolygon[i]->newHole[k], hole_x+1, hole_y-1,1,-1))
                    {
                        change_x = 1;
                        change_y = -1;
                    }
                    if(inorout_merge(clip, hole_x+change_x, hole_y+change_y, change_x, change_y))
                    {
                        case2 = false;
                        swap(a.newPolygon.at(i)->newHole.at(k), a.newPolygon.at(i)->newHole.at(a.newPolygon.at(i)->newHole.size()-1));
                        for(unsigned q=0;q<a.newPolygon.at(i)->newHole.at(a.newPolygon.at(i)->newHole.size()-1).size();q++)
                            delete a.newPolygon.at(i)->newHole.at(a.newPolygon.at(i)->newHole.size()-1).at(q);
                        a.newPolygon.at(i)->newHole.pop_back();
                        add_merge = false;
                    }
                    else
                        k++;
                }
            }
            else
            {

                x_temp = a.newPolygon.at(i)->newNode.at(0)->x;
                y_temp = a.newPolygon.at(i)->newNode.at(0)->y;
                change_x = 0;
                change_y = 0;
                if(inorout_merge(a.newPolygon.at(i)->newNode, x_temp-1, y_temp-1,-1,-1))
                {
                    change_x = -1;
                    change_y = -1;
                }
                else if(inorout_merge(a.newPolygon.at(i)->newNode, x_temp+1, y_temp+1,1,1))
                {
                    change_x = 1;
                    change_y = 1;
                }
                else if(inorout_merge(a.newPolygon.at(i)->newNode, x_temp-1, y_temp+1,-1,1))
                {
                    change_x = -1;
                    change_y = 1;
                }
                else if(inorout_merge(a.newPolygon.at(i)->newNode, x_temp+1, y_temp-1,1,-1))
                {
                    change_x = 1;
                    change_y = -1;
                }
                if(inorout_merge(clip, x_temp+change_x, y_temp+change_y, change_x, change_y))
                {
                    swap(subject.at(i), subject.at(subject.size()-1));
                    swap(a.newPolygon.at(i), a.newPolygon.at(a.newPolygon.size()-1));
                    swap(intersection_points.at(i),intersection_points.at(intersection_points.size()-1));
                    for(unsigned de = 0;de<a.newPolygon.at(a.newPolygon.size()-1)->newNode.size();de++)
                        delete a.newPolygon.at(a.newPolygon.size()-1)->newNode[de];
                    for(unsigned de = 0;de<a.newPolygon.at(a.newPolygon.size()-1)->newHole.size();de++)
                    {
                        for(unsigned f = 0;f<a.newPolygon.at(a.newPolygon.size()-1)->newHole[de].size();f++)
                            delete a.newPolygon.at(a.newPolygon.size()-1)->newHole[de][f];
                    }
                    delete a.newPolygon.at(a.newPolygon.size()-1);
                    a.newPolygon.pop_back();
                    subject.pop_back();
                    intersection_points.pop_back();
                    continue;
                }
            }
            i++;
        }
        else
        {
            add_merge = false;
	    remains.push_back(i);
            i++;
        }
    }
    if(add_merge)
    {
        a.newPolygon.push_back(b.newPolygon.at(0));
        vector < vector < Node* > > ().swap(subject);
        vector < Node* >().swap(clip);
        vector < vector < Node* > > ().swap(intersection_points);
        return a;
    }

    vector <int> ans; //only used to expand rows
    vector <Node*> intersect_oneD;  //used to check if there's any intersection point that hasn't been used
    vector <int> numofpolygon; //store the number of polygons that has intersection points
    for(unsigned i=0;i<intersection_points.size();i++)
    {
        if(intersection_points.at(i).size()!=0)
            numofpolygon.push_back(i);
        for(unsigned j=0;j<intersection_points.at(i).size();j++)
            intersect_oneD.push_back(intersection_points.at(i).at(j));
    }
    for(int j=0;j<intersect_oneD.size();j++)
    {
        for(int h=j+1;h<intersect_oneD.size();h++)
        {
            if((intersect_oneD.at(j)->x == intersect_oneD.at(h)->x)&&(intersect_oneD.at(j)->y == intersect_oneD.at(h)->y))
            {
                intersect_oneD.at(j)->repeat_appear = true;
                intersect_oneD.at(h)->repeat_appear = true;
                intersect_oneD.at(j)->repeat_point = h;
                intersect_oneD.at(h)->repeat_point = j;
            }
        }
    }
    const int intersize = intersect_oneD.size();
    if(intersect_oneD.size()%2!=0)
    {
        for(int j=0;j<intersize;j++)
        {
            for(int k=0;k<subject.size();k++)
            {
                int counter = 0;
                for(int m=0;m<subject.at(k).size();m++)
                {
                    if((subject.at(k).at(m)->x == intersect_oneD.at(j)->x)&&(subject.at(k).at(m)->y == intersect_oneD.at(j)->y))
                    {
                        counter++;
                    }
                }
                if(counter == 2)
                {
                    intersect_oneD.push_back(new Node(intersect_oneD.at(j)->x, intersect_oneD.at(j)->y, true));
                    intersect_oneD.at(intersect_oneD.size()-1)->clip_into_subject = !intersect_oneD.at(j)->clip_into_subject;
                    intersect_oneD.at(j)->repeat_appear = true;
                    intersect_oneD.at(intersect_oneD.size()-1)->repeat_appear = true;
                    intersect_oneD.at(j)->repeat_point = intersect_oneD.size()-1;
                    intersect_oneD.at(intersect_oneD.size()-1)->repeat_point = j;
                    intersect_oneD.at(intersect_oneD.size()-1)->next_1 = intersect_oneD.at(j)->next_1;
                    intersect_oneD.at(j)->next_1 = intersect_oneD.at(intersect_oneD.size()-1);
                    intersect_oneD.at(intersect_oneD.size()-1)->next_2 = intersect_oneD.at(j)->next_2;
                    intersect_oneD.at(j)->next_2 = intersect_oneD.at(intersect_oneD.size()-1);
                    break;
                }
            }
        }
    }
    vector< vector <int> > answer;


    for(unsigned i=0;i<intersect_oneD.size();i++)
    {
        if((intersect_oneD.at(i)->Used == false)&&(intersect_oneD.at(i)->repeat_appear == false))
        {
            answer.push_back(ans);
            Node* temp = intersect_oneD.at(i);
            bool now_in_or_out = !temp->clip_into_subject;
            int changing = 0;

            while((temp != intersect_oneD.at(i))||(temp->Used == false))
            {
                if((temp->clip_into_subject == false)&&(now_in_or_out == true)&&(temp->repeat_appear == false))
                {
                    now_in_or_out = false;
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);
                    temp = temp->next_2;  //first clip vertex or directly an exit intersection point
                    if(temp->Inters == false)  //first clip vertex
                    {
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                        {
                            temp = temp->next_1;
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                        }
                        temp = temp->next_2;
                    }
                }
                else if((temp->clip_into_subject == true)&&(now_in_or_out == false)&&(temp->repeat_appear == false))
                {
                    now_in_or_out = true;
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);
                    temp = temp->next_1;  //first clip vertex or directly an exit intersection point
                    if(temp->Inters == false)  //first clip vertex
                    {
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                        {
                            temp = temp->next_1;
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                        }
                        temp = temp->next_2;
                    }
                }
                else if((now_in_or_out == true)&&(temp->repeat_appear == true))
                {
                    temp->repeat_appear = false;
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);

                    temp = intersect_oneD.at(temp->repeat_point);
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);

                    if(!temp->clip_into_subject)
                        temp = intersect_oneD.at(temp->repeat_point);
                    now_in_or_out = true;
                    temp = temp->next_1;  //first clip vertex or directly an exit intersection point
                    if(temp->Inters == false)  //first clip vertex
                    {
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                        {
                            temp = temp->next_1;
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                        }
                        temp = temp->next_2;
                    }
                }
                else if((now_in_or_out == false)&&(temp->repeat_appear == true))
                {
                    temp->repeat_appear = false;
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);

                    temp = intersect_oneD.at(temp->repeat_point);
                    temp->Used = true;
                    answer.at(answer.size()-1).push_back(temp->x);
                    answer.at(answer.size()-1).push_back(temp->y);
                    if(temp->clip_into_subject)
                        temp = intersect_oneD.at(temp->repeat_point);
                    now_in_or_out = false;
                    temp = temp->next_2;  //first clip vertex or directly an exit intersection point
                    if(temp->Inters == false)  //first clip vertex
                    {
                        answer.at(answer.size()-1).push_back(temp->x);
                        answer.at(answer.size()-1).push_back(temp->y);
                        while(temp->next_2 == nullptr)  // if this vertex can't reach intersection point
                        {
                            temp = temp->next_1;
                            answer.at(answer.size()-1).push_back(temp->x);
                            answer.at(answer.size()-1).push_back(temp->y);
                        }
                        temp = temp->next_2;
                    }
                }

            }
            answer.at(answer.size()-1).push_back(intersect_oneD.at(i)->x);
            answer.at(answer.size()-1).push_back(intersect_oneD.at(i)->y);

        }

    }

    if(answer.size()>0)
        Initialize_nodeorhole_after_merge(answer, a, numofpolygon, clip);
    else if((intersect_oneD.size()==8)&&(numofpolygon.size()>1))
    {
    for(unsigned i=0;i<intersect_oneD.size();i++)
    {
        if((intersect_oneD.at(i)->Used == false)&&(intersect_oneD.at(i)->clip_into_subject == true))
        {
            answer.push_back(ans);
            Node* temp = intersect_oneD.at(i);

            while((temp != intersect_oneD.at(i))||(temp->Used == false))
            {
                temp->Used = true;
                answer.at(answer.size()-1).push_back(temp->x);
                answer.at(answer.size()-1).push_back(temp->y);
		if((temp->Inters))
		{
		    intersect_oneD.at(temp->repeat_point)->Used = true;
		    if(!temp->clip_into_subject)
		        temp = intersect_oneD.at(temp->repeat_point);
		    temp = temp->next_1;
		}
		else if((temp->next_2 == nullptr))
		{
		    temp = temp->next_1;

		}
		else if((temp->next_2->Inters))
		{
		    temp = temp->next_2;
		    if(!temp->clip_into_subject)
		        temp = intersect_oneD.at(temp->repeat_point);
                }
            }
            answer.at(answer.size()-1).push_back(intersect_oneD.at(i)->x);
            answer.at(answer.size()-1).push_back(intersect_oneD.at(i)->y);

        }

    }
    Initialize_nodeorhole_after_merge(answer, a, numofpolygon, clip);
    }
    else
    {
        int x_temp = clip.at(0)->x;
        int y_temp = clip.at(0)->y;
        double change_x = 0;
        double change_y = 0;
        if(inorout_merge(clip, x_temp-1, y_temp-1,-1,-1))
        {
            change_x = -1;
            change_y = -1;
        }
        else if(inorout_merge(clip, x_temp+1, y_temp+1,1,1))
        {
            change_x = 1;
            change_y = 1;
        }
        else if(inorout_merge(clip, x_temp-1, y_temp+1,-1,1))
        {
            change_x = -1;
            change_y = 1;
        }
        else if(inorout_merge(clip, x_temp+1, y_temp-1,1,-1))
        {
            change_x = 1;
            change_y = -1;
        }
	for(int w=0;w<remains.size();w++)
	{
            if(inorout_merge(a.newPolygon.at(remains.at(w))->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y))
            {
                int k = 0;
                const int const_newHole_size = a.newPolygon.at(remains.at(w))->newHole.size();
                int counting_Hole = 0;
                while(counting_Hole < const_newHole_size)
                {
                    counting_Hole++;
                    int hole_x = a.newPolygon.at(remains.at(w))->newHole.at(k).at(0)->x;
                    int hole_y = a.newPolygon.at(remains.at(w))->newHole.at(k).at(0)->y;
                    change_x = 0;
                    change_y = 0;
                    if(inorout_merge(a.newPolygon.at(remains.at(w))->newHole[k], hole_x-1, hole_y-1,-1,-1))
                    {
                        change_x = -1;
                        change_y = -1;
                    }
                    else if(inorout_merge(a.newPolygon.at(remains.at(w))->newHole[k], hole_x+1, hole_y+1,1,1))
                    {
                        change_x = 1;
                        change_y = 1;
                    }
                    else if(inorout_merge(a.newPolygon.at(remains.at(w))->newHole[k], hole_x-1, hole_y+1,-1,1))
                    {
                        change_x = -1;
                        change_y = 1;
                    }
                    else if(inorout_merge(a.newPolygon.at(remains.at(w))->newHole[k], hole_x+1, hole_y-1,1,-1))
                    {
                        change_x = 1;
                        change_y = -1;
                    }
                    if(inorout_merge(clip, hole_x+change_x, hole_y+change_y, change_x, change_y))
                    {
                        swap(a.newPolygon.at(remains.at(w))->newHole.at(k), a.newPolygon.at(remains.at(w))->newHole.at(a.newPolygon.at(remains.at(w))->newHole.size()-1));
                        for(unsigned q=0;q<a.newPolygon.at(remains.at(w))->newHole.at(a.newPolygon.at(remains.at(w))->newHole.size()-1).size();q++)
                            delete a.newPolygon.at(remains.at(w))->newHole.at(a.newPolygon.at(remains.at(w))->newHole.size()-1).at(q);
                        a.newPolygon.at(remains.at(w))->newHole.pop_back();
                    }
                    else
                        k++;
                }
            }
	}
    }

    vector < vector < Node* > > ().swap(subject);
    vector< vector <int> > ().swap(answer);
    vector < Node* >().swap(clip);
    vector < Node* >().swap(intersect_oneD);
    vector < vector < Node* > > ().swap(intersection_points);
    vector <int>().swap(numofpolygon);
    return a;

}


void Structure::Initialize_nodeorhole_after_merge(const vector< vector < int > > & coordinate, Structure& a,vector<int>& num_of_newPolygon, const vector <Node*> &clip)
{
    vector < vector <Node*> > holes;
    vector < vector <Node*> > new_add_holes;
    vector <Node*> temporary;
    sort(num_of_newPolygon.begin(),num_of_newPolygon.end());
    int num_of_newpoly = 0;
    //load all the holes in the original polygons
    for(size_t k=0;k<num_of_newPolygon.size();k++)
    {
        for(size_t i=0;i<a.newPolygon[num_of_newPolygon.at(k)]->newHole.size();i++)
        {
            if(a.newPolygon[num_of_newPolygon.at(k)]->newHole.size()==0)
            {
                break;
            }
            holes.push_back(a.newPolygon[num_of_newPolygon.at(k)]->newHole.at(i));
        }
    }



    //erase all the holes in the original polygons
    for(size_t k=0;k<num_of_newPolygon.size();k++)
    {
        a.newPolygon[num_of_newPolygon.at(k)]->newHole.clear();
    }


    bool check_if_polygon_add = true;
    int num_of_holes = 0;

    for(unsigned ab=0;ab<coordinate.size();ab++)
    {
        int left_min = coordinate[ab][0]+1;
        bool iscounter = false;
        vector <int> newone = {};


        for(unsigned i=0;i<coordinate[ab].size();i+=2)
        {
            if(i < coordinate[ab].size()-2)
            {
                if((coordinate[ab][i]!=coordinate[ab][i+2])||(coordinate[ab][i+1]!=coordinate[ab][i+3]))
                {
                    newone.push_back(coordinate[ab][i]);
                    newone.push_back(coordinate[ab][i+1]);
                }
            }
            else if(i == coordinate[ab].size()-2)
            {
                newone.push_back(coordinate[ab][i]);
                newone.push_back(coordinate[ab][i+1]);
            }

            if(newone.size()>=6)
            {
                if((newone.at(newone.size()-1)==newone.at(newone.size()-3))&&(newone.at(newone.size()-3)==newone.at(newone.size()-5)))
                {
                    newone.erase(newone.end()-3);
                    newone.erase(newone.end()-3);
                }
                else if((newone.at(newone.size()-2)==newone.at(newone.size()-4))&&(newone.at(newone.size()-4)==newone.at(newone.size()-6)))
                {
                    newone.erase(newone.end()-3);
                    newone.erase(newone.end()-3);
                }
            }
            if(newone.size()>=4)
            {
                if((newone.at(newone.size()-1)==newone.at(newone.size()-3))&&(newone.at(newone.size()-2)==newone.at(newone.size()-4)))
                {
                    newone.erase(newone.end()-1);
                    newone.erase(newone.end()-1);
                }
            }
        }
        if(newone.size()<=4)
            continue;
        if((newone.at(newone.size()-1)==newone.at(newone.size()-3))&&(newone.at(1)==newone.at(3)))
        {
            newone.erase(newone.end()-1);
            newone.erase(newone.end()-1);
            newone.at(0) = newone.at(newone.size()-2);
            newone.at(1) = newone.at(newone.size()-1);
        }
        if((newone.at(newone.size()-2)==newone.at(newone.size()-4))&&(newone.at(0)==newone.at(2)))
        {
            newone.erase(newone.end()-1);
            newone.erase(newone.end()-1);
            newone.at(0) = newone.at(newone.size()-2);
            newone.at(1) = newone.at(newone.size()-1);
        }

        for(unsigned i=0;i<newone.size();i+=2)
        {
            if(i < newone.size()-2)
            {
                if((newone.at(i)==newone.at(i+2))&&(left_min >= newone.at(i)))
                {
                    if(newone.at(i+1)>newone.at(i+3))
                    {
                        left_min = newone.at(i);
                        iscounter = true;
                    }
                    else if(newone.at(i+1)<newone.at(i+3))
                    {
                        left_min = newone.at(i);
                        iscounter = false;
                    }
                }
            }
        }
        // if y is first big then small -> counterclockwise
        if(newone.size()<8)
            continue;
        if(iscounter) //check if it's counterclockwise
        {
            //check_if_polygon_delete = true;
            a.newPolygon.push_back(new Polygon(newone, true));
            ++num_of_newpoly;
            check_if_polygon_add = false;
        }
        else
        {
            if(newone.size()!=0){
            new_add_holes.push_back(temporary);
            for(unsigned i=0;i<newone.size()-2;i+=2)
            {
                new_add_holes.at(new_add_holes.size()-1).push_back(new Node(newone[i], newone[i+1], false));
            }
            ++num_of_holes;

            for(unsigned i=0; i<new_add_holes.at(new_add_holes.size()-1).size()-1; i++)
                new_add_holes.at(new_add_holes.size()-1).at(i)->next_1 = new_add_holes.at(new_add_holes.size()-1).at(i+1);
            new_add_holes.at(new_add_holes.size()-1).at(new_add_holes.at(new_add_holes.size()-1).size()-1)->next_1 = new_add_holes.at(new_add_holes.size()-1).at(0);
            }
        }
    }


    if(num_of_newpoly > 1)
    {
        cout<<"bad situation"<<endl;
        vector <int> tools = {0,0,0,2,2,2,2,0,0,0};
        Structure temporary(tools, true);
        vector < Polygon* >().swap(temporary.newPolygon);
        temporary.newPolygon.push_back(a.newPolygon.at(a.newPolygon.size()-1));
        a.newPolygon.pop_back();
        while(num_of_newpoly>1)
        {
            num_of_newpoly--;
            Structure badstruct(tools, true);
            vector < Polygon* >().swap(badstruct.newPolygon);
            badstruct.newPolygon.push_back(a.newPolygon.at(a.newPolygon.size()-1));
            a.newPolygon.pop_back();
            temporary = temporary.Find_intersection_merge(temporary, badstruct);
        }
        a.newPolygon.push_back(temporary.newPolygon.at(0));
    }



    for(int i=0;i<new_add_holes.size();i++)
    {
        int x_temp = new_add_holes.at(i).at(0)->x;
        int y_temp = new_add_holes.at(i).at(0)->y;
        int change_x = 0;
        int change_y = 0;
        if(inorout_merge(new_add_holes.at(i), x_temp-1, y_temp-1,-1,-1))
        {
            change_x = -1;
            change_y = -1;
        }
        else if(inorout_merge(new_add_holes.at(i), x_temp+1, y_temp+1,1,1))
        {
            change_x = 1;
            change_y = 1;
        }
        else if(inorout_merge(new_add_holes.at(i), x_temp-1, y_temp+1,-1,1))
        {
            change_x = -1;
            change_y = 1;
        }
        else if(inorout_merge(new_add_holes.at(i), x_temp+1, y_temp-1,1,-1))
        {
            change_x = 1;
            change_y = -1;
        }
        int s = 0;
        int counting_s = 0;
        const int const_hole_size = holes.size();
        while(counting_s < const_hole_size)
        {
            counting_s++;
            if(inorout_merge(holes.at(s), x_temp+change_x, y_temp+change_y, change_x, change_y))
            {
                swap(holes.at(s), holes.at(holes.size()-1));
                for(unsigned q=0;q<holes.at(holes.size()-1).size();q++)
                    delete holes.at(holes.size()-1).at(q);
                holes.pop_back();
                continue;

            }
            s++;
        }
    }
    for(int j=0;j<new_add_holes.size();j++)
    {
        holes.push_back(new_add_holes.at(j));
    }

    int counting_ss = 0;
    int ss = 0;
    const int const_hole_size_ = holes.size();
    while(counting_ss < const_hole_size_)
    {
        counting_ss++;
        int xx_temp = holes.at(ss).at(0)->x;
        int yy_temp = holes.at(ss).at(0)->y;
        int change_xx = 0;
        int change_yy = 0;
        if(inorout_merge(holes.at(ss), xx_temp-1, yy_temp-1,-1,-1))
        {
            change_xx = -1;
            change_yy = -1;
        }
        else if(inorout_merge(holes.at(ss), xx_temp+1, yy_temp+1,1,1))
        {
            change_xx = 1;
            change_yy = 1;
        }
        else if(inorout_merge(holes.at(ss), xx_temp-1, yy_temp+1,-1,1))
        {
            change_xx = -1;
            change_yy = 1;
        }
        else if(inorout_merge(holes.at(ss), xx_temp+1, yy_temp-1,1,-1))
        {
            change_xx = 1;
            change_yy = -1;
        }
        if(inorout_merge(clip, xx_temp+change_xx, yy_temp+change_yy, change_xx, change_yy))
        {
            swap(holes.at(ss), holes.at(holes.size()-1));
            for(unsigned q=0;q<holes.at(holes.size()-1).size();q++)
                delete holes.at(holes.size()-1).at(q);
            holes.pop_back();
            continue;
        }
        ss++;
    }




    reverse(num_of_newPolygon.begin(),num_of_newPolygon.end());

    if(check_if_polygon_add)
    {
        int counting_Poly = 0;
        int poly = 0;
        int the_biggest_poly = 0;
        const int const_poly_size = num_of_newPolygon.size();
        while(counting_Poly < const_poly_size)
        {
            counting_Poly++;
            int x_temp = clip.at(0)->x;
            int y_temp = clip.at(0)->y;
            int change_x = 0;
            int change_y = 0;
            if(inorout_merge(clip, x_temp-1, y_temp-1,-1,-1))
            {
                change_x = -1;
                change_y = -1;
            }
            else if(inorout_merge(clip, x_temp+1, y_temp+1,1,1))
            {
                change_x = 1;
                change_y = 1;
            }
            else if(inorout_merge(clip, x_temp-1, y_temp+1,-1,1))
            {
                change_x = -1;
                change_y = 1;
            }
            else if(inorout_merge(clip, x_temp+1, y_temp-1,1,-1))
            {
                change_x = 1;
                change_y = -1;
            }

            if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(poly))->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y))
            {
                for(unsigned j=0;j<holes.size();j++)
                {
                    x_temp = holes.at(j).at(0)->x;
                    y_temp = holes.at(j).at(0)->y;
                    change_x = 0;
                    change_y = 0;
                    if(inorout_merge(holes.at(j), x_temp-1, y_temp-1,-1,-1))
                    {
                        change_x = -1;
                        change_y = -1;
                    }
                    else if(inorout_merge(holes.at(j), x_temp+1, y_temp+1,1,1))
                    {
                        change_x = 1;
                        change_y = 1;
                    }
                    else if(inorout_merge(holes.at(j), x_temp-1, y_temp+1,-1,1))
                    {
                        change_x = -1;
                        change_y = 1;
                    }
                    else if(inorout_merge(holes.at(j), x_temp+1, y_temp-1,1,-1))
                    {
                        change_x = 1;
                        change_y = -1;
                    }

                    if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(poly))->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y))
                    {
                        a.newPolygon.at(num_of_newPolygon.at(poly))->newHole.push_back(temporary);
                        a.newPolygon.at(num_of_newPolygon.at(poly))->newHole.at(a.newPolygon.at(num_of_newPolygon.at(poly))->newHole.size()-1) = holes.at(j);
                        if(j == holes.size()-1)
                            the_biggest_poly = poly;
                    }
                }
            }
            poly++;
        }
        counting_Poly = 0;
        poly = 0;
        while(counting_Poly < const_poly_size)
        {
            counting_Poly++;
            if(the_biggest_poly == poly)
            {
                poly++;
                continue;
            }
            int x_temp = a.newPolygon.at(num_of_newPolygon.at(poly))->newNode.at(0)->x;
            int y_temp = a.newPolygon.at(num_of_newPolygon.at(poly))->newNode.at(0)->y;
            int change_x = 0;
            int change_y = 0;
            if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(poly))->newNode, x_temp-1, y_temp-1,-1,-1))
            {
                change_x = -1;
                change_y = -1;
            }
            else if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(poly))->newNode, x_temp+1, y_temp+1,1,1))
            {
                change_x = 1;
                change_y = 1;
            }
            else if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(poly))->newNode, x_temp-1, y_temp+1,-1,1))
            {
                change_x = -1;
                change_y = 1;
            }
            else if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(poly))->newNode, x_temp+1, y_temp-1,1,-1))
            {
                change_x = 1;
                change_y = -1;
            }
            if(inorout_merge(a.newPolygon.at(num_of_newPolygon.at(the_biggest_poly))->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y))
            {
                swap(a.newPolygon.at(num_of_newPolygon.at(poly)), a.newPolygon.at(a.newPolygon.size()-1));
                vector < vector < Node* > > ().swap(a.newPolygon.at(a.newPolygon.size()-1)->newHole);
                for(unsigned ii=0;ii<a.newPolygon.at(a.newPolygon.size()-1)->newNode.size();ii++)
                    delete a.newPolygon.at(a.newPolygon.size()-1)->newNode[ii];
                delete a.newPolygon.at(a.newPolygon.size()-1);
                a.newPolygon.pop_back();
            }
            poly++;

        }
    }
    else  //delete all original newPolygon, and load all holes into the new newPolygon
    {
        for(unsigned i=0;i<holes.size();i++)
        {
            a.newPolygon.at(a.newPolygon.size()-1)->newHole.push_back(temporary);
            a.newPolygon.at(a.newPolygon.size()-1)->newHole.at(a.newPolygon.at(a.newPolygon.size()-1)->newHole.size()-1) = holes.at(i);
        }
        int counting_Poly = 0;
        int poly = 0;
        const int const_poly_size = num_of_newPolygon.size();
        while(counting_Poly < const_poly_size)
        {
            counting_Poly++;
            swap(a.newPolygon.at(num_of_newPolygon.at(poly)), a.newPolygon.at(a.newPolygon.size()-1));
            for(unsigned ii=0;ii<a.newPolygon.at(a.newPolygon.size()-1)->newNode.size();ii++)
                delete a.newPolygon.at(a.newPolygon.size()-1)->newNode[ii];
            delete a.newPolygon.at(a.newPolygon.size()-1);
            a.newPolygon.pop_back();
            poly++;
        }
    }
    vector < vector < Node* > > ().swap(new_add_holes);
    vector < vector < Node* > > ().swap(holes);
}



void Structure::Initialize_nodeorhole_after_clip(const vector< vector < int > > & coordinate, Structure& a, unsigned num_of_newPolygon, bool &sub_intersect_change, bool &must)
{
    vector < vector <Node*> > holes = a.newPolygon[num_of_newPolygon]->newHole;
    vector <Node*> temporary;

    bool check_if_polygon_delete = false;
    int num_of_newpoly = 0;
    int num_of_newhole = 0;
    for(unsigned ab=0;ab<coordinate.size();ab++)
    {
        int left_min = coordinate[ab][0]+1;
        bool iscounter = false;
        vector <int> newone;
	if(coordinate[ab].size()<8)
	    continue;
        for(unsigned i=0;i<coordinate[ab].size();i+=2)
        {
            if(i < coordinate[ab].size()-2)
            {
                if((coordinate[ab][i]!=coordinate[ab][i+2])||(coordinate[ab][i+1]!=coordinate[ab][i+3]))
                {
                    newone.push_back(coordinate[ab][i]);
                    newone.push_back(coordinate[ab][i+1]);
                }
            }
            else if(i == coordinate[ab].size()-2)
            {
                newone.push_back(coordinate[ab][i]);
                newone.push_back(coordinate[ab][i+1]);
            }

            if(newone.size()>=6)
            {
                if((newone.at(newone.size()-1)==newone.at(newone.size()-3))&&(newone.at(newone.size()-3)==newone.at(newone.size()-5)))
                {
                    newone.erase(newone.end()-3);
                    newone.erase(newone.end()-3);
                }
                else if((newone.at(newone.size()-2)==newone.at(newone.size()-4))&&(newone.at(newone.size()-4)==newone.at(newone.size()-6)))
                {
                    newone.erase(newone.end()-3);
                    newone.erase(newone.end()-3);
                }
            }
            if(newone.size()>=4)
            {
                if((newone.at(newone.size()-1)==newone.at(newone.size()-3))&&(newone.at(newone.size()-2)==newone.at(newone.size()-4)))
                {
                    newone.erase(newone.end()-1);
                    newone.erase(newone.end()-1);
                }
            }
        }
        if(newone.size()<=4)
            continue;
        if((newone.at(newone.size()-1)==newone.at(newone.size()-3))&&(newone.at(1)==newone.at(3)))
        {
            newone.erase(newone.end()-1);
            newone.erase(newone.end()-1);
            newone.at(0) = newone.at(newone.size()-2);
            newone.at(1) = newone.at(newone.size()-1);
        }
        if((newone.at(newone.size()-2)==newone.at(newone.size()-4))&&(newone.at(0)==newone.at(2)))
        {
            newone.erase(newone.end()-1);
            newone.erase(newone.end()-1);
            newone.at(0) = newone.at(newone.size()-2);
            newone.at(1) = newone.at(newone.size()-1);
        }
        for(unsigned i=0;i<newone.size();i+=2)
        {
            if(i < newone.size()-2)
            {
                if((newone.at(i)==newone.at(i+2))&&(left_min >= newone.at(i)))
                {
                    if(newone.at(i+1)>newone.at(i+3))
                    {
                        left_min = newone.at(i);
                        iscounter = true;
                    }
                    else if(newone.at(i+1)<newone.at(i+3))
                    {
                        left_min = newone.at(i);
                        iscounter = false;
                    }
                }
            }
        }
        if(newone.size()<8)
            continue;
        // if y is first big then small -> counterclockwise
        if(iscounter) //check if it's counterclockwise
        {
            check_if_polygon_delete = true;
            a.newPolygon.push_back(new Polygon(newone, true));
            ++num_of_newpoly;
        }
        else
        {
            ++num_of_newhole;

            holes.push_back(temporary);
            for(unsigned i=0;i<newone.size()-2;i+=2)
            {
                holes.at(holes.size()-1).push_back(new Node(newone[i], newone[i+1], false));
            }
            for(unsigned i=0; i<holes.at(holes.size()-1).size()-1; i++)
                holes.at(holes.size()-1).at(i)->next_1 = holes.at(holes.size()-1).at(i+1);
            holes.at(holes.size()-1).at(holes.at(holes.size()-1).size()-1)->next_1 = holes.at(holes.size()-1).at(0);
        }
    }
    for(int j=0;j<num_of_newpoly;j++)
    {
        if((holes.size()==0)||(num_of_newhole==0))
            break;
        int x_temp = a.newPolygon.at(a.newPolygon.size()-j-1)->newNode.at(0)->x;
        int y_temp = a.newPolygon.at(a.newPolygon.size()-j-1)->newNode.at(0)->y;
        int change_x = 0;
        int change_y = 0;
        if(inorout_merge(a.newPolygon.at(a.newPolygon.size()-j-1)->newNode, x_temp-1, y_temp-1,-1,-1))
        {
            change_x = -1;
            change_y = -1;
        }
        else if(inorout_merge(a.newPolygon.at(a.newPolygon.size()-j-1)->newNode, x_temp+1, y_temp+1,1,1))
        {
            change_x = 1;
            change_y = 1;
        }
        else if(inorout_merge(a.newPolygon.at(a.newPolygon.size()-j-1)->newNode, x_temp-1, y_temp+1,-1,1))
        {
            change_x = -1;
            change_y = 1;
        }
        else if(inorout_merge(a.newPolygon.at(a.newPolygon.size()-j-1)->newNode, x_temp+1, y_temp-1,1,-1))
        {
            change_x = 1;
            change_y = -1;
        }
        if(inorout_merge(holes.at(holes.size()-1), x_temp+change_x, y_temp+change_y,change_x,change_y))
        {
            check_if_polygon_delete = false;
        }
    }

    if(num_of_newhole>0)
	check_if_polygon_delete = false;
    else
        check_if_polygon_delete = true;
    if(!check_if_polygon_delete)//no new polygon, only consider the holes
    {
        a.newPolygon.at(num_of_newPolygon)->newHole.clear();
        for(unsigned num_hole=0;num_hole<holes.size()-1;num_hole++)
        {
            int x_temp = holes.at(num_hole).at(0)->x;
            int y_temp = holes.at(num_hole).at(0)->y;
            int change_x = 0;
            int change_y = 0;
            if(inorout_merge(holes.at(num_hole), x_temp-1, y_temp-1,-1,-1))
            {
                change_x = -1;
                change_y = -1;
            }
            else if(inorout_merge(holes.at(num_hole), x_temp+1, y_temp+1,1,1))
            {
                change_x = 1;
                change_y = 1;
            }
            else if(inorout_merge(holes.at(num_hole), x_temp-1, y_temp+1,-1,1))
            {
                change_x = -1;
                change_y = 1;
            }
            else if(inorout_merge(holes.at(num_hole), x_temp+1, y_temp-1,1,-1))
            {
                change_x = 1;
                change_y = -1;
            }
            if((!inorout_merge(holes.at(holes.size()-1), x_temp+change_x, y_temp+change_y,change_x,change_y))&&(inorout_merge(a.newPolygon.at(num_of_newPolygon)->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y)))  //not in the new hole and still in the new polygon
            {
                a.newPolygon.at(num_of_newPolygon)->newHole.push_back(holes.at(num_hole));
            }
        }
        a.newPolygon.at(num_of_newPolygon)->newHole.push_back(holes.at(holes.size()-1));
    }
    else  //new polygon occurs, check holes whether they are still in polygon
    {
        for(unsigned num_hole=0;num_hole<holes.size();num_hole++)
        {
            int x_temp = holes.at(num_hole).at(0)->x;
            int y_temp = holes.at(num_hole).at(0)->y;
            int change_x = 0;
            int change_y = 0;
            bool isdelete = true;
            if(inorout_merge(holes.at(num_hole), x_temp-1, y_temp-1,-1,-1))
            {
                change_x = -1;
                change_y = -1;
            }
            else if(inorout_merge(holes.at(num_hole), x_temp+1, y_temp+1,1,1))
            {
                change_x = 1;
                change_y = 1;
            }
            else if(inorout_merge(holes.at(num_hole), x_temp-1, y_temp+1,-1,1))
            {
                change_x = -1;
                change_y = 1;
            }
            else if(inorout_merge(holes.at(num_hole), x_temp+1, y_temp-1,1,-1))
            {
                change_x = 1;
                change_y = -1;
            }

            const int const_numpoly = num_of_newpoly;
            int count_poly = 0;
            int poly = a.newPolygon.size()-1;
            while(count_poly < const_numpoly)
            {
                count_poly++;
                if(inorout_merge(a.newPolygon.at(poly)->newNode, x_temp+change_x, y_temp+change_y,change_x,change_y))  //still in the new polygon
                {
                    isdelete = false;
                    a.newPolygon.at(poly)->newHole.push_back(holes.at(num_hole));
                    break;
                }
                poly--;
            }
        }
    }

    if(check_if_polygon_delete)
    {
        sub_intersect_change = true;
        if(num_of_newhole==0)
            must = true;

    }
    vector < vector <Node*> > ().swap(holes);
}



int Structure::split_H(Structure &a, ostream &output)
{
    int count = 0;
    for(int i=0;i<a.newPolygon.size();i++)
    {
        if(a.newPolygon.at(i)->newHole.size()==0)
	{
	    if(a.newPolygon.at(i)->newNode.size()==4)
	    {
	        output<<"RECT"<<" "<<a.newPolygon.at(i)->boxl<<" "<<a.newPolygon.at(i)->boxb<<" "<<a.newPolygon.at(i)->boxr<<" "<<a.newPolygon.at(i)->boxt<<" "<<";"<<endl;
		count++;
	    }
	    else
	    {
	    	vector<int> answer_temp;
		vector < vector <int> > modifyanswer;
		vector<int> usefully;
		for(int s=0;s<a.newPolygon.at(i)->newNode.size();s++)
		{
		    answer_temp.push_back(a.newPolygon.at(i)->newNode.at(s)->y);
		}
		sort(answer_temp.begin(),answer_temp.end());
		vector<int> answer;
		for(int s=0;s<answer_temp.size()-1;s++)
		{
		     if(answer_temp.at(s)!=answer_temp.at(s+1))
		     {
		         answer.push_back(answer_temp.at(s));
		     }
		}
		answer.push_back(answer_temp.at(answer_temp.size()-1));

		for(int s=1;s<answer.size();s++)
		{
		    vector<int> finalanswer;
		    int previous_y = answer.at(s-1);
		    int temp_y = answer.at(s);
		    if(s == answer.size()-1)
		    {
		        for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
			{
			    if(a.newPolygon.at(i)->newNode.at(k)->y == a.newPolygon.at(i)->boxt)
			    {
			        finalanswer.push_back(a.newPolygon.at(i)->newNode.at(k)->x);
			    }
			}
			sort(finalanswer.begin(),finalanswer.end());
			for(int q=0;q<finalanswer.size();q+=2)
			{
			    modifyanswer.push_back(usefully);
			    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
			    modifyanswer.at(modifyanswer.size()-1).push_back(previous_y);
			    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
			    modifyanswer.at(modifyanswer.size()-1).push_back(temp_y);
			}
		        break;
		    }


		    for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
		    {
		        Node* temps = a.newPolygon.at(i)->newNode.at(k);
		        if(temps->x == temps->next_1->x)
			{
			    if((temps->y < temp_y) && (temps->next_1->y > temp_y))
			    {
			        finalanswer.push_back(temps->x);
			    }
			    else if((temps->y > temp_y) && (temps->next_1->y < temp_y))
			    {
			        finalanswer.push_back(temps->x);
			    }
			}
			else
			{
			    if((temps->y == temp_y)&&(temps->x > temps->next_1->x))
			    {
			    	if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			    	{
				    if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
					finalanswer.push_back(temps->next_1->x);
				    }
				    else
				    {
				    	finalanswer.push_back(temps->x);
				    }
			    	}
			    	else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			    	{
			            if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
				    }
			            else
			            {
			       	        finalanswer.push_back(temps->next_1->x);
				    }

			        }
			        else if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			        {
				    if(!inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
			                finalanswer.push_back(temps->next_1->x);
				    }
			        }
			        else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			        {
				    if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
			                finalanswer.push_back(temps->next_1->x);
				    }
			        }
			    }
			    else if((temps->y == temp_y)&&(temps->x < temps->next_1->x))
			    {
			    	if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			    	{
				    if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->next_1->x);
				    }
				    else
				    {
				        finalanswer.push_back(temps->x);
				    }

			        }
			        else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			        {
				    if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
				    }
			            else
			            {
			       	        finalanswer.push_back(temps->next_1->x);
				    }

			        }
			    	else if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			     	{
				    if(!inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
			            {
				        finalanswer.push_back(temps->x);
			                finalanswer.push_back(temps->next_1->x);
				    }
			        }
			        else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			        {
				    if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
			            	finalanswer.push_back(temps->next_1->x);
				    }
			    	}
			    }
			}
		    }
		    sort(finalanswer.begin(),finalanswer.end());
		    for(int q=0;q<finalanswer.size();q+=2)
		    {
			modifyanswer.push_back(usefully);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
			modifyanswer.at(modifyanswer.size()-1).push_back(previous_y);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
			modifyanswer.at(modifyanswer.size()-1).push_back(temp_y);
	            }

		}

		for(int f=0;f<modifyanswer.size();f++)
		{
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    for(int r=f+1;r<modifyanswer.size();r++)
		    {
	            if((modifyanswer.at(f).at(0)==modifyanswer.at(r).at(0))&&(modifyanswer.at(f).at(2)==modifyanswer.at(r).at(2)))
		        {
		             if((modifyanswer.at(f).at(3) == modifyanswer.at(r).at(1)))
			     {
			         modifyanswer.at(f).at(3) = modifyanswer.at(r).at(3);
				 modifyanswer.at(r).at(0) = -1;
			     }
		        }
		    }
		}
		for(int f=0;f<modifyanswer.size();f++)
                {
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    else
		    {
		        output<<"RECT"<<" "<<modifyanswer.at(f).at(0)<<" "<<modifyanswer.at(f).at(1)<<" "<<modifyanswer.at(f).at(2)<<" "<<modifyanswer.at(f).at(3)<<" "<<";"<<endl;
			count++;
		    }
		}
	    }
	}
	else
	{
	    vector<int> answer_temp;
	    vector < vector <int> > modifyanswer;
	    vector<int> usefully;
	    for(int s=0;s<a.newPolygon.at(i)->newNode.size();s++)
	    {
		answer_temp.push_back(a.newPolygon.at(i)->newNode.at(s)->y);
	    }
	    for(int s=0;s<a.newPolygon.at(i)->newHole.size();s++)
	    {
	    	for(int k=0;k<a.newPolygon.at(i)->newHole.at(s).size();k++)
		{
	             answer_temp.push_back(a.newPolygon.at(i)->newHole.at(s).at(k)->y);
		}
	    }
	    sort(answer_temp.begin(),answer_temp.end());
	    vector<int> answer;
	    for(int s=0;s<answer_temp.size()-1;s++)
	    {
		 if(answer_temp.at(s)!=answer_temp.at(s+1))
		 {
		     answer.push_back(answer_temp.at(s));
		 }
	    }
	    answer.push_back(answer_temp.at(answer_temp.size()-1));

	    for(int s=1;s<answer.size();s++)
	    {
		vector<int> finalanswer;
		int previous_y = answer.at(s-1);
		int temp_y = answer.at(s);
		if(s == answer.size()-1)
		{
		    for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
   		    {
			if(a.newPolygon.at(i)->newNode.at(k)->y == a.newPolygon.at(i)->boxt)
			{
			    finalanswer.push_back(a.newPolygon.at(i)->newNode.at(k)->x);
			}
		    }
		    sort(finalanswer.begin(),finalanswer.end());
	            for(int q=0;q<finalanswer.size();q+=2)
		    {
			modifyanswer.push_back(usefully);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
			modifyanswer.at(modifyanswer.size()-1).push_back(previous_y);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
			modifyanswer.at(modifyanswer.size()-1).push_back(temp_y);
		    }
		    break;
		}


		for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
		{
		    Node* temps = a.newPolygon.at(i)->newNode.at(k);
		    if(temps->x == temps->next_1->x)
		    {
			if((temps->y < temp_y) && (temps->next_1->y > temp_y))
		        {
	 		    finalanswer.push_back(temps->x);
			}
			else if((temps->y > temp_y) && (temps->next_1->y < temp_y))
			{
			    finalanswer.push_back(temps->x);
			}
		    }
		    else
		    {
			if((temps->y == temp_y)&&(temps->x > temps->next_1->x))
			{
			    if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			    {
				if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->next_1->x);
				}
				else
				{
				    finalanswer.push_back(temps->x);
				}

			    }
			    else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			    {
				if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->x);
				}
			        else
			        {
			       	    finalanswer.push_back(temps->next_1->x);
				}

			    }
			    else if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			    {
				if(!inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->x);
			            finalanswer.push_back(temps->next_1->x);
				}
			    }
			    else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x + 1, temps->y, 1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x - 1, temps->y, -1)))
			    {
				if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->x);
			            finalanswer.push_back(temps->next_1->x);
				}
			    }
			}
			else if((temps->y == temp_y)&&(temps->x < temps->next_1->x))
			{
			    if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			    {
				if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->next_1->x);
				}
				else
				{
				    finalanswer.push_back(temps->x);
				}

			    }
			    else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			    {
				if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->x);
				}
			        else
			        {
			       	    finalanswer.push_back(temps->next_1->x);
				}

			    }
			    else if((inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			    {
				if(!inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->x);
			            finalanswer.push_back(temps->next_1->x);
				}
			    }
			    else if((!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, temps->y, -1))&&(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->next_1->x + 1, temps->y, 1)))
			    {
				if(inorout_left(a.newPolygon.at(i)->newNode, (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				{
				    finalanswer.push_back(temps->x);
			            finalanswer.push_back(temps->next_1->x);
				}
			    }
			}

		    }
		}

		for(int k=0;k<a.newPolygon.at(i)->newHole.size();k++)
		{
		    for(int h=0;h<a.newPolygon.at(i)->newHole.at(k).size();h++)
		    {
		        Node* temps = a.newPolygon.at(i)->newHole.at(k).at(h);
		        if(temps->x == temps->next_1->x)
			{
			    if((temps->y < temp_y) && (temps->next_1->y > temp_y))
			    {
			        finalanswer.push_back(temps->x);
			    }
			    else if((temps->y > temp_y) && (temps->next_1->y < temp_y))
			    {
			        finalanswer.push_back(temps->x);
			    }
			}
			else
			{
			    if((temps->y == temp_y)&&(temps->x > temps->next_1->x))
			    {
			    	if((!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x + 1, temps->y, 1))&&(!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x - 1, temps->y, -1)))
				{
				    if(inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
					finalanswer.push_back(temps->next_1->x);
				    }
				}
				else if((inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x + 1, temps->y, 1))&&(!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x - 1, temps->y, -1)))
				{
				    if(inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
					finalanswer.push_back(temps->next_1->x);
				    }
				    else
				    {
					finalanswer.push_back(temps->x);
				    }

				}
				else if((!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x + 1, temps->y, 1))&&(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x - 1, temps->y, -1)))
				{
				    if(inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
					finalanswer.push_back(temps->x);
				    }
				    else
				    {
					finalanswer.push_back(temps->next_1->x);
				    }

				}
				else if((inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x + 1, temps->y, 1))&&(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x - 1, temps->y, -1)))
				{
				    if(!inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
					finalanswer.push_back(temps->next_1->x);
				    }
				}
			    }
			    else if((temps->y == temp_y)&&(temps->x < temps->next_1->x))
			    {
			    	if((!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, temps->y, -1))&&(!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x + 1, temps->y, 1)))
				{
				    if(inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
					finalanswer.push_back(temps->next_1->x);
				    }
				}
				else if((inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, temps->y, -1))&&(!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x + 1, temps->y, 1)))
				{
				    if(inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
					finalanswer.push_back(temps->next_1->x);
				    }
				    else
				    {
					finalanswer.push_back(temps->x);
				    }

				}
				else if((!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, temps->y, -1))&&(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x + 1, temps->y, 1)))
				{
				    if(inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
					finalanswer.push_back(temps->x);
				    }
				    else
				    {
					finalanswer.push_back(temps->next_1->x);
				    }

				}
				else if((inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, temps->y, -1))&&(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->next_1->x + 1, temps->y, 1)))
				{
				    if(!inorout_left(a.newPolygon.at(i)->newHole.at(k), (temps->x + temps->next_1->x)/2, temps->y - 1, -1))
				    {
				        finalanswer.push_back(temps->x);
					finalanswer.push_back(temps->next_1->x);
				    }
				}
			    }
			}
		    }
		}

		sort(finalanswer.begin(),finalanswer.end());
	        for(int q=0;q<finalanswer.size();q+=2)
		{
		    modifyanswer.push_back(usefully);
		    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
		    modifyanswer.at(modifyanswer.size()-1).push_back(previous_y);
		    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
		    modifyanswer.at(modifyanswer.size()-1).push_back(temp_y);
	        }

	    }
	        for(int f=0;f<modifyanswer.size();f++)
		{
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    for(int r=f+1;r<modifyanswer.size();r++)
		    {
	            if((modifyanswer.at(f).at(0)==modifyanswer.at(r).at(0))&&(modifyanswer.at(f).at(2)==modifyanswer.at(r).at(2)))
		        {
		             if((modifyanswer.at(f).at(3) == modifyanswer.at(r).at(1)))
			     {
			         modifyanswer.at(f).at(3) = modifyanswer.at(r).at(3);
				 modifyanswer.at(r).at(0) = -1;
			     }
		        }
		    }
		}
		for(int f=0;f<modifyanswer.size();f++)
                {
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    else
		    {
		        output<<"RECT"<<" "<<modifyanswer.at(f).at(0)<<" "<<modifyanswer.at(f).at(1)<<" "<<modifyanswer.at(f).at(2)<<" "<<modifyanswer.at(f).at(3)<<" "<<";"<<endl;
			count++;
		    }
		}

	}
    }
    cout<<"total polygon:"<<count<<endl;
    return count;
}




int Structure::split_V(Structure &a, ostream &output)
{
    int count = 0;
    for(int i=0;i<a.newPolygon.size();i++)
    {
        if(a.newPolygon.at(i)->newHole.size()==0)
	{
	    if(a.newPolygon.at(i)->newNode.size()==4)
	    {
	        output<<"RECT"<<" "<<a.newPolygon.at(i)->boxl<<" "<<a.newPolygon.at(i)->boxb<<" "<<a.newPolygon.at(i)->boxr<<" "<<a.newPolygon.at(i)->boxt<<" "<<";"<<endl;
		count++;
	    }
	    else
	    {
	    	vector<int> answer_temp;
		vector < vector <int> > modifyanswer;
		vector<int> usefully;
		for(int s=0;s<a.newPolygon.at(i)->newNode.size();s++)
		{
		    answer_temp.push_back(a.newPolygon.at(i)->newNode.at(s)->x);
		}
		sort(answer_temp.begin(),answer_temp.end());
		vector<int> answer;
		for(int s=0;s<answer_temp.size()-1;s++)
		{
		     if(answer_temp.at(s)!=answer_temp.at(s+1))
		     {
		         answer.push_back(answer_temp.at(s));
		     }
		}
		answer.push_back(answer_temp.at(answer_temp.size()-1));

		for(int s=1;s<answer.size();s++)
		{
		    vector<int> finalanswer;
		    int previous_x = answer.at(s-1);
		    int temp_x = answer.at(s);
		    if(s == answer.size()-1)
		    {
		        for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
			{
			    if(a.newPolygon.at(i)->newNode.at(k)->x == a.newPolygon.at(i)->boxr)
			    {
			        finalanswer.push_back(a.newPolygon.at(i)->newNode.at(k)->y);
			    }
			}
			sort(finalanswer.begin(),finalanswer.end());
			for(int q=0;q<finalanswer.size();q+=2)
			{
			    if(q+1 == finalanswer.size())
			        break;
			    modifyanswer.push_back(usefully);
			    modifyanswer.at(modifyanswer.size()-1).push_back(previous_x);
			    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
			    modifyanswer.at(modifyanswer.size()-1).push_back(temp_x);
			    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));

			}
		        break;
		    }


		    for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
		    {
		        Node* temps = a.newPolygon.at(i)->newNode.at(k);
		        if(temps->y == temps->next_1->y)
			{
			    if((temps->x < temp_x) && (temps->next_1->x > temp_x))
			    {
			        finalanswer.push_back(temps->y);
			    }
			    else if((temps->x > temp_x) && (temps->next_1->x < temp_x))
			    {
			        finalanswer.push_back(temps->y);
			    }
			}
			else
			{
			    if((temps->x == temp_x)&&(temps->y > temps->next_1->y))
			    {
			    	if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			    	{
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->next_1->y);
				    }
				    else
				    {
				    	finalanswer.push_back(temps->y);
				    }
			    	}
			    	else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			    	{
			            if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
				    }
			            else
			            {
			       	        finalanswer.push_back(temps->next_1->y);
				    }

			        }
			        else if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			        {
				    if(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			        else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			        {
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			    }
			    else if((temps->x == temp_x)&&(temps->y < temps->next_1->y))
			    {
				if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			    	{
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->next_1->y);
				    }
				    else
				    {
				    	finalanswer.push_back(temps->y);
				    }
			    	}
			    	else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			    	{
			            if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
				    }
			            else
			            {
			       	        finalanswer.push_back(temps->next_1->y);
				    }

			        }
			        else if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			        {
				    if(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			        else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			        {
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			    }
			}
		    }
		    sort(finalanswer.begin(),finalanswer.end());
		    for(int q=0;q<finalanswer.size();q+=2)
		    {
		        if(q+1 == finalanswer.size())
			    break;
			modifyanswer.push_back(usefully);
			modifyanswer.at(modifyanswer.size()-1).push_back(previous_x);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
			modifyanswer.at(modifyanswer.size()-1).push_back(temp_x);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
	            }

		}

		for(int f=0;f<modifyanswer.size();f++)
		{
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    for(int r=f+1;r<modifyanswer.size();r++)
		    {
	            if((modifyanswer.at(f).at(1)==modifyanswer.at(r).at(1))&&(modifyanswer.at(f).at(3)==modifyanswer.at(r).at(3)))
		        {
		             if((modifyanswer.at(f).at(2) == modifyanswer.at(r).at(0)))
			     {
			         modifyanswer.at(f).at(2) = modifyanswer.at(r).at(2);
				 modifyanswer.at(r).at(0) = -1;
			     }
		        }
		    }
		}
		for(int f=0;f<modifyanswer.size();f++)
                {
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    else
		    {
		        output<<"RECT"<<" "<<modifyanswer.at(f).at(0)<<" "<<modifyanswer.at(f).at(1)<<" "<<modifyanswer.at(f).at(2)<<" "<<modifyanswer.at(f).at(3)<<" "<<";"<<endl;
			count++;
		    }
		}
	    }
	}
	else
	{
	    vector<int> answer_temp;
	    vector < vector <int> > modifyanswer;
	    vector<int> usefully;
	    for(int s=0;s<a.newPolygon.at(i)->newNode.size();s++)
	    {
		answer_temp.push_back(a.newPolygon.at(i)->newNode.at(s)->x);
	    }
	    for(int s=0;s<a.newPolygon.at(i)->newHole.size();s++)
	    {
	    	for(int k=0;k<a.newPolygon.at(i)->newHole.at(s).size();k++)
		{
	             answer_temp.push_back(a.newPolygon.at(i)->newHole.at(s).at(k)->x);
		}
	    }
	    sort(answer_temp.begin(),answer_temp.end());
	    vector<int> answer;
	    for(int s=0;s<answer_temp.size()-1;s++)
	    {
		 if(answer_temp.at(s)!=answer_temp.at(s+1))
		 {
		     answer.push_back(answer_temp.at(s));
		 }
	    }
	    answer.push_back(answer_temp.at(answer_temp.size()-1));

	    for(int s=1;s<answer.size();s++)
	    {
		vector<int> finalanswer;
		int previous_x = answer.at(s-1);
		int temp_x = answer.at(s);
		if(s == answer.size()-1)
		{
		    for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
   		    {
			if(a.newPolygon.at(i)->newNode.at(k)->x == a.newPolygon.at(i)->boxr)
			{
			    finalanswer.push_back(a.newPolygon.at(i)->newNode.at(k)->y);
			}
		    }
		    sort(finalanswer.begin(),finalanswer.end());
	            for(int q=0;q<finalanswer.size();q+=2)
		    {
			if(q+1 == finalanswer.size())
			    break;
			modifyanswer.push_back(usefully);
			modifyanswer.at(modifyanswer.size()-1).push_back(previous_x);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
			modifyanswer.at(modifyanswer.size()-1).push_back(temp_x);
			modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
		    }
		    break;
		}


		for(int k=0;k<a.newPolygon.at(i)->newNode.size();k++)
		{
		    Node* temps = a.newPolygon.at(i)->newNode.at(k);
		    if(temps->y == temps->next_1->y)
		    {
			if((temps->x < temp_x) && (temps->next_1->x > temp_x))
		        {
	 		    finalanswer.push_back(temps->y);
			}
			else if((temps->x > temp_x) && (temps->next_1->x < temp_x))
			{
			    finalanswer.push_back(temps->y);
			}
		    }
		    else
		    {
			    if((temps->x == temp_x)&&(temps->y > temps->next_1->y))
			    {
			    	if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			    	{
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->next_1->y);
				    }
				    else
				    {
				    	finalanswer.push_back(temps->y);
				    }
			    	}
			    	else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			    	{
			            if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
				    }
			            else
			            {
			       	        finalanswer.push_back(temps->next_1->y);
				    }

			        }
			        else if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			        {
				    if(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			        else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y + 1, 1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y - 1, -1)))
			        {
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			    }
			    else if((temps->x == temp_x)&&(temps->y < temps->next_1->y))
			    {
				if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			    	{
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->next_1->y);
				    }
				    else
				    {
				    	finalanswer.push_back(temps->y);
				    }
			    	}
			    	else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			    	{
			            if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
				    }
			            else
			            {
			       	        finalanswer.push_back(temps->next_1->y);
				    }

			        }
			        else if((inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			        {
				    if(!inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			        else if((!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->y - 1, -1))&&(!inorout_left(a.newPolygon.at(i)->newNode, temps->x, temps->next_1->y + 1, 1)))
			        {
				    if(inorout_bottom(a.newPolygon.at(i)->newNode, temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
			                finalanswer.push_back(temps->next_1->y);
				    }
			        }
			    }

		    }
		}

		for(int k=0;k<a.newPolygon.at(i)->newHole.size();k++)
		{
		    for(int h=0;h<a.newPolygon.at(i)->newHole.at(k).size();h++)
		    {
		        Node* temps = a.newPolygon.at(i)->newHole.at(k).at(h);
		        if(temps->y == temps->next_1->y)
			{
			    if((temps->x < temp_x) && (temps->next_1->x > temp_x))
			    {
			        finalanswer.push_back(temps->y);
			    }
			    else if((temps->x > temp_x) && (temps->next_1->x < temp_x))
			    {
			        finalanswer.push_back(temps->y);
			    }
			}
			else
			{
			    if((temps->x == temp_x)&&(temps->y > temps->next_1->y))
			    {
			    	if((!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y + 1, 1))&&(!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y - 1, -1)))
				{
				    if(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
					finalanswer.push_back(temps->next_1->y);
				    }
				}
				else if((inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y + 1, 1))&&(!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y - 1, -1)))
				{
				    if(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->next_1->y);
				    }
				    else
				    {
					finalanswer.push_back(temps->y);
				    }

				}
				else if((!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y + 1, 1))&&(inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y - 1, -1)))
				{
				    if(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->y);
				    }
				    else
				    {
					finalanswer.push_back(temps->next_1->y);
				    }

				}
				else if((inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y + 1, 1))&&(inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y - 1, -1)))
				{
				    if(!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
					finalanswer.push_back(temps->next_1->y);
				    }
				}
			    }
			    else if((temps->x == temp_x)&&(temps->y < temps->next_1->y))
			    {
			    	if((!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y - 1, -1))&&(!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y + 1, 1)))
				{
				    if(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
					finalanswer.push_back(temps->next_1->y);
				    }
				}
				else if((inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y - 1, -1))&&(!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y + 1, 1)))
				{
				    if(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->next_1->y);
				    }
				    else
				    {
					finalanswer.push_back(temps->y);
				    }

				}
				else if((!inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y - 1, -1))&&(inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y + 1, 1)))
				{
				    if(inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
					finalanswer.push_back(temps->y);
				    }
				    else
				    {
					finalanswer.push_back(temps->next_1->y);
				    }

				}
				else if((inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->y - 1, -1))&&(inorout_left(a.newPolygon.at(i)->newHole.at(k), temps->x, temps->next_1->y + 1, 1)))
				{
				    if(!inorout_bottom(a.newPolygon.at(i)->newHole.at(k), temps->x - 1, (temps->y + temps->next_1->y)/2, -1))
				    {
				        finalanswer.push_back(temps->y);
					finalanswer.push_back(temps->next_1->y);
				    }
				}
			    }
			}
		    }
		}

		sort(finalanswer.begin(),finalanswer.end());
	        for(int q=0;q<finalanswer.size();q+=2)
		{
		    if(q+1 == finalanswer.size())
			 break;
		    modifyanswer.push_back(usefully);
		    modifyanswer.at(modifyanswer.size()-1).push_back(previous_x);
		    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q));
		    modifyanswer.at(modifyanswer.size()-1).push_back(temp_x);
		    modifyanswer.at(modifyanswer.size()-1).push_back(finalanswer.at(q+1));
	        }

	    }
	        for(int f=0;f<modifyanswer.size();f++)
		{
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    for(int r=f+1;r<modifyanswer.size();r++)
		    {
	            if((modifyanswer.at(f).at(1)==modifyanswer.at(r).at(1))&&(modifyanswer.at(f).at(3)==modifyanswer.at(r).at(3)))
		        {
		             if((modifyanswer.at(f).at(2) == modifyanswer.at(r).at(0)))
			     {
			         modifyanswer.at(f).at(2) = modifyanswer.at(r).at(2);
				 modifyanswer.at(r).at(0) = -1;
			     }
		        }
		    }
		}
		for(int f=0;f<modifyanswer.size();f++)
                {
		    if(modifyanswer.at(f).at(0)==-1)
		        continue;
		    else
		    {
		        output<<"RECT"<<" "<<modifyanswer.at(f).at(0)<<" "<<modifyanswer.at(f).at(1)<<" "<<modifyanswer.at(f).at(2)<<" "<<modifyanswer.at(f).at(3)<<" "<<";"<<endl;
			count++;
		    }
		}

	}
    }
    cout<<"total polygon:"<<count<<endl;
    return count;
}
