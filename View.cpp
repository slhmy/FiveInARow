// View.cpp
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <windowsx.h>
#include "View.h"
#include "resource.h"
#include "FiveInARow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("FiveInARow");
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;

	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	//wndclass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName	= TEXT("FiveInARow");
	wndclass.lpszClassName	= szAppName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox (NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName,					// window class name
						TEXT("FiveInARow"),			// window caption
						WS_OVERLAPPEDWINDOW,		// window style
						CW_USEDEFAULT,				// initial x position
						CW_USEDEFAULT,				// initial y position
						CW_USEDEFAULT,				// initial x size
						CW_USEDEFAULT,				// initial y size
						NULL,						// parent window handl
						NULL,						// window menu handle
						hInstance,					// program instance handle
						NULL);						// creation parameters
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static FiveInARow g;
	static POINT hand;
	HDC hdc;
	PAINTSTRUCT	ps;
	static SIZE size;

	switch(message)
	{
    case WM_MOUSEMOVE:
                SetCursor(LoadCursor(NULL, IDC_HAND));
                hand.x = GET_X_LPARAM(lParam);
                hand.y = GET_Y_LPARAM(lParam);
                g.X = (hand.x)/(size.cx/g.width) + 1;
                if (g.X > g.width) g.X = g.width;
                g.Y = (hand.y)/(size.cy/g.width) + 1;
                if (g.Y > g.width) g.Y = g.width;
                g.ChangeCursor();
                g.Print();
                InvalidateRect(hwnd, NULL, TRUE);
                break;
    case WM_LBUTTONDOWN:
                SetCursor(LoadCursor(NULL, IDC_HAND));
                g.PutC();
                g.Memorize();
                g.Print();
                if(g.Pd() == 1)
                {
                    if (MessageBox(hwnd, TEXT("玩家1获胜，是否重新开始游戏?(选否可以自行选择悔棋)"), TEXT("帮助"), MB_YESNO) == IDYES)
                            {
                                g.Restart();
                                InvalidateRect(hwnd, NULL, TRUE);
                            }
                }
                else
                    if (g.cb.player == g.AI_num)
                    {
                        g.AI_cal();
                        g.PutC();
                        g.Print();
                        InvalidateRect(hwnd, NULL, TRUE);
                        g.Memorize();
                        if (g.Pd() == 2)
                        {
                            if (MessageBox(hwnd, TEXT("玩家2获胜，是否重新开始游戏?(选否可以自行选择悔棋)"), TEXT("帮助"), MB_YESNO) == IDYES)
                            {
                                g.Restart();
                                InvalidateRect(hwnd, NULL, TRUE);
                            }
                        }
                    }
                    g.Print();
                    InvalidateRect(hwnd, NULL, TRUE);
                break;
	case WM_CREATE:
				WinReSizing(hwnd);
				return 0;
	case WM_SIZE:
				size.cx = LOWORD(lParam);
				size.cy = HIWORD(lParam);
				return 0;
	case WM_PAINT:
                if (!g.flag)
                {
                    g.flag = true;
                    MessageBox(hwnd, TEXT("当前为人机对战模式，可自行更换游戏模式"), TEXT("帮助"), MB_OK);
                }
				hdc = BeginPaint(hwnd, &ps);
				Draw(hwnd, hdc, size, g);
				EndPaint(hwnd, &ps);
				return 0;
	case WM_COMMAND:								// 处理菜单命令
				switch(LOWORD(wParam))
				{
				case IDM_RESTART:
						g.Restart();
						InvalidateRect(hwnd, NULL, TRUE);
						break;
				case IDM_EXIT:
                        if (MessageBox(hwnd, TEXT("真的退出吗？"), TEXT("Exit"), MB_ICONSTOP|MB_YESNO) == IDYES)
                            DestroyWindow(hwnd);
                        else
                            return 0;
						break;
                case IDM_Player_vs_Player:
                        g.AI_num = 0;
                        break;
                case IDM_Player_vs_AI:
                        if (MessageBox(hwnd, TEXT("是否选择先手"), TEXT("提示"), MB_YESNO) == IDYES)
                        {
                            g.AI_num = 2;
                        }
                        else
                        {
                            g.AI_num = 1;
                            if (g.cb.player == g.AI_num)
                            {
                                g.AI_cal();
                                g.PutC();
                                g.Print();
                                InvalidateRect(hwnd, NULL, TRUE);
                                g.Memorize();
                                if (g.Pd() == 2)
                                {
                                    if (MessageBox(hwnd, TEXT("玩家2获胜，是否重新开始游戏?(选否可以自行选择悔棋)"), TEXT("帮助"), MB_YESNO) == IDYES)
                                    {
                                        g.Restart();
                                        InvalidateRect(hwnd, NULL, TRUE);
                                    }
                                }
                            }
                            g.Print();
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                        break;
                case IDM_ADD_W:
                        if (!g.Add_w())
                        {
                            MessageBox(hwnd, TEXT("已经加到最大啦！(这么大的棋盘还满足不了你嘛？)"), TEXT("提示"), MB_OK);
                        }
                        g.Init();
                        InvalidateRect(hwnd, NULL, TRUE);
                        break;
                case IDM_MINUS_W:
                        if (!g.Minus_w())
                        {
                            MessageBox(hwnd, TEXT("已经减到最小啦！（再小就连不成五子啦！）"), TEXT("提示"), MB_OK);
                        }
                        g.Init();
                        InvalidateRect(hwnd, NULL, TRUE);
                        break;
				case IDM_HELP:
						MessageBox(hwnd, TEXT("FiveInARow主菜单中有重新开始与退出选项，Game Modes栏中可自由选择玩家对战或人机对战，Undo为悔棋键"), TEXT("帮助(F1)"), MB_OK);
						break;
                case IDM_Undo:
                        g.Recall();
                        g.Print();
                        InvalidateRect(hwnd, NULL, TRUE);
                        break;
				}
				InvalidateRect(hwnd, NULL, TRUE);	// 使客户区无效，以便重绘视口的内容
				return 0;
	case WM_KEYDOWN:								// 处理键盘消息
				switch(wParam)
				{
                case VK_F2:
                        g.Restart();
                        InvalidateRect(hwnd, NULL, TRUE);
                        break;
				case VK_F1:
						SendMessage(hwnd, WM_COMMAND, IDM_HELP, 0);
						break;
				case VK_ESCAPE:
						{
							int response = MessageBox(hwnd, TEXT("真的退出吗？"), TEXT("Exit"), MB_ICONSTOP|MB_YESNO);
							if(response==IDYES)
								SendMessage(hwnd, WM_DESTROY, 0, 0);
						}
						break;
                case VK_F9:
                        if (!g.Add_w())
                                MessageBox(hwnd, TEXT("已经加到最大啦！(这么大的棋盘还满足不了你嘛？)"), TEXT("提示"), MB_OK);
                        InvalidateRect(hwnd, NULL, TRUE);
                        break;
                case VK_F8:
                        if (!g.Minus_w())
                                MessageBox(hwnd, TEXT("已经减到最小啦！（再小就连不成五子啦！）"), TEXT("提示"), MB_OK);
                        InvalidateRect(hwnd, NULL, TRUE);
                        break;
                case VK_UP:
                        g.Up();
                        g.ChangeCursor();
                        g.Print();
						InvalidateRect(hwnd, NULL, TRUE);
						break;
				case VK_DOWN:
                        g.Down();
                        g.ChangeCursor();
                        g.Print();
						InvalidateRect(hwnd, NULL, TRUE);
						break;
				case VK_LEFT:
                        g.Left();
                        g.ChangeCursor();
                        g.Print();
						InvalidateRect(hwnd, NULL, TRUE);
						break;
				case VK_RIGHT:
                        g.Right();
                        g.ChangeCursor();
                        g.Print();
						InvalidateRect(hwnd, NULL, TRUE);
						break;
                case VK_SPACE:
                        g.PutC();
                        g.Memorize();
                        g.Print();
                        if(g.Pd() == 1)
                        {
                            if (MessageBox(hwnd, TEXT("玩家1获胜，是否重新开始游戏?(选否可以自行选择悔棋)"), TEXT("帮助"), MB_YESNO) == IDYES)
                            {
                                g.Restart();
                                InvalidateRect(hwnd, NULL, TRUE);
                            }
                        }
                        else
                        {
                            if (g.cb.player == g.AI_num)
                            {
                                g.AI_cal();
                                g.PutC();
                                g.Print();
                                InvalidateRect(hwnd, NULL, TRUE);
                                g.Memorize();
                                if (g.Pd() == 2)
                                {
                                    if (MessageBox(hwnd, TEXT("玩家2获胜，是否重新开始游戏?(选否可以自行选择悔棋)"), TEXT("帮助"), MB_YESNO) == IDYES)
                                    {
                                        g.Restart();
                                        InvalidateRect(hwnd, NULL, TRUE);
                                    }
                                }
                            }
                            g.Print();
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
				}
				InvalidateRect(hwnd, NULL, TRUE);
				return 0;
    case WM_CLOSE:
                if (MessageBox(hwnd, TEXT("真的退出吗？"), TEXT("Exit"), MB_ICONSTOP|MB_YESNO) == IDYES)
                    DestroyWindow(hwnd);
                else
                    return 0;
	case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void WinReSizing(HWND hwnd)
{
	int w=500, h=500;
	RECT rc;
	GetClientRect(GetWindow(hwnd, 1), &rc);		// 获取整个操作系统桌面尺寸
	WINDOWPLACEMENT WinPlacement;

	GetWindowPlacement(hwnd, &WinPlacement);
	rc.left = (rc.right-rc.left-w)/2;
	rc.top = (rc.bottom-rc.top-h)/2;
	rc.right = rc.left+w;
	rc.bottom = rc.top+h;
	WinPlacement.rcNormalPosition = rc;
	SetWindowPlacement(hwnd, &WinPlacement);
}

int mylog(int n)
{
	int m, p;
	for(m=0,p=1; p<n; p*=2)
		m++;
	return m;
}

unsigned long ColormapHsv(int n, int Num)		// 仿MatLab的Hsv颜色设置
{
	double x = double(n)/(Num-1);
	int r, g, b;
	if(x<0) x=0; else if(x>1) x=1;
	if(x<3.0/16)
	{
		r = 255;
		g = int(255*(16*x/3));
		if(g<0) g=0; else if(g>255) g=255;
		b = 0;
	}
	else if(x<6.0/16)
	{
		r = int(255*(2-16*x/3));
		if(r<0) r=0; else if(r>255) r=255;
		g = 255;
		b = 0;
	}
	else if(x<8.0/16)
	{
		r = 0;
		g = 255;
		b = int(255*(8*x-3));
		if(b<0) b=0; else if(b>255) b=255;
	}
	else if(x<10.0/16)
	{
		r = 0;
		g = int(255*(5-8*x));
		if(g<0) g=0; else if(g>255) g=255;
		b = 255;
	}
	else if(x<13.0/16)
	{
		r = int(255*(16*x-10)/3);
		if(r<0) r=0; else if(r>255) r=255;
		g = 0;
		b = 255;
	}
	else
	{
		r = 255;
		g = 0;
		b = int(255*(0.9062*(13-16*x)/3+1));
		if(b<0) b=0; else if(b>255) b=255;
	}
	return RGB(r, g, b);
}

COLORREF InvColor(COLORREF color)
{
	int r, g, b;
	r = 255 - (color >> 16);
	g = 255 - ((color >> 8 ) & 255);
	b = 255 - (color & 255);
	return (r<<16)|(g<<8)|b;
}

void DrawBlock(HDC hdc, int left, int top, int right, int bottom, int num)
{
	HPEN hPen;
	HBRUSH hBrush;
	static COLORREF colors[200], invColor;

	for(int i=0; i<200; i++)
		colors[i] = ColormapHsv(i, 200);

	int w=right-left, h=bottom-top;

	invColor = InvColor(colors[123]);
	hPen = (HPEN)GetStockObject(WHITE_PEN);

	hPen = CreatePen(NULL, NULL, invColor);
	SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(invColor);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, left, top, right, bottom);
	DeleteObject(hPen);
	DeleteObject(hBrush);

    hPen = (HPEN)GetStockObject(BLACK_PEN);
    SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	MoveToEx(hdc, left, (top+bottom)/2, NULL);
	LineTo(hdc, right, (top+bottom)/2);
	MoveToEx(hdc, (left+right)/2, top, NULL);
	LineTo(hdc, (left+right)/2, bottom);
	DeleteObject(hPen);
	DeleteObject(hBrush);

    if (num == 1) {
        hPen = (HPEN)GetStockObject(NULL_PEN);
        SelectObject(hdc, hPen);
        hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/20, top-(top-bottom)/20, right+(left-right)/20, bottom+(top-bottom)/20);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
    if (num == 2) {
        hPen = (HPEN)GetStockObject(NULL_PEN);
        SelectObject(hdc, hPen);
        hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/20, top-(top-bottom)/20, right+(left-right)/20, bottom+(top-bottom)/20);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
    if (num == 3) {
        hPen = (HPEN)GetStockObject(NULL_PEN);
        SelectObject(hdc, hPen);
        hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/20, top-(top-bottom)/20, right+(left-right)/20, bottom+(top-bottom)/20);
        hBrush = CreateSolidBrush(InvColor(colors[100]));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/3, top-(top-bottom)/3, right+(left-right)/3, bottom+(top-bottom)/3);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
    if (num == 4) {
        hPen = (HPEN)GetStockObject(NULL_PEN);
        SelectObject(hdc, hPen);
        hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/20, top-(top-bottom)/20, right+(left-right)/20, bottom+(top-bottom)/20);
        hBrush = CreateSolidBrush(InvColor(colors[100]));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/3, top-(top-bottom)/3, right+(left-right)/3, bottom+(top-bottom)/3);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
    if (num == 5) {
        hPen = (HPEN)GetStockObject(NULL_PEN);
        SelectObject(hdc, hPen);
        hBrush = CreateSolidBrush(InvColor(colors[100]));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, left-(left-right)/3, top-(top-bottom)/3, right+(left-right)/3, bottom+(top-bottom)/3);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void Draw(HWND hwnd, HDC hdc, SIZE size, const FiveInARow &g)
{
	RECT rect;
	POINT Orgpt;

	GetClientRect(GetWindow(hwnd, 1), &rect);		// 获取整个操作系统桌面尺寸
	SetMapMode(hdc, MM_ANISOTROPIC);				// 设置映射模式

	SetViewportExtEx(hdc, size.cx, size.cy, NULL);	// 设置窗口、视口范围及视口原点
	SetWindowExtEx(hdc, 2000, 1000, NULL);
	SetViewportOrgEx(hdc, 0, 0, NULL);

	DPtoLP(hdc, (PPOINT)&size, 1);					// 将设备坐标转变为逻辑坐标
	SelectObject(hdc, GetStockObject(BLACK_PEN));
	SetBkMode(hdc, TRANSPARENT);

	GetWindowOrgEx(hdc, &Orgpt);
	LPtoDP(hdc, &Orgpt, 1);							// 将逻辑坐标转换为设备坐标
	GetViewportOrgEx(hdc, &Orgpt);

	int w=size.cx/(g.width), h=size.cy/(g.width);
	for(int i=1; i<=g.width; i++)
	{
		for(int j=1; j<=g.width; j++)
			DrawBlock(hdc, (j-1)*w, (i-1)*h, j*w, i*h, g.display[i][j]);
	}
}
