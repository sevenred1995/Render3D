#pragma once
#include "Render3DDevice.h"
#include <tchar.h>
#include <iostream>
#include <memory>
#include "MeshDemo.h"
#pragma region Winodw
using namespace std;

float width  = 800.0f;
float height = 600.f;

static HWND    handle;              //窗口句柄
static HDC     window_dc;           //窗口HDC
static HBITMAP window_hb = NULL;
static HBITMAP window_ob = NULL; //老的位图画刷
unsigned int*  window_fb = NULL; //frame buffer

long window_pitch;

int window_width, window_height, window_exit = 0;
int window_mx = 0, window_my = 0, window_mb = 0;
int window_keys[512];	// 当前键盘按下状态

int   window_init(int w, int h, const TCHAR* title);
void  window_dispatch();


#ifdef _MSC_VER
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#endif
#define IDM_OPT1     301  
#define IDM_OPT2     302 
#define IDM_OPT3     303 
int select_Num = 1;
HMENU hRoot;
static LRESULT wndproc(HWND, UINT, WPARAM, LPARAM);
void createMyMenu();
void setBackGround(unsigned int * ptr);
bool LightState = false;
////////////////////////////////渲染所需要的资源///////////

shared_ptr<MeshDemo> g_pMeshDemo = make_shared<MeshDemo>();

//////////////////////////////////////////////////////////


