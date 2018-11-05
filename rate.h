int RATE_THRESHOLD = 50;
void rate(string s, double *res = NULL)
{
    if(titles.lower_bound(s) == titles.end())
    {
        cout << "could not find title " << s << endl;
        return;
    }
    string title = titles.lower_bound(s)->A;
    fast_map<string, vector<pair<double, double> > > likability;
    double sum = 0;
    int cnt = 0;
    for(auto &i: anime_z)
    {
        if(i.B.size()>=RATE_THRESHOLD && i.B.count(title))
        {
            sum += anime[i.A][title];
            cnt++;
        }
    }
    if(res != NULL)
    {
        if(cnt != 0)
            *res = sum / cnt;
        else *res = DEFAULT_RATING;
        return;
    }
    if(cnt == 0)
    {
        cout << "no data" << endl;
        return;
    }
    cout << "data count: " << cnt << endl;
    cout << "real rating for " << title << ": " << sum / cnt << endl;
    if(globalAnimeRatings.count(title))
    {
        cout << "MAL rating: " << globalAnimeRatings[title] << endl;
        cout << "diff: " << pm(sum/cnt - globalAnimeRatings[title]) << endl;
    }
}
void raten(string s)
{
    int numTitles = atoi(s.c_str());
    vector<double> ratings;
    for(int i=0; i<min(numTitles, (int)globalAnimeList.size()); i++)
    {
        double res;
        rate(globalAnimeList[i], &res);
        ratings.push_back(res);
    }
    auto cflags = cout.flags();
    cout.setf(ios_base::left);
    cout << setw(40) << "anime" << setw(8) << "real" << setw(8) << "MAL" << "diff\n";
    for(int i=0; i<60; i++)
        cout << "-";
    cout << "\n";
    for(int i=0; i<ratings.size(); i++)
    {
        double &r = globalAnimeRatings[globalAnimeList[i]];
        cout << setw(40) << cut(globalAnimeList[i], 39) << setw(8) << round(ratings[i], 2) <<
            setw(8) << r << pm(round(ratings[i] - r, 2)) << "\n";
    }
    cout.flush();
    cout.setf(cflags);
}
void rates(string s, bool rev)
{
    if(!anime.count(s))
    {
        cout << "could not find user " << s << endl;
        return;
    }
    vector<pair<pair<double, int>, string> > u;
    for(auto &i: anime_z)
    {
        int cnt;
        double r = correlation(s, i.A, "", &cnt);
        if(cnt >= RATE_THRESHOLD)
        {
            u.emplace_back(mp(r, cnt), i.A);
        }
    }
    sort(u.begin(), u.end());
    if(rev)
        reverse(u.begin(), u.end());
    auto cflags = cout.flags();
    cout.setf(ios_base::left);
    cout << setw(40) << "user" << setw(8) << "r" << "shared\n";
    for(int i=0; i<56; i++)
        cout << "-";
    cout << "\n";
    for(int i=0; i<min(u.size(), (unsigned)listSize); i++)
    {
        cout << setw(40) << cut(u[i].B, 39) << setw(8) << pm(round(u[i].A.A, 3)) << u[i].A.B << "\n";
    }
    cout.flush();
    cout.setf(cflags);
}
void rateu(string s)
{
    if(!anime.count(s))
    {
        cout << "could not find user " << s << endl;
        return;
    }
    vector<pair<double, string> > a;
    for(auto &i: anime[s])
        a.emplace_back(i.B, i.A);
    sort(a.begin(), a.end());
    reverse(a.begin(), a.end());
    auto cflags = cout.flags();
    cout.setf(ios_base::left);
    cout << setw(40) << "anime" << "rating\n";
    for(int i=0; i<48; i++)
        cout << "-";
    cout << "\n";
    for(auto &i: a)
    {
        cout << setw(40) << cut(i.B, 39) << i.A << "\n";
    }
    cout.flush();
    cout.setf(cflags);
}
void setRthreshold(string s)
{
    RATE_THRESHOLD = atoi(s.c_str());
}
