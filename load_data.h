void load_users()
{
    ifstream fin("data/users.txt");
    if(fin.fail())
        cout << "failed to load users\n";
    string s;
    set<string> uset;
    while(getline(fin, s))
    {
        if(!uset.count(s))
        {
            uset.insert(s);
            users.push_back(s);
        }
    }
    cout << users.size() << " unique users in list\n";
    cout.flush();
    fin.close();
    for(auto &i: users)
    {
        fin.open(("data/" + i));
        if(!fin.fail())
        {
            auto &cur = anime[i];
            getline(fin, s); //get the first line, which is the time
            while(getline(fin, s))
            {
                for(int j=s.size()-1; j>=0; j--)
                {
                    if(s[j] == ' ')
                    {
                        cur[s.substr(0, j)] = atoi(s.substr(j+1).c_str());
                        break;
                    }
                }
            }
            for(auto &j: cur)
                titles[j.A]++;
        }
        fin.close();
    }
    anime_z = anime;
    for(auto &i: anime_z)
    {
        toZ(i.B);
        auto &z_vec = anime_z_vec[i.A];
        for(auto &j: i.B)
            z_vec.emplace_back(j.A, j.B);
    }
    cout << anime.size() << " users successfully loaded\n";
    cout << titles.size() << " anime found\n";
    cout.flush();
}
void removeUnicode(string &x)
{
    static vector<pair<string, string> > seq;
    static bool init = false;
    if(!init)
    {
        init = true;
        seq.emplace_back("/", "\\/");
        seq.emplace_back("\u2605", "\\u2605");
        seq.emplace_back("\u03a8", "\\u03a8");
        seq.emplace_back("\u25b3", "\\u25b3");
        seq.emplace_back("\u00e4", "\\u00e4");
        seq.emplace_back("&amp;amp;", "&");
        seq.emplace_back("&amp;#039;", "\'");
    }
    for(auto &i: seq)
    {
        bool hasMore = true;
        unsigned prv = 0;
        while(hasMore)
        {
            hasMore = false;
            auto pos = x.find(i.A, prv);
            if(pos != string::npos)
            {
                x.erase(pos, i.A.size());
                x.insert(pos, i.B);
                hasMore = true;
                prv = pos + i.B.size();
            }
        }
    }
}
void load_anime()
{
    ifstream fin("anime_ratings.txt", ios::binary);
    if(fin.fail())
        cout << "failed to load anime ratings\n";
    string line;
    getline(fin, line);
    getline(fin, line);
    while(getline(fin, line))
    {
        vector<string> dat = split(line, '\t');
        if(dat.size()>2 && !globalAnimeRatings.count(dat[1]))
        {
            removeUnicode(dat[1]);
            globalAnimeRatings[dat[1]] = atof(dat[2].c_str());
            globalAnimeList.push_back(dat[1]);
        }
    }
    cout << globalAnimeRatings.size() << " anime in global ratings list" << endl;
}
