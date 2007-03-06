#include "cc.h"

using namespace isibus;

int main(int argc, char* argv)
{
	ControlCenter cc;
	
	cc.startControl();
	
	Ivy::ivyMainLoop();
	
	return 0;
} 
