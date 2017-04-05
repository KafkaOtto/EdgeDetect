#include "warping.h"
Warp::Warp(int w, int h) {
    width = w;
    height = h;
}
vector<pair<int, int>> sort(vector<pair<int, int>> points) {
    
}
CImg<double> Warp::inverse(CImg<double> m) {
    int n = m.width();
    cout << "width " <<  m.width() << " height " << m.height() << endl;
    CImg<double> ret(n, n, 1, 1, 0); 
    CImg<double>  A(m);
    cimg_forX(ret, i) {
        ret(i, i, 0) = 1.0;
    }

    for (int j = 0; j < n; j++) {
        int p = j;
        double maxV = abs(A(j, j, 0));
	for (int i = j+1; i < n; ++i) {
	    if (maxV < abs(A(i, j, 0))) {
	        p = i;
		maxV = abs(A(i, j, 0));
	    }
	}
	if (maxV < 1e-20) {
            return ret;
	}
        if (j != p) {
	    swap_row(A, j, p);
	    swap_row(ret, j, p);
	}
	double d = A(j, j, 0);
	for (int i = j; i < n; ++i) A(j, i, 0) /= d;
	for (int i = 0; i < n; ++i) ret(j, i, 0) /= d;
	for (int i = 0; i < n; ++i) {
	    if (i != j) {
	        double q = A(i, j, 0);
		for (int k = j; k < n; ++k) {
		    A(i, k, 0) -= q*A(j, k, 0);
		}
		for (int k = 0; k < n; ++k) {
		    ret(i, k, 0) -= q*ret(j, k, 0);
		}
	    }
	}
    }  
    return ret;
}
/*
  交换Cimg对象对应i行j行的数据
*/
void Warp::swap_row(CImg<double>& m, int i, int j) {
    cimg_forY(m, y) {
        double tmp = m(i, y, 0);
	m(i, y, 0) = m(j, y, 0);
	m(j, y, 0) = tmp;
    }
}
/*
  矩阵乘法
*/
CImg<double> Warp::multiply(CImg<double> a, CImg<double> b) {
    int rowA = a.height(), colA = a.width();
    int rowB = b.height(), colB = b.width();
    CImg<double> res(colB, rowA, 1, 1, 0);
    if (colA != rowB) {
        cout << "wrong matrixs mul" << endl;
        return res;
    }
    for (int i = 0; i < rowA; ++i) {
	for (int j = 0; j < colB; ++j) {
	    for (int k = 0; k < colA; ++k) {
		res(i, j, 0) += a(i, k, 0)*b(k, j, 0);
	    }	
	}
    }
    return res;
}
/*
  矩阵转置
*/
CImg<double> Warp::transpose(CImg<double> m) {
    CImg<double> mt(m.height(), m.width(), 1, 1, 0);
    cimg_forXY(m, x, y) {
        mt(y, x, 0) = m(x, y, 0);
    }
    return mt;
}
