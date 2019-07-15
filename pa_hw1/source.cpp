#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

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

vector<int> edge(int x, int y, int w, int h)
{
    vector<int> ans;
    if(x!=0){
        ans.push_back(x-1);
        ans.push_back(y);
    }
    if(x!=w-1){
        ans.push_back(x+1);
        ans.push_back(y);
    }
    if(y!=0){
        ans.push_back(x);
        ans.push_back(y-1);
    }
    if(y!=h-1){
        ans.push_back(x);
        ans.push_back(y+1);
    }
    return ans;
}
vector<int> modify_matrix(vector < vector < char > > &arr, int w, int h ,int x, int y, const char change, const char temp)
{
    vector<int> ret;
    if(x!=0)
    {
        if(arr[y][x-1] == temp)
        {
            arr[y][x-1] = change;
            ret.push_back(x-1);
            ret.push_back(y);
        }
                //connect_component(arr, checkarray, w, h, x-1, y, arr[y][x-1]);
    }
    if(x!=w-1)
    {
        if(arr[y][x+1] == temp)
        {
            arr[y][x+1] = change;
            ret.push_back(x+1);
            ret.push_back(y);
        }
                //connect_component(arr, checkarray, w, h, x+1, y, arr[y][x+1]);
    }
    if(y!=0)
    {
        if(arr[y-1][x] == temp)
        {
            arr[y-1][x] = change;
            ret.push_back(x);
            ret.push_back(y-1);
        }
                //connect_component(arr, checkarray, w, h, x, y-1, arr[y-1][x]);
    }
    if(y!=h-1)
    {
        if(arr[y+1][x] == temp)
        {
            arr[y+1][x] = change;
            ret.push_back(x);
            ret.push_back(y+1);
        }
                //connect_component(arr, checkarray, w, h, x, y+1, arr[y+1][x]);
    }
    return ret;

}

vector<int> connect_component(const vector < vector < char > > &arr, vector < vector < int > > &checkarray, int w, int h ,int x, int y, const char temp)
{


        //if(check)
          //  connected++;
        //cout<<"good87"<<endl;
        //int neighbor[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
        vector<int> ret; 
        if(x!=0)
        {
            if((arr[y][x-1] == temp)&&(checkarray[y][x-1]==0))//&&(checkarray[(x-1)+w*y] == 0))
            {
                checkarray[y][x-1] = 1;
                ret.push_back(x-1);
                ret.push_back(y);
            }
                //connect_component(arr, checkarray, w, h, x-1, y, arr[y][x-1]);
        }
        if(x!=w-1)
        {
            if((arr[y][x+1] == temp)&&(checkarray[y][x+1] == 0))
            {
                checkarray[y][x+1] = 1;
                ret.push_back(x+1);
                ret.push_back(y);
            }
                //connect_component(arr, checkarray, w, h, x+1, y, arr[y][x+1]);
        }
        if(y!=0)
        {
            if((arr[y-1][x] == temp)&&(checkarray[y-1][x] == 0))
            {
                checkarray[y-1][x] = 1;
                ret.push_back(x);
                ret.push_back(y-1);
            }
                //connect_component(arr, checkarray, w, h, x, y-1, arr[y-1][x]);
        }
        if(y!=h-1)
        {
            if((arr[y+1][x] == temp)&&(checkarray[y+1][x] == 0))
            {
                checkarray[y+1][x] = 1;
                ret.push_back(x);
                ret.push_back(y+1);
            }
                //connect_component(arr, checkarray, w, h, x, y+1, arr[y+1][x]);
        }

        return ret;

        //vector<int> neighbor = edge(x, y, w, h);
        //cout<<"good9487"<<endl;

	//vector<int>().swap(neighbor);
}


int main(int argc, char* argv[]) {

	ifstream in(argv[1]);
	string input;
	vector<string> Content;
	while (getline(in, input)) {
		Content.push_back(input);
	}


	in.close();

    vector<string> v = split (Content[0], " ");

    int w = atoi(v[0].c_str());
    int h = atoi(v[1].c_str());
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    char change = *argv[4];
    //int connected = 0;

//    cout<<"good1"<<endl;

    vector < vector < char > > a(h, vector<char>(w,'0'));

    for(int j=1; j<=h; j++)
    {
        vector<string> v = split (Content[j], " ");
        for(int i=0; i<w; i++)
            a[j-1][i] = *v[i].c_str();
    }

    //cout<<"good2"<<endl;
    
 	  vector <string>().swap(Content);
	  vector <string>().swap(v);
    
    vector<int> modify;
    vector<int> modify_next;

    const char tempor = a[y][x];

    a[y][x] = change;
    if(tempor!=change)
    {
        modify_next = modify_matrix(a, w, h, x, y, change, tempor); 
        for(unsigned i=0;i<modify_next.size();i++)
        {
            modify.push_back(modify_next[i]);
        }
        while(modify.size()!=0){
            modify_next = modify_matrix(a, w, h, modify[0], modify[1], change, tempor);    
            for(unsigned i=0;i<modify_next.size();i++)
            {
                modify.push_back(modify_next[i]);
            }   
            modify.erase(modify.begin(), modify.begin() + 2);

        }
    }
    vector<int>().swap(modify);
    vector<int>().swap(modify_next);



//	v.shrink_to_fit();
	//Content.shrink_to_fit();	
/*    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
            cout<<a[i][j];
        cout<<endl;
    }
*/
		

    //int checkarray[h][w]={0};
    vector < vector < int > > checkarray(h, vector<int>(w, 0));
    //cout<<"oops"<<endl;
    long int dynamic_check = 0;  //use dynamic programming
    //cout<<"good4"<<endl;
    //cout<<"Content:"<<Content.capacity()<<endl;
    //cout<<"v:"<<v.capacity()<<endl;
    long int connected = 0;
    while(dynamic_check < w*h)
    {
        vector<int> next;
        vector<int> next_next;
        if(checkarray[dynamic_check/w][dynamic_check%w]==0){
            checkarray[dynamic_check/w][dynamic_check%w] = 1;
            next = connect_component(a, checkarray, w, h, dynamic_check%w, dynamic_check/w, a[dynamic_check/w][dynamic_check%w]);
            while(next.size()!=0)
            {
                next_next = connect_component(a, checkarray, w, h, next[0], next[1], a[next[1]][next[0]]);
                for(unsigned k=0;k<next_next.size();k++)
                {
                    next.push_back(next_next[k]);
                }
                next.erase(next.begin(), next.begin() + 2); 
            }
                //checkarray[dynamic_check] = 1;
            vector<int>().swap(next);

            ++connected;    
        }
    
        dynamic_check++;
        //cout<<dynamic_check<<endl;
    }
    //cout<<"good5"<<endl;

  /*  cout<<connected<<endl;

    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
            cout<<checkarray[i][j];
        cout<<endl;
    }*/

    //cout<<"good6"<<endl;

    string writeFile = argv[5];
    ofstream out(writeFile.c_str());
    out<<w<<" "<<h<<endl;
    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w-1; j++)
            out<<a[i][j]<<" ";
        out<<a[i][w-1]<<endl;
    }
    out<<connected<<endl;
    out.close();

	return 0;

}

