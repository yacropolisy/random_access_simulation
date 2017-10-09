#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <deque>
#include <algorithm>

#include "cell.h"
#include "ue.h"
#include "const.h"

mt19937 mt1(1);
mt19937 mt2(1);
mt19937 mt3(1);
mt19937 mt4(1);
uniform_real_distribution<> Uniform(0.0,1.0);
uniform_int_distribution<> Uniint(0,Interval0);
uniform_int_distribution<> backoff(0,Backoff_Indicator);

//反応率計算
double calpr(int waittime, double alpha, double u0, int goal){
  double a,b;
  a=exp(u0*exp(-alpha*(waittime)));
  b=exp(u0*exp(-alpha*(0)));
  if(goal > waittime){
    return a/(a+b);
  } else {
    return 0.0;
  }
}

//指数分布生成
double randexp_ed(){
  double a = 0.0;
  while(a < 10.0){
    a = -log(Uniform(mt1))/Lambda_ed;
  }
  return a;
}

//混雑度で並び替えるための構造体
struct slot{
  int s;
  int j;
  long long flow;

  bool operator<( const slot& right ) const {
        return flow > right.flow;
    }
};

using namespace std;


int main(){

  int i,j,k,l,m,t,r,s,waittime,wt;
  int counter[M];
  string str;

  r = 0;
  waittime = Waittime;
  for(wt = 10; wt < 310; wt += 10){
    waittime = wt * 1000 / DeltaT;
    cout <<  "wt: " << wt << endl;

    for(r = 1 ; r < 11; r++){

      cout << r << endl;


      mt1.seed(r);
      mt2.seed(r);
      mt3.seed(r);
      mt4.seed(r);


      //waitのflag
      int waitflag[T/S][M];
      for(s = 0; s < T/S; s++){
        for(j = 0; j < M; j++){
          waitflag[s][j] = 0;
        }
      }

      //初期化resul
      ofstream fout;
      string filename;
      ifstream::pos_type pos[N][2];
      for(int init=0; init < N ; init++){
        for(int init2=0; init2<2;init2++){
          pos[init][init2]=0;
        }
      }
      int h = H;

      //ログデータ読み込み、ユーザの位置入手用
      cood uv[N];
      int bv[N];

      //基地局の配置 =>cell.cpp
      cood cell[M];
      initcell(cell);

      //定数の定義
      double p[4] = {P0, P1, P2, P3};
      int interval[3] = {Interval0, Interval1, Interval2};
      double alpha[4] = {Alpha0, Alpha1, Alpha2, Alpha3};
      double u0[4] = {U00, U01, U02, U03};
      int goal[4] = {Goal0, Goal1, Goal2, Goal3};

      vector < int > traffic_delay[4];


      //UEの初期化
      vector<ue> iot;
      iot.clear();
      ue init_iot;
      traffic init_iot_traffic;

      init_iot_traffic.trans_number = 0;
      init_iot_traffic.wait = false;

      for(i = 0; i < N; i++){
        for(k = 0; k < 4; k++){
          if(Uniform(mt3) < p[k]){
            init_iot.active[k] = true;
    //        init_iot.firsttime[k] = Uniint(mt2)/10;
            if(k < 3){
              init_iot_traffic.starttime = init_iot_traffic.starttime2 = Uniint(mt2)%interval[k];
            } else {
              init_iot_traffic.starttime = init_iot_traffic.starttime2 = randexp_ed();
            }
    //        init_iot_traffic.traffic_id = k;
            init_iot.traffic_list[k].clear();
            init_iot.traffic_list[k].push_back(init_iot_traffic);
          } else {
            init_iot.active[k] = false;
          }
        }

          iot.push_back(init_iot);

      }


      //traffic_flow読み込み
      vector<slot> rank;
      slot tmp_slot;

      ifstream fin;
      filename = "./result/flow" + to_string(r) + ".txt";
      fin.open(filename);
      if(!fin){
        cout << "read file error" << endl;
      }
      for(s = 0; s < T/S; s++){
        for(j = 0; j < M; j++){
          fin >> str;
          tmp_slot.flow = stol(str);
          tmp_slot.s = s;
          tmp_slot.j = j;
          rank.push_back(tmp_slot);
        }
      }

      //混雑度並び替え
      sort(rank.begin(), rank.end());



      for(i = 0; i < TOP; i++){
        waitflag[rank[i].s][rank[i].j] = 1;
      }


      ////////////////////////////////////////////////////////////////////////////
      //  シミュレーション開始
      ////////////////////////////////////////////////////////////////////////////

      for(t = 0; t < T ; t++){

        //端末の位置を取得

        if(t%(1000/DeltaT) == 0){
          //logデータ読み込んでユーザの位置を入手 =>cell.cpp
          readlog(uv,0,N,h,t/(1000/DeltaT),pos);
          setcell(cell,uv,bv,N);
          for(i = 0; i < N; i++){
            iot[i].locate = bv[i];
          }
        }

        //counter（トラフィックフロー）の初期化

        for(j = 0; j < M ; j++){
          counter[j] = 0;
        }

        //trafficの発生

        //1.まず各基地局のトラフィックの発生数をチェック
        for(i = 0; i < N ; i++){

          //フィールド内にいない場合はパス
          if(iot[i].locate == -1){
            continue;
          }
          //発生数チェック
          for(k = 0; k < 4; k++){
            if(iot[i].active[k] == true){
              if(iot[i].traffic_list[k][0].starttime2 <= t){
                //次のトラッフィックの発生を追加
                if(iot[i].traffic_list[k][0].trans_number == 0 && iot[i].traffic_list[k][0].wait == false){
                  if(k < 3){
                    init_iot_traffic.starttime = init_iot_traffic.starttime2 = t + interval[k];
                  } else {
                    init_iot_traffic.starttime = init_iot_traffic.starttime2 = t + randexp_ed();
                  }
                  //init_iot_traffic.traffic_id = k;
                  iot[i].traffic_list[k].push_back(init_iot_traffic);
                }

                //通信待機の処理
                if(waitflag[t/S][iot[i].locate] == 1){
                  if(calpr(waittime, alpha[k], u0[k], goal[k]) > Uniform(mt3) && iot[i].traffic_list[k][0].wait == false){
                    iot[i].traffic_list[k][0].starttime2 += waittime;
                    iot[i].traffic_list[k][0].wait = true;
                  } else {
                    //端末がいる基地局のフロー数増加
                    counter[iot[i].locate]++;
                  }
                } else {
                  //端末がいる基地局のフロー数増加
                  counter[iot[i].locate]++;
                }
              }
            }
          }
        }
        //2.次にtrafficの処理：競合なしなら削除、競合してたらバックオフ待って再送
        for(i = 0; i < N ; i++){

          //フィールド内にいない場合はパス
          if(iot[i].locate == -1){
            continue;
          }

          for(k = 0; k < 4; k++){
            if(iot[i].active[k] == true){
              if(iot[i].traffic_list[k][0].starttime2 <= t){
                //traffic処理
                if(counter[iot[i].locate] == 1){
                  traffic_delay[k].push_back(iot[i].traffic_list[k][0].starttime2 - iot[i].traffic_list[k][0].starttime);
                  iot[i].traffic_list[k].pop_front();
                } else {
                  iot[i].traffic_list[k][0].starttime2 += backoff(mt1);
                  iot[i].traffic_list[k][0].trans_number++;
                }

                //traffic_listの並び替え
                sort(iot[i].traffic_list[k].begin(), iot[i].traffic_list[k].end());
              }
            }
          }
        } //iのループ
      } //tのループ



      //record

      for(k = 0 ; k < 4 ; k++){
        filename = "./result/delay"+to_string(k)+"_wait"+to_string(wt)+"_"+to_string(r)+".csv";
        fout.open(filename);
        for(i = 0; i < traffic_delay[k].size(); i++){
          fout << traffic_delay[k][i] << endl;
        }
        fout.close();
      }
    }//r
  }//wt


  return 0;
}
