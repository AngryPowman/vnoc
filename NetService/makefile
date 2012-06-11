# author by AngryPowman, for VNOC server
# revision : 3
CXX=g++
VPATH=src:test:../Message
CXXFLAGS=-std=c++0x  
CUR_DIR = $(shell pwd)
INCS := $(CUR_DIR)/include

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -I$(INCS)  $< -o $@

all : Server test_bin
Server : VNOCServer.o NetService.o AsioTcpServer.o MessageParser.o PackMessage.o MsgDef.o VnocProtocol.o 
	$(CXX) -lpthread $^ -o bin/$@
test_bin: EchoTestServer

EchoTestServer : EchoTestServer.o  AsioTcpServer.o
	$(CXX) -lpthread $^ -o bin/$@
clean :
	rm -fr bin/*
	rm -f *.o
