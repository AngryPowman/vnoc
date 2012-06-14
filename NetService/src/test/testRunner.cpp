#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "..\testUserManage.hpp"
#include <iostream>
using namespace std;

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run( "", false );

  	testUserManage test;
	cout << "AuthenticateTest Return : "<<test.AuthenticateTest()<<endl;

  return wasSuccessful;
}
