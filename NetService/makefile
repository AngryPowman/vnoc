# author by AngryPowman, for VNOC server
# revision : 3
CXX=g++
VPATH=src:test:../Message:../program_options
CXXFLAGS=-std=c++0x  
CUR_DIR = $(shell pwd)
INCS := $(CUR_DIR)/include
PO_OBJ=cmdline.o options_description.o utf8_codecvt_facet.o config_file.o parsers.o value_semantic.o convert.o positional_options.o variables_map.o split.o
Server_OBJ=VNOCServer.o NetService.o AsioTcpServer.o MessageParser.o PackMessage.o MsgDef.o VnocProtocol.o Config.o $(PO_OBJ) 
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

