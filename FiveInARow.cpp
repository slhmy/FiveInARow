#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <iomanip>
#include "FiveInARow.h"
using namespace std;

FiveInARow::FiveInARow()
{
	memset(wins, false, sizeof(wins));
	memset(display, 0, sizeof(display));
	memset(cb.a, 0, sizeof(cb.a));
	memset(cursor, 0, sizeof(cursor));

	case_n = 0;
	for (int i = 1; i <= width  ; i++)
    	for (int j = 1; j <= width - 4; j++){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i][j+k][case_n] = true;
        	case_n++;
    	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = 1; j <= width; j++){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i+k][j][case_n] = true;
        	case_n++;
    	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = 1; j <= width - 4; j++){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i+k][j+k][case_n] = true;
        	case_n++;
    	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = width; j > 4; j--){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i+k][j-k][case_n] = true;
        	case_n++;
    	}
    display[(width+1)/2][(width+1)/2] = 5;
    cursor[(width+1)/2][(width+1)/2] = 8;
    X = (width+1)/2; Y = (width+1)/2;
    OLD_X = X;    OLD_Y = Y;    flag = false;
    u = Y; v = X; cb.player = 1;
    mem.push_back(cb); step = 0;
}

void FiveInARow::Init()
{
    memset(wins, false, sizeof(wins));
	memset(display, 0, sizeof(display));
	memset(cb.a, 0, sizeof(cb.a));
	memset(cursor, 0, sizeof(cursor));

	case_n = 0;
	for (int i = 1; i <= width  ; i++)
    	for (int j = 1; j <= width - 4; j++){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i][j+k][case_n] = true;
        	case_n++;
    	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = 1; j <= width; j++){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i+k][j][case_n] = true;
        	case_n++;
    	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = 1; j <= width - 4; j++){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i+k][j+k][case_n] = true;
        	case_n++;
    	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = width; j > 4; j--){
   	   	  	for (int k = 0; k < 5; k++)
            	wins[i+k][j-k][case_n] = true;
        	case_n++;
    	}
    display[(width+1)/2][(width+1)/2] = 5;
    cursor[(width+1)/2][(width+1)/2] = 8;
    X = (width+1)/2; Y = (width+1)/2;
    OLD_X = X;    OLD_Y = Y;    flag = false;
    u = Y; v = X; cb.player = 1;
    mem.push_back(cb); step = 0;
}

void FiveInARow::Restart()
{
    mem.clear();
	memset(display, 0, sizeof(display));
	memset(cb.a, 0, sizeof(cb.a));
	memset(cursor, 0, sizeof(cursor));
    display[(width+1)/2][(width+1)/2] = 5;
    cursor[(width+1)/2][(width+1)/2] = 8;
    X = (width+1)/2; Y = (width+1)/2;
    u = Y; v = X; cb.player = 1; AI_num = 2;
    mem.push_back(cb); step = 0;
    OLD_X = X;    OLD_Y = Y;
}

void FiveInARow::Print()
{
    for(int i = 1; i <= width; i++)
  		for(int j = 1; j <= width; j++)
  		{
  			if(cursor[i][j] == 8)
            {
                if(cb.a[i][j] == 0)	    display[i][j] = 5;
                else if(cb.a[i][j] == 1)	display[i][j] = 3;
                else if(cb.a[i][j] == 2)	display[i][j] = 4;
            }
  			else if(cb.a[i][j] == 0)	display[i][j] = 0;
  			else if(cb.a[i][j] == 1)	display[i][j] = 1;
  			else if(cb.a[i][j] == 2)	display[i][j] = 2;
  		}
}

void FiveInARow::ChangeCursor()
{
    cursor[OLD_Y][OLD_X] = 0;
    cursor[Y][X] = 8;
    OLD_X = X;    OLD_Y = Y;
    u = Y; v = X;
}

