#pragma once
#include <hge.h>
#include <hgesprite.h>
#include "Common.h"

class Board
{
public:
	Board(int _winPosX, int _winPosY, int _cellSize, HGE* _hge = 0);
	Board(Board& cb) = delete;
	~Board();

	bool Clicked(size_t& x, size_t& y);
	void Render();
	void reset();

	static char* getWinMsg(ResultGame s);
	FigureColor getCurrentColor() const { return curColor; }
	FigureColor getColor(int x, int y);
	void setCurrentColor(FigureColor c) { curColor = c; }

	bool moveFigure(int row, int col, int curCellRow, int curCellCol, FigureColor c);
	bool isAvailableToMove(int row, int col, int curCellRow, int curCellCol, FigureColor c);
	bool isSelectCell(size_t row, size_t col, size_t curCellRow, size_t curCellCol, FigureColor c);
	bool isEmptyCell(GridLocation gl);
	bool isBlackCellClosed(GridLocation gl);
	ResultGame checkFinishGame();

	char field[BOARD_LEN][BOARD_LEN];
	bool endGame;
	
private:
	int winPosX;
	int winPosY;
	int sellSize;
	HGE* hge;
	HTEXTURE BTxt;
	HTEXTURE WTxt;
	hgeSprite* BSpr;
	hgeSprite* WSpr;
	FigureColor curColor; //current color

	void loadsrc();
	void delsrc();
};

