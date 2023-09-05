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
    map<crop, int> cp;
    vector<crop> crop_info;
    Solver(int t, int h, int w, int i0, int k, v3b f, map<crop,int> cp, vector<crop> crop_info)
    : t(t), h(h), w(w), i0(i0), k(k), f(f), cp(cp), crop_info(crop_info) {}

    void solve(){
        //{作物番号,植える区画y,植える区画x，植える時期}
        v2i ans;

        //作物を植える82の区画に0~81の番号をふる
        vector<vec2> section(18*4);
        for(int i=0; i<18; i++) section[i] = {0, i+1};
        for(int i=0; i<18; i++) section[i+18] = {i+1, 19};
        for(int i=0; i<18; i++) section[i+18*2] = {19, 18-i};
        for(int i=0; i<18; i++) section[i+18*3] = {18-i, 0};

        //禁止マス(i0=0のときに注意)
        v2b imp_sec(h, vb(w, false));
        if(i0==0) imp_sec[0][1] = true;
        else imp_sec[i0][0] = true;
        
        for(int i=0; i<h; i++){
            if(f[i][0][1]) imp_sec[i][0] = true;
            if(f[i][w-1][3]) imp_sec[i][w-1] = true;
        }
        for(int i=0; i<w; i++){
            if(f[0][i][2]) imp_sec[0][i] = true;
            if(f[h-1][i][0]) imp_sec[h-1][i] = true;
        }

        //それぞれの区画にどの番号の作物を植えているか
        vi nowcrop(18*4);

        //収穫時期が早い順にpriority
        //<収穫時期，作物番号>
        priority_queue<pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>>> pq; 
        for(int i=0; i<k; i++){
            int hv = crop_info[i].d;
            pq.push({hv, i});
        }

        //最初に71個の作物を植える．出入り口に面する区画（i0,0)には植えない
        //いままでにいくつ作物を植えたか
        int cp_cnt = 0;
        for(int i=0; i<(int)section.size(); i++){
            if(imp_sec[section[i].y][section[i].x]) continue;
            //作物の番号を入手する
            int cpnum = pq.top().second;
            pq.pop();
            //i区画にcpnumを植えたという情報
            nowcrop[i] = cpnum;
            //植える作物を追加
            ans.push_back({cpnum+1, section[i].y, section[i].x, 1});
            cp_cnt++;
        }
        // cout << "initial planting OK" << endl;

        //収穫後，残りの作物を植えられるだけ植える
        int sec_num = 0;
        for(int i=cp_cnt; i<(int)crop_info.size(); i++){
            // cout << sec_num << " ";
            if(imp_sec[section[sec_num].y][section[sec_num].x]){
                // cout << sec_num << endl;
                sec_num = (sec_num+1)%72;
                continue;
            }
            //sec_numに植えられている作物の番号
            int cpold = nowcrop[sec_num];
            //その作物の収穫時期
            int hv = crop_info[cpold].d;
            //これから植えようとする作物の植え付け時期と番号
            int cpnew = pq.top().second;
            pq.pop();
            int pl = crop_info[cpnew].s;

            //i番目の作物の植え付け時期より収穫時期が大きければi番目を植える．そうでなければその作物をスキップ
            if(pl > hv){
                ans.push_back({cpnew+1, section[sec_num].y, section[sec_num].x, hv+1});
                nowcrop[sec_num] = cpnew;
                cp_cnt++;
                sec_num = (sec_num+1)%72;
                //植えている作物のデータを変える
            }
        }

//Submit/////////////////////////////////
        cout << (int)ans.size() << endl;
        for(int i=0; i<(int)ans.size(); i++){
            for(int j=0; j<(int)ans[i].size(); j++){
                cout << ans[i][j] << " ";
            }
            cout << endl;
        }
//Local/////////////////////////////////////////
        // ofstream ansout("ans.txt");

        // ansout << (int)ans.size() << endl;
        // for(int i=0; i<(int)ans.size(); i++){
        //     for(int j=0; j<(int)ans[i].size(); j++){
        //         ansout << ans[i][j] << " ";
        //     }
        //     ansout << endl;
        // }
    }
};


int main(){

//Local/////////////////////////////////////////
    // ifstream input("input.txt");
    // int t, h, w, i0;
    // input >> t >> h >> w >> i0;

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
    //         char c; input >> c;
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
    //         char c; input >> c;
    //         if(c=='1'){
    //             f[i][j][1] = 1;
    //             if(i!=w-2) f[i][j+1][3] = 1;
    //         }
    //     }
    // }
    // int k; input >> k;
    // map<crop, int> cp;
    // //作物の番号から植えつけ，収穫時期の情報を得る
    // vector<crop> crop_info(k);
    // for(int i=0; i<k; i++){
    //     int s, d; input >> s >> d;
    //     crop_info[i] = {s, d};
    //     cp.insert({{d, s}, i});
    // }

    // //map作成に使うデータ
    // ofstream output("mapdata.txt");
    // output << t << " " << h << " " << w << " " << i0 << endl;

    // for(int i=0; i<h-1; i++){
    //     for(int j=0; j<w; j++){
    //         if(f[i][j][2]) output << 1;
    //         else output << 0;
    //     }
    //     output << endl;
    // }
    // for(int i=0; i<h; i++){
    //     for(int j=0; j<w-1; j++){
    //         if(f[i][j][1]) output << 1;
    //         else output << 0;
    //     }
    //     output << endl;
    // }
    // output << endl;
///////////////////////////////////////////////////////////
    

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
                if(i!=w-2) f[i][j+1][3] = 1;
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

/////////////////////////////////////////////

    Solver solver(t, h, w, i0, k, f, cp, crop_info);
    solver.solve();

    // cout << "Completed Successfully" << endl;
    return 0;
}