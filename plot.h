void draw_axes(int MAX_RATING = 10)
{
    int W, H;
    SDL_GetWindowSize(getWindow(), &W, &H);
    renderClear(255, 255, 255);
    setColor(0, 0, 0);
    for(int i=0; i<MAX_RATING; i++)
    {
        drawLine(0, H*i/MAX_RATING-1, W, H*i/MAX_RATING-1);
        drawLine(W*i/MAX_RATING-1, 0, W*i/MAX_RATING-1, H);
        if(i != 0)
        {
            drawText(to_str(i), W*i/MAX_RATING - H*0.0125, H*0.95, H*0.05);
            drawText(to_str(i), 0, 0.975*H - H*i/MAX_RATING, H*0.05);
        }
    }
}
void plot(vector<pair<double, double> > data)
{
    sdl_settings::load_config();
    initSDL("Plot");
    int W, H;
    SDL_GetWindowSize(getWindow(), &W, &H);
    cout << "r = " << correlation(data) << endl;
    const static int MAX_RATING = 10;
    draw_axes(MAX_RATING);
    for(auto &i: data)
    {
        fillCircle(W*i.A/MAX_RATING, H - H*i.B/MAX_RATING, W/200);
    }
    updateScreen();
    while(true)
    {
        while(SDL_WaitEvent(&input))
        {
            switch(input.type)
            {
            case SDL_QUIT:
                SDL_DestroyWindow(getWindow());
                SDL_DestroyRenderer(getRenderer());
                return;
            }
        }
    }
}
void plot_discrete(vector<pair<double, double> > data)
{
    sdl_settings::load_config();
    initSDL("Plot");
    int W, H;
    SDL_GetWindowSize(getWindow(), &W, &H);
    cout << "r = " << correlation(data) << endl;
    const static int MAX_RATING = 10;
    draw_axes(MAX_RATING);
    map<pair<double, double>, int> freq;
    for(auto &i: data)
        freq[i]++;
    for(auto &i: freq)
    {
        fillCircle(W*i.A.A/MAX_RATING, H - H*i.A.B/MAX_RATING, W/200 * sqrt(i.B));
    }
    updateScreen();
    while(true)
    {
        while(SDL_WaitEvent(&input))
        {
            switch(input.type)
            {
            case SDL_QUIT:
                SDL_DestroyWindow(getWindow());
                SDL_DestroyRenderer(getRenderer());
                return;
            }
        }
    }
}
void plotr(string s)
{
    if(!anime.count(s))
    {
        cout << "could not find user " << s << endl;
        return;
    }
    vector<pair<double, double> > data;
    for(auto &i: anime[s])
    {
        if(i.B!=0 && globalAnimeRatings.count(i.A))
            data.emplace_back(globalAnimeRatings[i.A], i.B);
    }
    plot(data);
}
void plotu(string s)
{
    vector<string> u = split(s);
    if(u.size() != 2)
    {
        cout << "this function needs two arguments" << endl;
        return;
    }
    if(!anime.count(u[0]))
    {
        cout << "could not find user " << u[0] << endl;
        return;
    }
    if(!anime.count(u[1]))
    {
        cout << "could not find user " << u[1] << endl;
        return;
    }
    vector<pair<double, double> > data;
    for(auto &i: anime[u[0]])
    {
        if(i.B!=0 && anime[u[1]].count(i.A) && anime[u[1]][i.A]!=0)
            data.emplace_back(i.B, anime[u[1]][i.A]);
    }
    cout << "shared = " << data.size() << "\n";
    plot_discrete(data);
}
