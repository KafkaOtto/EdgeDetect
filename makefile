main: EdgeRunner.cpp cal.o CImg.h
	g++ -o main EdgeRunner.cpp cal.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11
cal.o: cal.cpp cal.h CImg.h
	g++ -c -o cal.o cal.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11
