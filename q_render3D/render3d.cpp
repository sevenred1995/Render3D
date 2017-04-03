#pragma once
#include "IRenderPipelineDevice.h"
#include <tchar.h>
#include<iostream>
#include "FileLoader.h"
#include "GeometryMeshGenerator.h"
#pragma region Winodw
using namespace std;

float width = 800.0f;
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
int   window_close(void);
void  window_update();
void  window_dispatch();

#ifdef _MSC_VER
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#endif
#define IDM_OPT1     301  
#define IDM_OPT2     302 
int select_Num = 1;
HMENU hRoot;
static LRESULT wndproc(HWND, UINT, WPARAM, LPARAM);


void createMyMenu();

int   window_init(int w, int h, const TCHAR* title)
{
	createMyMenu();
	WNDCLASS wc = { CS_BYTEALIGNCLIENT,(WNDPROC)wndproc,0,0,0,
		NULL,NULL,NULL,NULL,L"SCREEN3" };
	RECT rect = { 0,0,w,h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hdc;
	window_close();
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc))
		return -1;
	handle = CreateWindow(L"SCREEN3", title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, hRoot, wc.hInstance, NULL);
	if (handle == NULL)return -2;

	window_exit = 0;
	hdc = GetDC(handle);
	window_dc = CreateCompatibleDC(hdc);
	ReleaseDC(handle, hdc);

	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),w,-h,1,32,BI_RGB,
		w*h * 4,0,0,0,0 } };
	window_hb = CreateDIBSection(window_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (window_hb == NULL) return -3;

	window_ob = (HBITMAP)SelectObject(window_dc, window_hb);
	window_fb = (unsigned int*)ptr;
	window_width = w;
	window_height = h;
	window_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CXSCREEN) - wy) / 2;
	if (sy < 0)
		sy = 0;
	SetWindowPos(handle, NULL, 10, 10, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(handle);

	ShowWindow(handle, SW_NORMAL);
	window_dispatch();


	memset(window_keys, 0, sizeof(int) * 512);
	memset(window_fb, 0, w*h * 4);
	return 0;
}
static LRESULT wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	//test
	switch (msg) {
	case WM_CLOSE:   window_exit = 1; break;
	case WM_KEYDOWN: window_keys[wParam & 511] = 1; break;
	case WM_KEYUP:   window_keys[wParam & 511] = 0; break;
	case WM_MOUSEWHEEL :
		if ((INT)wParam > 0)
		{

		}
		else 
		{

		} 
		break;
	case WM_DESTROY: PostQuitMessage(0);

	case WM_COMMAND:
	{
		// 取出资源Id值  
		// 并判断用户选择了哪个菜单项  
		switch (LOWORD(wParam))
		{
		  case IDM_OPT1:
			  select_Num = IDM_OPT1;
			  MessageBox(hWnd, L"渲染出球体", L"提示", MB_OK);
			  break;
		  case IDM_OPT2:
			  select_Num = IDM_OPT2;
			  MessageBox(hWnd, L"渲染出立方体", L"提示", MB_OK);
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
		L"渲染球体");
	AppendMenu(pop1,
		MF_STRING, 
		IDM_OPT2,
		L"渲染立方体");
}
int   window_close(void)
{
	if (window_dc)
	{
		if (window_ob)
		{
			SelectObject(window_dc, window_ob);
			window_ob = NULL;
		}
		DeleteDC(window_dc);
		window_dc = NULL;
	}
	if (window_hb)
	{
		DeleteObject(window_hb);
		window_hb = NULL;
	}
	if (handle)
	{
		CloseWindow(handle);
		handle = NULL;
	}
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
			break;
	}
}
void  window_update()
{
	HDC hDC = GetDC(handle);
	BitBlt(hDC, 0, 0, window_width, window_height, window_dc, 0, 0, SRCCOPY);
	ReleaseDC(handle, hDC);
	window_dispatch();
}
#pragma endregion
IRenderPipelineDevice* renderDevice;
void setBackGround(unsigned int * ptr)
{
	for (int i = 0; i < 800; i++)
	{
		for (int x = 0; x < 600; x++)
			ptr[x * 800 + i] = (46 << 16) | (46 << 8) | 87;
	}
}
void setMatrix(float pos,float theata)
{
	Matrix4x4 world;
	Matrix4x4 scale;
	Matrix4x4 rotate;
	matrix_set_translate(&world, 0, 0, -8.5);//position
	matrix_set_scale(&scale, 1.0, 1.0, 1.0);//scal
	matrix_set_rotate(&rotate, 0, PI, 0, theata);//rotation
	Matrix4x4 m;
	matrix4x4_mul(&m, &rotate, &world);//C=TR
	renderDevice->setWorldMatrix(m);//世界矩阵
	Vector4 eye = { 0, 0, pos, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 1, 0, 1 };
	Matrix4x4 view;
	matrix_set_lookat(&view, eye, at, up);
	renderDevice->setViewMatrix(view);
	Matrix4x4 proj;
	float aspect = width / height;
	matrix_set_perspective(&proj, PI*0.5f, aspect, 1.0f, 500.0f);
	renderDevice->setProjMatrix(proj);
}
void initMem(renderInitData* initData,void *fp)
{
	unsigned int* ptr = new unsigned int(initData->bufferHeight*initData->bufferWidth);
	unsigned int* frameBuffer=NULL;
	frameBuffer = (unsigned int*)fp;
	for (unsigned int j=0;j<initData->bufferHeight;j++)
	{
		int offset = j*(initData->bufferWidth);
		initData->mFrameBuffer[j] = frameBuffer + offset;
	}
}
#include <time.h>
int main()
{
	Texture2D mTexture;
	//mTexture.loadPPM("Media/earth.ppm");
	mTexture.loaddBit(L"Media/Wood.bmp");

	renderInitData initData; 
	initData.bufferWidth = 800;
	initData.bufferHeight= 600;
	initData.framebuffer = new std::vector<unsigned int>(800*600);
	initData.zBuffer     = new std::vector<float>(800*600);
	renderDevice = new IRenderPipelineDevice();
	renderDevice->init(initData);
	float pos = 6;
	float alpha = 1;
	setMatrix(pos,alpha);
	TCHAR *title = _T("Render3d");

	if (window_init(800, 600, title))
		return -1;
	IGeometryMeshGenerator* g = new IGeometryMeshGenerator();
	DrawData drawData = { 0 };
	drawData.pIndexBuffer  = new std::vector<unsigned int>();
	drawData.pVertexBuffer = new std::vector<Vertex>();
	g->ImportFile_OBJ("Media/skull.txt", *drawData.pVertexBuffer, *drawData.pIndexBuffer);
	//g->creatSphere(2, 50, 50, true, *drawData.pVertexBuffer, *drawData.pIndexBuffer);
	//g->createBox(3, 3, 3, 2, 2, 2, *drawData.pVertexBuffer, *drawData.pIndexBuffer);
	while (window_exit==0&&window_keys[VK_ESCAPE]==0)
	{
		setMatrix(pos,alpha);
		window_update();
		setBackGround(window_fb);
		if (window_keys[VK_UP])    pos -= 0.1f;
		if (window_keys[VK_DOWN])  pos += 0.1f;
		if (window_keys[VK_LEFT])  alpha += .1f;
		if (window_keys[VK_RIGHT]) alpha -= .1f;
		
		cout << "hello------" << endl;
		//设置光照
		DirectionalLight light1;
		light1.ambient   = Vector3{ 0.2f, 0.2f, 0.2f };
		light1.diffuse   = Vector3{ 0.5f, 0.5f, 0.5f };
		light1.direction = Vector3{ 0.57735f, -0.57735f, 0.57735f };
		light1.specular  = Vector3{ 0.5f, 0.5f, 0.5f };
		light1.mSpeaularIntensity =1.0f;
		light1.isEnabled = true;
		light1.mDiffuseInstensity = 1.0f;

		DirectionalLight light2;
		light2.ambient = Vector3{ 0.0f, 0.0f, 0.0f };
		light2.diffuse = Vector3{ 0.20f, 0.20f, 0.20f };
		light2.direction = Vector3{ -0.57735f, -0.57735f, 0.57735f };
		light2.specular = Vector3{ 0.25f, 0.25f, 0.25f };
		light2.mSpeaularIntensity = 1.0f;
		light2.isEnabled = true;
		light2.mDiffuseInstensity = 1.0f;

		DirectionalLight light3;
		light3.ambient   = Vector3{ 0.0f, 0.0f, 0.0f};
		light3.diffuse   = Vector3{ 0.2f, 0.2f, 0.2f };
		light3.direction = Vector3{ 0.0f, -0.707f, -0.707f };
		light3.specular  = Vector3{ 0.0f, 0.0f, 0.0f };
		light3.mSpeaularIntensity = 1.0f;
		light3.isEnabled = true;
		light3.mDiffuseInstensity = 1.0f;
		//设置材质
		Material material;
		material.ambient  = { 0.8f, 0.8f, 0.8f };
		material.diffuse  = { 0.8f, 0.8f, 0.8f };
		material.specular = { 0.8f, 0.8f, 0.8f };
		material.specularSmoothLev = 6.0f;

		renderDevice->setLight(0, light1);
		renderDevice->setLight(1, light2);
		renderDevice->setLight(2, light3);
		renderDevice->setLightingEnable(true);//开启或
		renderDevice->setMaterial(material);
		//renderDevice->setTexture(&mTexture);
		/*for (int i = 0; i < drawData.pVertexBuffer->size(); i++)
			cout << drawData.pVertexBuffer->at(i).tex.x<<" "<< drawData.pVertexBuffer->at(i).tex.y << endl;*/
		drawData.vCount = drawData.pVertexBuffer->size();
		renderDevice->drawTriangle(drawData, (int*)window_fb);
		//Sleep(1);
	}
	delete g;
	delete drawData.pIndexBuffer;
	delete drawData.pVertexBuffer;
	delete renderDevice;

	return 0;
}