#include <Windows.h>
#include <cmath>

void Swap2Points(int& x1, int& y1, int& x2, int& y2)
{
	int temp = x1;
	x1 = x2;
	x2 = temp;

	temp = y1;
	y1 = y2;
	y2 = temp;
}

int Round(int x)
{
	return (int)(x + 0.5);
}

void LineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	double m = (double)dy / dx;

	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)
			Swap2Points(x1, y1, x2, y2);

		SetPixel(hdc, x1, y1, c);

		int x = x1;
		double y = y1;

		while (x < x2)
		{
			x++;
			y += m;
			SetPixel(hdc, x, Round(y), c);
		}
	}
	else
	{
		if (y1 > y2)
			Swap2Points(x1, y1, x2, y2);

		SetPixel(hdc, x1, y1, c);

		double x = x1;
		int y = y1;

		while (y < y2)
		{
			y++;
			x += 1 / m;
			SetPixel(hdc, Round(x), y, c);
		}
	}
}

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc + x, yc - y, c);
}
void Draw8Lines(HDC hdc, int xs, int ys, int x, int y)
{
	LineDDA(hdc, xs, ys, xs + x, ys + y, RGB(100, 0, 0));
	LineDDA(hdc, xs, ys, xs + y, ys + x, RGB(0, 100, 0));
	LineDDA(hdc, xs, ys, xs + y, ys - x, RGB(0, 0, 100));
	LineDDA(hdc, xs, ys, xs - x, ys + y, RGB(100, 100, 0));
	LineDDA(hdc, xs, ys, xs - x, ys - y, RGB(100, 0, 100));
	LineDDA(hdc, xs, ys, xs - y, ys - x, RGB(100, 100, 100));
	LineDDA(hdc, xs, ys, xs - y, ys + x, RGB(10, 50, 20));
	LineDDA(hdc, xs, ys, xs + x, ys - y, RGB(30, 10, 70));
}

void BresenhamCircle(HDC hdc, int xc, int yc, int R, COLORREF c)
{

	int x = 0;
	int y = R;

	int d = 1 - R;
	int d1 = 3;
	int d2 = 5 - 2 * R;

	Draw8Lines(hdc, xc, yc, 0, R);
	Draw8Points(hdc, xc, yc, 0, R, c);

	while (x < y)
	{
		if (d < 0)
		{
			d += d1;
			d1 += 2;
			d2 += 2;
			x++;
		}
		else
		{
			d += d2;
			d1 += 2;
			d2 += 4;
			x++;
			y--;
		}
		Draw8Lines(hdc, xc, yc, x, y);
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}

int xs, ys, xe, ye, r;

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	HDC hdc = GetDC(hwnd);
	switch (mcode)
	{
	case WM_LBUTTONDOWN:
		xs = LOWORD(lp);
		ys = HIWORD(lp);
		break;
	case WM_RBUTTONDOWN:
		xe = LOWORD(lp);
		ye = HIWORD(lp);
		r = sqrt((xe - xs) * (xe - xs) +
			(ye - ys) * (ye - ys));
		BresenhamCircle(hdc, xs, ys, r, RGB(0, 0, 0));
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, mcode, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}