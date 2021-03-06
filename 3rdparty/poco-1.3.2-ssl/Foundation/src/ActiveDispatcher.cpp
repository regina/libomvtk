//
// ActiveDispatcher.cpp
//
// $Id: //poco/1.3/Foundation/src/ActiveDispatcher.cpp#2 $
//
// Library: Foundation
// Package: Threading
// Module:  ActiveObjects
//
// Copyright (c) 2006-2007, Applied Informatics Software Engineering GmbH.
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


#include "Poco/ActiveDispatcher.h"
#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"


namespace Poco {


namespace
{
	class MethodNotification: public Notification
	{
	public:
		MethodNotification(ActiveRunnableBase::Ptr pRunnable):
			_pRunnable(pRunnable)
		{
		}
		
		ActiveRunnableBase::Ptr runnable() const
		{
			return _pRunnable;
		}
		
	private:
		ActiveRunnableBase::Ptr _pRunnable;
	};
	
	class StopNotification: public Notification
	{
	};
}


ActiveDispatcher::ActiveDispatcher()
{
	_thread.start(*this);
}


ActiveDispatcher::ActiveDispatcher(Thread::Priority prio)
{
	_thread.setPriority(prio);
	_thread.start(*this);
}


ActiveDispatcher::~ActiveDispatcher()
{
	try
	{
		stop();
	}
	catch (...)
	{
	}
}


void ActiveDispatcher::start(ActiveRunnableBase::Ptr pRunnable)
{
	poco_check_ptr (pRunnable);

	_queue.enqueueNotification(new MethodNotification(pRunnable));
}


void ActiveDispatcher::cancel()
{
	_queue.clear();
}


void ActiveDispatcher::run()
{
	AutoPtr<Notification> pNf = _queue.waitDequeueNotification();
	while (pNf && !dynamic_cast<StopNotification*>(pNf.get()))
	{
		MethodNotification* pMethodNf = dynamic_cast<MethodNotification*>(pNf.get());
		poco_check_ptr (pMethodNf);
		ActiveRunnableBase::Ptr pRunnable = pMethodNf->runnable();
		pRunnable->duplicate(); // run will release
		pRunnable->run();
		pNf = _queue.waitDequeueNotification();
	}
}


void ActiveDispatcher::stop()
{
	_queue.clear();
	_queue.wakeUpAll();
	_queue.enqueueNotification(new StopNotification);
	_thread.join();
}


} // namespace Poco
