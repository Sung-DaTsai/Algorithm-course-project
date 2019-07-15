#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Reclist.h"
using namespace std;

Reclist::Reclist(const vector<int>& coordinate, bool sorc)
{
    if(sorc)
        Initialize_node_subject(coordinate);
    else
        Initialize_node_clip(coordinate);

}


Reclist::~Reclist(){
    
}


//This function is used only for initialize subject polygon(counterclockwise)
void Reclist::Initialize_node_subject(const vector<int>& coordinate)
{
    if(coordinate.size()%4 == 0)
        return;
    int left_min = coordinate[0]+1;
    bool iscounter = false;
    boxl = coordinate[0];
    boxr = coordinate[0];
    boxb = coordinate[1];
    boxt = coordinate[1];
    for(unsigned i=0; i<coordinate.size(); i+=2)
    {
        newNode.push_back(new Node(coordinate[i], coordinate[i+1], false));
        if(i < coordinate.size()-2)
        {
            if((coordinate[i]==coordinate[i+2])&&(left_min >= coordinate[i]))
            {
                if(coordinate[i+1]>coordinate[i+3])
                {
                    left_min = coordinate[i];
                    iscounter = true;
                }
                else if(coordinate[i+1]<coordinate[i+3])
                {
                    left_min = coordinate[i];
                    iscounter = false;
                }
            }

        }
        if(boxl>coordinate[i])
            boxl = coordinate[i];
        if(boxr<coordinate[i])
            boxr = coordinate[i];
        if(boxb>coordinate[i+1])
            boxb = coordinate[i+1];
        if(boxt<coordinate[i+1])
            boxt = coordinate[i+1];
    }

    // if y is first big then small -> counterclockwise
    if(!iscounter) //check if it's counterclockwise; if it's clockwise then reverse
        reverse(newNode.begin(), newNode.end());

    delete newNode.at(newNode.size()-1);
    newNode.pop_back();
    
 

    //link nodes: next_1
    //head = newNode[0];

    for(unsigned i=0; i<newNode.size()-1; i++)
    {
        newNode[i]->next_1 = newNode[i+1];
    }
    newNode[newNode.size()-1]->next_1 = newNode[0];
    
    
    /*cout<<"newNode size"<<newNode.size()<<endl;
    
    for(unsigned i=0;i<newNode.size();i++)
    {
        cout<<"newNode.at(i)->x"<<newNode.at(i)->x<<"newNode.at(i)->y"<<newNode.at(i)->y<<endl;
    }*/
    
    vector<int> polygon_into_hole;
    if(newNode.size()>8)
    {
        for(unsigned i=0;i<newNode.size();i+=2)
        {
            for(unsigned j=i+4;j<newNode.size();j+=2)
            {
                if((newNode.at(i)->x == newNode.at(j)->x)&&(newNode.at(i)->y == newNode.at(j)->y))
                {
                    polygon_into_hole.push_back(i);
                    polygon_into_hole.push_back(j);
                }
                else if((newNode.at(i+1)->x == newNode.at(j+1)->x)&&(newNode.at(i+1)->y == newNode.at(j+1)->y))
                {
                    polygon_into_hole.push_back(i+1);
                    polygon_into_hole.push_back(j+1);
                }
            }
        }
    }
    
    //cout<<"wronging"<<endl;
    if(polygon_into_hole.size()!=0)
    {
        Node* temp;
        vector<Node*> temporary;
        //cout<<"success"<<endl;
        if(polygon_into_hole.size()>=2)
        {
            vector<int> hole_pointsize;
            vector<int> sorted_list;
            vector<int> delete_list;
            //cout<<"success"<<endl;
            unsigned subtract;
            /*for(size_t j=0;j<newNode.size();j++)
            {
                cout<<"polygon point:  "<<newNode.at(j)->x<<"  ";
                cout<<newNode.at(j)->y<<endl;
            }*/
            //cout<<endl;
            
            for(unsigned i=0;i<polygon_into_hole.size();i+=2)
            {
                left_min = (newNode.at(polygon_into_hole.at(i))->x) + 1;
                bool iscounter_in = true;
                bool iscounter_out = true;
                for(unsigned j=polygon_into_hole.at(i);j<polygon_into_hole.at(i+1);j++)
                {
                    if((newNode.at(j)->x == newNode.at(j+1)->x)&&(left_min >= newNode.at(j)->x))
                    {
                        if(newNode.at(j)->y > newNode.at(j+1)->y)
                        {
                            left_min = newNode.at(j)->x;
                            iscounter_in = true;
                        }
                        else if(newNode.at(j)->y < newNode.at(j+1)->y)
                        {
                            left_min = newNode.at(j)->x;
                            iscounter_in = false;
                        }
                    }
                }
                if(iscounter_in)
                {
                    Node* temper = newNode.at(polygon_into_hole.at(i+1));
                    left_min = (newNode.at(polygon_into_hole.at(i+1))->x) + 1;
                    if((temper->x == temper->next_1->x)&&(left_min >= temper->x))
                    {
                        if(temper->y > temper->next_1->y)
                        {
                            left_min = temper->x;
                            iscounter_out = true;
                        }
                        else if(temper->y < temper->next_1->y)
                        {
                            left_min = temper->x;
                            iscounter_out = false;
                        }
                    }
                    temper = temper->next_1;
                    while((temper->x != newNode.at(polygon_into_hole.at(i))->x)||(temper->y != newNode.at(polygon_into_hole.at(i))->y))
                    {
                        if((temper->x == temper->next_1->x)&&(left_min >= temper->x))
                        {
                            if(temper->y > temper->next_1->y)
                            {
                                left_min = temper->x;
                                iscounter_out = true;
                            }
                            else if(temper->y < temper->next_1->y)
                            {
                                left_min = temper->x;
                                iscounter_out = false;
                            }
                        }
                        temper = temper->next_1;
                    }
                }
                if(!iscounter_in)
                {
                    subtract = polygon_into_hole.at(i+1) - polygon_into_hole.at(i);
                    hole_pointsize.push_back(subtract);
                }
                else if(!iscounter_out)
                {
                    swap(polygon_into_hole.at(i), polygon_into_hole.at(i+1));
                    subtract = newNode.size() - polygon_into_hole.at(i) + polygon_into_hole.at(i+1);
                    hole_pointsize.push_back(subtract);
                }
                else
                {
                    hole_pointsize.push_back(-1);
                }
                /*
                else if(polygon_into_hole.size()==2)
                {
                    if((polygon_into_hole.at(1)-polygon_into_hole.at(0)) > (newNode.size()/2))
                    {

                        temp = newNode.at(polygon_into_hole.at(0))->next_1;
                        newNode.at(polygon_into_hole.at(0))->next_1 = newNode.at(polygon_into_hole.at(1))->next_1;
                        newNode.at(polygon_into_hole.at(1))->next_1 = temp;                        
                        notice_1 = polygon_into_hole.at(0);
                        notice_2 = polygon_into_hole.at(1);
                    }
                    else
                    {
                        temp = newNode.at(polygon_into_hole.at(0))->next_1;
                        newNode.at(polygon_into_hole.at(0))->next_1 = newNode.at(polygon_into_hole.at(1))->next_1;
                        newNode.at(polygon_into_hole.at(1))->next_1 = temp;
                        notice_1 = polygon_into_hole.at(0);
                        notice_2 = polygon_into_hole.at(1);
                    }
                    notice = true;
                    return;
                }
                */
            }
            sorted_list = hole_pointsize;
            sort(sorted_list.begin(), sorted_list.end());
            for(int i=0;i<sorted_list.size();i++)
            {
                if(sorted_list.at(i)<=0)
                    continue;
                for(int j=0;j<hole_pointsize.size();j++)
                {
                    if(sorted_list.at(i) == hole_pointsize.at(j))
                    {
                        //cout<<"i,j:"<<i<<j<<endl;
                        delete_list.push_back(polygon_into_hole.at(j*2));
                        delete_list.push_back(polygon_into_hole.at(j*2+1));
                        hole_pointsize.at(j) = -1;
                        break;
                    }
                }
            }

            vector<int> delete_newNode;
            for(int k=0;k<delete_list.size();k+=2)
            {
                int initial_x = newNode.at(delete_list.at(k))->x;
                int initial_y = newNode.at(delete_list.at(k))->y;
                Node* initial = newNode.at(delete_list.at(k))->next_1;
                temp = newNode.at(delete_list.at(k))->next_1;
                newNode.at(delete_list.at(k))->next_1 = newNode.at(delete_list.at(k+1))->next_1;
                newNode.at(delete_list.at(k+1))->next_1 = temp;
                newHole.push_back(temporary);
                if(delete_list.at(k)>delete_list.at(k+1))
                {
                    for(unsigned i=delete_list.at(k)+1;i<newNode.size();i++)
                    {
                        //newHole.at(newHole.size()-1).push_back(newNode.at(i));
                        delete_newNode.push_back(i);
                    }
                    for(unsigned i=0;i<=delete_list.at(k+1);i++)
                    {
                        //newHole.at(newHole.size()-1).push_back(newNode.at(i));
                        delete_newNode.push_back(i);
                    }
                }
                else
                {
                    for(unsigned i=delete_list.at(k)+1;i<=delete_list.at(k+1);i++)
                    {
                        //newHole.at(newHole.size()-1).push_back(newNode.at(i));
                        delete_newNode.push_back(i);
                    }
                }
                
                
                while((initial->x != initial_x)||(initial->y != initial_y))
                {
                    newHole.at(newHole.size()-1).push_back(initial);
                    initial = initial->next_1;
                }
                newHole.at(newHole.size()-1).push_back(initial);
            }
            sort(delete_newNode.begin(), delete_newNode.end());
            vector<int> delete_newNode_first;
	    if(delete_newNode.size()!=0)
	        delete_newNode_first.push_back(delete_newNode.at(0));
            for(int j=1;j<delete_newNode.size();j++)
            {
                if(delete_newNode.at(j) != delete_newNode_first.at(delete_newNode_first.size()-1))
                    delete_newNode_first.push_back(delete_newNode.at(j));
            }
            for(int i=delete_newNode_first.size()-1;i>=0;i--)
            {
                if(delete_newNode_first.at(i)>=newNode.size())
                    continue;
                else
                {
                    newNode.erase(newNode.begin()+delete_newNode_first.at(i));
                }
            }
            

            /*for(size_t j=0;j<newNode.size();j++)
            {
                cout<<"polygon point:  "<<newNode.at(j)->x;
                cout<<newNode.at(j)->y<<endl;
            }
            cout<<endl;

            for(unsigned j=0;j<newHole.size();j++)
            {
                cout<<"holes number:   "<<j<<endl;
                for(unsigned k=0;k<newHole.at(j).size();k++)
                {
                    cout<<"holes point:  "<<newHole.at(j).at(k)->x<<" ";
                    cout<<newHole.at(j).at(k)->y<<endl;
                }
            }*/

            
            vector<int>().swap(hole_pointsize);
            vector<int>().swap(sorted_list);
            vector<int>().swap(delete_list);
            vector<int>().swap(delete_newNode);            
        }
    }

}



