#include "Button.h"

Button::Button(int xx, int yy, int ww, int hh, UINT c, const string& s, HGE* p, hgeFont* f)
	: x(xx), y(yy), wid(ww), hei(hh), str(s), hge(p), fnt(f), color(c), mouseOn(false), lastck(false)
{
	htxt = 0;
	spr = 0;
}

Button::~Button()
{
	rmpic();
}

void Button::Render()
{

	if (spr)
	{
		spr->Render(x, y);
		if (mouseOn)
		{
			hge->Gfx_RenderLine(x, y, x + wid, y, 0xFFFF0000);
			hge->Gfx_RenderLine(x, y, x, y + hei, 0xFFFF0000);
			hge->Gfx_RenderLine(x, y + hei, x + wid, y + hei, 0xFFFF0000);
			hge->Gfx_RenderLine(x + wid, y, x + wid, y + hei, 0xFFFF0000);
		}
	}
	else
	{
		UINT nclr;
		if (mouseOn) nclr = 0xFFFF0000;
		else nclr = color;
		hge->Gfx_RenderLine(x, y, x + wid, y, nclr);
		hge->Gfx_RenderLine(x, y, x, y + hei, nclr);
		hge->Gfx_RenderLine(x, y + hei, x + wid, y + hei, nclr);
		hge->Gfx_RenderLine(x + wid, y, x + wid, y + hei, nclr);
		if (fnt) fnt->Render(x, y, 0, str.c_str());
	}
}

bool Button::Clicked()
{
	bool ck;
	if (hge->Input_GetKeyState(HGEK_LBUTTON))
	{
		float xx, yy;
		hge->Input_GetMousePos(&xx, &yy);
		ck = (xx >= x && xx <= x + wid && yy >= y && yy <= y + hei);
	}
	else ck = false;

	bool retn = lastck && !ck;
	lastck = ck;
	return retn;
}

bool Button::setpic(const string& fname)
{
	rmpic();

	htxt = hge->Texture_Load(fname.c_str());
	if (!htxt) return false;
	spr = new hgeSprite(htxt, 0, 0, wid, hei);

	if(!spr)
	{
		hge->Texture_Free(htxt);
		return false;
	}

	return true;
}

void Button::rmpic()
{
	if (spr)
	{
		delete spr;
		spr = 0;
	}
	if (htxt)
	{
		hge->Texture_Free(htxt);
		htxt = 0;
	}
}

void Button::JdgMsOn()
{
	float xx, yy;
	hge->Input_GetMousePos(&xx, &yy);
	mouseOn = (xx >= x && xx <= x + wid && yy >= y && yy <= y + hei);
}