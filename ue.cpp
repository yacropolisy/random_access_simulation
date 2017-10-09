
#include <iostream>
#include <cmath>
#include <deque>
#include <random>


#include "ue.h"
using namespace std;

void ue::pop_traffic_list(int k){
  traffic_list[k].pop_front();
};
