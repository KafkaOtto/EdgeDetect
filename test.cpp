 #include "CImg.h"  
using namespace cimg_library;
#include <vector>  
#include <iostream>
#include "warping.h"
using namespace std;

#define PI 3.14159265358979323846  
 
int main() {
    Warp* warp = new Warp(794, 1123);
    CImg<double> b(3, 3, 1, 1, 0);
    b(0, 0, 0) = 3;
    b(0, 1, 0) = 5;
    b(0, 2, 0) = 33;
    b(1, 0, 0) = 4;
    b(1, 1, 0) = 124;
    b(1, 2, 0) = 794;
    b(2, 0, 0) = 55;
    b(2, 1, 0) = 1123;
    b(2, 2, 0) = 3;
    CImg<double> c = warp->transpose(b);
    cimg_forX(b, x) {
	cimg_forY(b, y) {
	    cout << " " << b(x, y, 0);	
	}
	cout << endl;
    }
    cimg_forX(c, x) {
	cimg_forY(c, y) {
	    cout << " " << c(x, y, 0);	
	}
	cout << endl;
    }
    CImg<double> d = warp->multiply(b, b);
    cimg_forX(d, x) {
	cimg_forY(d, y) {
	    cout << " " << d(x, y, 0);	
	}
	cout << endl;
    }
    CImg<double> e = warp->inverse(b);
    cimg_forX(e, x) {
	cimg_forY(e, y) {
	    cout << " " << e(x, y, 0);	
	}
	cout << endl;
    }
    return 0;
  
}
