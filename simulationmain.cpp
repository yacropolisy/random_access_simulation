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

//指数分布生成
double randexp_ed(){
  double a = 0.0;
  while(a < 10.0){
    a = -log(Uniform(mt1))/Lambda_ed;
  }
  return a;
}

using namespace std;

int main(){

  int i,j,k,l,m,t,r,s;
  int counter[M];
  long long int traffic_flow[T/S][M];
  for(s = 0; s < T/S; s++){
    for(j = 0; j < M; j++){
      traffic_flow[s][j] = 0;
    }
  }

  r = 0;
  for(r = 1; r < 11; r++){

    mt1.seed(r);
    mt2.seed(r);
    mt3.seed(r);
    mt4.seed(r);

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

        //k = 0 ~ 2の時
        for(k = 0; k < 3; k++){
          if(iot[i].active[k] == true){
            if(iot[i].traffic_list[k][0].starttime2 <= t){
              //端末がいる基地局のフロー数増加
              counter[iot[i].locate]++;

              //次のトラッフィックの発生を追加
              if(iot[i].traffic_list[k][0].trans_number == 0){
                init_iot_traffic.starttime = init_iot_traffic.starttime2 = t + interval[k];
  //              init_iot_traffic.traffic_id = k;
                iot[i].traffic_list[k].push_back(init_iot_traffic);
              }
            }
          }
        }
        k = 3;
        //k = 3 の時
        if(iot[i].active[k] == true){
          if(iot[i].traffic_list[k][0].starttime2 <= t){
            //端末がいる基地局のフロー数増加
            counter[iot[i].locate]++;

            //次のトラッフィックの発生を追加
            if(iot[i].traffic_list[k][0].trans_number == 0){
              init_iot_traffic.starttime = init_iot_traffic.starttime2 = t + randexp_ed();
    //          init_iot_traffic.traffic_id = k;
              iot[i].traffic_list[k].push_back(init_iot_traffic);
            }
          }
        }
        traffic_flow[t/S][iot[i].locate] += counter[iot[i].locate];
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
      filename = "./result/delay"+to_string(k)+"_"+to_string(r)+".csv";
      fout.open(filename);
      for(i = 0; i < traffic_delay[k].size(); i++){
        fout << traffic_delay[k][i] << endl;
      }
      fout.close();
    }

    filename = "./result/flow" + to_string(r) + ".txt";
    fout.open(filename);
    for(s = 0; s < T/S; s++){
      for (j = 0; j < M; j++){
        fout << traffic_flow[s][j] << endl;
      }
    }
    fout.close();

  }

  return 0;
}
