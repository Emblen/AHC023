#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <queue>
#include <map>
#include <fstream>
#define INF 1e9
#define vi vector<int>
#define vb vector<bool>
#define vll vector<long long>
#define v2i vector<vector<int>>
#define v2b vector<vector<bool>>
#define v2ll vector<vector<long long>>
#define v3b vector<vector<vector<bool>>>
#define all(x) x.begin(),x.end()
using namespace std;
using ll = long long;

struct crop{
    int s, d;
};

struct vec2{
    int y, x; 
    bool operator<(const vec2 right) const {
        return y == right.y ? x < right.x : y < right.y;
    }
};


struct Feild{

};

struct Solver{
    const int t, h, w, i0;
    v3b f;
    map<crop, int> cp;
    Solver(int t, int h, int w, int i0, v3b f, map<crop,int> cp)
    : t(t), h(h), w(w), i0(i0), f(f), cp(cp) {}

    void solve(){
        ofstream ansout("ans.txt");
        v2i ans;

        //作物を植える82の区画に0~81の番号をふる
        vector<vec2> section(18*4);
        for(int i=0; i<18; i++) section[i] = {0, i+1};
        for(int i=0; i<18; i++) section[i+18] = {i+1, 20};
        for(int i=0; i<18; i++) section[i+18*2] = {20, 18-i};
        for(int i=0; i<18; i++) section[i+18*3] = {i-18, 0};

        //最初に81個の作物を植える．出入り口に面する区画（i0,0)には植えない
        for(int i=0; i<(int)section.size(); i++){

        }
        
    }
};


int main(){
    ifstream input("input.txt");

    int t, h, w, i0;
    input >> t >> h >> w >> i0;

    //{N, E, S, W}
    v3b f(h, v2b(w, vb(4, 0)));
    //西側と東側に柵を張る
    for(int i=0; i<h; i++){
        f[i][0][3] = 1;
        f[i][w-1][1] = 1;
    }
    //北側と南側に柵を張る
    for(int i=0; i<w; i++){
        f[0][i][0] = 1;
        f[h-1][i][2] = 1;
    }
    //西側の出入り口になる柵を排除
    f[i0][0][3] = 0;
    //横方向の水路を張る(h-2ライン*w)
    for(int i=0; i<h-1; i++){
        for(int j=0; j<w; j++){
            char c; input >> c;
            if(c=='1'){
                //(i,j)区画の南側と(i+1,j)区画の北側に水路を敷設
                f[i][j][2] = 1;
                if(i!=h-2) f[i+1][j][0] = 1;
            }
        }
    }
    //縦方向の水路を張る(w-2ライン*h)
    for(int i=0; i<h; i++){
        for(int j=0; j<w-1; j++){
            char c; input >> c;
            if(c=='1'){
                f[i][j][1] = 1;
                if(i!=w-2) f[i][j+1][3] = 1;
            }
        }
    }
    int k; input >> k;
    map<crop, int> cp;
    //作物の番号から植えつけ，収穫時期の情報を得る
    vector<crop> crop_info(k);
    for(int i=0; i<k; i++){
        int s, d; cin >> s >> d;
        crop_info[i] = {s, d};
        cp.insert({{d, s}, i});
    }
    sort(all(cp));
    
    // int t, h, w, i0;
    // cin >> t >> h >> w >> i0;

    // //{N, E, S, W}
    // v3b f(h, v2b(w, vb(4, 0)));
    // for(int i=0; i<h; i++){
    //     f[i][0][3] = 1;
    //     f[i][w-2][1] = 1;
    // }
    // for(int i=0; i<w; i++){
    //     f[0][i][0] = 1;
    //     f[h-2][i][2] = 1;
    // }
    // for(int i=0; i<h-1; i++){
    //     for(int j=0; j<w; j++){
    //         char c; cin >> c;
    //         if(c=='1'){
    //             f[i][j][2] = 1;
    //             if(i!=h-2) f[i+1][j][0] = 1;
    //         }
    //     }
    // }
    // for(int i=0; i<w-1; i++){
    //     for(int j=0; j<h; j++){
    //         char c; cin >> c;
    //         if(c=='1'){
    //             f[j][i][1] = 1;
    //             if(i!=w-2) f[j][i+1][3] = 1;
    //         }
    //     }
    // }
    // int k; cin >> k;
    // vector<crop> cp(k);
    // for(int i=0; i<k; i++) cin >> cp[i].s >> cp[i].d;


    ofstream output("mapdata.txt");
    output << t << " " << h << " " << w << " " << i0 << endl;

    for(int i=0; i<h-1; i++){
        for(int j=0; j<w; j++){
            if(f[i][j][2]) output << 1;
            else output << 0;
        }
        output << endl;
    }
    for(int i=0; i<h; i++){
        for(int j=0; j<w-1; j++){
            if(f[i][j][1]) output << 1;
            else output << 0;
        }
        output << endl;
    }
    output << endl;



    Solver solver(t, h, w, i0, f, cp);
    solver.solve();

    // cout << "OK" << endl;
    return 0;
}