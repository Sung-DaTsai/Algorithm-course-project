#include "Node.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int Node::num_source = 0;
int Node::num_sink = 0;

Node::Node(int a, int b, int c)
    :x(a),y(b)
{
    if(c >= 0)
    {
        issource = true;
        flow = c;
        last_flow = c;
        ++num_source;
    }
    else
    {
        issource = false;
        flow = -c;
        last_flow = -c;
        ++num_sink;
    }
}

Node::~Node()
{

}

int Node::get_numsource()
{
    return num_source;
}

int Node::get_numsink()
{
    return num_sink;
}

void Node::construct_flow(vector <Node*> & a, vector <Node*> & b)  // a: source, b: sink
{
    vector <int> temporary;
    for(unsigned i=0;i<a.size();i++)
    {
        for(unsigned j=0;j<b.size();j++)
        {
            a.at(i)->flow_network.push_back(temporary);
            a.at(i)->length_flow_capacity.push_back(temporary);
            b.at(j)->length_flow_capacity.push_back(temporary);

            //calculate wire length
            int wirelength = abs(a.at(i)->x - b.at(j)->x) + abs(a.at(i)->y - b.at(j)->y);
            a.at(i)->length_flow_capacity.at(j).push_back(wirelength);
            a.at(i)->flow_network.at(j).push_back(wirelength);
            b.at(j)->length_flow_capacity.at(i).push_back(-wirelength);

            a.at(i)->length_flow_capacity.at(j).push_back(0);
            a.at(i)->flow_network.at(j).push_back(0);
            b.at(j)->length_flow_capacity.at(i).push_back(0);

            b.at(j)->length_flow_capacity.at(i).push_back(0);
            if(a.at(i)->flow >= b.at(j)->flow)
            {
                a.at(i)->length_flow_capacity.at(j).push_back(b.at(j)->flow);
                a.at(i)->flow_network.at(j).push_back(b.at(j)->flow);
            }
            else
            {
                a.at(i)->length_flow_capacity.at(j).push_back(a.at(i)->flow);
                a.at(i)->flow_network.at(j).push_back(a.at(i)->flow);
            }
        }

    }

    int count_b = 0;
    for(unsigned i=0;i<a.size();i++)
    {
        while(count_b < b.size())
        {
            if(b.at(count_b)->last_flow <= a.at(i)->last_flow )
            {
                a.at(i)->flow_network.at(count_b).at(1) += b.at(count_b)->last_flow;
                a.at(i)->length_flow_capacity.at(count_b).at(2) -= b.at(count_b)->last_flow;
                b.at(count_b)->length_flow_capacity.at(i).at(2) -= b.at(count_b)->last_flow;
                a.at(i)->last_flow -= b.at(count_b)->last_flow;
                b.at(count_b)->last_flow = 0;
                count_b++;
            }
            else
            {
                a.at(i)->flow_network.at(count_b).at(1) += a.at(i)->last_flow;
                a.at(i)->length_flow_capacity.at(count_b).at(2) -= a.at(i)->last_flow;
                b.at(count_b)->length_flow_capacity.at(i).at(2) -= a.at(i)->last_flow;
                b.at(count_b)->last_flow -= a.at(i)->last_flow;
                a.at(i)->last_flow = 0;
                break;
            }
        }
    }
}

