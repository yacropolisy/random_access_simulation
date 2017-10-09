#ifndef INCLUDED_ue_h
#define INCLUDED_ue_h

#include <vector>
#include <random>
#include <deque>

#include "const.h"

using namespace std;

struct traffic{
  int starttime;
  int starttime2;
//  int traffic_id;
  int trans_number;
  bool wait;

  bool operator<( const traffic& right ) const {
        return starttime2 < right.starttime2;
    }
};

/*
struct pu{
  int interval;
  bool active;
  int firsttime;
};

struct ed{
  int nexttime;
  bool active;
};
*/
class ue{
public:
  int locate;
//  int firsttime[4];
  bool active[4];
  deque <traffic> traffic_list[4];
  void pop_traffic_list(int k);
};
#endif
