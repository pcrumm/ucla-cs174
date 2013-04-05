all:
	g++ -L/opt/local/lib -I/opt/local/include -lglut -lGLEW main.cpp -o project1
clean:
	rm -rf *o project1