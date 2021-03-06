//
// NotificationCenterTest.cpp
//
// $Id: //poco/1.3/Foundation/testsuite/src/NotificationCenterTest.cpp#1 $
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


#include "NotificationCenterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/AutoPtr.h"


using Poco::NotificationCenter;
using Poco::Observer;
using Poco::NObserver;
using Poco::Notification;
using Poco::AutoPtr;


class TestNotification: public Notification
{
};


NotificationCenterTest::NotificationCenterTest(const std::string& name): CppUnit::TestCase(name)
{
}


NotificationCenterTest::~NotificationCenterTest()
{
}


void NotificationCenterTest::test1()
{
	NotificationCenter nc;
	nc.postNotification(new Notification);
}


void NotificationCenterTest::test2()
{
	NotificationCenter nc;
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.postNotification(new Notification);
	assert (_set.size() == 1);
	assert (_set.find("handle1") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
}


void NotificationCenterTest::test3()
{
	NotificationCenter nc;
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle2));
	nc.postNotification(new Notification);
	assert (_set.size() == 2);
	assert (_set.find("handle1") != _set.end());
	assert (_set.find("handle2") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle2));
}


void NotificationCenterTest::test4()
{
	NotificationCenter nc;
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle2));
	nc.postNotification(new Notification);
	assert (_set.size() == 2);
	assert (_set.find("handle1") != _set.end());
	assert (_set.find("handle2") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle2));
	_set.clear();
	nc.postNotification(new Notification);
	assert (_set.empty());
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle3));
	nc.postNotification(new Notification);
	assert (_set.size() == 1);
	assert (_set.find("handle3") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle3));
}


void NotificationCenterTest::test5()
{
	NotificationCenter nc;
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.addObserver(Observer<NotificationCenterTest, TestNotification>(*this, &NotificationCenterTest::handleTest));
	nc.postNotification(new Notification);
	assert (_set.size() == 1);
	assert (_set.find("handle1") != _set.end());
	_set.clear();
	nc.postNotification(new TestNotification);
	assert (_set.size() == 2);
	assert (_set.find("handle1") != _set.end());
	assert (_set.find("handleTest") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.removeObserver(Observer<NotificationCenterTest, TestNotification>(*this, &NotificationCenterTest::handleTest));
}


void NotificationCenterTest::testAuto()
{
	NotificationCenter nc;
	nc.addObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
	nc.postNotification(new Notification);
	assert (_set.size() == 1);
	assert (_set.find("handleAuto") != _set.end());
	nc.removeObserver(NObserver<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handleAuto));
}


void NotificationCenterTest::testDefaultCenter()
{
	NotificationCenter& nc = NotificationCenter::defaultCenter();
	nc.addObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
	nc.postNotification(new Notification);
	assert (_set.size() == 1);
	assert (_set.find("handle1") != _set.end());
	nc.removeObserver(Observer<NotificationCenterTest, Notification>(*this, &NotificationCenterTest::handle1));
}


void NotificationCenterTest::handle1(Poco::Notification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;	
	_set.insert("handle1");
}


void NotificationCenterTest::handle2(Poco::Notification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;	
	_set.insert("handle2");
}


void NotificationCenterTest::handle3(Poco::Notification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<Notification> nf = pNf;	
	_set.insert("handle3");
}


void NotificationCenterTest::handleTest(TestNotification* pNf)
{
	poco_check_ptr (pNf);
	AutoPtr<TestNotification> nf = pNf;	
	_set.insert("handleTest");
}


void NotificationCenterTest::handleAuto(const AutoPtr<Notification>& pNf)
{
	_set.insert("handleAuto");
}


void NotificationCenterTest::setUp()
{
	_set.clear();
}


void NotificationCenterTest::tearDown()
{
}


CppUnit::Test* NotificationCenterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NotificationCenterTest");

	CppUnit_addTest(pSuite, NotificationCenterTest, test1);
	CppUnit_addTest(pSuite, NotificationCenterTest, test2);
	CppUnit_addTest(pSuite, NotificationCenterTest, test3);
	CppUnit_addTest(pSuite, NotificationCenterTest, test4);
	CppUnit_addTest(pSuite, NotificationCenterTest, test5);
	CppUnit_addTest(pSuite, NotificationCenterTest, testAuto);
	CppUnit_addTest(pSuite, NotificationCenterTest, testDefaultCenter);

	return pSuite;
}