//This function is used only for initialize clip polygon(clockwise)
void Reclist::Initialize_node_clip(const vector<int>& coordinate)
{
    int left_min = coordinate[0]+1;
    bool iscounter = false;
    boxl = coordinate[0];
    boxr = coordinate[0];
    boxb = coordinate[1];
    boxt = coordinate[1];
    for(unsigned i=0; i<coordinate.size(); i+=2)
    {
        newNode.push_back(new Node(coordinate[i], coordinate[i+1], false));
        if(i < coordinate.size()-2)
        {
            if((coordinate[i]==coordinate[i+2])&&(left_min >= coordinate[i]))
            {
                if(coordinate[i+1]>coordinate[i+3])
                {
                    left_min = coordinate[i];
                    iscounter = true;
                }
                else if(coordinate[i+1]<coordinate[i+3])
                {
                    left_min = coordinate[i];
                    iscounter = false;
                }
            }

        }
        if(boxl>coordinate[i])
            boxl = coordinate[i];
        if(boxr<coordinate[i])
            boxr = coordinate[i];
        if(boxb>coordinate[i+1])
            boxb = coordinate[i+1];
        if(boxt<coordinate[i+1])
            boxt = coordinate[i+1];

    }

    // if y is first big then small -> counterclockwise
    if(iscounter) //check if it's counterclockwise, then reverse to clockwise
        reverse(newNode.begin(), newNode.end());

    delete newNode.at(newNode.size()-1);
    newNode.pop_back();


    //link nodes: next_1
    //head = newNode[0];

    for(unsigned i=0; i<newNode.size()-1; i++)
    {
        newNode[i]->next_1 = newNode[i+1];
    }
    newNode[newNode.size()-1]->next_1 = newNode[0];
}



