#include<vector>
#include<cmath>
#include<iostream>
#define PI 3.14159265358979323846


using namespace std;
vector<pair<double, double>> calLine(vector<pair<int, int>> peaks, int dis) {
    vector<pair<double, double>> lines;
    for (int i = 0; i < peaks.size(); i++) {
        double angle = (double)peaks[i].first*PI/180.0;
	double k = -cos(angle)/sin(angle);
	double m = (peaks[i].second*2-dis)/sin(angle);
        lines.push_back(make_pair(k, m)); 
    }
    return lines;
}
vector<pair<double, double>> intersection(vector<pair<double, double>> lines, int width, int height) {
    vector<pair<double, double>> points;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = i+1; j < lines.size(); j++) {
	    double k1 = lines[i].first;
            double k2 = lines[j].first;
	    double m1 = lines[i].second;
	    double m2 = lines[j].second;
            double x = (m2-m1)*1.0/(k1-k2);
	    double y = (k1*m2-k2*m1)/(k1-k2);
	    if (x >= 0 && x < width && y >= 0 && y < height) {
	        points.push_back(make_pair(x, y));
	    }
	}
    }
    return points;
}
int xInter(int angle, int polar, int x) {
    double a = (double)angle*PI/180;
    double k = -cos(a)/sin(a);
    double m = (double)polar/sin(a);
    return (k*x+m);
}
int yInter(int angle, int polar, int y) {
    double a = (double)angle*PI/180;
    double k = -cos(a)/sin(a);
    double m = (double)polar/sin(a);
    return ((y-m)/k);
}
bool isValid(int angle, int polar, int width, int height) {
   int x1 = xInter(angle, polar, 0);
   int x2 = xInter(angle, polar, height-1);
   int y1 = yInter(angle, polar, 0);
   int y2 = yInter(angle, polar, width-1);
   return (x1 >= 0 && x1 <= width-1) ||
          (x2 >= 0 && x2 <= width-1) ||
          (y1 >= 0 && y1 <= height-1) ||
          (y2 >= 0 && y2 <= height-1);
}
double getDis(double x, double y) {
    return sqrt(x*x+y*y);
}

