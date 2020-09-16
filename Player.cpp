#include "Player.h"

Player::Player(Board* cb, FigureColor _color) : board(cb), color(_color), countStep(0), numberFig(0),
		currentPos{ 0, 0 } {}

Human::Human(Board* board, FigureColor _color) : Player(board, _color), isSelect(false)
{
	reset();
}

PlayResult Human::step()
{
	size_t row, col;
	if (board->Clicked(row, col))
	{
		if (board->isSelectCell(row, col, currentPos.row, currentPos.col, color))
		{
			currentPos.row = row;
			currentPos.col = col;

			isSelect = true;
			return PlayResult::NO_MOVE;
		}

		if (isSelect && board->isAvailableToMove(row, col, currentPos.row, currentPos.col, color))
		{
			board->field[row][col] = color;
			board->field[currentPos.row][currentPos.col] = FigureColor::NONE_COLOR;

			++countStep;
			isSelect = false;
			return PlayResult::SUCCESS;
		}
	}

	return PlayResult::NO_MOVE;
}

void Human::reset()
{
	fillFigures();
	currentPos.row = currentPos.col = countStep = 0;
}

void Human::fillFigures()
{
	for (int i = 7; i > whiteNum; i--)
		for (int j = 7; j > whiteNum; j--)
		{
			board->field[i][j] = color;
			++numberFig;
		}
}

// AI player-------------------------------------------------------------------

AI::AI(Board* cb, FigureColor _color) : Player(cb, _color)
{
	reset();
}

size_t AI::findMinDistance(const std::vector<GridLocation> figures, const GridLocation& goal)
{
	size_t index = 0;
	int a = 0, b = 0, summ = 200;
	for (auto it = figures.begin(); it != figures.end(); it++)
	{
		if (goal.row <= it->row)
			a = it->row - goal.row;
		else
			a = goal.row - it->row;

		if (goal.col <= it->col)
			b = it->col - goal.col;
		else
			b = goal.col - it->col;

		if (summ > a + b)
		{
			summ = a + b;
			index = std::distance(figures.begin(), it);
		}
	}

	return index;
}

PlayResult AI::step()
{
	Graph graph(8, 8);
	findBarrier(graph, currentPos);
	GridLocation nextStep;
	std::pair<bool, GridLocation> gl;
	gl.first = false;

	// Target selection
	for (auto it = goals.begin(); it != goals.end(); it++)
	{
		if (board->isEmptyCell(*it))
		{
			currentGoal = std::distance(goals.begin(), it);
			break;
		}

		if (board->isBlackCellClosed(*it) && !goals.empty() && goals.size() > 1 && (goals.begin() == it))
		{
			if (figures.size() > 1)
			{
				figures.erase(std::remove(figures.begin(), figures.end(), *it), figures.end());
				currentFigure = (currentFigure > 0) ? (currentFigure - 1) : (currentFigure);
			}

			goals.erase(it);
			currentGoal = (currentGoal > 0) ? (currentGoal - 1) : (currentGoal);
			break;
		}
	}

	// Figure selection
	nextStep = searchGoal(graph, figures.at(currentFigure), goals.at(currentGoal)).second;
	if ((nextStep == figures.at(currentFigure)) || ((countStep % 3) == 0))
	{
		for (auto it = figures.begin(); it != figures.end(); it++)
		{
			gl = searchGoal(graph, (*it), goals.at(currentGoal));
			if (gl.first)
			{
				currentFigure = std::distance(figures.begin(), it);
				nextStep = gl.second;
				break;
			}
		}
	}
	else
	{
		++currentFigure;
		if ((currentFigure > figures.size() - 1))
		{
			currentFigure = 0;
		}
		nextStep = searchGoal(graph, figures.at(currentFigure), goals.at(currentGoal)).second;
	}

	if(board->moveFigure(nextStep.row, nextStep.col, figures.at(currentFigure).row, figures.at(currentFigure).col, color))
		figures[currentFigure] = nextStep;

	++countStep;
	return PlayResult::SUCCESS;
}

void AI::reset()
{
	fillGoals();
	fillFigures();
	currentPos.row = currentPos.col = currentFigure = currentGoal = countStep = 0;
}

void AI::findBarrier(Graph& graph, GridLocation& current)
{
	for (size_t i = 0; i < 8; i++)
		for (size_t j = 0; j < 8; j++)
			if ((board->field[i][j] != FigureColor::NONE_COLOR))
			{
				addBarrier(graph, i, j);
			}
}

void AI::fillFigures()
{
	if (!figures.empty())
		figures.clear();

	for (int i = blackNum; i >= 0; i--)
		for (int j = blackNum; j >= 0; j--)
		{
			board->field[i][j] = color;
			figures.push_back(GridLocation{ i, j });
		}

	numberFig = figures.size();
}

void AI::fillGoals()
{
	if (!goals.empty())
		goals.clear();

	for (int i = 7; i > whiteNum; i--)
		for (int j = 7; j > whiteNum; j--)
		{
			goals.push_back(GridLocation{ i, j });
		}
}

std::pair<bool, GridLocation> AI::searchGoal(const Graph& graph, GridLocation start, GridLocation goal)
{
	std::pair<bool, GridLocation> gl;
	gl.first = false; gl.second = start;

	std::queue<GridLocation> frontier;
	frontier.push(start);
	std::unordered_map<GridLocation, GridLocation> cameFrom;
	cameFrom[start] = start;

	while (!frontier.empty()) {
		auto current = frontier.front();
		frontier.pop();

		if (current == goal) {
			break;
		}

		for (auto& next : graph.neighbors(current)) {
			if (!cameFrom.count(next)) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}

	// Find back path
	if (cameFrom.empty())
	{
		return gl;
	}

	auto it = cameFrom.find(goal);
	std::pair<GridLocation, GridLocation> p;
	if (it != cameFrom.end())
	{
		p = *it;
		gl.first = true;
	}
	else
	{
		auto max = get_max(cameFrom);
		p = max;
	}

	while (p.second != start) {
		auto it = cameFrom.find(p.second);
		p = *it;
	}

	gl.second = p.first;

	return gl;
}

Graph::Graph(int _width, int _height) : width(_width), height(_height)
{
	DIRS = { GridLocation{1, 0}, GridLocation{0, -1}, GridLocation{-1, 0}, GridLocation{0, 1} };
}

bool Graph::inBounds(GridLocation id) const
{
	return 0 <= id.row && id.row < width && 0 <= id.col && id.col < height;
}

bool Graph::passable(GridLocation id) const
{
	return obstacles.find(id) == obstacles.end();
}

std::vector<GridLocation> Graph::neighbors(GridLocation id) const
{
	std::vector<GridLocation> results;

	for (GridLocation dir : DIRS)
	{
		GridLocation next{ id.row + dir.row, id.col + dir.col };
		if (inBounds(next) && passable(next))
		{
			results.push_back(next);
		}
	}

	if ((id.row + id.col) % 2 == 0)
		std::reverse(results.begin(), results.end());

	return results;
}

