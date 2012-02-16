# author by AngryPowman, for VNOC server
# revision : 3
CXX=g++
VPATH=src:test
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

all : Server
Server : VNOCServer.o NetService.o 
	$(CXX) $^ -o bin/$@

clean :
	rm -fr bin/*
	rm -f *.o
