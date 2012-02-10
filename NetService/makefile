# author by AngryPowman, for VNOC server
# revision : 2

all : Server
Server : 
	g++ -c VNOCServer.cpp
	g++ -c src/NetService.cpp
	g++ -o Server VNOCServer.o NetService.o
	mv VNOCServer.o NetService.o Server bin/

clean :
	rm -fr bin/*
