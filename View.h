// View.h
#ifndef MY_VIEW_H
#define MY_VIEW_H
#include <vector>
#include <string>
#include <windows.h>
#include <commctrl.h>
#include "FiveInARow.h"

int mylog(int n);								// 特别地：mylog(0)为0
unsigned long ColormapHsv(int n, int Num);		// 仿MatLab的Hsv颜色设置
COLORREF InvColor(COLORREF color);				// 返回给定颜色的补色

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void WinReSizing(HWND hwnd);
void Draw(HWND hwnd, HDC hdc, SIZE size, const FiveInARow &g);
void DrawBlock(HDC hdc, int left, int top, int right, int bottom, int num);

#endif
