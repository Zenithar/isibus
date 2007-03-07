//
// C++ Interface: runnable
//
// Description: 
//
//
// Author: Thibault Normand <thibault.normand@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __RUNNABLE_H_
#define __RUNNABLE_H_

namespace isibus {
	
	class Runnable {
	public:
		virtual void* run(void* arg) = 0;
	};
}

#endif //__RUNNABLE_H_
