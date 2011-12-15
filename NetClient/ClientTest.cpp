
// #include <iostream>
// using namespace std; 
//extern "C"
// #include "SString.h"

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "NetClient.h"
#include "XmlAnalysis.h"
#include "XmlTransform.h"
// #include <algorithm>
// #include <assert.h>
// #include <string>
// #include <vector>
// #include <utility>
// #include <list>
// #include <set>
// #include <map>
// #include <memory>
using namespace std;

NetClient netClient;
XmlAnalysis xml;


int main(int argc, char** argv) 
{	
	char szHost[256] = {0};
	char szClient [] = "Hello Server£¡";
	char x1 [] = "<ReturnMessage MsgType = \"15\" MsgID = \"54\" />";
	char x2 [] = "<Version ver = \"15.12.1.0\"/>";
	char x3 [] = "<Update version = \"15.12.1.2\" Control = \"Test\"/>";
	Login lg;
	ReturnMessage rm;
	Version vr;
	Update up;
	
	netClient.CreateSocket(VNOC_TCP);
	netClient.Build("127.0.0.1",5555);
	netClient.Result(szHost,256);
	xml.Resolve(szHost,&lg);
	cout<<lg._Name<<endl;
	cout<<lg._Password<<endl;

	xml.Resolve(x1,&rm);
	cout<<rm.MsgID<<endl;
	cout<<rm.MsgType<<endl;

	xml.Resolve(x2,&vr);
	cout<<vr._ver<<endl;

	xml.Resolve(x3,&up);
	cout<<up._ver<<endl;
	cout<<up.Control<<endl;

	cout<<WSAGetLastError()<<endl;

	return 0;
}


