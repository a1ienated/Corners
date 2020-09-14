#pragma once
#include <hge.h>
#include <hgefont.h>
#include <time.h>

class Timer
{
private:
	int timebase;
	int x;
	int y;
	HGE* hge;
	hgeFont* fnt;

public:
	Timer(int xx, int yy, HGE* h, hgeFont* f);

	void SetFont(hgeFont* f) { fnt = f; }
	hgeFont* GetFont() const { return fnt; }

	void Render();
	void reset();
};
