vector<string> users;
map<string, map<string, double> > anime, anime_z;
map<string, vector<pair<string, double> > > anime_z_vec;
map<string, int> titles;
int ASIZE_MIN;
int ASIZE_MAX;
struct anime_compare //broken :(
{
    bool hasEllipsis(const string &x, int pos) const
    {
        if(x.size()<3 || x.size()-pos > 3)
            return false;
        return x.substr(x.size()-3) == "...";
    }
    bool operator()(const string &a, const string &b) const
    {
        for(int i=0; i<min(a.size(), b.size()); i++)
        {
            if(a[i] != b[i])
            {
                if(hasEllipsis(a, i) || hasEllipsis(b, i))
                    return false;
                return a[i] < b[i];
            }
        }
        return false;
    }
};
map<string, double> globalAnimeRatings;
vector<string> globalAnimeList;
template<class T> string to_string(T x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}
template<class T> string pm(T x)
{
	if(x < 0)
		return to_string(x);
	return "+" + to_string(x);
}
string cut(string x, int len)
{
    if(x.size() <= len)
        return x;
    x.erase(len, x.size()-len);
    for(int i=x.size()-3; i<x.size(); i++)
        x[i] = '.';
    return x;
}
vector<string> split(string &s, char c = ' ')
{
    vector<string> res;
    int prv = 0;
    s += c;
    for(int i=0; i<s.size(); i++)
    {
        if(s[i] == c)
        {
            if(i != prv)
                res.push_back(s.substr(prv, i-prv));
            prv = i+1;
        }
    }
    s.pop_back();
    return res;
}
double pmr2(double x)
{
    return x<0? -x*x: x*x;
}
double pmr(double x)
{
    return x<0? -sqrt(-x): sqrt(x);
}
void toZ(vector<double> &x)
{
    double sum = 0;
    for(auto &i: x)
        sum += i;
    double avg = sum / x.size();
    double sd = 0;
    for(auto &i: x)
        sd += pow(i - avg, 2);
    sd = sqrt(sd / x.size());
    for(auto &i: x)
    {
        if(sd == 0)
            i = 0;
        else i = (i - avg) / sd;
    }
}
void toZ(map<string, double> &x)
{
    for(auto i=x.begin(); i!=x.end(); )
    {
        if(i->B == 0)
            i = x.erase(i);
        else i++;
    }
    if(x.size() == 0)
        return;
    double sum = 0;
    for(auto &i: x)
        sum += i.B;
    double avg = sum / x.size();
    double sd = 0;
    for(auto &i: x)
        sd += pow(i.B - avg, 2);
    sd = sqrt(sd / x.size());
    for(auto &i: x)
    {
        if(sd == 0)
            i.B = 0;
        else i.B = (i.B - avg) / sd;
    }
}
double correlation(vector<double> &numA, vector<double> &numB)
{
    toZ(numA);
    toZ(numB);
    double sum = 0;
    for(int i=0; i<numA.size(); i++)
        sum += numA[i] * numB[i];
    if(numA.size() == 0)
        return 0;
    return sum / numA.size();
}
double correlation(vector<pair<double, double> > &x)
{
    if(x.size() == 0)
        return 0;
    vector<double> numA, numB;
    for(auto &i: x)
    {
        numA.push_back(i.A);
        numB.push_back(i.B);
    }
    return correlation(numA, numB);
}
double correlation(string u1, string u2, string bad = "", int *cnt = NULL)
{
    vector<double> a, b;
    for(auto &i: anime_z[u1])
    {
        if(i.A!=bad && anime_z[u2].count(i.A))
        {
            a.push_back(i.B);
            b.push_back(anime_z[u2][i.A]);
        }
    }
    if(cnt != NULL)
        *cnt = a.size();
    return correlation(a, b);
}
double correlation_no_z(string u1, string u2, string bad = "", int *cnt = NULL)
{
    vector<double> a, b;
    for(auto &i: anime[u1])
    {
        if(i.A!=bad && i.B!=0 && anime[u2].count(i.A) && anime[u2][i.A]!=0)
        {
            a.push_back(i.B);
            b.push_back(anime[u2][i.A]);
        }
    }
    if(cnt != NULL)
        *cnt = a.size();
    return correlation(a, b);
}
double average(map<string, double> &u)
{
    int cnt = 0;
    double res = 0;
    for(auto &i: u)
    {
        if(i.B != 0)
        {
            cnt++;
            res += i.B;
        }
    }
    if(cnt == 0)
        return 0;
    return res / cnt;
}
double rdev(map<string, double> &u)
{
    double res1 = 0, res2 = 0;
    int cnt = 0;
    for(auto &i: u)
        if(i.B!=0 && globalAnimeRatings.count(i.A))
        {
            cnt++;
            res1 += i.B;
            res2 += globalAnimeRatings[i.A];
        }
    if(cnt == 0)
        return 0;
    return (res1 - res2) / cnt;
}
void showProgress(int cur, int n, int W = 50)
{
    cout << "\r[";
    for(int i=0; i<cur*W/n; i++)
        cout << "=";
    for(int i=0; i<W - cur*W/n; i++)
        cout << " ";
    cout << "] " << cur*100/n << "%";
    cout.flush();
}
void linearRegression(vector<pair<double, double> > v, double *M, double *B)
{
    double V1 = 0, V2 = 0, V3 = 0, V4 = 0;
    for(auto &i: v)
    {
        V1 += i.B;
        V2 += i.A * i.A;
        V3 += i.A;
        V4 += i.A * i.B;
    }
    double N = V1 * V2 - V3 * V4;
    double D = v.size() * V2  - V3 * V3;
    if(D == 0)
        *B = 0;
    else *B = N / D;
    N = v.size() * V4 - V1 * V3;
    D = v.size() * V2 - V3 * V3;
    if(D == 0)
        *M = 1;
    else *M = N / D;
}
