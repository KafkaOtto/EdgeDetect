 #include "CImg.h"  
using namespace cimg_library;
#include <vector>  
#include <iostream>
#include "cal.h"
#include "hough.h"
using namespace std;

#define PI 3.14159265358979323846  

vector<pair<int, int>> points[180][2976];  
int main() {
    char str[30];
    cout << "input the path of image" << endl;
    cin.getline(str, 30);
    CImg<double> image(str); 
    int gradlimit, threadhold;
    cout << "input the limit of grad -- threadhold for hough" << endl;
    cin >> gradlimit >> threadhold;
    Hough* h = new Hough(gradlimit, threadhold);
    CImg<double> grayImg(image);   
    int width = image.width(), height = image.height();
    int maxDistance = h->getDis(width, height);
    CImg<double> gradNum(width, height, 1, 1, 0);
    CImg<double> houghPic(180, maxDistance, 1, 1, 0);
    double color[] = {0, 255, 0}; 
    //去除噪点
    h->toGray(grayImg);
    grayImg.blur(3); 
    h->grad(grayImg, gradNum);
    h->count(gradNum, houghPic, maxDistance);
    vector<pair<int, int>> peaks = h->selectMax(houghPic);
    vector<pair<double, double>> lines = h->calLine(peaks, maxDistance);
    vector<pair<double, double>> points_ = h->intersection(lines, width, height);
    for (int mm = 0; mm < peaks.size(); mm++) {
	cout << "k: " << peaks[mm].first << " m: " << peaks[mm].second << endl;
    }
    for (int mm = 0; mm < lines.size(); mm++) {
	cout << "k: " << lines[mm].first << " m: " << lines[mm].second << endl;
    }
    for (int mm = 0; mm < points_.size(); mm++) {
	cout << "x: " << points_[mm].first << " y: " << points_[mm].second << endl;
	image.draw_circle(points_[mm].first, points_[mm].second, 25, color);
    }
    image.display();
 
  
} 