int   window_init(int w, int h, const TCHAR* title)
{
	createMyMenu();
	WNDCLASS wc = { CS_BYTEALIGNCLIENT,(WNDPROC)wndproc,0,0,0,
		NULL,NULL,NULL,NULL,L"SCREEN3" };

	int wx, wy, sx, sy;
	HDC hdc;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc))
		return -1;
	RECT rect = { 0,0,w,h };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	handle = CreateWindow(L"SCREEN3", title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, hRoot, wc.hInstance, NULL);
	if (handle == NULL)return -2;
	//window_exit = 0;
	//hdc = GetDC(handle);
	//window_dc = CreateCompatibleDC(hdc);
	//ReleaseDC(handle, hdc);
	//BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),w,-h,1,32,BI_RGB,
	//	w*h * 4,0,0,0,0 } };
	//window_hb = CreateDIBSection(window_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	//if (window_hb == NULL) return -3;
	//window_ob = (HBITMAP)SelectObject(window_dc, window_hb);
	//window_fb = (unsigned int*)ptr;
	//window_width = w;
	//window_height = h;
	//window_pitch = w * 4;
	
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CXSCREEN) - wy) / 2;
	if (sy < 0)
		sy = 0;
	SetWindowPos(handle, NULL, 10, 10, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(handle);
	ShowWindow(handle, SW_NORMAL);
	memset(window_keys, 0, sizeof(int) * 512);
	window_dispatch();
	memset(window_fb, 0, w*h * 4);
	return 0;
}
void  window_dispatch()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_pMeshDemo->Update(.0f);
			g_pMeshDemo->Render();
			InvalidateRect(handle, nullptr, true);
			UpdateWindow(handle);
		}
	}
}
static LRESULT wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//test
	PAINTSTRUCT ps;
	HDC hdc;
	//双缓冲绘图
	static BITMAPINFO s_bitmapInfo;
	static HDC s_hdcBackbuffer;			//后缓冲设备句柄	
	static HBITMAP s_hBitmap;
	static HBITMAP s_hOldBitmap;
	static void*   s_pData;
	switch (msg) {
	case WM_CREATE:
	{
		g_pMeshDemo->Init(GetModuleHandle(NULL), hWnd);
		//初始化设备无关位图header
		BITMAPINFOHEADER bmphdr = { 0 };
		bmphdr.biSize = sizeof(BITMAPINFOHEADER);
		bmphdr.biWidth = width;
		bmphdr.biHeight = -height;
		bmphdr.biPlanes = 1;
		bmphdr.biBitCount = 32;
		bmphdr.biSizeImage = height * width * 4;
		//创建后缓冲区
		//先创建一个内存dc
		s_hdcBackbuffer = CreateCompatibleDC(nullptr);
		LPVOID ptr;
		//获得前置缓冲区dc
		HDC hdc = GetDC(hWnd);//reinterpret_cast<void**>(&g_pMeshDemo->GetDevice()->GetFrameBuffer())
		if (!(s_hBitmap = CreateDIBSection(nullptr, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS,
			&ptr, nullptr, 0)))
		{
			MessageBox(nullptr, L"create dib section failed!", L"error", MB_OK);
			return 0;
		}
		g_pMeshDemo->GetDevice()->GetFrameBuffer() = (unsigned int*)ptr;
		window_fb = (unsigned int*)ptr;
		memset(window_fb, 0, width*height * 4);
		//将bitmap装入内存dc
		setBackGround(window_fb);
		s_hOldBitmap = (HBITMAP)SelectObject(s_hdcBackbuffer, s_hBitmap);
		//释放dc
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//把backbuffer内容传到frontbuffer
		//setBackGround(window_fb);
		BitBlt(ps.hdc, 0, 0, width, height, s_hdcBackbuffer, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:    break;
	case WM_KEYDOWN: 
		g_pMeshDemo->OnKeyDown(wParam);
		if (wParam == VK_ESCAPE)
		{
			SelectObject(s_hdcBackbuffer, s_hOldBitmap);
			DeleteDC(s_hdcBackbuffer);
			DeleteObject(s_hOldBitmap);
			::DestroyWindow(hWnd);
		}
		break;
	case WM_KEYUP:   window_keys[wParam & 511] = 0; break;
	case WM_MOUSEWHEEL :
		if ((INT)wParam > 0)
		{

		}
		else 
		{

		} 
		break;
	case WM_DESTROY: 
		PostQuitMessage(0);
	case WM_ERASEBKGND:
		return true;
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		g_pMeshDemo->OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		g_pMeshDemo->OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
	     //OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEMOVE:
		g_pMeshDemo->OnMouseMove(wParam,LOWORD(lParam),HIWORD(lParam));
	case WM_COMMAND:
	{
		// 取出资源Id值  
		// 并判断用户选择了哪个菜单项  
		switch (LOWORD(wParam))
		{
		  case IDM_OPT1:
			  select_Num = IDM_OPT1;
			  g_pMeshDemo->OnMenuLightSwitch(LightState);
			  break;
		  case IDM_OPT2:
			  select_Num = IDM_OPT2;
			  g_pMeshDemo->OnLoadBit();
			  break;
		  case IDM_OPT3:
			  g_pMeshDemo->OnRemoveBit();
			  break;
		  default:
			 break;
		}
	}
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
void  createMyMenu() {
	hRoot=CreateMenu();
	if (!hRoot)return;
	HMENU pop1 = CreatePopupMenu();
	AppendMenu(hRoot,
		MF_POPUP,
		(UINT_PTR)pop1,
		L"操作");
	// 一种方法是使用AppendMenu函数  
	AppendMenu(pop1,
		MF_STRING,
		IDM_OPT1,
		L"光照开关");
	AppendMenu(pop1,
		MF_STRING, 
		IDM_OPT2,
		L"加载纹理");
	AppendMenu(pop1,
		MF_STRING,
		IDM_OPT3,
		L"卸载纹理");

}
#pragma endregion
void setBackGround(unsigned int * ptr)
{
	for (int i = 0; i < 800; i++)
	{
		for (int x = 0; x < 600; x++)
			ptr[x * 800 + i] = (50 << 16) | (46 << 8) | 187;
	}
}
int main()
{
	cout << "说明：按下按键1,2,3更换渲染物体,上下左右键旋转物体,按下右键缩放物体" << endl;
	TCHAR *title = _T("Render3d");
	if (window_init(800, 600, title))
		return -1;
	return 0;
}