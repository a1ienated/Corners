#pragma once
#include <string>
#include <hge.h>
#include <hgefont.h>

using namespace std;

class Button
{
private:

	int x;
	int y;
	int wid;
	int hei;
	UINT color;
	bool mouseOn;
	bool lastck;
	HGE* hge;
	hgeFont* fnt;
	string str;

	HTEXTURE htxt;
	hgeSprite* spr;

	Button(Button& b);

public:

	void Render();
	bool Clicked();

	Button(int xx, int yy, int ww, int hh, UINT c, const string& s = "", HGE* p = 0, hgeFont* f = 0);
	~Button();

	bool setpic(const string& fname);
	void rmpic();

	void JdgMsOn();

};



