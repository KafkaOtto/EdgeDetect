#ifndef CAL_H
#define CAL_H
#include<vector>
#include<iostream>

using namespace std;
double getDis(double x, double y);
vector<pair<double, double>> calLine(vector<pair<int, int>> peaks, int d);
vector<pair<double, double>> intersection(vector<pair<double, double>> lines, int width, int height);
#endif

