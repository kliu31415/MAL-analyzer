int listSize = 80;
void getSimilar(string s)
{
    if(titles.lower_bound(s) == titles.end())
    {
        cout << "can't find " << s << "\n";
        return;
    }
    string title = titles.lower_bound(s)->A;
    map<string, vector<pair<double, double> > > likability;
    ASIZE_MIN = 5;
    ASIZE_MAX = 1e6;
    for(auto &i: anime_z)
    {
        if(i.B.size()>ASIZE_MIN && i.B.size()<ASIZE_MAX && i.B.count(title))
        {
            double rating = i.B[title];
            for(auto &j: i.B)
            {
                if(j.A != title)
                    likability[j.A].emplace_back(rating, j.B);
            }
        }
    }
    vector<pair<string, pair<double, int> > > likeFinal;
    vector<int> lsize;
    for(auto &i: likability)
        lsize.push_back(i.B.size());
    sort(lsize.begin(), lsize.end());
    for(auto &i: likability)
    {
        if(lsize.size() <= listSize || i.B.size() >= lsize[lsize.size()-listSize])
        {
            likeFinal.emplace_back(i.A, mp(correlation(i.B), i.B.size()));
        }
    }
    cout << "affinity to " << title << "\n";
    auto cflags = cout.flags();
    cout.setf(ios_base::left);
    cout << setw(40) << "anime" << setw(16) << "affinity" << "samples\n";
    for(int i=0; i<64; i++)
        cout << "-";
    cout << "\n";
    sort(likeFinal.begin(), likeFinal.end(), [](pair<string, pair<double, int> > a, pair<string, pair<double, int> > b)->bool{return a.B.A > b.B.A;});
    for(auto &i: likeFinal)
    {
        cout << setw(40) << cut(i.A, 39) << setw(16) << pm(i.B.A) << i.B.B << "\n";
    }
    cout.flush();
    cout.setf(cflags);
}
void setLsize(string s)
{
    listSize = atoi(s.c_str());
}
