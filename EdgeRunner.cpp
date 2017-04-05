 #include "CImg.h"  
using namespace cimg_library;
#include <vector>  
#include <iostream>
#include "warping.h"
#include "hough.h"
using namespace std;

#define PI 3.14159265358979323846  
 
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
    vector<pair<double, double>> ori = h->intersection(lines, width, height);
    Warp* warp = new Warp(794, 1123);
    CImg<double> resImg(795, 1124, 1, 1, 0);
    CImg<double> A(8, 8, 1, 1, 0);
    CImg<double> b(1, 8, 1, 1, 0);
    int A4[4][4] = {{0, 0}, {794, 0}, {0, 1123}, {794, 1123}};
    int indexb = 0;
    for (int mm = 0; mm < ori.size(); mm++) {
	cout << "x: " << ori[mm].first << " y: " << ori[mm].second << endl;
        b(0, indexb++, 0) = ori[mm].first;
	b(0, indexb++, 0) = ori[mm].second;
    }
    for (int i = 0; i < 8; i++) {
        if (i%2 != 0) {
	    A(i, 5, 0) = 1;
	    A(i, 3, 0) = A4[i/2][0];
	    A(i, 4, 0) = A4[i/2][1];
	    A(i, 6, 0) = -A(i, 3, 0)*b(0, i, 0);
	    A(i, 7, 0) = -A(i, 4, 0)*b(0, i, 0);
	}
	else {
	    A(i, 2, 0) = 1.0;
	    A(i, 0, 0) = A4[i/2][0];
	    A(i, 1, 0) = A4[i/2][1];
	    A(i, 6, 0) = -A(i, 0, 0)*b(0, i, 0);
	    A(i, 7, 0) = -A(i, 1, 0)*b(0, i, 0);
	}
    }
    cimg_forX(A, x) {
	cimg_forY(A, y) {
	    cout << " " << A(x, y, 0);	
	}
	cout << endl;
    }
    CImg<double> ata = warp->inverse(warp->multiply(warp->transpose(A), A));
    cimg_forX(ata, x) {
	cimg_forY(ata, y) {
	    cout << " " << ata(x, y, 0);	
	}
	cout << endl;
    }
    CImg<double> homo = warp->multiply(warp->multiply(ata, warp->transpose(A)), b);
    cimg_forXY(homo, x, y) {
        cout << "homo " << homo(x, y, 0) << endl;
    }
   cimg_forXY(resImg, x, y) {
        double deno = x*homo(0, 6, 0)+y*homo(0, 7, 0)+1.0;
	int orix = (int)(x*homo(0, 0, 0)+y*homo(0, 1, 0)+homo(0, 2, 0))/deno;
        int oriy = (int)(x*homo(0, 3, 0)+y*homo(0, 4, 0)+homo(0, 5, 0))/deno;
	//cout << "orix " << orix << " oriy " << oriy << endl;
        if (orix < 0 || oriy < 0 || orix > width || oriy > height) {}
	else {
	    resImg.atXY(x, y) = image.atXY(orix, oriy);
	    //resImg(x, y, 1) = image(orix, oriy, 1);
	    //resImg(x, y, 2) = image(orix, oriy, 2);	
	}
    }
    resImg.display();
    return 0;
  
}
