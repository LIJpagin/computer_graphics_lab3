#include "Object2D.h"
#include <vector>
#include <ctime>

char szClassName[] = "CG_WAPI_Template";
const int WidthWndClass = 600, HeightWndClass = 600;
std::vector <Object2D> objects(4);

void setup() {
	srand(time(NULL));
	int vertices = 3;
	for (auto i = 0; i < objects.size(); i++) {
		COLORREF color = rand() % 256;
		color <<= 8, color |= rand() % 256, color <<= 8, color |= rand() % 256;
		objects[i].installPen(color, rand() % 3 + 3);
		objects[i].regularPolygon(vertices++, rand() % (WidthWndClass - 201) + 100,
			rand() % (HeightWndClass - 201) + 10, rand() % 100 + 50);
	}
}

void draw(HDC hdc) {
	objects.front().drawPaintedContour(hdc);
	for (auto i = 0; i < objects.size(); i++) {
		//objects[i].drawOpenContour(hdc);
		objects[i].drawClosedContour(hdc);
		//objects[i].drawBresenham(hdc);
	}
}

void control(int key) {
	if (key == int('O')) Object2D::changeObject();
	if (key == int('P')) objects[Object2D::activeObject()].changePoint();
	if (key == int('Q')) objects[Object2D::activeObject()].rotation(-1.);
	if (key == int('E')) objects[Object2D::activeObject()].rotation(1.);
	if (key == int('X')) objects[Object2D::activeObject()].scaling(1.02);
	if (key == int('Z')) objects[Object2D::activeObject()].scaling(0.98);
	if (key == int('W')) objects[Object2D::activeObject()].moving(0., -2.);
	if (key == int('A')) objects[Object2D::activeObject()].moving(-2., 0.);
	if (key == int('S')) objects[Object2D::activeObject()].moving(0., 2.);
	if (key == int('D')) objects[Object2D::activeObject()].moving(2., 0.);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps; RECT Rect; HDC hdc, hCmpDC; HBITMAP hBmp;

	switch (messg) {
	case WM_PAINT: {
		GetClientRect(hWnd, &Rect);
		hdc = BeginPaint(hWnd, &ps);

		// �������� �������� ��������� ��� ������� �����������
		hCmpDC = CreateCompatibleDC(hdc);
		hBmp = CreateCompatibleBitmap(hdc, Rect.right - Rect.left, Rect.bottom - Rect.top);
		SelectObject(hCmpDC, hBmp);

		// �������� ������� ������
		LOGBRUSH br;
		br.lbStyle = BS_SOLID;
		br.lbColor = 0x282828;
		HBRUSH brush;
		brush = CreateBrushIndirect(&br);
		FillRect(hCmpDC, &Rect, brush);
		DeleteObject(brush);

		// ���������
		draw(hCmpDC);

		// �������� ����������� �� �������� ��������� �� �����
		SetStretchBltMode(hdc, COLORONCOLOR);
		BitBlt(hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, hCmpDC, 0, 0, SRCCOPY);

		// ������� �������� ��������� �������
		DeleteDC(hCmpDC);
		DeleteObject(hBmp);
		hCmpDC = NULL;

		EndPaint(hWnd, &ps);
	} break;

	case WM_ERASEBKGND:
		return 1;
		break;

	case WM_KEYDOWN:
		int key;
		key = int(wParam);
		control(key);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, messg, wParam, lParam));
	}
	return (0);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	MSG lpMsg;
	WNDCLASS WndClass;
	setup();

	// ��������� ��������� ������ ����
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = (LPCWSTR)szClassName;

	// ������������ ����� ����
	if (!RegisterClass(&WndClass)) {
		MessageBox(NULL, (LPCWSTR)"�� ���� ���������������� ����� ����!", (LPCWSTR)"������", MB_OK);
		return 0;
	}

	// ������� �������� ���� ����������
	hWnd = CreateWindow(
		(LPCWSTR)szClassName, // ��� ������ 
		L"������������ ������� ������������ ������ �1", // ����� ���������
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // ����� ���� 
		50, 50, // ������� ������ �������� ���� 
		WidthWndClass, HeightWndClass, // ������ � ������ ���� 
		(HWND)NULL, // ��������� �� ������������ ���� NULL 
		(HMENU)NULL, // ������������ ���� ������ ���� 
		(HINSTANCE)hInstance, // ��������� �� ������� ����������
		NULL); // ���������� � �������� lParam � ������� WM_CREATE


	// ���� ���� �� ������� �������
	if (!hWnd) {
		MessageBox(NULL, (LPCWSTR)"�� ������� ������� ������� ����!", (LPCWSTR)"������", MB_OK);
		return 0;
	}

	// ���������� ����
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ��������� ���� ��������� ��������� �� �������� ����������
	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return lpMsg.wParam;
}