void FiveInARow::Up()
{
    Y=Y-1;
  	cursor[Y][X]=8;
  	cursor[Y+1][X]=0;
  	u = Y; v = X;
}

void FiveInARow::Down()
{
    Y=Y+1;
  	/*cursor[Y][X]=8;
  	cursor[Y-1][X]=0;
  	u = Y; v = X;*/
}

void FiveInARow::Left()
{
    X=X-1;
  	/*cursor[Y][X]=8;
  	cursor[Y][X+1]=0;
  	u = Y; v = X;*/
}

void FiveInARow::Right()
{
    X=X+1;
    /*cursor[Y][X]=8;
  	cursor[Y][X-1]=0;
  	u = Y; v = X;*/
}

bool FiveInARow::Add_w()
{
    if (width < MAXW)
    {
        width++;
        return true;
    }
    else return false;
}

bool FiveInARow::Minus_w()
{
    if (width > MINW)
    {
        width--;
        return true;
    }
    else return false;
}

void FiveInARow::PutC()
{
    if(cb.a[u][v] == 0)
    {
        if(cb.player==1)
        {
            cb.a[u][v]=1;
            cb.player=2;
        }
        else if(cb.player==2)
        {
            cb.a[u][v]=2;
            cb.player=1;
        }
    }
    u = Y; v = X;
}

void FiveInARow::Memorize()
{
    mem.push_back(cb);
    step++;
}

bool FiveInARow::Recall()
{
    if (step > 0)
    {
        mem.erase(mem.begin()+step);
        step--;
        cb = mem[step];
        if (cb.player == AI_num)
        {
            mem.erase(mem.begin()+step);
            step--;
            cb = mem[step];
        }
        if (step == -1)
        {
            mem.push_back(cb);
            step++;
            return false;
        }
        return true;
    }
}

