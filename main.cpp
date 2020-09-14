// main.cpp : Defines the entry point for the application.
#include <Windows.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <hge.h>
#include <hgegui.h>
#include <hgefont.h>

#include "Common.h"
#include "Board.h"
#include "Player.h"
#include "Button.h"
#include "Board.h"
#include "Cursor.h"
#include "Timer.h"
//#include "FileDlg.h"

using namespace std;

HGE* HGEInit();
void HGEFinit(HGE* h);

// Forward declarations of functions included in this code module:
bool FrameFunc();
bool RenderFunc();

// Global Variables:
HGE* hge = 0;
HTEXTURE TiTxt;
hgeFont* HFont;

// Checkerboard object
//Board board;
Player* player;
Human* wplayer;
AI* bplayer;

// Game interface
Board* board;
Cursor* cursor;
Button* exitBtn;
Button* resetBtn;
Timer* timer;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Engine and handle initialization
	hge = HGEInit();
	if (!hge->System_Initiate())
	{
		MessageBoxA(0, "HGE initialization failed!", 0, 0);
		return 0;
	}
	TiTxt = hge->Texture_Load("img//title.jpg");
	HFont = new hgeFont("img//hel.fnt");
	HFont->SetColor(ARGB(255, 0, 0, 0));

	// Rendering control initialization
	board = new Board(BOARD_POS_X, BOARD_POS_Y, CELL_SIZE, hge);
	cursor = new Cursor(BOARD_POS_X, BOARD_POS_Y, CELL_SIZE, 0xFF0000FF, hge); //blue
	resetBtn = new Button(BTN_X, ROW_6, BTN_WIDTH, BTN_HEIGHT, ARGB(255, 0, 0, 0), "Reset", hge, HFont);
	exitBtn = new Button(BTN_X, ROW_7, BTN_WIDTH, BTN_HEIGHT, ARGB(255, 0, 0, 0), "Exit", hge, HFont);
	timer = new Timer(BTN_X, ROW_3, hge, HFont);

	// Players init
	wplayer = new Human(board, FigureColor::WHITE);
	bplayer = new AI(board, FigureColor::BLACK);
	player = wplayer;
	// Start
	hge->System_Start();

	delete HFont;
	hge->Texture_Free(TiTxt);
	delete board, cursor;
	delete exitBtn, timer;
	delete wplayer, bplayer;

	HGEFinit(hge);
	return 0;
}

HGE* HGEInit()
{
	HGE* hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_SCREENWIDTH, WIN_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, WIN_HEIGHT);
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, false);
	hge->System_SetState(HGE_HIDEMOUSE, false);
	hge->System_SetState(HGE_TITLE, "Corners");
	hge->System_SetState(HGE_SHOWSPLASH, false);
	return hge;
}

void HGEFinit(HGE* hge)
{
	hge->System_Shutdown();
	hge->Release();
}

bool FrameFunc()
{
	ResultGame s = NO_WIN;
	PlayResult res;

	// Exit program
	if (exitBtn->Clicked())
		return true;

	if (resetBtn->Clicked())
	{
		board->reset();
		bplayer->reset();
		wplayer->reset();
		timer->reset();
		return false;
	}

	//Refresh cursor
	cursor->Move();

	if (board->getCurrentColor() == BLACK)
		player = bplayer;
	else player = wplayer;

	res = player->step();
	if (res == PlayResult::SUCCESS)
	{
		if (player->getCountStep() > 92)
			s = board->checkFinishGame();

		if (s != ResultGame::NO_WIN && !board->endGame)
		{
			board->endGame = true;
			MessageBoxA(0, Board::getWinMsg(s), "", 0);
		}

		board->setCurrentColor((board->getCurrentColor() == BLACK) ? (WHITE) : (BLACK));
	}

	return false;
}

bool RenderFunc()
{
	uint32_t green = ARGB(255, 0x4c, 0x99, 0x00);
	uint32_t orange = ARGB(255, 0xcc, 0x66, 0x00);
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(orange);

	board->Render();
	cursor->Render();

	// Number of steps
	HFont->printf(BTN_X, ROW_0, 0, "Black: %d", bplayer->getCountStep());
	HFont->printf(BTN_X, ROW_1, 0, "White: %d", wplayer->getCountStep());

	HFont->printf(BTN_X, ROW_4, 0, "Row: %d", player->getCellRow());
	HFont->printf(BTN_X, ROW_5, 0, "Col: %d", player->getCellCol());

	std::string p = (board->getCurrentColor() == BLACK) ? "Black" : "White";
	HFont->printf(BTN_X, ROW_2, 0, "Now: %s", p.c_str());

	timer->Render();
	exitBtn->Render();
	resetBtn->Render();

	hge->Gfx_EndScene();
	return false;
}
