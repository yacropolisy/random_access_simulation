#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

#include "cell.h"
#include "const.h"
using namespace std;

//セルの座標を設定
void initcell(cood cell[]){
  for(int j=0;j<M;j++){
    if(j%16<8){
      cell[j].x=1000*(j%16)+500;
      cell[j].y=866*(2*(j/16));
    }else{
      cell[j].x=1000*((j%8));
      cell[j].y=866*(2*(j/16)+1);
    }
  }
}

//時刻ｔにおけるタクシーログデータの取り込み
void readlog(cood usercood[],int startid,int n, int h, int t, ifstream::pos_type pos[][2]){
  ifstream fin;
  string filename,ss,x,y;
  stringstream stio;
  for(int i=0;i<n;i++){
    filename="data/splitedID"+to_string(i+startid)+"_"+to_string(h)+".txt";
    fin.open(filename);
    if(!fin){
      cout<<"open error:"<< filename <<endl;
    }
    fin.seekg(pos[i][t%2]);
    getline(fin,ss);
    pos[i][(t+1)%2] = fin.tellg();
    stio.str(ss);
    stio >>x >>x >>y; //ファイル中、1列目はtimeなので捨てる

    usercood[i].x=stod(x)+8000;
    usercood[i].y=stod(y)+10000;
    fin.close();
  }
}

void setcell(cood cell[],cood usercood[],int b[],int n){
  double tmp,min;
  int i,j,cellnum;
  for(int i=0;i<n;i++){
    cellnum=-1;
    min=1400000;
    for(int j=0;j<M;j++){
      tmp=(cell[j].x-usercood[i].x)*(cell[j].x-usercood[i].x)+(cell[j].y-usercood[i].y)*(cell[j].y-usercood[i].y);
      if(min>tmp){
        min=tmp;
        cellnum=j;
      }
    }
    b[i]=cellnum;
  }
}
