//ウィンドウ表示の関係でShift-JISを使用しています
#include <windows.h>

#define BUTTON_START  10
#define BUTTON_STOP   11
#define BUTTON_SUBMIT 12

static HWND LEFT, RIGHT, COSTS;

LRESULT CALLBACK WndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp);

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance , PSTR lpCmdLine , int nCmdShow ) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("Prototype");

	if (!RegisterClass(&winc)) return -1;

	// ?ｿｽ?ｿｽ?ｿｽC?ｿｽ?ｿｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE
	hwnd = CreateWindow(
			TEXT("Prototype") , TEXT("Prototype") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			NULL , NULL , hInstance , NULL
	);
	ShowWindow(hwnd, SW_MAXIMIZE);

	//?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ}?ｿｽ\?ｿｽ?ｿｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE
	LEFT = CreateWindow(
		TEXT("STATIC") , NULL ,
		WS_CHILD | WS_VISIBLE | WS_BORDER ,
		20 , 50 , 450 , 450 ,
		hwnd , NULL , hInstance , NULL
	);	

	// ?ｿｽr?ｿｽ?ｿｽ?ｿｽ}?ｿｽ\?ｿｽ?ｿｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE
	RIGHT = CreateWindow(
		TEXT("STATIC") , NULL ,
		WS_CHILD | WS_VISIBLE | WS_BORDER ,
		450 + 40 , 50, 450 , 450 ,
		hwnd , NULL , hInstance , NULL
	);

	// ?ｿｽR?ｿｽX?ｿｽg?ｿｽ\?ｿｽ?ｿｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE
	COSTS = CreateWindow(
		TEXT("EDIT") , NULL ,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY ,
		20 , 560 , 675 , 90 ,
		hwnd , (HMENU)BUTTON_SUBMIT , hInstance , NULL
	);

	// ?ｿｽJ?ｿｽn?ｿｽ{?ｿｽ^?ｿｽ?ｿｽ
	CreateWindow(
		TEXT("BUTTON") , TEXT("START") ,
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON ,
		1100 , 650 , 70 , 30 ,
		hwnd , (HMENU)BUTTON_START , hInstance , NULL
	);

	// ?ｿｽI?ｿｽ?ｿｽ?ｿｽ{?ｿｽ^?ｿｽ?ｿｽ
	CreateWindow(
		TEXT("BUTTON") , TEXT("STOP") ,
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON ,
		1180 , 650 , 60 , 30 ,
		hwnd , (HMENU)BUTTON_STOP , hInstance , NULL
	);

	// ?ｿｽ?ｿｽ?ｿｽM?ｿｽ{?ｿｽ^?ｿｽ?ｿｽ
    CreateWindow(
		TEXT("BUTTON") , TEXT("SUBMIT") ,
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON ,
		1250 , 650 , 80 , 30 ,
		hwnd , (HMENU)BUTTON_SUBMIT , hInstance , NULL
	);

	if (hwnd == NULL) return -1;

	while(GetMessage(&msg , NULL , 0 , 0)) DispatchMessage(&msg);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	HDC hdc, left, right, costs, leftBuffer, rightBuffer;
	PAINTSTRUCT ps;
	static int choiceCost = 0, exchangeCost = 0;
	static TCHAR str[8];
	static HBITMAP leftHBitmap, rightHBitmap;
	static BITMAP leftBitmap, rightBitmap;
	RECT rectL, rectR;

	switch (msg) {
		//?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽI?ｿｽ?ｿｽ
		case WM_DESTROY:
			DeleteObject(leftHBitmap);
			DeleteObject(rightHBitmap);
			PostQuitMessage(0);
			return 0;
		//?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
		case WM_CREATE:
			leftHBitmap = LoadBitmap(
				((LPCREATESTRUCT)lp)->hInstance ,
				TEXT("CompletionFigure")
			);
			GetObject(leftHBitmap, sizeof(BITMAP), &leftBitmap);
			rightHBitmap = LoadBitmap(
				((LPCREATESTRUCT)lp)->hInstance ,
				TEXT("HalfWayFigure")
			);
			GetObject(rightHBitmap, sizeof(BITMAP), &rightBitmap);
			return 0;
		case WM_COMMAND:{
				switch(wp){
					//?ｿｽJ?ｿｽn?ｿｽ{?ｿｽ^?ｿｽ?ｿｽ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽN
					case BUTTON_START:
						// MessageBox(hwnd , TEXT("BUTTON_START") , TEXT("START") , MB_OK);
						choiceCost++;
						// exchangeCost++;
						InvalidateRect(hwnd , NULL , TRUE);
						InvalidateRect(COSTS , NULL , TRUE);
						break;
					//?ｿｽI?ｿｽ?ｿｽ?ｿｽ{?ｿｽ^?ｿｽ?ｿｽ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽN
					case BUTTON_STOP:
						// MessageBox(hwnd , TEXT("BUTTON_STOP") , TEXT("STOP") , MB_OK);
						// choiceCost++;
						exchangeCost++;
						InvalidateRect(hwnd , NULL , TRUE);
						InvalidateRect(COSTS , NULL , TRUE);
						break;
					//?ｿｽ?ｿｽ?ｿｽM?ｿｽ{?ｿｽ^?ｿｽ?ｿｽ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽN
					case BUTTON_SUBMIT:
						// MessageBox(hwnd , TEXT("BUTTON_SUBMIT") , TEXT("SUBMIT") , MB_OK);
						choiceCost++;
						exchangeCost++;
						InvalidateRect(hwnd , NULL , TRUE);
						InvalidateRect(COSTS , NULL , TRUE);
						break;
				}
				return 0;
			}
		case WM_PAINT:
			hdc = BeginPaint(hwnd , &ps);
			left = BeginPaint(LEFT , &ps);
			right = BeginPaint(RIGHT, &ps);
			costs = BeginPaint(COSTS, &ps);

			GetClientRect(LEFT, &rectL);
			leftBuffer = CreateCompatibleDC(left);
			SelectObject(leftBuffer, leftHBitmap);
			StretchBlt(
					left,
					0 ,	0,
					rectL.right, rectL.bottom ,
					leftBuffer , 
					0 , 0 ,
					leftBitmap.bmWidth , leftBitmap.bmHeight ,
					SRCCOPY
			);
			DeleteDC(leftBuffer);
			
			GetClientRect(RIGHT, &rectR);
			rightBuffer = CreateCompatibleDC(right);
			SelectObject(rightBuffer, rightHBitmap);
			StretchBlt(
					right,
					0 ,	0 ,
					rectR.right , rectR.bottom ,
					rightBuffer ,
					0 , 0 ,
					rightBitmap.bmWidth , rightBitmap.bmHeight ,
					SRCCOPY
			);
			DeleteDC(rightBuffer);

			TextOut(hdc , 20 , 20 , TEXT("Completion Figure") , lstrlen(TEXT("Completion Figure")));
			TextOut(hdc , 450 + 40 , 20 , TEXT("HalfWay Figure") , lstrlen(TEXT("HalfWay Figure")));
			TextOut(hdc , 20 , 530 , TEXT("Costs") , lstrlen(TEXT("Costs")));

			wsprintf(str , "%s%d" , TEXT("?ｿｽI?ｿｽ?ｿｽ?ｿｽR?ｿｽX?ｿｽg?ｿｽF"), choiceCost);
			TextOut(costs , 5 , 0 , str , lstrlen(str));
			wsprintf(str , "%s%d" , TEXT("?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽR?ｿｽX?ｿｽg?ｿｽF"), exchangeCost);
			TextOut(costs , 5 , 20 , str , lstrlen(str));

			EndPaint(hwnd , &ps);
			EndPaint(LEFT , &ps);
			EndPaint(RIGHT , &ps);
			EndPaint(COSTS , &ps);
			return 0;
	}
	return DefWindowProc(hwnd , msg , wp , lp);
}
