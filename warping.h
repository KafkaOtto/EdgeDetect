#include "CImg.h"
#include<iostream>
using namespace cimg_library;
#include<vector>
using namespace std;
class Warp {
  private:
    int width;
    int height;
  public:
    Warp(int, int);
    CImg<double> inverse(CImg<double> m);
    CImg<double> transpose(CImg<double> m);
    vector<pair<int, int>> sort(vector<pair<int, int>>);
    void swap_row(CImg<double>& m, int i, int j);
    CImg<double> multiply(CImg<double> a, CImg<double> b);
};
