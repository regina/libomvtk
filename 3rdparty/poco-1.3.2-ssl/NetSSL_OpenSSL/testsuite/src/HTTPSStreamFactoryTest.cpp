//
// HTTPSStreamFactoryTest.cpp
//
// $Id: //poco/1.3/NetSSL_OpenSSL/testsuite/src/HTTPSStreamFactoryTest.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "HTTPSStreamFactoryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "HTTPSTestServer.h"
#include <sstream>
#include <memory>


using Poco::Net::HTTPSStreamFactory;
using Poco::Net::NetException;
using Poco::Net::HTTPException;
using Poco::URI;
using Poco::StreamCopier;


HTTPSStreamFactoryTest::HTTPSStreamFactoryTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPSStreamFactoryTest::~HTTPSStreamFactoryTest()
{
}


void HTTPSStreamFactoryTest::testNoRedirect()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://localhost/large");
	uri.setPort(server.port());
	std::auto_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assert (ostr.str() == HTTPSTestServer::LARGE_BODY);
}


void HTTPSStreamFactoryTest::testEmptyPath()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://localhost");
	uri.setPort(server.port());
	std::auto_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assert (ostr.str() == HTTPSTestServer::SMALL_BODY);
}


void HTTPSStreamFactoryTest::testRedirect()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://localhost/redirect");
	uri.setPort(server.port());
	std::auto_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assert (ostr.str() == HTTPSTestServer::LARGE_BODY);
}


void HTTPSStreamFactoryTest::testProxy()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory("proxy.aon.at", 8080);
	URI uri("https://wwws.appinf.com/");
	std::auto_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assert (ostr.str().length() > 0);
}


void HTTPSStreamFactoryTest::testError()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://localhost/notfound");
	uri.setPort(server.port());
	try
	{
		std::istream* pStr = factory.open(uri);
		fail("not found - must throw");
	}
	catch (HTTPException& exc)
	{
		std::string m = exc.displayText();
	}
}


void HTTPSStreamFactoryTest::setUp()
{
}


void HTTPSStreamFactoryTest::tearDown()
{
}


CppUnit::Test* HTTPSStreamFactoryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPSStreamFactoryTest");

	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testNoRedirect);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testEmptyPath);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testRedirect);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testProxy);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testError);

	return pSuite;
}
