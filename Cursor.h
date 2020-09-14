#pragma once
#include <hge.h>

class Cursor
{
public:
	Cursor(int xx, int yy, int ll, UINT c, HGE* p);

	void Render();
	void Move();

private:
	int x;
	int y;
	int len;
	int xi;
	int yi;
	UINT color;
	HGE* hge;
};

