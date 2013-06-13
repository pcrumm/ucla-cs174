all:
	g++ -g -L/opt/local/lib -I/opt/local/include -I/System/Library/Frameworks/OpenGL.framework/Headers -lgl -lglut -lglu -lGLEW InitShader.cpp main.cpp planet.cpp -o project3
clean:
	rm -rf *o project3 *.dSYM *.tgz
dist:
	tar cvzf project3-phil-crumm.tgz *.h *.cpp *.md *.glsl makefile
