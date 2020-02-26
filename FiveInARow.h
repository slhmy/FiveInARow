// Game2048.h
#ifndef FiveInARow_H
#define FiveInARow_H
#include <vector>

const int MAXW = 30;
const int MINW = 5;

struct chessboard
{
    int a[55][55];
    int player = 1;
};

class FiveInARow
{
public:
	FiveInARow();
	void Up(), Down(), Left(), Right(), PutC(), Print(), AI_cal(), Memorize(), Restart(), Init(), ChangeCursor();
	bool Add_w(), Minus_w();
	bool Recall();
	int Pd();
    bool flag;
    int display[55][55], cursor[55][55];
    int width = 15, step;
    int AI_num = 2;
    bool wins[55][55][100005];
    int case_n;
    chessboard cb;
    std::vector <chessboard> mem;
    int Black_win[100005], Black_score[55][55], White_win[100005], White_score[55][55];
    int u, v, X, Y, OLD_X, OLD_Y;
};

#endif
