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




//反応率計算
double calpr(int waittime, double alpha, double u0, double goal){
  double a,b;
  a=exp(u0*exp(-alpha*(waittime)));
  b=exp(u0*exp(-alpha*(0)));
  if(goal > waittime){
    return a/(a+b);
  } else {
    return 0.0;
  }
}


int main(){

  double alpha[4] = {Alpha0, Alpha1, Alpha2, Alpha3};
  double u0[4] = {U00, U01, U02, U03};
  int goal[4] = {Goal0, Goal1, Goal2, Goal3};
  int waittime;
  string filename;
  ofstream fout;
  for(int k = 0; k < 4; k++){
    filename = "pr_"+to_string(k)+".csv";
    fout.open(filename);
    for(waittime = 0; waittime < 3001; waittime++){
      fout << waittime/10.0 << "," << calpr(waittime, alpha[k], u0[k], goal[k]) << endl;
    }
    fout.close();
  }
}
