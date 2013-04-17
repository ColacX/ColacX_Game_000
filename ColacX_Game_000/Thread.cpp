#include "Thread.h"

#include <Windows.h>

void Windows_Sleep( int milliseconds )
{
	Sleep( milliseconds );
}

void Thread::Sleep( int milliseconds )
{
	Windows_Sleep( milliseconds );
}
