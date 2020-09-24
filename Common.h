#pragma once
#include <functional>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <array>
#include <queue>
#include <set>
#include <map>
#include <utility>

constexpr auto WIN_WIDTH = 700;
constexpr auto WIN_HEIGHT = 540;
constexpr auto BOARD_POS_X = 30;
constexpr auto BOARD_POS_Y = 30;
constexpr auto CELL_SIZE = 60;
constexpr auto BOARD_LEN = 8;

enum FigureColor { BLACK = -1, NONE_COLOR, WHITE };
enum ResultGame { NO_WIN, BLACK_WIN, WHITE_WIN, EQUAL_WIN };
enum class PlayResult { NO_MOVE = -1, FAILED, SUCCESS };

constexpr auto BTN_X = (BOARD_POS_X + BOARD_LEN * CELL_SIZE + 20);
constexpr auto BTN_WIDTH = 60;
constexpr auto BTN_HEIGHT = 30;
constexpr auto ROW_0 = 60;
constexpr auto ROW_1 = 100;
constexpr auto ROW_2 = 140;
constexpr auto ROW_3 = 180;
constexpr auto ROW_4 = 220;
constexpr auto ROW_5 = 260;
constexpr auto ROW_6 = 300;
constexpr auto ROW_7 = 340;

struct GridLocation
{
	int row, col;
};

inline bool operator == (GridLocation a, GridLocation b) {
	return a.row == b.row && a.col == b.col;
}

inline bool operator != (GridLocation a, GridLocation b) {
	return !(a == b);
}
inline bool operator < (GridLocation a, GridLocation b) {
	return std::tie(a.row, a.col) < std::tie(b.row, b.col);
}

inline bool operator > (GridLocation a, GridLocation b) {
	return std::tie(a.row, a.col) > std::tie(b.row, b.col);
}

namespace std {
	/* implement hash function so we can put GridLocation into an unordered_set */
	template <> struct hash<GridLocation>
	{
		typedef GridLocation argument_type;
		typedef std::size_t result_type;
		std::size_t operator()(const GridLocation& id) const noexcept {
			return std::hash<int>()(id.row ^ (id.col << 4));
		}
	};
}

struct comparator {
	bool operator() (const GridLocation& a, const GridLocation& b) const {
		return a > b;
	}
};

inline std::pair<GridLocation, GridLocation> get_max(const std::unordered_map<GridLocation, GridLocation> x) {
	using pairtype = std::pair<GridLocation, GridLocation>;
	return *std::max_element(x.begin(), x.end(), [](const pairtype& p1, const pairtype& p2) {
		return p1 < p2; });
}

struct Graph
{
	Graph(int _width, int _height);
	std::vector<GridLocation> neighbors(GridLocation id) const;
	void addBarrier(char(*_field)[BOARD_LEN]);

private:
	int width, height;
	std::array<GridLocation, 4> DIRS;
	std::unordered_set<GridLocation> obstacles;
	bool inBounds(GridLocation id) const;
	bool passable(GridLocation id) const;
};





