#include "CImg.h"
using namespace cimg_library;
#include <vector>  
#include <iostream>
using namespace std;

class Hough {
  private:
    int threadhold;
    int gradlimit;
  public:
    Hough(int g, int t);
    void toGray(CImg<double>& gray);
    void grad(CImg<double>& gradImg, CImg<double>& gradNum);
    void count(CImg<double>& gradNum, CImg<double>& hough, int);
    vector<pair<int, int>> selectMax(CImg<double>& hough);
    double getDis(double x, double y);
    vector<pair<double, double>> calLine(vector<pair<int, int>> peaks, int d);
    vector<pair<double, double>> intersection(vector<pair<double, double>> lines, int width, int height);
};
