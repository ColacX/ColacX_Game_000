#include <Windows.h>
#include "Thread.h"
using namespace ColacX;

//windows functions
void SleepWindows( int milliseconds )
{
	Sleep( milliseconds );
}

//thread methods
Thread::Thread()
{
}

void Thread::Sleep( int milliseconds )
{
	SleepWindows( milliseconds );
}
