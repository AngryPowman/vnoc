#ifdef CPPTEST
#include "Stdafx.h"
#include "testRunner.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <fstream>

VOID CTestRunner::StartTest(XMessage_StartTest* msg)
{
    CppUnit::TestResult    controller;
    CppUnit::TestResultCollector result;

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.eventManager().addListener( &result );
    runner.run();
 
    std::ofstream xmlFileOut("cpptestresults.xml");
    CppUnit::XmlOutputter xmlOut(&result, xmlFileOut);
    xmlOut.write();
}

#endif