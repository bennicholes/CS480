
# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL

# For Macs uncomment the next line and comment out the previous one
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++
# Assuming you want to use a recent compiler

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x

all: ../bin/Matrix

../bin/main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -c $(LIBS)

../bin/shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) ../src/shader.cpp -c $(LIBS)

../bin/Matrix: ../bin/main.o ../bin/shader.o
	$(CC) $(CXXFLAGS) -o ../bin/Matrix main.o shader.o $(LIBS)

clean:
	rm *.o ../bin/Matrix
