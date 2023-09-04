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

    v3b f(h-1, v2b(w-1, vb(4, false)));
    for(int i=0; i<h-1; i++){
        for(int j=0; j<w-1; j++){

        }
    }
    for(int i=0; i<h-1; i++){
        for(int j=0; j<w-1; j++){
            char c; cin >> c;
            
        }
    }
    
    return 0;
}