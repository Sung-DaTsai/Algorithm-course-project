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
void modify_matrix(vector < vector < char > > &arr, int w, int h ,int x, int y, char change, char temp)
{
    arr[y][x] = change;
    vector<int> neighbor = edge(x, y ,w, h);
    for(int i=0;i<neighbor.size();i+=2)
    {
        if(arr[neighbor[i+1]][neighbor[i]] == temp)
        {
            arr[neighbor[i+1]][neighbor[i]] = change;
            modify_matrix(arr, w, h, neighbor[i], neighbor[i+1], change, temp);
        }
    }

}

void connect_component(const vector < vector < char > > &arr, vector < vector < int > > &checkarray, int w, int h ,int x, int y, const char temp, int &connected, bool check)
{
    if (checkarray[y][x]==0)
    {
        checkarray[y][x] = 1;

        if(check)
            connected++;

        vector<int> neighbor = edge(x, y, w, h);

        for(int i=0;i<neighbor.size();i+=2)
        {
            if(arr[neighbor[i+1]][neighbor[i]] == temp)
                connect_component(arr, checkarray, w, h, neighbor[i], neighbor[i+1], arr[neighbor[i+1]][neighbor[i]], connected, false);
        }
    }
}


int main(int argc, char* argv[]) {

	ifstream in("390286_sample.in");   //argv[1]
	string input;
	vector<string> Content;
	while (getline(in, input)) {
		Content.push_back(input);
	}


	in.close();

    vector<string> v = split (Content[0], " ");

    int w = atoi(v[0].c_str());
    int h = atoi(v[1].c_str());
    int x = 1;//atoi(argv[2])
    int y = 3;//atoi(argv[3])
    char change = 'B';//*argv[4]
    int connected = 0;



    vector < vector < char > > a(h, vector<char>(w,'0'));

    for(int j=1; j<=h; j++)
    {
        vector<string> v = split (Content[j], " ");
        for(int i=0; i<w; i++)
            a[j-1][i] = *v[i].c_str();
    }


    modify_matrix(a, w, h, x, y, change, a[y][x]);
    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
            cout<<a[i][j];
        cout<<endl;
    }

    vector < vector < int > > checkarray(h, vector<int>(w, 0));

    int dynamic_check = 0;  //use dynamic programming

    while(dynamic_check < w*h)
    {
        connect_component(a, checkarray, w, h, dynamic_check%w, dynamic_check/w, a[dynamic_check/w][dynamic_check%w], connected, true);
        dynamic_check++;
    }

    cout<<connected<<endl;

    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
            cout<<checkarray[i][j];
        cout<<endl;
    }


    string writeFile = "sample1.out"; //argv[5]
    ofstream out(writeFile.c_str());
    out<<w<<" "<<h<<endl;
    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w-1; j++)
            out<<a[i][j]<<" ";
        out<<a[i][w-1]<<endl;
    }
    out<<connected;
    out.close();


	return 0;

}

