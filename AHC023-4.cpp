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

string inputfile = "";
string outputfile = "";

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
    v2i dist_map;
    vector<vector<vec2>> dist_numvec;
    v2i now_crop;
    map<crop, int> cp;
    vector<crop> crop_info;
    Solver(int t, int h, int w, int i0, int k, v3b f, map<crop,int> cp, vector<crop> crop_info)
    : t(t), h(h), w(w), i0(i0), k(k), f(f), cp(cp), crop_info(crop_info), dist_map(h, vi(w, 0)), dist_numvec(100), now_crop(h, vi(w,-1)) {}

    void solve(){
        //各区画の出入り口からの距離とその最長距離
        int mx_dist = calc_dist();
        //右側までの最短経路
        path_toright();
        set<vec2> safe_section = calc_safe_section();

        //出入り口からの距離が近くて安全な区画
        vector<vec2> short_safe_section;
        for(int i=0; i<100; i++){
            for(int j=0; j<(int)dist_numvec[i].size(); j++){
                vec2 a = dist_numvec[i][j];
                if(safe_section.count(a)){
                    short_safe_section.push_back(a);
                }
            }
        }
    
        //4期作をするs
        v2i crop_4period(4);
        for(int i=0; i<k; i++){
            int s = crop_info[i].s;
            int d = crop_info[i].d;
            if(1<=s && s<=25 && 1<=d && d<=25) crop_4period[0].push_back(i);
            else if(26<=s && s<=50 && 26<=d && d<=50) crop_4period[1].push_back(i);
            else if(51<=s && s<=75 && 51<=d && d<=75) crop_4period[2].push_back(i);
            else if(76<=s && s<=100 && 76<=d && d<=100) crop_4period[3].push_back(i);
        }

        v2i ans;

        for(int period=0; period<4; period++){
            //栽培期間が長い順にキューに入れる
            priority_queue<pair<int, int>> pq_long, pq_late;
            for(int i=0; i<(int)crop_4period[period].size(); i++){
                int cpnum = crop_4period[period][i];
                int s = crop_info[cpnum].s;
                int d = crop_info[cpnum].d;
                pq_long.push({d-s, cpnum});
            }
            // 最大400個とって収穫時期の遅い順にソート
            int period_cropnum = min(400, (int)crop_4period[period].size());

            for(int i=0; i<period_cropnum; i++){
                int cpnum = pq_long.top().second;
                pq_long.pop();
                int d = crop_info[cpnum].d;
                pq_late.push({d, cpnum});
            }   
            int cnt = 0;
            //収穫時期の遅い順に出入り口からの距離が長い区画を割り当てる
            for(int i=mx_dist; i>=0; i--){
                for(int j=0; j<(int)dist_numvec[i].size(); j++){
                    int cpnum = pq_late.top().second;
                    pq_late.pop();
                    int y = dist_numvec[i][j].y;
                    int x = dist_numvec[i][j].x;
                    ans.push_back({cpnum+1, y, x, period*25+1});
                    now_crop[y][x] = cpnum;
                    cnt++;
                    if(cnt==period_cropnum) break;
                }
                if(cnt==period_cropnum) break;
            }   

            //追加の栽培
            if(pq_long.empty() || (int)safe_section.size()==0) continue;
            //残っている作物を収穫時期が早い順に並び替える
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq_early;
            while(!pq_long.empty()){
                int cpnum = pq_long.top().second;
                pq_long.pop();
                int d = crop_info[cpnum].d;
                pq_early.push({d, cpnum});
            }
            //安全な区画に，植える時期が現在植えている作物の収穫時期+2以内であれば収穫時期+2に植える，そうでなければスキップ
            int i = 0;
            int siz = (int)short_safe_section.size();
            while(!pq_early.empty()){
                int cpnum = pq_early.top().second;
                int s = crop_info[cpnum].s;
                pq_early.pop();
                //安全な区画の座標
                vec2 pos = short_safe_section[i];
                //現在植えている作物の収穫時期
                int pre_d = crop_info[now_crop[pos.y][pos.x]].d;

                if(pre_d+3<=s && pre_d+3<100){
                    ans.push_back({cpnum+1, pos.y, pos.x, pre_d+3});
                    now_crop[pos.y][pos.x] = cpnum;
                    i = (i+1)%siz;
                } 
            }
        }
        
        //答え出力
//Local//////////////////////
        // ofstream ansout(outputfile);
        // ansout << (int)ans.size() << endl;
        // for(int i=0; i<(int)ans.size(); i++){
        //     for(int j=0; j<(int)ans[i].size(); j++){
        //         ansout << ans[i][j] << " ";
        //     }
        //     ansout << endl;
        // }
//Submit///////////////////////
        cout << (int)ans.size() << endl;
        for(int i=0; i<(int)ans.size(); i++){
            for(int j=0; j<(int)ans[i].size(); j++){
                cout << ans[i][j] << " ";
            }
            cout << endl;
        }
    }
    //周りに水路がない区画
    set<vec2> calc_safe_section(){
        vector<vec2> dydx = {{-1,0}, {0,1}, {1,0}, {0,-1}}; //{N, E, S, W}
        set<vec2> issafe;

        for(int i=0; i<h; i++){
            for(int j=0; j<w; j++){
                bool flag = true;
                //区画(i,j)が水路や柵に接していればもちろんだめ
                for(int d=0; d<4; d++) if(f[i][j][d]) flag = false;

                for(int d=0; d<(int)dydx.size(); d++){
                    if(!flag) break;
                    int y = i + dydx[d].y;
                    int x = j + dydx[d].x;
                    if(y<0 || y>=h || x<0 || x>=w) continue;
                    if(d%2 && (f[y][x][0] || f[y][x][2])) flag = false;
                    else if (!(d%2) && (f[y][x][1] || f[y][x][3])) flag = false;
                }
                if(flag) issafe.insert({i,j});
            }
        }
        v2i safe_pos(h, vi(w,0));
    
        // ofstream distout("dist.txt");
        // for(int i=0; i<h; i++){
        //     for(int j=0; j<w; j++){
        //         distout << safe_pos[i][j] << " ";
        //     }
        //     distout << endl;
        // }
        return issafe;

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
        // ofstream distout("dist.txt");
        // for(int i=0; i<h; i++){
        //     for(int j=0; j<w; j++){
        //         distout << dist_map[i][j] << " ";
        //     }
        //     distout << endl;
        // }
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

    //出入り口からマップの中心について点対称な柵までの最短パスを求める
    vector<vec2> path_toright(){
        v2b visit(h, vb(w, false));
        vector<vector<vec2>> par(h, vector<vec2>(w, {-1,-1}));
        vector<vec2> dydx = {{-1,0},{0,1},{1,0},{0,-1}};//{N, E, S, W}の順
        v2i dist(h, vi(w, 0));
        vec2 target = {19-i0, 19};
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
};

vector<char> fill0(int n){
    vector<char> s(4,'0');
    s[0] = n%10+'0';
    s[1] = n/10+'0';
    cout << s[0] <<" " << s[1] << endl;
    return s;
}


int main(){
    // for(int testcase=0; testcase<100; testcase++){
    //     vector<char> s = fill0(testcase);
    //     string num;
    //     for(int z=0; z<4; z++) num.push_back(s[3-z]);
    //     inputfile = "in/"+num+".txt";
    //     outputfile = "out/"+num+".txt";
    //     cout << inputfile << " " << outputfile << endl;

    //     //Local/////////////////////////////////////////
    //     ifstream input(inputfile);
    //     int t, h, w, i0;
    //     input >> t >> h >> w >> i0;

    //     //{N, E, S, W}
    //     v3b f(h, v2b(w, vb(4, 0)));
    //     //西側と東側に柵を張る
    //     for(int i=0; i<h; i++){
    //         f[i][0][3] = 1;
    //         f[i][w-1][1] = 1;
    //     }
    //     //北側と南側に柵を張る
    //     for(int i=0; i<w; i++){
    //         f[0][i][0] = 1;
    //         f[h-1][i][2] = 1;
    //     }
    //     //西側の出入り口になる柵を排除
    //     f[i0][0][3] = 0;

    //     //横方向の水路を張る(h-2ライン*w)
    //     for(int i=0; i<h-1; i++){
    //         for(int j=0; j<w; j++){
    //             char c; input >> c;
    //             if(c=='1'){
    //                 //(i,j)区画の南側と(i+1,j)区画の北側に水路を敷設
    //                 f[i][j][2] = 1;
    //                 if(i!=h-2) f[i+1][j][0] = 1;
    //             }
    //         }
    //     }
    //     //縦方向の水路を張る(w-2ライン*h)
    //     for(int i=0; i<h; i++){
    //         for(int j=0; j<w-1; j++){
    //             char c; input >> c;
    //             if(c=='1'){
    //                 f[i][j][1] = 1;
    //                 if(j!=w-2) f[i][j+1][3] = 1;
    //             }
    //         }
    //     }
    //     int k; input >> k;
    //     map<crop, int> cp;
    //     //作物の番号から植えつけ，収穫時期の情報を得る
    //     vector<crop> crop_info(k);
    //     for(int i=0; i<k; i++){
    //         int s, d; input >> s >> d;
    //         crop_info[i] = {s, d};
    //         cp.insert({{d, s}, i});
    //     }

    //     //map作成に使うデータ
    //     ofstream output("mapdata.txt");
    //     output << t << " " << h << " " << w << " " << i0 << endl;

    //     for(int i=0; i<h-1; i++){
    //         for(int j=0; j<w; j++){
    //             if(f[i][j][2]) output << 1;
    //             else output << 0;
    //         }
    //         output << endl;
    //     }
    //     for(int i=0; i<h; i++){
    //         for(int j=0; j<w-1; j++){
    //             if(f[i][j][1]) output << 1;
    //             else output << 0;
    //         }
    //         output << endl;
    //     }
    //     output << endl;

    //     //20の期間を1ピリオドとするとどれくらい植えられるか
    //     vi period(4,0);
    //     int lost = 0;
    //     for(int i=0; i<k; i++){
    //         int s = crop_info[i].s;
    //         int d = crop_info[i].d;
    //         if(1<=s && s<=25 && 1<=d && d<=25) period[0]++;
    //         else if(26<=s && s<=50 && 26<=d && d<=50) period[1]++;
    //         else if(51<=s && s<=75 && 51<=d && d<=75) period[2]++;
    //         else if(76<=s && s<=100 && 76<=d && d<=100) period[3]++;
    //         else lost++;
    //     }
    //     cout << k << endl;
    //     for(auto v:period) cout << v << " ";
    //     cout << endl;
    //     cout << lost << endl;

    //     //栽培期間のヒストグラムを出してみたい
    //     ofstream hist("hist.txt");
    //     for(int i=0; i<k; i++) if(crop_info[i].d - crop_info[i].s <= 5) hist << crop_info[i].s << endl;
    //     int cnt = 0; 
    //     vector<int> half_period(2);
    //     for(int i=0; i<k; i++){
    //         int s = crop_info[i].s;
    //         int d = crop_info[i].d;
    //         if(d-s < 25) continue;

    //         if(1<=s && s<=50 && 1<=d && d<=50) half_period[0]++;
    //         else if(51<=s && s<=100 && 51<=d && d<=100) half_period[1]++;
    //         else cnt++;
    //         // hist << crop_info[i].d - crop_info[i].s << endl;
    //     }
    //     cout << endl;
    //     cout << half_period[0] << " " << half_period[1] << endl;
    //     cout << cnt << endl;
        
    //     Solver solver(t, h, w, i0, k, f, cp, crop_info);
    //     solver.solve();
// ///////////////////////////////////////////////////////////
    
    // }



//Submit/////////////////////////////////////////////////
    int t, h, w, i0;
    cin >> t >> h >> w >> i0;

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
            char c; cin >> c;
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
            char c; cin >> c;
            if(c=='1'){
                f[i][j][1] = 1;
                if(j!=w-2) f[i][j+1][3] = 1;
            }
        }
    }
    int k; cin >> k;
    map<crop, int> cp;
    //作物の番号から植えつけ，収穫時期の情報を得る
    vector<crop> crop_info(k);
    for(int i=0; i<k; i++){
        int s, d; cin >> s >> d;
        crop_info[i] = {s, d};
        cp.insert({{d, s}, i});
    }

///////////////////////////////////////////

    Solver solver(t, h, w, i0, k, f, cp, crop_info);
    solver.solve();

    // cout << "Completed Successfully" << endl;
    return 0;
}