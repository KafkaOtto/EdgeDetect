main: EdgeRunner.cpp warp.o hough.o CImg.h
	g++ -o main EdgeRunner.cpp warp.o hough.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11
warp.o: warping.cpp warping.h CImg.h
	g++ -c -o warp.o warping.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11
hough.o: hough.cpp hough.h CImg.h
	g++ -c -o hough.o hough.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11
