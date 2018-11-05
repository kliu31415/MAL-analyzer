void sa_cluster1(string s)
{
    int size = 0, cnt = 0;
    for(int i=0; i<s.size(); i++)
    {
        if(s[i] == ' ')
        {
            size = atoi(s.substr(0, i).c_str());
            cnt = atoi(s.substr(i+1).c_str());
            break;
        }
    }
    if(size<=1 || cnt<=1 || cnt>=size)
        return;
    initAnimeInfo(size);
    vector<int> cur;
    while(cur.size() < cnt)
    {
        int nxt = randuz() % size;
        if(find(cur.begin(), cur.end(), nxt) == cur.end())
            cur.push_back(nxt);
    }
    double curv = 0;
    for(int i=0; i<cur.size(); i++)
        for(int j=i+1; j<cur.size(); j++)
            curv += aval[cur[i]][cur[j]];
    double temp = cnt*(cnt-1)/20.0, mult = 0.9999;
    for(int iter=0; iter<1e5; iter++)
    {
        int r1 = randuz() % cnt;
        int r2 = randuz() % size;
        if(find(cur.begin(), cur.end(), r2) != cur.end()) //can't swap
        {
            iter--;
            continue;
        }
        swap(cur[0], cur[r1]);
        double newv = curv;
        for(int i=1; i<cnt; i++)
        {
            newv -= aval[cur[i]][cur[0]];
            newv += aval[cur[i]][r2];
        }
        if(newv - curv > -temp*randf())
        {
            curv = newv;
            cur[0] = r2;
        }
        temp *= mult;
    }
    curv = 0;
    for(int i=0; i<cur.size(); i++)
        for(int j=i+1; j<cur.size(); j++)
            curv += aval[cur[i]][cur[j]];
    curv /= (cnt * (cnt-1)/2);
    cout << curv;
    for(auto &i: cur)
        cout << " | " << rtmap[i];
    cout << "\n";
}
double try_sagroup1(int size, int cnt, vector<int> *cur)
{
    while(true)
    {
        for(int i=0; i<cnt; i++)
            cur[i].clear();
        for(int i=0; i<size; i++)
            cur[randuz() % cnt].push_back(i);
        bool good = true;
        for(int i=0; i<cnt; i++)
        {
            if(cur[i].size() == 0)
            {
                good = false;
                break;
            }
        }
        if(good)
            break;
    }
    double curv = 0;
    for(int _i=0; _i<cnt; _i++)
    {
        auto &i = cur[_i];
        for(int j=0; j<i.size(); j++)
            for(int k=j+1; k<i.size(); k++)
                curv += aval[i[j]][i[k]];
    }
    double temp = (size/(double)cnt)*((size/(double)cnt)-1)*cnt/20.0, mult = 0.9999;
    for(int iter=0; iter<1e6; iter++)
    {
        int g1 = randuz() % cnt, g2 = randuz() % cnt;
        if(cur[g1].size()==0 || cur[g2].size()==0)
        {
            iter--;
            continue;
        }
        int m1 = randuz() % cur[g1].size();
        int m2 = randuz() % cur[g2].size();
        swap(cur[g1][m1], cur[g1][0]);
        swap(cur[g2][m2], cur[g2][0]);
        double newv = curv;
        for(int i=1; i<cur[g1].size(); i++)
        {
            newv -= aval[cur[g1][i]][cur[g1][0]];
            newv += aval[cur[g1][i]][cur[g2][0]];
        }
        for(int i=1; i<cur[g2].size(); i++)
        {
            newv -= aval[cur[g2][i]][cur[g2][0]];
            newv += aval[cur[g2][i]][cur[g1][0]];
        }
        if(newv - curv > -temp*randf())
        {
            curv = newv;
            swap(cur[g1][0], cur[g2][0]);
        }
        temp *= mult;
    }
    curv = 0;
    int div = 0;
    for(int _i=0; _i<cnt; _i++)
    {
        auto &i = cur[_i];
        for(int j=0; j<i.size(); j++)
            for(int k=j+1; k<i.size(); k++)
                curv += aval[i[j]][i[k]];
        div += i.size() * (i.size()-1) / 2;
    }
    return curv / div;
}
void sa_group1(string s)
{
    int size = 0, cnt = 0;
    for(int i=0; i<s.size(); i++)
    {
        if(s[i] == ' ')
        {
            size = atoi(s.substr(0, i).c_str());
            cnt = atoi(s.substr(i+1).c_str());
            break;
        }
    }
    if(size<=1 || cnt<=1 || cnt>=size)
        return;
    initAnimeInfo(size);
    vector<int> best[cnt], cur[cnt];
    double curv = -1e9;
    for(int i=0; i<5; i++)
    {
        double res = try_sagroup1(size, cnt, cur);
        if(res > curv)
        {
            curv = res;
            for(int j=0; j<cnt; j++)
                best[j] = cur[j];
        }
    }
    int gnum = 0;
    cout << curv << "\n";
    for(auto &i: cur)
    {
        sort(i.begin(), i.end()); //why not
        cout << "#" << ++gnum << ": ";
        curv = 0;
        for(int j=0; j<i.size(); j++)
            for(int k=j+1; k<i.size(); k++)
                curv += aval[i[j]][i[k]];
        int div = i.size() * (i.size()-1) / 2;
        if(div != 0)
            cout << curv/div;
        else cout << "n/a";
        for(auto &j: i)
            cout << " | " << rtmap[j];
        cout << "\n";
    }
}
