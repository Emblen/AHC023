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
    bool operator<(const crop right) const {
        return d == right.d ? s < right.s : d < right.d;
    }
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
    const int t, h, w, i0, k;
    v3b f;
    v2i dist_map;
    vector<vector<vec2>> dist_numvec;
    map<crop, int> cp;
    vector<crop> crop_info;
    Solver(int t, int h, int w, int i0, int k, v3b f, map<crop,int> cp, vector<crop> crop_info)
    : t(t), h(h), w(w), i0(i0), k(k), f(f), cp(cp), crop_info(crop_info), dist_map(h, vi(w, 0)), dist_numvec(100) {}

    void solve(){
        //各区画の出入り口からの距離とその最長距離
        int mx_dist = calc_dist();
        
        //n期作をしよう

        //収穫時期の早い順にキューに入れる
        priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> pq_early;
        for(int i=0; i<k; i++){
            int s = crop_info[i].s;
            int d = crop_info[i].d;
            pq_early.push({d,i});
        }

        //400個ずつ植える。400個の中で収穫時期が最も遅いものを保存する。
        //最初の400個を植える
        v2i ans;
        int tmp_mxd = 0;
        for(int i=0; i<=mx_dist; i++){
            for(int j=0; j<(int)dist_numvec[i].size(); j++){
                int cpnum = pq_early.top().second;
                int d = pq_early.top().first;
                tmp_mxd = max(tmp_mxd, d);
                pq_early.pop();

                int y = dist_numvec[i][j].y;
                int x = dist_numvec[i][j].x;
                ans.push_back({cpnum+1, y, x, 1});
            }
        }
        //出入り口からの距離が小さい順に座標を並べたもの
        vector<vec2> nearly_exit;
        for(int i=0; i<=mx_dist; i++){
            for(int j=0; j<(int)dist_numvec[i].size(); j++){
                nearly_exit.push_back(dist_numvec[i][j]);
            }
        }

        bool pqempty = false;
        int sec_cnt = 0;
        int pre_mxd = tmp_mxd;
        while(!pq_early.empty()){
            int cpnum = pq_early.top().second;
            int d = pq_early.top().first;
            tmp_mxd = max(tmp_mxd, d);
            
            
            if(sec_cnt==400){
                pre_mxd = tmp_mxd;
                sec_cnt = 0;
            }
        }
        
    
        //栽培期間が長いものから400個とって収穫時期の遅い順にソート
        // priority_queue<pair<int, int>> pq_long, pq_late;
        // for(int i=0; i<k; i++){
        //     int s = crop_info[i].s;
        //     int d = crop_info[i].d;
        //     pq_long.push({d-s, i});
        // }
        // for(int i=0; i<400; i++){
        //     int cpnum = pq_long.top().second;
        //     pq_long.pop();
        //     int d = crop_info[cpnum].d;
        //     pq_late.push({d, cpnum});
        // }   
        // //収穫時期の遅い順に出入り口からの距離が長い区画を割り当てる
        // v2i ans;
        // for(int i=mx_dist; i>=0; i--){
        //     for(int j=0; j<(int)dist_numvec[i].size(); j++){
        //         int cpnum = pq_late.top().second;
        //         pq_late.pop();
        //         int y = dist_numvec[i][j].y;
        //         int x = dist_numvec[i][j].x;
        //         ans.push_back({cpnum+1, y, x, 1});
        //     }
        // }
        //答え出力
//Local//////////////////////
        ofstream ansout("ansout.txt");
        ansout << (int)ans.size() << endl;
        for(int i=0; i<(int)ans.size(); i++){
            for(int j=0; j<(int)ans[i].size(); j++){
                ansout << ans[i][j] << " ";
            }
            ansout << endl;
        }
//Submit///////////////////////
        // cout << (int)ans.size() << endl;
        // for(int i=0; i<(int)ans.size(); i++){
        //     for(int j=0; j<(int)ans[i].size(); j++){
        //         cout << ans[i][j] << " ";
        //     }
        //     cout << endl;
        // }
    }

    //各区画の出入り口からの距離をBFSで計算する
    int calc_dist(){
        v2b visit(h, vb(w, false));
        priority_queue<pair<int, vec2>, vector<pair<int, vec2>>, greater<pair<int, vec2>>> pq;
        vector<vec2> dydx = {{-1,0},{0,1},{1,0},{0,-1}};//{N, E, S, W}の順
        pq.push({0,{i0,0}});
        
        //BFSで出入り口からの距離を求める
        while(!pq.empty()){
            int dist = pq.top().first;
            vec2 pos = pq.top().second;
            pq.pop();
            if(visit[pos.y][pos.x]) continue;
            visit[pos.y][pos.x] = true;
            dist_map[pos.y][pos.x] = dist;

            int dir = -1;
            for(auto d:dydx){
                dir++;
                int ny = pos.y + d.y;
                int nx = pos.x + d.x;
                if(ny<0 || ny>=h || nx<0 || nx>=w || visit[ny][nx] || f[pos.y][pos.x][dir]) continue;
                pq.push({dist+1, {ny, nx}});
            }
        }
        ofstream distout("dist.txt");
        for(int i=0; i<h; i++){
            for(int j=0; j<w; j++){
                distout << dist_map[i][j] << " ";
            }
            distout << endl;
        }
        //距離がiの区画を保存する
        int mx_dist = -1;
        for(int i=0; i<h; i++){
            for(int j=0; j<w; j++){
                int num = dist_map[i][j];
                mx_dist = max(mx_dist, num);
                dist_numvec[num].push_back({i,j});
            }
        }
        return mx_dist;
    }
};


