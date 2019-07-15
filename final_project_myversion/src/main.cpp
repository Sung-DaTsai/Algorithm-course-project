#include <iostream>
#include <vector>
#include "Structure.h"
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> ans;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        ans.push_back (token);
    }

    ans.push_back (s.substr (pos_start));
    return ans;
}


int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    string inputStr;
    vector<string> inputContent;
    vector<string> operation;
    vector<int> operation_row;
    int count_end = 0;
    int now_operation;
    int spilt_at_last = 0;
    string merge1("MERGE");
    string clip("CLIPPER");
    string enddata("END DATA");
    while(getline(in, inputStr,';'))  // parse input file
    {
        inputContent.push_back(inputStr);
        if(inputContent.size()==1)
        {
            for(unsigned i=10;i<inputContent.at(0).length();i++)
            {
                if(inputContent.at(0).substr(i, 1)=="M")
                {
                    operation.push_back(inputContent.at(0).substr(i, 1));
                    operation_row.push_back(-1);
                    operation_row.push_back(-1);
                }
                else if(inputContent.at(0).substr(i, 1)=="C")
                {
                    operation.push_back(inputContent.at(0).substr(i, 1));
                    operation_row.push_back(-1);
                    operation_row.push_back(-1);
                }
                else if(inputContent.at(0).substr(i, 1)=="S")
                {
                    if(inputContent.at(0).substr(i+1, 1)=="V")
                        spilt_at_last = 1;
                    else if(inputContent.at(0).substr(i+1, 1)=="H")
                        spilt_at_last = 2;
                    else if(inputContent.at(0).substr(i+1, 1)=="O")
                        spilt_at_last = 3;
                    break;
                }
                else if(isdigit(inputContent.at(0).at(i)))
                {
                    operation.at(operation.size()-1).append(inputContent.at(0), i, 1);
                }
            }
        }
        else
        {
            for(unsigned i=0;i<inputContent.at(inputContent.size()-1).length();i++)
            {
                if(inputContent.at(inputContent.size()-1).substr(i, 1)=="P")
                {
                    break;
                }
                else if(inputContent.at(inputContent.size()-1).substr(i, 1)=="D")
                {    
                    string tempors;
                    if(inputContent.at(inputContent.size()-1).substr(i+5, 1)=="M")
                    {
                        tempors.append(inputContent.at(inputContent.size()-1), i+11, 1);
                        int sub = i+12;
                        while(isdigit(inputContent.at(inputContent.size()-1).at(sub)))
                        {
                            tempors.append(inputContent.at(inputContent.size()-1), sub, 1);
                            sub++;
                        }
                        for(unsigned k=0;k<operation.size();k++)
                        {
                            if(operation.at(k)==tempors)
                            {
                                operation_row.at(2*k) = inputContent.size();
                                now_operation = 2*k+1;
                                break;
                            }
                        }
                        break;
                    }
                    else if(inputContent.at(inputContent.size()-1).substr(i+5, 1)=="C")
                    {
                        tempors.append(inputContent.at(inputContent.size()-1), i+13, 1);
                        int sub = i+14;
                        while(isdigit(inputContent.at(inputContent.size()-1).at(sub)))
                        {
                            tempors.append(inputContent.at(inputContent.size()-1), sub, 1);
                            sub++;
                            if(inputContent.at(inputContent.size()-1).length()==sub)
                                break;
                        }
                        for(unsigned k=0;k<operation.size();k++)
                        {
                            if(operation.at(k)==tempors)
                            {
                                operation_row.at(2*k) = inputContent.size();
                                now_operation = 2*k+1;
                                break;
                            }
                        }
                        break;
                    }
                }
                else if(inputContent.at(inputContent.size()-1).substr(i, 1)=="E")
                {
                    if(count_end == operation.size())
                        break;
                    operation_row.at(now_operation) = inputContent.size()-2;
                    i = i+7;
                    count_end++;
                }
            }
        }
    }
    in.close();

    

    bool multiple = false;
    vector <int> poly;
    vector<string> v = split (inputContent[2], " ");
    for(unsigned i=1; i<v.size()-1; i++)
        poly.push_back(atol(v[i].c_str()));
    Structure total(poly, true);
    vector <int> ().swap(poly);
    vector <string> ().swap(v);
    
    for(unsigned i=0;i<operation.size();i++)
    {
        
        if(operation.at(i).substr(0,1)=="M")  // merge operation
        {
            for(unsigned k=operation_row.at(2*i);k<=operation_row.at(2*i+1);k++)
            {
                v = split (inputContent[k], " ");
                for(unsigned ii=1; ii<v.size()-1; ii++)
                    poly.push_back(atol(v[ii].c_str()));
                Structure merging(poly, true);
                total.Find_intersection_merge(total, merging);
                vector <int> ().swap(poly);
                vector <string> ().swap(v);
            }
            
        }
        else if(operation.at(i).substr(0,1)=="C")  // clip operation
        {
            for(unsigned k=operation_row.at(2*i);k<=operation_row.at(2*i+1);k++)
            {
                v = split (inputContent[k], " ");
                for(unsigned ii=1; ii<v.size()-1; ii++)
                    poly.push_back(atol(v[ii].c_str()));
                Structure clipping(poly, false);
                total.Find_intersection_clip(total, clipping);
                vector <int> ().swap(poly);
                vector <string> ().swap(v);
            }
        }
    }

    
    string finalfile = argv[2];
    ofstream output(finalfile.c_str());
    // split operation
    if(spilt_at_last == 1)
        total.split_V(total, output);
    else if(spilt_at_last == 2)
	      total.split_H(total, output);
    else if(spilt_at_last == 3)
    {
        string finalfileV = "dummy_V.txt";
        ofstream outputV(finalfileV.c_str());
	      int V_num = total.split_V(total, outputV);
        outputV.close();
        string finalfileH = "dummy_H.txt";
        ofstream outputH(finalfileH.c_str());
	      int H_num = total.split_H(total, outputH);
        outputH.close();
        if(V_num < H_num)
            total.split_V(total, output);
        else
            total.split_H(total, output);
    }
    output.close();

    return 0;
}
