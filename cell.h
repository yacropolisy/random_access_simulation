#ifndef INCLUDED_cell_h
#define INCLUDED_cell_h

#include <vector>
#include <fstream>
#include "const.h"

using namespace std;
struct cood{
  double x;
  double y;
};
void initcell(cood cell[]);
void readlog(cood usercood[],int startid,int n, int h, int t, ifstream::pos_type pos[][2]);
void setcell(cood cell[],cood user[],int b[],int n);
void setnj(vector<int> nj[]);

#endif
