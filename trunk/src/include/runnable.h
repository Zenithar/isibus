/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007                                                    *
 *   IsiBus 2007                  					   *
 ***************************************************************************/

#ifndef __RUNNABLE_H_
#define __RUNNABLE_H_

namespace isibus {
	
	class Runnable {
	public:
		virtual void* run(void* arg) = 0;
	};
}

#endif //__RUNNABLE_H_
