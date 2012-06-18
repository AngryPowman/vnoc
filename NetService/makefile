# author by AngryPowman, for VNOC server
# revision : 3
CXX=g++
VPATH=src:test:../Message
CXXFLAGS=-std=c++0x  
CUR_DIR = $(shell pwd)
INCS := $(CUR_DIR)/include
Server_OBJ=VNOCServer.o NetService.o AsioTcpServer.o MessageParser.o PackMessage.o MsgDef.o VnocProtocol.o
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -I$(INCS)  $< -o $@

all : Server test_bin
Server : $(Server_OBJ) 
	$(CXX) -lpthread $^ -o bin/$@
test_bin: EchoTestServer

EchoTestServer : EchoTestServer.o  AsioTcpServer.o
	$(CXX) -lpthread $^ -o bin/$@
clean :
	rm -fr bin/*
	rm -f *.o
