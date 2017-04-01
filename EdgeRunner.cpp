 #include "CImg.h"  
using namespace cimg_library;
#include <vector>  
#include <iostream>
#include "cal.h"
using namespace std;

#define PI 3.14159265358979323846  

vector<pair<int, int>> points[180][2976];  
int main() {
    while(1) {
	    char str[30];
	    cout << "input the path of image" << endl;
	    cin.getline(str, 30);
	    CImg<double> image(str); 
	    int gradlimit, threadhold, distance;
            cout << "input the limit of grad -- threadhold for hough" << endl;
	    cin >> gradlimit >> threadhold; 
	    CImg<double> grayImg(image);   
	    int width = image.width();
	    int height = image.height();
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
	  
	    //去除噪点
	    grayImg.blur(3);  
	  
	    CImg<double> gradNum(width, height, 1, 1, 0);  
	    int maxDistance = getDis(width, height);  
	    CImg<double> hough(180, maxDistance, 1, 1, 0);  
	    // 定义3*3领域矩阵I  
	    CImg_3x3(I, double);  
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
			points[m][p].push_back(make_pair(x, y));
		    }
		}
	    }
	    const double color[] = {0, 255, 0};
	    CImg<double> dest(width, height, 1, 3, 0);
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
	    vector<pair<double, double>> points_ = intersection(calLine(lines, maxDistance), width, height);
	    for (int mm = 0; mm < lines.size(); mm++) {
		cout << "y = x* " << lines[mm].first << " + " << lines[mm].second << endl;
		vector<pair<int, int>> temp = points[lines[mm].first][lines[mm].second];
		for (int i = 0; i < temp.size(); i++) {
		    dest(temp[i].first, temp[i].second, 0) = 255;		
		} 
	    }
	    for (int mm = 0; mm < points_.size(); mm++) {
		cout << "k: " << points_[mm].first << " m: " << points_[mm].second << endl;
		dest.draw_circle(points_[mm].first, points_[mm].second, 25, color);
	    }
	    dest.display();

    }  
  
} 
