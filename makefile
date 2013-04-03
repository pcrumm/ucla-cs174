all:
	gcc -L/opt/local/lib -I/opt/local/include -lglut -lGLEW main.c -o project1
clean:
	rm -rf *o project1