bool Node::optimal(vector <Node*> & a, vector <Node*> & b)  // a: source, b: sink
{
    vector <Node*> total;
    int flow_to_change_first = 3000000;
    int flow_to_change = 3000000;
    for(unsigned i=0;i<a.size();i++)
    {
        total.push_back(a.at(i));
    }
    for(unsigned i=0;i<b.size();i++)
    {
        total.push_back(b.at(i));
    }


        vector<int> distance;
        for(unsigned i=0;i<total.size();i++)
            distance.push_back(3000000);
        distance.at(0) = 0;

        for(unsigned i=0;i<total.size()-1;i++)
        {
            for(unsigned j=0;j<total.size();j++)
            {
                for(unsigned k=0;k<total.at(j)->length_flow_capacity.size();k++)
                {
                    if(total.at(j)->length_flow_capacity.at(k).at(2)>0) // have edge from source to sink
                    {
                        if((distance.at(j)!=3000000)&&(distance.at(k+a.size())>distance.at(j)+total.at(j)->length_flow_capacity.at(k).at(0)))
                        {
                            distance.at(k+a.size()) = distance.at(j) + total.at(j)->length_flow_capacity.at(k).at(0);
                            total.at(k+a.size())->parent_num = j;
                            total.at(k+a.size())->parent_num_oneside = j;
                        }
                    }
                    else if(total.at(j)->length_flow_capacity.at(k).at(2)<0) // have edge from sink to source
                    {
                        if((distance.at(j)!=3000000)&&(distance.at(k)>distance.at(j)+total.at(j)->length_flow_capacity.at(k).at(0)))
                        {
                            distance.at(k) = distance.at(j) + total.at(j)->length_flow_capacity.at(k).at(0);
                            total.at(k)->parent_num = j;
                            total.at(k)->parent_num_oneside = j-a.size();
                        }
                    }
                }
            }
        }

        vector<int> flow_to_change_vec;
        int temps = 30000000;
        for(unsigned j=0;j<total.size();j++)
        {
            for(unsigned k=0;k<total.at(j)->length_flow_capacity.size();k++)
            {
                if(total.at(j)->length_flow_capacity.at(k).at(2)>0) // have edge from source to sink
                {
                    if((distance.at(j)!=3000000)&&(distance.at(k+a.size())>distance.at(j)+total.at(j)->length_flow_capacity.at(k).at(0)))
                    {

                        vector<int> candidate;
                        flow_to_change_vec.push_back(abs(total.at(total.at(j)->parent_num)->length_flow_capacity.at(j).at(2)));
                        int self_num = total.at(j)->parent_num_oneside;
                        int self_num_total = total.at(j)->parent_num;
                        Node* backward = total.at(total.at(j)->parent_num);
                        //cout<<backward->x<<" "<<backward->y<<endl;
                        //cout<<total.at(j)->x<<" "<<total.at(j)->y<<endl;
                        candidate.push_back(total.at(j)->x);
                        candidate.push_back(total.at(j)->y);
                        int temp_x, temp_y;
                        bool finish = true;
                        bool countingaorb = false;
                        while(finish)
                        {
                            for(unsigned i=0;i<candidate.size();i+=2)
                            {
                                //cout<<"candidate x,y:"<<candidate.at(i)<<" "<<candidate.at(i+1)<<endl;
                                if((backward->x == candidate.at(i))&&(backward->y == candidate.at(i+1)))
                                {
                                    flow_to_change_vec.push_back(abs(total.at(backward->parent_num)->length_flow_capacity.at(self_num).at(2)));
                                    temp_x = candidate.at(i);
                                    temp_y = candidate.at(i+1);
                                    finish = false;
                                    if(i%4==0)
                                        countingaorb = false;
                                    else
                                        countingaorb = true;
                                    for(int ss=i/2;ss<flow_to_change_vec.size();ss++)
                                    {
                                        temps = min(temps, flow_to_change_vec.at(ss));
                                        //cout<<"flow_to_change_vec.at(ss): "<<flow_to_change_vec.at(ss)<<endl;
                                        //cout<<"temps: "<<temps<<endl;
                                    }
                                    flow_to_change = temps;
                                    //cout<<"flow_to_change: "<<flow_to_change<<endl;
                                    break;
                                }
                                else if(i==candidate.size()-2)
                                {
                                    //temps = flow_to_change_vec.at(flow_to_change_vec.size()-1);
                                    flow_to_change_vec.push_back(abs(total.at(backward->parent_num)->length_flow_capacity.at(self_num).at(2)));
                                    candidate.push_back(backward->x);
                                    candidate.push_back(backward->y);
                                    self_num = backward->parent_num_oneside;
                                    self_num_total = backward->parent_num;
                                    backward = total.at(backward->parent_num);
                                    //cout<<"backward x,y: "<<backward->x<<" "<<backward->y<<endl;
                                    break;
                                }
                            }
                        }
                        
                        if(countingaorb)
                        {
                            total.at(backward->parent_num)->length_flow_capacity.at(self_num).at(2) -= flow_to_change;
                            total.at(backward->parent_num)->flow_network.at(self_num).at(1) += flow_to_change;
                            total.at(self_num_total)->length_flow_capacity.at(backward->parent_num_oneside).at(2) -= flow_to_change;
                            self_num = backward->parent_num_oneside;
                            self_num_total = backward->parent_num;
                            backward = total.at(backward->parent_num);
                        }
                        else
                        {
                            total.at(backward->parent_num)->length_flow_capacity.at(self_num).at(2) += flow_to_change;
                            total.at(self_num_total)->flow_network.at(backward->parent_num_oneside).at(1) -= flow_to_change;
                            total.at(self_num_total)->length_flow_capacity.at(backward->parent_num_oneside).at(2) += flow_to_change;
                            self_num = backward->parent_num_oneside;
                            self_num_total = backward->parent_num;
                            backward = total.at(backward->parent_num);
                        }

                        while((backward->x != temp_x)||(backward->y != temp_y))
                        {
                            if(countingaorb)
                            {
                                total.at(backward->parent_num)->length_flow_capacity.at(self_num).at(2) += flow_to_change;
                                total.at(self_num_total)->flow_network.at(backward->parent_num_oneside).at(1) -= flow_to_change;
                                total.at(self_num_total)->length_flow_capacity.at(backward->parent_num_oneside).at(2) += flow_to_change;
                                self_num = backward->parent_num_oneside;
                                self_num_total = backward->parent_num;
                                backward = total.at(backward->parent_num);
                                countingaorb = false;
                            }
                            else
                            {
                                total.at(backward->parent_num)->length_flow_capacity.at(self_num).at(2) -= flow_to_change;
                                total.at(backward->parent_num)->flow_network.at(self_num).at(1) += flow_to_change;
                                total.at(self_num_total)->length_flow_capacity.at(backward->parent_num_oneside).at(2) -= flow_to_change;
                                self_num = backward->parent_num_oneside;
                                self_num_total = backward->parent_num;
                                backward = total.at(backward->parent_num);
                                countingaorb = true;
                            }
                        }
                        return true;
                    }
                }
            }
        }

    return false;

}
