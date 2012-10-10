# author by AngryPowman, for VNOC server
# revision : 4
# revisioned by Tydus
CXX=g++
LD=g++
VPATH=src:test:../Message:../program_options:src/mysql
CUR_DIR := $(shell pwd)
INCS := -I$(CUR_DIR)/include -I$(CUR_DIR)/dependencies/build/include
LIBS := -L$(CUR_DIR)/lib -L$(CUR_DIR)/dependencies/build/lib

CXXFLAGS=-std=c++0x $(INCS)
LDFLAGS := $(LIBS)

PO_OBJ=cmdline.o options_description.o utf8_codecvt_facet.o config_file.o parsers.o value_semantic.o convert.o positional_options.o variables_map.o split.o
Server_OBJ=VNOCServer.o NetService.o AsioTcpServer.o MessageParser.o PackMessage.o MsgDef.o VnocProtocol.o Config.o UserManage.o UserStorage.o SQLUserStorage.o FileUserStorage.o MysqlWrapper.o $(PO_OBJ) 

all : Server EchoTestServer

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

Server : $(Server_OBJ)
	mkdir -p bin
	$(LD) $(LIBS) -lpthread -lmysql $^ -o bin/$@

EchoTestServer : EchoTestServer.o  AsioTcpServer.o
	mkdir -p bin
	$(LD) $(LIBS) -lpthread $^ -o bin/$@
clean :
	rm -fr bin/*
	rm -f *.o *.obj

