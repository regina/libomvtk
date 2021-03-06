//
// NamedEventTest.cpp
//
// $Id: //poco/1.3/Foundation/testsuite/src/NamedEventTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "NamedEventTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NamedEvent.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Timestamp.h"


using Poco::NamedEvent;
using Poco::Thread;
using Poco::Runnable;
using Poco::Timestamp;


static NamedEvent testEvent("TestEvent");


namespace
{
	class TestEvent: public Runnable
	{
	public:
		void run()
		{
		
			testEvent.wait();
			_timestamp.update();
		}

		const Timestamp& timestamp() const
		{
			return _timestamp;
		}

	private:
		Timestamp _timestamp;
	};
}


NamedEventTest::NamedEventTest(const std::string& name): CppUnit::TestCase(name)
{
}


NamedEventTest::~NamedEventTest()
{
}


void NamedEventTest::testNamedEvent()
{
	Thread thr1;
	TestEvent te;
	thr1.start(te);
	Timestamp now;
	Thread::sleep(2000);
	testEvent.set();
	thr1.join();
	assert (te.timestamp() > now);

	Thread thr2;
	thr2.start(te);
	now.update();
	Thread::sleep(2000);
	testEvent.set();
	thr2.join();
	assert (te.timestamp() > now);
}


void NamedEventTest::setUp()
{
}


void NamedEventTest::tearDown()
{
}


CppUnit::Test* NamedEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NamedEventTest");

	CppUnit_addTest(pSuite, NamedEventTest, testNamedEvent);

	return pSuite;
}
