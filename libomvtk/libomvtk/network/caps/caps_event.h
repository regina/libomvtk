#ifndef GUARD_LIBOMVTK_NETWORK_CAPS_EVENT_H_INCLUDED
#define GUARD_LIBOMVTK_NETWORK_CAPS_EVENT_H_INCLUDED

#include "../../types/base_types.h"

namespace omvtk
{
	// If you add an event here 
	// don't forget to add it in the init() method
	// in caps_event.cpp! 
	enum CapsEvent
	{
		EventQueueGet	
	};	

	struct CapsEventLookup		 
	{
		static CapsEvent lookup(String const & name);
		static String const & lookup(CapsEvent event_id);
	};

}

#endif //GUARD_LIBOMVTK_NETWORK_CAPS_EVENT_H_INCLUDED

