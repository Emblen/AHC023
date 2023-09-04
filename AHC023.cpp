#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <queue>
#include <map>
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
    int t, h, w, i0;
    cin >> t >> h >> w >> i0;

    //{N, E, S, W}
    v3b f(h-1, v2b(w-1, vb(4, 0)));
    for(int i=0; i<h-1; i++){
        f[i][0][3] = 1;
        f[i][w-2][1] = 1;
    }
    for(int i=0; i<w-1; i++){
        f[0][i][0] = 1;
        f[h-2][i][2] = 1;
    }
    for(int i=0; i<h-1; i++){
        for(int j=0; j<w-1; j++){
            char c; cin >> c;
            if(c=='1'){
                f[i][j][2] = 1;
                if(i!=h-1) f[i+1][j][0] = 1;
            }
        }
    }
    for(int i=0; i<w-1; i++){
        for(int j=0; j<h-1; j++){
            char c; cin >> c;
            if(c=='1'){
                f[j][i][1] = 1;
                if(i!=h-1) f[j][i+1][3] = 1;
            }
        }
    }
    int k; cin >> k;
    vector<crop> cp(k);
    for(int i=0; i<k; i++) cin >> cp[i].s >> cp[i].d;

    
    
    return 0;
}