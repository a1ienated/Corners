#include "Common.h"

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

void Graph::addBarrier(char(*_field)[BOARD_LEN])
{
	for (int i = 0; i < BOARD_LEN; i++)
		for (int j = 0; j < BOARD_LEN; j++)
			if ((_field[i][j] != FigureColor::NONE_COLOR))
			{
				obstacles.insert(GridLocation{ i, j });
			}
}