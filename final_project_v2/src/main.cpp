#include <boost/polygon/polygon.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

namespace bp = boost::polygon;
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
    typedef bp::polygon_90_with_holes_data<long long>    ComplexPolygon;
    typedef bp::point_data<long long>                    Point;
    typedef bp::polygon_90_set_data<long long>           PolygonSet;
    typedef vector<bp::rectangle_data<long long>>        RectSet;
    typedef vector<bp::polygon_90_with_holes_data<long long>>   PolygonVec;
    using namespace boost::polygon::operators;

    ifstream in(argv[1]);         // input file
    string inputStr;
    vector<string> inputContent;
    vector<string> operation;
    vector<int> operation_row;
    int count_end = 0;            // count the number of operations
    int now_operation;
    int split_at_last = 0;        // split operation
    string merge1("MERGE");       // use for parsing
    string clip("CLIPPER");
    string enddata("END DATA");
    while(getline(in, inputStr,';'))
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
                        split_at_last = 1;
                    else if(inputContent.at(0).substr(i+1, 1)=="H")
                        split_at_last = 2;
                    else if(inputContent.at(0).substr(i+1, 1)=="O")
                        split_at_last = 3;
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

    PolygonSet total;
    vector<long long> poly;
    vector<string> v;
    vector<Point> pts;
    
    for(unsigned i=0;i<operation.size();i++)
    {
        if(operation.at(i).substr(0,1)=="M")   // do merge operation
        {
            for(unsigned k=operation_row.at(2*i);k<=operation_row.at(2*i+1);k++)
            {
                //cout<<k<<inputContent.at(k)<<endl;
                v = split (inputContent[k], " ");
                for(unsigned ii=1; ii<v.size()-3; ii+=2)
		{
		    Point temp_point{atoll(v[ii].c_str()), atoll(v[ii+1].c_str())};
		    pts.push_back(temp_point);
		}

		ComplexPolygon p;
                bp::set_points(p, pts.begin(), pts.end());
		total += p;
                vector <long long> ().swap(poly);
                vector <string> ().swap(v);
		vector <Point> ().swap(pts);


            }

        }
        else if(operation.at(i).substr(0,1)=="C")   // do clip operation
        {
	    PolygonSet total_clip;
            for(unsigned k=operation_row.at(2*i);k<=operation_row.at(2*i+1);k++)
            {
		//cout<<k<<inputContent.at(k)<<endl;
		v = split (inputContent[k], " ");
                for(unsigned ii=1; ii<v.size()-3; ii+=2)
		{
		    Point temp_point{atoll(v[ii].c_str()), atoll(v[ii+1].c_str())};
		    pts.push_back(temp_point);
		}
		ComplexPolygon p;
                bp::set_points(p, pts.begin(), pts.end());
		total_clip += p; 
                vector <long long> ().swap(poly);
                vector <string> ().swap(v);
		vector <Point> ().swap(pts);
            }
	    total -= total_clip;
        }
    }


    string finalfile = argv[2];    // output file
    ofstream output(finalfile.c_str());
    bp::orientation_2d_enum horizontal = bp::HORIZONTAL;
    bp::orientation_2d_enum vertical = bp::VERTICAL;
    RectSet rectangles;
    if(split_at_last == 1)  // split vertically
    {
        total.get_rectangles<RectSet>(rectangles, vertical);
        for (const auto& polygon : rectangles)
        {
	    output << "RECT " << std::to_string(xl(polygon)) << " " << std::to_string(yl(polygon))  << " "  << std::to_string(xh(polygon)) << " "  << std::to_string(yh(polygon))  << " ;" << '\n';
        }
    }
    else if(split_at_last == 2)  // split horizontally
    {
        total.get_rectangles<RectSet>(rectangles, horizontal);
        for (const auto& polygon : rectangles)
        {
	    output << "RECT " << std::to_string(xl(polygon)) << " " << std::to_string(yl(polygon))  << " "  << std::to_string(xh(polygon)) << " "  << std::to_string(yh(polygon))  << " ;" << '\n';
        }
    }
    else if(split_at_last == 3)  // split optimally
    {
	PolygonVec temp_poly;
	total.get_polygons<PolygonVec>(temp_poly);
	for (const auto& polygons : temp_poly)
	{
	    PolygonSet temp_set;
	    temp_set += polygons;
	    RectSet temp_rectangle_H;
	    RectSet temp_rectangle_V;
	    temp_set.get_rectangles<RectSet>(temp_rectangle_H, horizontal);
	    temp_set.get_rectangles<RectSet>(temp_rectangle_V, vertical);
	    if(temp_rectangle_H.size() >= temp_rectangle_V.size())
	    {
		for (const auto& polygon : temp_rectangle_V)
		{
		    output << "RECT " << std::to_string(xl(polygon)) << " " << std::to_string(yl(polygon))  << " "  << std::to_string(xh(polygon)) << " "  << std::to_string(yh(polygon))  << " ;" << '\n';
		}
	    }
	    else
	    {
		for (const auto& polygon : temp_rectangle_H)
		{
	            output << "RECT " << std::to_string(xl(polygon)) << " " << std::to_string(yl(polygon))  << " "  << std::to_string(xh(polygon)) << " "  << std::to_string(yh(polygon))  << " ;" << '\n';
		}
	    }
	}

    }
    output.close();

    return 0;
}


