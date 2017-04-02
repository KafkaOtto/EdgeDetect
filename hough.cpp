#include "CImg.h"
#include "hough.h"
using namespace cimg_library;
#include <vector>  
#include <iostream>
using namespace std;
#define PI 3.14159265358979323846
Hough::Hough(int g, int t) {
    gradlimit = g;
    threadhold = t;
}
void Hough::toGray(CImg<double>& grayImg) {
    //将图片转化为灰度级 
    cimg_forXY(grayImg, x, y) {  
        double R = grayImg(x, y, 0, 0);  
        double G = grayImg(x, y, 0, 1);  
        double B = grayImg(x, y, 0, 2);  
        double Gray = (R + G  + B )/3.0;  
        grayImg(x, y, 0, 0) = Gray;  
        grayImg(x, y, 0, 1) = Gray;  
        grayImg(x, y, 0, 2) = Gray;  
    }
}
void Hough::grad(CImg<double>& grayImg, CImg<double>& gradNum) {
    // 定义3*3领域矩阵I  
    CImg_3x3(I, double);
    cout << gradlimit << endl; 
    // 遍历计算梯度值  
    cimg_for3x3(grayImg, x, y, 0, 0, I, double) {  
        const double ix = Inc - Ipc;  
        const double iy = Icp - Icn;  
        double grad = std::sqrt(ix*ix + iy*iy);  
        //利用梯度算子，如果梯度大于梯度阀值，说明该点是边缘点
        if (grad > gradlimit) {
            gradNum(x, y, 0) = 255;
        }
        else
            gradNum(x, y, 0) = 0;
    }
}
void Hough::count(CImg<double>& gradNum, CImg<double>& hough, int maxDistance) {
     double sinvalue[180];
     double cosvalue[180];
     for (int i = 0; i < 180; i++) {
         sinvalue[i] = sin(i*PI/180);
         cosvalue[i] = cos(i*PI/180);
     }	    
     //遍历梯度图
     cimg_forXY(gradNum, x, y) {
         if (gradNum(x, y, 0) == 255.0) {
             for (int m = 0; m < 180; m++) {
                 int p = (int)(x*cosvalue[m]+y*sinvalue[m]);
                 p = p/2+maxDistance/2;
   	         hough(m, p, 0) += 1;
             }
	 }
       }
}
vector<pair<int, int>> Hough::selectMax(CImg<double>& hough) {
    vector<pair<int, int>> lines;
    cimg_forXY(hough, i, j) {
	if (hough(i, j, 0) > threadhold) {
            bool newline = true;
            for (int k = 0; k < lines.size(); k++) {
                if ((abs(lines[k].first-i) < 10 ||
                    abs(500-lines[k].first+i) < 5) &&
                    (abs(lines[k].second-j) < 50)) {
		    if (hough(i, j, 0) > hough(lines[k].first, lines[k].second, 0)) {
		        lines[k].first = i;
		        lines[k].second = j;
	            }
		    newline = false;
	        }
	    }
            if (newline) lines.push_back(make_pair(i, j));
        }
    }
    return lines;
}
vector<pair<double, double>> Hough::calLine(vector<pair<int, int>> peaks, int dis) {
    vector<pair<double, double>> lines;
    for (int i = 0; i < peaks.size(); i++) {
        double angle = (double)peaks[i].first*PI/180.0;
	double k = -cos(angle)/sin(angle);
	double m = (peaks[i].second*2-dis)/sin(angle);
        lines.push_back(make_pair(k, m)); 
    }
    return lines;
}
vector<pair<double, double>> Hough::intersection(vector<pair<double, double>> lines, int width, int height) {
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

double Hough::getDis(double x, double y) {
    return sqrt(x*x+y*y);
}

