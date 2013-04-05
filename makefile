all:
	g++ -L/opt/local/lib -I/opt/local/include -I/System/Library/Frameworks/OpenGL.framework/Headers -lgl -lglut -lGLEW InitShader.cpp main.cpp -o project1
clean:
	rm -rf *o project1 *.dSYM *.tgz
dist:
	tar cvzf project1-phil-crumm.tgz *.h *.cpp *.md *.glsl makefile