int main(){

//Local/////////////////////////////////////////
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
                if(j!=w-2) f[i][j+1][3] = 1;
            }
        }
    }
    int k; input >> k;
    map<crop, int> cp;
    //作物の番号から植えつけ，収穫時期の情報を得る
    vector<crop> crop_info(k);
    for(int i=0; i<k; i++){
        int s, d; input >> s >> d;
        crop_info[i] = {s, d};
        cp.insert({{d, s}, i});
    }

    //map作成に使うデータ
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
///////////////////////////////////////////////////////////
    

//Submit/////////////////////////////////////////////////
    // int t, h, w, i0;
    // cin >> t >> h >> w >> i0;

    // //{N, E, S, W}
    // v3b f(h, v2b(w, vb(4, 0)));
    // //西側と東側に柵を張る
    // for(int i=0; i<h; i++){
    //     f[i][0][3] = 1;
    //     f[i][w-1][1] = 1;
    // }
    // //北側と南側に柵を張る
    // for(int i=0; i<w; i++){
    //     f[0][i][0] = 1;
    //     f[h-1][i][2] = 1;
    // }
    // //西側の出入り口になる柵を排除
    // f[i0][0][3] = 0;

    // //横方向の水路を張る(h-2ライン*w)
    // for(int i=0; i<h-1; i++){
    //     for(int j=0; j<w; j++){
    //         char c; cin >> c;
    //         if(c=='1'){
    //             //(i,j)区画の南側と(i+1,j)区画の北側に水路を敷設
    //             f[i][j][2] = 1;
    //             if(i!=h-2) f[i+1][j][0] = 1;
    //         }
    //     }
    // }
    // //縦方向の水路を張る(w-2ライン*h)
    // for(int i=0; i<h; i++){
    //     for(int j=0; j<w-1; j++){
    //         char c; cin >> c;
    //         if(c=='1'){
    //             f[i][j][1] = 1;
    //             if(j!=w-2) f[i][j+1][3] = 1;
    //         }
    //     }
    // }
    // int k; cin >> k;
    // map<crop, int> cp;
    // //作物の番号から植えつけ，収穫時期の情報を得る
    // vector<crop> crop_info(k);
    // for(int i=0; i<k; i++){
    //     int s, d; cin >> s >> d;
    //     crop_info[i] = {s, d};
    //     cp.insert({{d, s}, i});
    // }

/////////////////////////////////////////////

    Solver solver(t, h, w, i0, k, f, cp, crop_info);
    solver.solve();

    // cout << "Completed Successfully" << endl;
    return 0;
}