void FiveInARow::AI_cal()
{
    memset(Black_win, 0, sizeof(Black_win));
	memset(White_win, 0, sizeof(White_win));
	int c1, c2, count = 0, P_num;
	if (AI_num == 2) P_num = 1;
	else P_num = 2;
	for (int i = 1; i <= width; i++){
    	for (int j = 1; j <= width - 4; j++){
    		c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i][j+k] == 1) c1++;
   	   	  		if (cb.a[i][j+k] == 2) c2++;
        	}
        	if (c1 == 0) White_win[count] = c2;
        	else White_win[count] = -1;
			if (c2 == 0) Black_win[count] = c1;
        	else Black_win[count] = -1;
        	count++;
    	}
	}
	for (int i = 1; i <= width - 4; i++){
    	for (int j = 1; j <= width; j++){
        	c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i+k][j] == 1) c1++;
   	   	  		if (cb.a[i+k][j] == 2) c2++;
        	}
        	if (c1 == 0) White_win[count] = c2;
        	else White_win[count] = -1;
			if (c2 == 0) Black_win[count] = c1;
        	else Black_win[count] = -1;
        	count++;
    	}
	}
	for (int i = 1; i <= width - 4; i++){
    	for (int j = 1; j <= width - 4; j++){
        	c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i+k][j+k] == 1) c1++;
   	   	  		if (cb.a[i+k][j+k] == 2) c2++;
        	}
        	if (c1 == 0) White_win[count] = c2;
        	else White_win[count] = -1;
			if (c2 == 0) Black_win[count] = c1;
        	else Black_win[count] = -1;
        	count++;
    	}
	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = width; j > 4; j--){
        	c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i+k][j-k] == 1) c1++;
   	   	  		if (cb.a[i+k][j-k] == 2) c2++;
        	}
        	if (c1 == 0) White_win[count] = c2;
        	else White_win[count] = -1;
			if (c2 == 0) Black_win[count] = c1;
        	else Black_win[count] = -1;
        	count++;
    	}

	int maxn = -1;
	memset(Black_score, 0, sizeof(Black_score));
	memset(White_score, 0, sizeof(White_score));
	for (int i = 1; i <= width; i++)
        for (int j = 1; j <= width; j++){
        	if (0 == cb.a[i][j]){
            	for (int k = 0; k < case_n; k++){
                	if (wins[i][j][k]){
                        if      (0 == Black_win[k]) Black_score[i][j] += 7;
                	    else if (1 == Black_win[k])	Black_score[i][j] += 35;
                	    else if (2 == Black_win[k])	Black_score[i][j] += 800;
                	    else if (3 == Black_win[k])	Black_score[i][j] += 15000;
                	    else if (4 == Black_win[k])	Black_score[i][j] += 800000;
                        if      (0 == White_win[k]) White_score[i][j] += 7;
                   		else if (1 == White_win[k])	White_score[i][j] += 15;
                    	else if (2 == White_win[k])	White_score[i][j] += 400;
                    	else if (3 == White_win[k])	White_score[i][j] += 1800;
                    	else if (4 == White_win[k])	White_score[i][j] += 100000;
                	}
            	}
        	}
            if (Black_score[i][j] > maxn){ maxn = Black_score[i][j]; u = i;	v = j; }
			else if (Black_score[i][j] == maxn){
				if (White_score[i][j] > White_score[u][v]){ u = i; v = j; }
				if (White_score[i][j] == White_score[u][v]){ int r = rand()%1; if(r == 1) { u = i; v = j; } }
            }
            if (White_score[i][j] > maxn){ maxn = White_score[i][j]; u = i; v = j; }
			else if (White_score[i][j] == maxn){
               	if (Black_score[i][j] > Black_score[u][v]){ u = i; v = j; }
               	if (Black_score[i][j] == Black_score[u][v]){ int r = rand()%1; if(r == 1) { u = i; v = j; } }
            }
        }
    cout << endl; cout <<endl;
    for (int i = 1; i <= width; i++)
    {
        for (int j = 1; j <= width; j++)
            std::cout << setw(6) << Black_score[i][j] << ' ';
        std::cout << endl;
    }
    cout << endl;
    for (int i = 1; i <= width; i++)
    {
        for (int j = 1; j <= width; j++)
            std::cout << setw(6) << White_score[i][j] << ' ';
        std::cout << endl;
    }
    cout << width << endl;
}



int FiveInARow::Pd()
{
	int c1, c2, count = 0;
	for (int i = 1; i <= width; i++){
    	for (int j = 1; j <= width - 4; j++){
    		c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i][j+k] == 1) c1++;
   	   	  		if (cb.a[i][j+k] == 2) c2++;
        	}
        	if (c1 == 5) return 1;
			if (c2 == 5) return 2;
        	count++;
    	}
	}
	for (int i = 1; i <= width - 4; i++){
    	for (int j = 1; j <= width; j++){
        	c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i+k][j] == 1) c1++;
   	   	  		if (cb.a[i+k][j] == 2) c2++;
        	}
        	if (c1 == 5) return 1;
			if (c2 == 5) return 2;
        	count++;
    	}
	}
	for (int i = 1; i <= width - 4; i++){
    	for (int j = 1; j <= width - 4; j++){
        	c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i+k][j+k] == 1) c1++;
   	   	  		if (cb.a[i+k][j+k] == 2) c2++;
        	}
        	if (c1 == 5) return 1;
			if (c2 == 5) return 2;
        	count++;
    	}
	}
	for (int i = 1; i <= width - 4; i++)
    	for (int j = width; j > 4; j--){
        	c1 = 0; c2 = 0;
   	   	  	for (int k = 0; k < 5; k++){
   	   	  		if (cb.a[i+k][j-k] == 1) c1++;
   	   	  		if (cb.a[i+k][j-k] == 2) c2++;
        	}
        	if (c1 == 5) return 1;
			if (c2 == 5) return 2;
        	count++;
    	}
    return 0;
}
