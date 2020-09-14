#include "Board.h"

Board::Board(int _winPosX, int _winPosY, int _cellSize, HGE* _hge)
	: winPosX(_winPosX), winPosY(_winPosY), sellSize(_cellSize), hge(_hge)
{
	reset();
	loadsrc();
}

Board::~Board() { delsrc(); }

void Board::reset()
{
	memset(field, FigureColor::NONE_COLOR, BOARD_LEN * BOARD_LEN); // Clear field
	curColor = FigureColor::WHITE;
	endGame = false;
}

ResultGame Board::checkFinishGame()
{
	int whiteNumber = 0, blackNumber = 0;

	// Searching for whites in the black house
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (field[i][j] == FigureColor::WHITE)
				whiteNumber++;

	// Searching for blacks in the white house
	for (int i = 5; i < 8; i++)
		for (int j = 5; j < 8; j++)
			if (field[i][j] == FigureColor::BLACK)
				blackNumber++;

	return (whiteNumber == 9) ? (ResultGame::WHITE_WIN) :
		((blackNumber == 9) ? (ResultGame::BLACK_WIN) :
			(((blackNumber == 9) && (whiteNumber == 9)) ? (ResultGame::EQUAL_WIN) : (ResultGame::NO_WIN)));
}

FigureColor Board::getColor(int x, int y)
{
	return FigureColor(field[y][x]);
}

bool Board::isAvailableToMove(int row, int col, int curCellRow, int curCellCol, FigureColor c)
{
	if (field[row][col] != FigureColor::NONE_COLOR || (c != FigureColor::BLACK && c != FigureColor::WHITE))
		return false;

	if ((row > curCellRow + 1) || (row < curCellRow - 1) || (col > curCellCol + 1) || (col < curCellCol - 1))
		return false;

	if (((row <= curCellRow - 1) && (col <= curCellCol - 1)) || ((row >= curCellRow + 1) && (col <= curCellCol - 1)))
		return false;

	if (((row <= curCellRow - 1) && (col >= curCellCol + 1)) || ((row >= curCellRow + 1) && (col >= curCellCol + 1)))
		return false;

	return true;
}

bool Board::moveFigure(int row, int col, int curCellRow, int curCellCol, FigureColor s)
{
	if (isAvailableToMove(row, col, curCellRow, curCellCol, s))
	{
		field[row][col] = s;
		field[curCellRow][curCellCol] = FigureColor::NONE_COLOR;
		return true;
	}

	return false;
}

bool Board::isSelectCell(size_t row, size_t col, size_t curCellRow, size_t curCellCol, FigureColor c)
{
	// Check for color matching and cell change
	if ((field[row][col] == c) && ((curCellRow != row) || (curCellCol != col)))
	{
		return true;
	}

	return false;
}

bool Board::isEmptyCell(GridLocation gl)
{
	return field[gl.row][gl.col] == FigureColor::NONE_COLOR;
}

bool Board::isBlackCellClosed(GridLocation gl)
{
	if ((gl.row < 5) && (gl.col < 5))
		return false;

	return field[gl.row][gl.col] == FigureColor::BLACK;
}

char* Board::getWinMsg(ResultGame result)
{
	char *s;
	if (result == BLACK_WIN) s = const_cast<char*>("Black wins!");
	else if (result == WHITE_WIN) s = const_cast < char*>("White wins!");
	else if (result == EQUAL_WIN) s = const_cast < char*>("Equal win!");
	else return s = const_cast < char*>("");

	return s;
}

void Board::loadsrc()
{
	BTxt = hge->Texture_Load("img//Black.png");
	WTxt = hge->Texture_Load("img//White.png");
	BSpr = new hgeSprite(BTxt, 0, 0, sellSize - 2, sellSize - 2);
	WSpr = new hgeSprite(WTxt, 0, 0, sellSize - 2, sellSize - 2);
}

void Board::delsrc()
{
	hge->Texture_Free(BTxt);
	hge->Texture_Free(WTxt);
	delete BSpr;
	delete WSpr;
}

void Board::Render()
{
	// Board
	for (int i = 0; i < 9; i++)
	{
		hge->Gfx_RenderLine(winPosX, winPosY + i * sellSize, winPosX + 8 * sellSize, winPosY + i * sellSize, ARGB(255, 0, 0, 0));
		hge->Gfx_RenderLine(winPosX + i * sellSize, winPosY, winPosX + i * sellSize, winPosY + 8 * sellSize, ARGB(255, 0, 0, 0));
	}

	// Figure
	for (int i = 0; i < BOARD_LEN; i++)//col
	{
		for (int j = 0; j < BOARD_LEN; j++)//row
		{
			int xx = winPosX + j * sellSize;
			int yy = winPosY + i * sellSize;
			FigureColor s = getColor(j, i);
			if (s == BLACK)
			{
				BSpr->Render(xx, yy);
			}
			else if (s == WHITE)
			{
				WSpr->Render(xx, yy);
			}
		}
	}
}

bool Board::Clicked(size_t& row, size_t& col)
{
	if (hge->Input_GetKeyState(HGEK_LBUTTON))
	{
		float x0, y0;
		hge->Input_GetMousePos(&x0, &y0);
		if (x0 >= winPosX && y0 >= winPosY && x0 < winPosX + BOARD_LEN * sellSize && y0 < winPosY + BOARD_LEN * sellSize)
		{
			// calc cursor cell
			row = (y0 - winPosY) / sellSize;
			col = (x0 - winPosX) / sellSize;
			return true;
		}
		else return false;
	}
	else return false;
}
