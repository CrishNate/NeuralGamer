#pragma once

#ifndef AUTOCOLIB_H
#define AUTOCOLIB_H

#include <windows.h>
#include <math.h>
#include <vector>
#include <ctime>
#include <memory>

#include "Color.h"

namespace AutoCO
{
	static RGBColor GetPixelW(HWND hwnd, POINT p)
	{
		HDC hdc = GetDC(hwnd);
		COLORREF clr = GetPixel(hdc, p.x, p.y);

		//ReleaseDC(hwnd, hdc);

		return RGBColor(
			GetRValue(clr),
			GetGValue(clr),
			GetBValue(clr)
			);
	}


	static RGBColor GetPixelI(BYTE* pImg, POINT p, UINT16 imgWidth)
	{
		return RGBColor(
			pImg[4 * (p.x + (p.y * imgWidth)) + 2],
			pImg[4 * (p.x + (p.y * imgWidth)) + 1],
			pImg[4 * (p.x + (p.y * imgWidth))]
			);
	}


	static BYTE* Screenshot(POINT a, POINT b)
	{
		// copy screen to bitmap
		HDC     hScreen = GetDC(NULL);
		HDC     hDC = CreateCompatibleDC(hScreen);
		uint16_t scrW = abs(b.x - a.x);
		uint16_t scrH = abs(b.y - a.y);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, scrW, scrH);
		HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
		BOOL    bRet = BitBlt(hDC, 0, 0, scrW, scrH, hScreen, a.x, a.y, SRCCOPY);
		SelectObject(hDC, old_obj);

		BITMAPINFOHEADER bmi = { 0 };
		bmi.biSize = sizeof(BITMAPINFOHEADER);
		bmi.biPlanes = 1;
		bmi.biBitCount = 32;
		bmi.biWidth = scrW;
		bmi.biHeight = -scrH;
		bmi.biCompression = BI_RGB;
		bmi.biSizeImage = 0;//3 * scrW * scrH;

		BYTE* scrDat = NULL;
		if (scrDat)
			free(scrDat);
		scrDat = (BYTE*)malloc(4 * scrW * scrH);

		GetDIBits(hDC, hBitmap, 0, scrH, scrDat, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

		// clean up
		SelectObject(hDC, old_obj);
		DeleteDC(hDC);
		ReleaseDC(NULL, hScreen);
		DeleteObject(hBitmap);

		return scrDat;
	}


	static void MouseControll(int flag)
	{
		INPUT inp[3];
		memset(inp, 0, sizeof(inp));

		inp[0].type = INPUT_MOUSE;
		inp[0].mi.dwFlags = flag;

		SendInput(3, inp, sizeof(INPUT));
	}


	static void KeyboardInput(char key, bool pressed)
	{
		INPUT key_press = { 0 };

		key_press.type = INPUT_KEYBOARD;
		key_press.ki.wVk = VkKeyScanEx(key, GetKeyboardLayout(NULL)) & 0xFF;
		key_press.ki.wScan = 0;
		key_press.ki.dwExtraInfo = 0;
		key_press.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP);

		SendInput(1, &key_press, sizeof INPUT);
	}


	static void SetCursorPos(uint16_t xPos, uint16_t yPos)
	{
		xPos = (xPos * (65535 / GetSystemMetrics(SM_CXSCREEN)));
		yPos = (yPos * (65535 / GetSystemMetrics(SM_CYSCREEN)));

		INPUT inp[3];
		memset(inp, 0, sizeof(inp));

		inp[0].type = INPUT_MOUSE;
		inp[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		inp[0].mi.dx = xPos;
		inp[0].mi.dy = yPos;

		SendInput(3, inp, sizeof(INPUT));
	}


	static wchar_t VKToChar(BYTE byte)
	{
		std::vector<BYTE> keys(256, 0);
		wchar_t buffer[1];
		ToUnicode(byte, MapVirtualKey(byte, MAPVK_VK_TO_VSC), keys.data(), buffer, 1, 0);

		return buffer[0];
	}
}
#endif /* AUTOCOLIB_H */