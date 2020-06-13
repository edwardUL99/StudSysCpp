CXX=g++
CXXFLAGS=-g -Wall

LIBS=-lmysqlcppconn

PROGRAMS=test
SOURCES=$(shell find ./sources -name "*.cpp")
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

all: studsys

studsys: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o studsys $(LIBS)

clean:
	$(RM) $(OBJECTS)