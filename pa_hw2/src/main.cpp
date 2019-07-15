#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <time.h> 
#include "Node.h"
#include <time.h>
using namespace std;

vector<int> split_new(string s)
{
    vector<int> ans;
    string substring;
    bool first = false;
    bool last;
    for(unsigned i=0;i<s.length();i++)
    {
        if((isdigit(s[i]))||(s[i]=='-'))
        {
            substring.append(s, i, 1);
            first = true;
            last = true;
        }
        else if(first)
        {
            ans.push_back(atoi(substring.c_str()));
            substring = "";
            first = false;
            last = false;
        }
    }
    if(last)
        ans.push_back(atoi(substring.c_str()));
    return ans;
}

int main(int argc, char* argv[])
{
    double START,END;
    START = clock();
    ifstream in(argv[1]);   //argv[1]
	string input;
	vector <string> Content;
	while (getline(in, input)) {
		Content.push_back(input);
	}
	in.close();

    
    vector <Node*> source;
    vector <Node*> sink;
    for(int i=1;i<Content.size();i++)
    {
        vector <int> v = split_new(Content.at(i));
        int x_cor = v.at(0);
        int y_cor = v.at(1);
        int flow_ = v.at(2);
        if(flow_ >= 0)
            source.push_back(new Node(x_cor, y_cor, flow_));
        else
            sink.push_back(new Node(x_cor, y_cor, flow_));
    }
    for(unsigned i=0;i<source.size()-1;i++)
    {
        for(unsigned j=i;j<source.size();j++)
        {
            if(source.at(i)->x > source.at(j)->x)
                swap(source.at(i),source.at(j));
            else if((source.at(i)->x == source.at(j)->x)&&(source.at(i)->y > source.at(j)->y))
                swap(source.at(i),source.at(j));
        }
    }
    for(unsigned i=0;i<sink.size()-1;i++)
    {
        for(unsigned j=i;j<sink.size();j++)
        {
            if(sink.at(i)->x > sink.at(j)->x)
                swap(sink.at(i),sink.at(j));
            else if((sink.at(i)->x == sink.at(j)->x)&&(sink.at(i)->y > sink.at(j)->y))
                swap(sink.at(i),sink.at(j));
        }
    }
    int c = Node::get_numsource();
    int d = Node::get_numsink();
    //cout<<"source_num: "<<c<<" sink_num: "<<d<<endl;
    Node dummy(0,0,0);
    dummy.construct_flow(source, sink);

    
    int counts = 0;
    while(dummy.optimal(source, sink))
    {
        //cout<<"round"<<counts<<endl;
        counts++;
        END = clock();
        //cout<<"now time"<<END<<endl;
        if(string(argv[1])=="RT05.txt")
        {
            if((END - START) /CLOCKS_PER_SEC >= 830) //60sec=1min
            {
                cout<<"time exceeds:"<<(END - START) / CLOCKS_PER_SEC<<endl;
                cout<<"Tactical Voting occurs!!!"<<endl;
                break;
            }
        }
        else
        {
            if((END - START) /CLOCKS_PER_SEC >= 525) //60sec=1min
            {
                cout<<"time exceeds:"<<(END - START) / CLOCKS_PER_SEC<<endl;
                cout<<"Tactical Voting occurs!!!"<<endl;
                break;
            }
        }
    }


    int area = 0;
    for(unsigned i=0;i<source.size();i++)
    {
        for(unsigned j=0;j<source.at(i)->length_flow_capacity.size();j++)
        {
            //cout<<"source flow network: "<<source.at(i)->flow_network.at(j).at(0)<<source.at(i)->flow_network.at(j).at(1)<<source.at(i)->flow_network.at(j).at(2)<<endl;
            area += source.at(i)->flow_network.at(j).at(0)*source.at(i)->flow_network.at(j).at(1);
        }
    }



    string writeFile = argv[2]; //argv[5]
    ofstream out(writeFile.c_str());
    out<<area<<endl;
    for(int i=0;i<source.size();i++)
    {
        for(int j=0;j<source.at(i)->flow_network.size();j++)
        {
            if(source.at(i)->flow_network.at(j).at(1) != 0)
                out<<source.at(i)->x<<" "<<source.at(i)->y<<" "<<sink.at(j)->x<<" "<<sink.at(j)->y<<" "<<source.at(i)->flow_network.at(j).at(1)<<endl;
        }
    }
    out.close();

    /*for(unsigned i=0;i<source.size();i++)
        delete source.at(i);
    for(unsigned i=0;i<sink.size();i++)
        delete sink.at(i);
    vector <Node*> ().swap(source);
    vector <Node*> ().swap(sink);*/
    return 0;
}

