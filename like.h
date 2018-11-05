const int AFFINITY_WEIGHT = 3;
const int DEFAULT_RATING = 6;
void like(string s, double *res = NULL)
{
    vector<string> dat = split(s);
    for(int i=2; i<dat.size(); i++)
        dat[1] += " " + dat[i];
    dat.resize(2);
    if(!anime_z.count(dat[0]))
    {
        cout << "could not find user " << dat[0] << endl;
        return;
    }
    if(titles.lower_bound(dat[1]) == titles.end())
    {
        cout << "could not find anime " << dat[1] << endl;
        return;
    }
    string title = titles.lower_bound(dat[1])->A;
    fast_map<string, vector<pair<double, double> > > likability;
    /*ASIZE_MAX = 1000;
    ASIZE_MIN = 5;
    for(auto &i: anime_z)
    {
        if(i.A!=dat[0] && i.B.size()>ASIZE_MIN && i.B.size()<ASIZE_MAX && i.B.count(title))
        {
            double rating = i.B[title];
            for(auto &j: anime_z_vec[i.A])
            {
                if(j.A != title)
                    likability[j.A].emplace_back(rating, j.B);
            }
        }
    }
    double sum = 0, r2sum = 0;
    int cnt = 0;
    for(auto &i: likability)
    {
        if(i.A!=title && anime_z[dat[0]].count(i.A) && anime_z[dat[0]][i.A]!=0)
        {
            double r2 = pmr2(correlation(i.B));
            r2sum += fabs(r2);
            sum += anime_z[dat[0]][i.A] * r2;
            cnt++;
        }
    }
    double sum = 0, r2sum = 0;
    int cnt = 0;
    for(auto &i: likability)
    {
        if(i.A!=title && anime_z[dat[0]].count(i.A) && anime_z[dat[0]][i.A]!=0)
        {
            double r2 = pmr2(correlation(i.B));
            r2sum += fabs(r2);
            sum += anime_z[dat[0]][i.A] * r2;
            cnt++;
        }
    }*/
    double sum = 0, r2sum = 0;
    int cnt = 0;
    ASIZE_MAX = 10000;
    ASIZE_MIN = 5;
    for(auto &i: anime_z)
    {
        if(i.A!=dat[0] && i.B.size()>ASIZE_MIN && i.B.size()<ASIZE_MAX && i.B.count(title))
        {
            int same;
            double r = correlation(dat[0], i.A, title, &same);
            double weight = r*r*r*sqrt(same);
            sum += weight * i.B[title];
            r2sum += fabs(weight);
            cnt++;
        }
    }
    if(res != NULL)
    {
        if(r2sum != 0)
            *res = sum / r2sum;
        else *res = DEFAULT_RATING;
        return;
    }
    if(r2sum == 0)
    {
        cout << "r^2 sum is 0 (no data)" << endl;
        return;
    }
    cout << "data count: " << cnt << endl;
    cout << "expected affinity for " << title << ": " << pm(sum / r2sum) << endl;
    if(globalAnimeRatings.count(title))
        cout << "expected rating: " << rdev(anime[dat[0]]) + globalAnimeRatings[title] + AFFINITY_WEIGHT * (sum / r2sum) << endl;
}
void liken(string s)
{
    vector<string> dat = split(s);
    if(!anime_z.count(dat[0]))
    {
        cout << "could not find user " << dat[0] << endl;
        return;
    }
    int n = atoi(dat[1].c_str());
    n = min(n, (int)globalAnimeList.size());
    vector<pair<string, double> > affinity;
    for(int i=0; i<n; i++)
    {
        showProgress(i, n);
        double temp;
        like(dat[0] + " " + globalAnimeList[i], &temp);
        affinity.emplace_back(globalAnimeList[i], rdev(anime[dat[0]]) + globalAnimeRatings[globalAnimeList[i]] + temp * AFFINITY_WEIGHT);
    }
    showProgress(n, n);
    cout << "\n";
    sort(affinity.begin(), affinity.end(), [](pair<string, double> &a, pair<string, double> &b)->bool{return a.B > b.B;});

    auto cflags = cout.flags();
    cout.setf(ios_base::left);
    cout << setw(40) << "anime" << "estimated\n";
    for(int i=0; i<56; i++)
        cout << "-";
    cout << "\n";
    for(int i=0; i<affinity.size(); i++)
    {
        cout << setw(40) << cut(affinity[i].A, 39) << setw(16) << affinity[i].B << "\n";
    }
    cout.flush();
    cout.setf(cflags);
}
void liker(string s)
{
    if(!anime_z.count(s))
    {
        cout << "could not find user " << s << endl;
        return;
    }
    vector<pair<string, double> > affinity;
    vector<pair<double, double> > coords;
    int cnt = 0;
    for(auto &i: anime[s])
    {
        showProgress(cnt++, anime[s].size());
        if(i.B == 0)
            continue;
        double temp;
        like(s + " " + i.A, &temp);
        affinity.emplace_back(i.A, temp);
        if(globalAnimeRatings.count(i.A))
            coords.emplace_back(rdev(anime[s]) + globalAnimeRatings[i.A] + temp * AFFINITY_WEIGHT, i.B);
    }
    showProgress(cnt, cnt);
    cout << "\n";
    sort(affinity.begin(), affinity.end(), [](pair<string, double> &a, pair<string, double> &b)->bool{return a.B > b.B;});

    auto cflags = cout.flags();
    cout.setf(ios_base::left);

    cout << setw(40) << "anime" << setw(16) << "estimated" << "score\n";
    for(int i=0; i<61; i++)
        cout << "-";
    cout << "\n";
    for(int i=0; i<affinity.size(); i++)
    {
        if(!globalAnimeRatings.count(affinity[i].A))
            cout << setw(40) << cut(affinity[i].A, 39) << setw(16) << "(n/a)" + to_str(affinity[i].B) << anime[s][affinity[i].A] << "\n";
        else cout << setw(40) << cut(affinity[i].A, 39) << setw(16) <<
            rdev(anime[s]) + globalAnimeRatings[affinity[i].A] + affinity[i].B * AFFINITY_WEIGHT << anime[s][affinity[i].A] << "\n";
    }
    cout.flush();
    /*double M, B;
    linearRegression(coords, &M, &B);
    cout << M << " " << B << endl;
    for(auto &i: coords)
    {
        i.A *= M;
        i.A += B;
    }*/
    for(auto &i: coords)
        i.A = min(max(0.0, i.A), 10.0);
    plot(coords);
    cout.setf(cflags);
}
