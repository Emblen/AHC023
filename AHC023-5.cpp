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

struct Solver{
    const int t, h, w, i0, k;
    v3b f;
    //出入り口からの距離
    v2i dist_map;
    //それぞれピリオドで作物を育てているかどうか
    v3b crop_exist;
    map<crop, int> cp;
    vector<crop> crop_info;
    Solver(int t, int h, int w, int i0, int k, v3b f, map<crop,int> cp, vector<crop> crop_info)
    : t(t), h(h), w(w), i0(i0), k(k), f(f), cp(cp), crop_info(crop_info), dist_map(h, vi(w, 0)), crop_exist(4, v2b(h, vb(w,0))) {}

    void solve(){
        //各区画の出入り口からの距離とその最長距離
        calc_dist();
        //右側までの最短経路
        vector<vec2> path = path_toright();
        //パスの北側と南側で区画を分割して値を割り当てる
        v2i sp_section = split_section(path);
        //区画ごとにn期作をする
        v2i ans = crop_period(sp_section);
        
        
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
    void calc_dist(){
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
    }

    //出入り口からマップの中心について点対称な柵までの最短パスを求める
    vector<vec2> path_toright(){
        v2b visit(h, vb(w, false));
        vector<vector<vec2>> par(h, vector<vec2>(w, {-1,-1}));
        vector<vec2> dydx = {{-1,0},{0,1},{1,0},{0,-1}};//{N, E, S, W}の順
        v2i dist(h, vi(w, 0));
        vec2 target = {19, 19};
        vec2 goal;

        priority_queue<pair<int, vec2>, vector<pair<int, vec2>>, greater<pair<int, vec2>>> pq;
        pq.push({0,{i0,0}});
        par[i0][0] = {i0, 0};

        while(!pq.empty()){
            int pd = pq.top().first;
            int py = pq.top().second.y;
            int px = pq.top().second.x;
            pq.pop();

            if(visit[py][px]) continue;
            visit[py][px] = true;
            dist[py][px] = pd;

            if(py==target.y && px==target.x){
                goal = {py, px};
                break;
            }

            int dir = -1;
            for(auto d:dydx){
                dir++;
                int ny = py + d.y;
                int nx = px + d.x;
                if(ny<0 || ny>=h || nx<0 || nx>=w || visit[ny][nx] || f[py][px][dir]) continue;
                pq.push({pd+1, {ny, nx}});
                par[ny][nx] = {py, px};
            }
        }
        vector<vec2> path;
        while(!(goal.y==i0 && goal.x==0)){
            path.push_back(goal);
            goal = par[goal.y][goal.x];
        }
        path.push_back(goal);
        reverse(all(path));

        // ofstream distout("dist.txt");
        // for(auto v:path) distout << v.y << " " << v.x << endl;

        return path;
    }
    //区画をいくつかのまとまりに分割する
    v2i split_section(vector<vec2> path){
        v2i sp_section(h, vi(w, -1));
        for(auto v:path) sp_section[v.y][v.x] = 0;
        for(int j=0; j<w; j++){
            for(int i=0; i<h; i++){
                if(sp_section[i][j]==0) break;
                sp_section[i][j] = 1;
            }
        }
        //分割した区画の番号の情報を出力する
        // ofstream distout("dist.txt");
        // for(int i=0; i<h; i++){
        //     for(int j=0; j<w; j++){
        //         distout << sp_section[i][j] << " ";
        //     }
        //     distout << endl;
        // }
        return sp_section;
    }

    v2i crop_period(v2i sp_section){
        //4期作をする
        v2i crop_4period(4);
        v2i crop_2period(2);
        for(int i=0; i<k; i++){
            int s = crop_info[i].s;
            int d = crop_info[i].d;
            if(1<=s && s<=25 && 1<=d && d<=25) crop_4period[0].push_back(i);
            else if(26<=s && s<=50 && 26<=d && d<=50) crop_4period[1].push_back(i);
            else if(51<=s && s<=75 && 51<=d && d<=75) crop_4period[2].push_back(i);
            else if(76<=s && s<=100 && 76<=d && d<=100) crop_4period[3].push_back(i);

            if(d-s>25){
                if(1<=s && s<=50 && 1<=d && d<=50) crop_2period[0].push_back(i);
                if(51<=s && s<=100 && 51<=d && d<=100) crop_2period[1].push_back(i);
            }
        }
        cout << "crop2 size: " << (int)crop_2period[0].size() << " " << (int)crop_2period[1].size() << endl;

        v2i ans;
        int total_score = 0;

        assign_section(ans, total_score, 2, crop_2period, sp_section, -1);
        assign_section(ans, total_score, 4, crop_4period, sp_section, 1);
        cout << total_score << endl;
        return ans;
    }

    //分割した区画の中で距離が大きい順にソートした配列を返す
    vector<vec2> sort_longdist(int val, v2i sp_section){
        vector<vec2> long_dist;
        priority_queue<pair<int, vec2>> pq;

        for(int i=0; i<h; i++){
            for(int j=0; j<w; j++){
                if(sp_section[i][j]==val){
                    pq.push({dist_map[i][j], {i,j}});
                }
            }
        }
        while(!pq.empty()){
            vec2 pos = pq.top().second;
            pq.pop();
            long_dist.push_back(pos);
        }
        return long_dist;
    }

    void assign_section(v2i& ans, int& total_score, int total_period, v2i crop_period, v2i sp_section, int sp_val){
        //グループ番号がsp_valである区画のうち，出入り口からの距離が長い順にソートした配列
        vector<vec2> long_dist = sort_longdist(sp_val, sp_section);

        for(int period=0; period<total_period; period++){
            //栽培期間が長いものから400個とって収穫時期の遅い順にソート
            priority_queue<pair<int, int>> pq_long, pq_late;
            for(int i=0; i<(int)crop_period[period].size(); i++){
                int cpnum = crop_period[period][i];
                int s = crop_info[cpnum].s;
                int d = crop_info[cpnum].d;
                pq_long.push({d-s, cpnum});
            }
            int period_cropnum = min(400, (int)crop_period[period].size());

            for(int i=0; i<period_cropnum; i++){
                int cpnum = pq_long.top().second;
                pq_long.pop();
                int d = crop_info[cpnum].d;
                pq_late.push({d, cpnum});
            }   
            //収穫時期の遅い順に出入り口からの距離が長い区画を割り当てる
            for(auto v:long_dist){
                int cpnum = pq_late.top().second;
                pq_late.pop();
                // cout << v.y << " " << v.x << endl;
                ans.push_back({cpnum+1, v.y, v.x, (100/total_period)*period+1});

                if(total_period==2){
                    crop_exist[period][v.y][v.x] = 1;
                    crop_exist[period+1][v.y][v.x] = 1;
                }
                else crop_exist[period][v.y][v.x] = 1;
                total_score += crop_info[cpnum].d - crop_info[cpnum].s + 1;

                if(pq_late.empty()) break;
            }

            if(total_period==2) continue;
            priority_queue<pair<int, vec2>> pq;
            for(int i=0; i<h; i++){
                for(int j=0; j<w; j++){
                    if(period%2) continue;
                    if(!crop_exist[period][i][j]) pq.push({dist_map[i][j], {i,j}}); 
                }
            }
            vector<vec2> remain_section;
            while(!pq.empty()){
                vec2 pos = pq.top().second;
                pq.pop();
                remain_section.push_back(pos);
            }
            for(auto v:remain_section){
                int cpnum = pq_late.top().second;
                pq_late.pop();
                // cout << v.y << " " << v.x << endl;
                ans.push_back({cpnum+1, v.y, v.x, (100/total_period)*period+1});
                crop_exist[period][v.y][v.x] = 1;
                total_score += crop_info[cpnum].d - crop_info[cpnum].s + 1;

                if(pq_late.empty()) break;
            }
        }   
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

    //20の期間を1ピリオドとするとどれくらい植えられるか
    vi period(4,0);
    int lost = 0;
    for(int i=0; i<k; i++){
        int s = crop_info[i].s;
        int d = crop_info[i].d;
        if(1<=s && s<=25 && 1<=d && d<=25) period[0]++;
        else if(26<=s && s<=50 && 26<=d && d<=50) period[1]++;
        else if(51<=s && s<=75 && 51<=d && d<=75) period[2]++;
        else if(76<=s && s<=100 && 76<=d && d<=100) period[3]++;
        else lost++;
    }
    cout << k << endl;
    for(auto v:period) cout << v << " ";
    cout << endl;
    cout << lost << endl;

    //栽培期間のヒストグラムを出してみたい
    ofstream hist("hist.txt");
    for(int i=0; i<k; i++) hist << crop_info[i].d - crop_info[i].s << endl;
    int cnt = 0; 
    vector<int> half_period(2);
    for(int i=0; i<k; i++){
        int s = crop_info[i].s;
        int d = crop_info[i].d;
        if(d-s < 25) continue;

        if(1<=s && s<=50 && 1<=d && d<=50) half_period[0]++;
        else if(51<=s && s<=100 && 51<=d && d<=100) half_period[1]++;
        else cnt++;
        // hist << crop_info[i].d - crop_info[i].s << endl;
    }
    cout << endl;
    cout << half_period[0] << " " << half_period[1] << endl;
    cout << cnt << endl;
    

// ///////////////////////////////////////////////////////////
    

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