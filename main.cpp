#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <cstring>
#include <thread>
#ifdef __GNUC__
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template<class T1, class T2 = null_type> struct fast_map: public gp_hash_table<T1, T2>
{
    int count(T1 x)
    {
        return this->find(x) != this->end();
    }
};
#define fast_map __gnu_pbds::gp_hash_table
#else
#define fast_map map
#endif
#define A first
#define B second
#define pii pair<int, int>
#define mp make_pair
using namespace std;
#include "basic_stuff.h"
#include "similar.h"
#include "graphData.h"
#include "simulated_anneal_cluster.h"
#include "load_data.h"
#include "plot.h"
#include "like.h"
#include "rate.h"
int main()
{
    ios::sync_with_stdio(0);
    load_users();
    load_anime();
    while(true)
    {
        string s;
        cout << ">";
        cout.flush();
        getline(cin, s);
        s += ' ';
        string instruction;
        for(int i=0; i<s.size()-1; i++)
        {
            if(s[i] == ' ')
            {
                instruction = s.substr(0, i);
                s = s.substr(i+1);
                break;
            }
        }
        s.pop_back();
        if(instruction == "dendrogram1")
            graphData(s);
        else if(instruction == "similar")
            getSimilar(s);
        else if(instruction == "lsize")
            setLsize(s);
        else if(instruction == "cluster1")
            sa_cluster1(s);
        else if(instruction == "group1")
            sa_group1(s);
        else if(instruction == "plotr")
            plotr(s);
        else if(instruction == "plotu")
            plotu(s);
        else if(instruction == "like")
            like(s);
        else if(instruction == "liker")
            liker(s);
        else if(instruction == "liken")
            liken(s);
        else if(instruction == "ratet")
            setRthreshold(s);
        else if(instruction == "rate")
            rate(s);
        else if(instruction == "raten")
            raten(s);
        else if(instruction == "rates")
            rates(s, true);
        else if(instruction == "rated")
            rates(s, false);
        else if(instruction == "rateu")
            rateu(s);
        else cout << "error\n";
    }
    return 0;
}
