#pragma once
#include "Board.h"
#include "Common.h"
#include "Board.h"

class Player
{
public:
	Player(Board* cb, FigureColor _color);

	size_t getCountStep() { return countStep; }
	size_t getCellRow() { return currentPos.row; }
	size_t getCellCol() { return currentPos.col; }
	size_t getNumberFig() { return numberFig; }
	virtual PlayResult step() = 0;
	virtual void reset() = 0;

protected:
	const int whiteNum = 4;
	const int blackNum = 2;

	GridLocation currentPos;
	Board* board;
	FigureColor color;
	size_t countStep;
	size_t numberFig;
};

class Human : public Player
{
public:
	Human(Board* board, FigureColor _color);
	PlayResult step() override;
	void reset() override;
	void fillFigures();

private:
	bool isSelect;
};

class AI : public Player
{
public:
	AI(Board* cb, FigureColor _color);
	PlayResult step() override;
	void reset() override;
	void fillFigures();
	void fillGoals();

	size_t findMinDistance(const std::vector<GridLocation> figures, const GridLocation& goal);
	std::pair<bool, GridLocation> searchGoal(const Graph& graph, GridLocation start, GridLocation goal);
	void findBarrier(Graph& graph, GridLocation& current);

private:
	std::vector<GridLocation> goals;
	std::vector<GridLocation> figures;
	//std::vector<std::pair<size_t, GridLocation>> figures;
	size_t currentFigure;
	size_t currentGoal;
};

