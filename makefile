all:
	g++ -g -L/opt/local/lib -I/opt/local/include -I/System/Library/Frameworks/OpenGL.framework/Headers -lgl -lglut -lglu -lGLEW InitShader.cpp main.cpp tga.cpp -o project4
clean:
	rm -rf *o project4 *.dSYM *.tgz
dist:
	tar cvzf project4-phil-crumm.tgz *.h *.cpp *.md *.glsl makefile
