all: XFakeApp XFakeKey

XFakeApp:
	gcc -o XFakeApp fakeapp_main.c -lX11

XFakeKey:
	g++ -o XFakeKey ./fakekey_main.cpp -L/usr/X11R6/lib -lX11
