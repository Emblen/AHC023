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
    
    Solver(){}

    void solve(){

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
        f[i][w-2][1] = 1;
    }
    //北側と南側に柵を張る
    for(int i=0; i<w; i++){
        f[0][i][0] = 1;
        f[h-2][i][2] = 1;
    }
    //西側の出入り口になる柵を排除
    f[i0][0][3] = 0;
    //横方向の水路を張る(h-2ライン*w)
    for(int i=0; i<h-1; i++){
        for(int j=0; j<w; j++){
            char c; input >> c;
            if(c=='1'){
                f[i][j][2] = 1;
            }
        }
    }
    //縦方向の水路を張る(w-2ライン*h)
    for(int i=0; i<w-1; i++){
        for(int j=0; j<h; j++){
            char c; input >> c;
            if(c=='1'){
                f[j][i][1] = 1;
                if(i!=w-2) f[j][i+1][3] = 1;
            }
        }
    }
    int k; input >> k;
    vector<crop> cp(k);
    for(int i=0; i<k; i++) input >> cp[i].s >> cp[i].d;



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


    cout << "OK" << endl;
    return 0;
}