#include "Timer.h"

Timer::Timer(int xx, int yy, HGE* h, hgeFont* f) : x(xx), y(yy), hge(h), fnt(f)
{
	reset();
}

void Timer::reset()
{
	timebase = time(0);
}

void Timer::Render()
{
	if (fnt)
	{
		int ntime = time(0);
		ntime -= timebase;
		int hr = ntime / 3600;
		int min = ntime % 3600 / 60;
		int sec = ntime % 3600 % 60;
		fnt->printf(x, y, 0, "Time %d:%d:%d", hr, min, sec);
	}
}