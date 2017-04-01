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

double getDis(double x, double y) {
    return sqrt(x*x+y*y);
}

