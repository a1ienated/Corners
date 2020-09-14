#include "Cursor.h"
#include "Common.h"

Cursor::Cursor(int xx, int yy, int ll, UINT c, HGE* p)
	: x(xx), y(yy), len(ll), hge(p), xi(0), yi(0), color(c) {}

void Cursor::Render()
{
	int xx = x + xi * len, yy = y + yi * len;
	hge->Gfx_RenderLine(xx, yy, xx + len, yy, color);
	hge->Gfx_RenderLine(xx, yy, xx, yy + len, color);
	hge->Gfx_RenderLine(xx, yy + len, xx + len, yy + len, color);
	hge->Gfx_RenderLine(xx + len, yy, xx + len, yy + len, color);
}

void Cursor::Move()
{
	float x0, y0;
	hge->Input_GetMousePos(&x0, &y0);
	if (x0 >= x && y0 >= y && x0 < x + BOARD_LEN * len && y0 < y + BOARD_LEN * len)
	{
		int xx = (x0 - x) / len;
		int yy = (y0 - y) / len;
		xi = xx;
		yi = yy;
	}
}