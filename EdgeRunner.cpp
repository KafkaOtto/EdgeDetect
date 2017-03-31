 #include "CImg.h"  
using namespace cimg_library;  
#include <vector>  
#include <iostream>
#include "cal.h"
using namespace std;

#define PI 3.14159265358979323846  

  
int main() {
    while(1) {
	    char str[30];
	    cout << "input the path of image" << endl;
	    cin.getline(str, 30);
	    CImg<double> image(str); 
	    int gradlimit, threadhold, distance;
            cout << "input the limit of grad -- threadhold for hough -- distance for calucating the max" << endl;
	    cin >> gradlimit >> threadhold >> distance; 
	    CImg<double> grayImg(image);   
	    int width = image.width();
	    int height = image.height();
	    //将图片转化为灰度级 
	    cimg_forXY(grayImg, x, y) {  
		double R = grayImg(x, y, 0, 0);  
		double G = grayImg(x, y, 0, 1);  
		double B = grayImg(x, y, 0, 2);  
		double Gray = R * 0.299 + G * 0.587 + B * 0.114;  
		grayImg(x, y, 0, 0) = Gray;  
		grayImg(x, y, 0, 1) = Gray;  
		grayImg(x, y, 0, 2) = Gray;  
	    }  
	  
	    //去除噪点
	    grayImg.blur(3);  
	  
	    CImg<double> gradnum(width, height, 1, 1, 0);  
	    int maxDistance = getDis(width, height);  
	    CImg<double> hough(360, maxDistance, 1, 1, 0);  
	    // 定义3*3领域矩阵I  
	    CImg_3x3(I, double);  
	    // 遍历计算梯度值  
	    cimg_for3x3(grayImg, x, y, 0, 0, I, double) {  
		const double ix = Inc - Ipc;  
		const double iy = Icp - Icn;  
		double grad = std::sqrt(ix*ix + iy*iy);  
		//利用梯度算子，如果梯度大于梯度阀值，说明该点是边缘点
		if (grad > gradlimit) { 
		    cimg_forX(hough, angle) {  
		        double rangle = (double)angle*PI / 180.0;  
		        int polar = (int)(x*cos(rangle) + y*sin(rangle));  
		        if (polar >= 0 && polar < hough.height()) {
			    //将经过该边缘点的极坐标上的直线加一
		            hough(angle, polar) += 1;
		        }  
		    }  
		}  
	    }
	    vector<pair<int, int>> peaks;
	    vector<double> values;
	    //遍历hough图，寻找一定区域上的极大值
	    cimg_forXY(hough, angle, polar) {
		bool newLine = true;
		//判断该直线是否在图像上
		if (isValid(angle, polar, width, height)) {
			//如果计数值大于计数的阀值，说明计数有效
			if (hough(angle, polar, 0, 0) > threadhold) {
			    for (int i = 0; i < peaks.size(); i++) {
				//遍历已经加入的计数有效的值，如果两个值在一定区域内，取计数值较大的
				if (getDis(peaks[i].first-angle, peaks[i].second-polar) < distance) {
				    if (values[i] < hough(angle, polar, 0, 0)) {
					peaks[i].first = angle;
					peaks[i].second = polar;
					values[i] = hough(angle, polar, 0, 0);
			
				    }
				    newLine = false;
				}
			    }
			    if (newLine) {
				peaks.push_back(make_pair(angle, polar));
				values.push_back(hough(angle, polar));
			    }
			}
		}
	    }
	    //lines为根据前面加入的值得到的直线，包括斜率截距
	    vector<pair<double, double>> lines = calLine(peaks);
	    const double color[] = {0, 255, 0};
	    for (int mm = 0; mm < lines.size(); mm++) {
		int x1 = (int)-lines[mm].second/lines[mm].first;
		int x2 = (int)(height-1-lines[mm].second)/lines[mm].first;
		int y1 = (int)lines[mm].second;
		int y2 = (int)(lines[mm].first*(width-1))+lines[mm].second;
		//这里需要区分直线斜率大于1小于-1的情况
		if (abs(lines[mm].first) > 1) {
		    image.draw_line(x1, 0, x2, height-1, color);	
		}
		else {
		    image.draw_line(0, y1, width-1, y2, color);	
		}
		cout << "y = x* " << lines[mm].first << " + " << lines[mm].second << endl;
	    }
	    vector<pair<double, double>> points = intersection(lines, width, height);
	    for (int mm = 0; mm < points.size(); mm++) {
		image.draw_circle(points[mm].first, points[mm].second, 25, color);
		cout << "x: " << lines[mm].first << " y: " << lines[mm].second << endl;
	    }
	    
	    image.display();
    }  
  
} 
