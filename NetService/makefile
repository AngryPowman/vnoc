# author by AngryPowman, for VNOC

all : Server
Server : 
	g++ -c VNOCServer.cpp
	g++ -c src/NetService.cpp
	g++ -o Server VNOCServer.o NetService.o
	mv VNOCServer.o NetService.o Server bin/
	echo "Build Finished."

clean :
	rm -fr bin/*
