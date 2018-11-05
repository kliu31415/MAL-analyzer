#include "sdl_base.h"
#include <SDL2/SDL_image.h>
#include <queue>
#define MAXS 200
int W, H;
vector<vector<double> > aval;
int par[MAXS], sz[MAXS];
int nval[MAXS];
struct node
{
    int l, r;
    string name;
    double r2;
    int sz;
    node(int _l, int _r, double _r2, int _sz)
    {
        l = _l;
        r = _r;
        r2 = _r2;
        sz = _sz;
    }
    node(string nm)
    {
        name = nm;
        r2 = 1;
        sz = 1;
    }
};
vector<node> nodes;
int getp(int x)
{
    if(x == par[x])
        return x;
    par[x] = getp(par[x]);
    return par[x];
}
void combine(int a, int b)
{
    a = getp(a);
    b = getp(b);
    if(a == b)
        return;
    if(sz[a] < sz[b])
    {
        par[a] = b;
        sz[b] += sz[a];
    }
    else
    {
        par[b] = a;
        sz[a] += sz[b];
    }
}
struct dsu
{
    vector<int> p;
    dsu(int size)
    {
        p.resize(size);
        for(int i=0; i<size; i++)
            p[i] = i;
    }
    int getp(int x)
    {
        if(x == p[x])
            return x;
        p[x] = getp(p[x]);
        return p[x];
    }
    void combine(int a, int b)
    {
        a = getp(a);
        b = getp(b);
        if(a == b)
            return;
        p[a] = b;
    }
};
const int nameLen = 30;
int xoffset, yoffset;
map<string, int> tmap;
map<int, string> rtmap;
map<string, map<string, double> > usedAnime;
map<pair<string, string>, double> affinity;
int getXpos(double r2)
{
    return xoffset + (W-xoffset)*(1 - (r2 + 1)/2);
}
void drawDendrogram(int nodepos, int cnt, double parR2 = -1, int pos = 1)
{
    node *x = &nodes[nodepos];
    x->r2 = pmr(x->r2);
    int textSize = H/(cnt+1);
    int ypos = (pos + x->sz/2.0)*H/(cnt+1);
    drawLine(getXpos(x->r2), ypos, getXpos(parR2), ypos, 200, 0, 0);
    if(x->sz == 1)
    {
        int ypos = pos*H/(cnt+1);
        string name = cut(x->name, nameLen);
        SDL_Texture *t = createText(name, textSize, 0, 0, 0, 255);
        renderCopy(t, (nameLen-name.size())*textSize/2, ypos, name.size()*textSize/2, textSize);
        SDL_DestroyTexture(t);
    }
    else
    {
        int mid = pos + nodes[x->l].sz;
        int ypos1 = (pos + nodes[x->l].sz/2.0)*H/(cnt+1);
        int ypos2 = (mid + nodes[x->r].sz/2.0)*H/(cnt+1);
        int xpos = getXpos(x->r2);
        drawLine(xpos, ypos1, xpos, ypos2, 200, 0, 0);
        drawDendrogram(x->l, cnt, x->r2, pos);
        drawDendrogram(x->r, cnt, x->r2, mid);
    }
}
void initAnimeInfo(int size)
{
    tmap.clear();
    rtmap.clear();
    usedAnime.clear();
    affinity.clear();
    nodes.clear();
    vector<pair<int, string> > popularity;
    for(auto &i: titles)
        popularity.emplace_back(i.B, i.A);
    sort(popularity.begin(), popularity.end());
    set<string> usedTitles;
    for(auto &i: titles)
    {
        if(usedTitles.size()<size && (popularity.size()<=size || i.B >= (popularity.end()-size)->A))
            usedTitles.insert(i.A);
    }
    for(auto &i: anime)
    {
        for(auto &j: i.B)
        {
            if(usedTitles.count(j.A))
                usedAnime[i.A][j.A] = j.B;
        }
    }
    for(auto &i: usedAnime)
        toZ(i.B);
    for(auto i=usedTitles.begin(); i!=usedTitles.end(); i++)
    {
        for(auto j=i; j!=usedTitles.end(); j++)
        {
            vector<pair<double, double> > v;
            for(auto &k: usedAnime)
            {
                if(k.B.count(*i) && k.B.count(*j))
                {
                    v.emplace_back(k.B[*i], k.B[*j]);
                }
            }
            affinity[mp(*i, *j)] = affinity[mp(*j, *i)] = pmr2(correlation(v));
        }
    }
    int cnt = 0;
    for(auto &i: usedTitles)
        tmap[i] = cnt++;
    for(auto &i: tmap)
        rtmap[i.B] = i.A;
    for(int i=0; i<size; i++)
    {
        par[i] = i;
        sz[i] = 1;
        nval[i] = i;
        nodes.emplace_back(rtmap[i]);
    }
    aval.clear();
    aval.resize(size);
    for(int i=0; i<size; i++)
        aval[i].resize(size, 0);
    for(auto &i: affinity)
    {
        aval[tmap[i.A.A]][tmap[i.A.B]] = i.B;
    }
}
void outputDendrogram(SDL_Texture *dendrogram, const char *name)
{
    renderCopy(dendrogram, 0, 0, W, H);
    SDL_Surface *s = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
    SDL_SetRenderTarget(getRenderer(), dendrogram);
    SDL_RenderReadPixels(getRenderer(), NULL, s->format->format, s->pixels, s->pitch);
    SDL_SetRenderTarget(getRenderer(), NULL);
    remove("dendrogram.png");
    IMG_SavePNG(s, "dendrogram.png");
    SDL_FreeSurface(s);
    updateScreen();
}
SDL_Texture *generateDendrogramTexture(int size)
{
    SDL_Texture *t = SDL_CreateTexture(getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W, H);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(getRenderer(), t);
    renderClear(255, 255, 255);
    xoffset = H*nameLen/size/2;
    yoffset = H/(size+1);
    drawDendrogram(nodes.size()-1, size);
    drawLine(xoffset, 0, xoffset, H-1, 0, 0, 0);
    drawLine(0, yoffset, W-1, yoffset);
    int textSize = H/(size+1);
    for(int i=-9; i<=9; i++)
    {
        string s = i>=0? "." + to_string(i): "-." + to_string(-i);
        int xpos = getXpos(i/10.0);
        drawText(s, xpos - textSize*s.size()/4, 0, textSize, 0, 0, 255);
        if(i == 0) //make this line special
            drawLine(xpos, yoffset, xpos, H-1, 0, 0, 255, 255);
        if(abs(i) == 5) //also special
            drawLine(xpos, yoffset, xpos, H-1, 0, 0, 255, 150);
        else drawLine(xpos, yoffset, xpos, H-1, 0, 0, 255, 50);
    }
    SDL_SetRenderTarget(getRenderer(), NULL);
    return t;
}
SDL_Texture *createDendrogram(int size = 100)
{
    initAnimeInfo(size);
    priority_queue<pair<double, pii>, vector<pair<double, pii> > > q;
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            q.push(mp(aval[i][j], mp(i, j)));
    while(!q.empty())
    {
        auto x = q.top();
        q.pop();
        int a = getp(x.B.A);
        int b = getp(x.B.B);
        if(a==b || aval[a][b]!=x.A)
            continue;
        double fa = sz[a] / (double)(sz[a] + sz[b]);
        double fb = sz[b] / (double)(sz[a] + sz[b]);
        for(int i=0; i<size; i++)
        {
            aval[a][i] = aval[b][i] = aval[i][a] = aval[i][b] = fa*aval[a][i] + fb*aval[b][i];
            q.push(mp(aval[a][i], mp(a, i)));
        }
        nodes.emplace_back(nval[a], nval[b], x.A, sz[a] + sz[b]);
        combine(a, b);
        nval[getp(a)] = nodes.size()-1;
    }
    return generateDendrogramTexture(size);
}
void graphData(string s)
{
    int size = atoi(s.c_str());
    sdl_settings::load_config();
    initSDL("Dendrogram");
    W = sdl_settings::WINDOW_W;
    H = sdl_settings::WINDOW_H * max(1.0, size/40.0); //make it bigger so it saves nicer
    int dH = H;
    SDL_Texture *dendrogram = createDendrogram(size);
    outputDendrogram(dendrogram, "Dendrogram.png");
    H = sdl_settings::WINDOW_H;
    int scroll = 0;
    while(true)
    {
        while(SDL_WaitEvent(&input))
        {
            SDL_Rect src;
            switch(input.type)
            {
            case SDL_QUIT:
                SDL_DestroyTexture(dendrogram);
                SDL_Quit();
                return;
            case SDL_MOUSEWHEEL:
                scroll -= input.wheel.y*60;
                scroll = max(0, min(scroll, dH-H));
                src = SDL_Rect{0, scroll, W, H};
                renderCopy(dendrogram, &src, NULL);
                SDL_RenderPresent(getRenderer()); //bug in updateScreen it seems
                break;
            }
        }
    }
}
