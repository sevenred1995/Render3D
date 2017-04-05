#pragma once
#include"Render3DDeviceContext.h"
#include"Render3DDevice.h"
#include"ShaderBase.h"
#include"GeometryMeshGenerator.h"

class MeshDemo
{
public:
	MeshDemo();
	~MeshDemo();
public:
	bool Init(HINSTANCE hInstance,HWND hWnd);
	void Update(float dt);
	void Render();
	void Clear();

	//鼠标消息回掉函数
	void OnMouseDown(WPARAM btnState,int x,int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnKeyDown(WPARAM keyState);

	//菜单消息处理：
	void  OnMenuLightSwitch(bool& State);
	void  OnLoadBit();
	void  OnRemoveBit();

private:
	void render(int state);
public:
	inline Render3DDevice*& GetDevice() { return m_pDevice; }
private:
	int             m_width, m_height;
	HINSTANCE       m_hInstance;
	HWND            m_hWnd;
	Render3DDevice* m_pDevice;
	Render3DDeviceContext* m_pImmediateContext;
	ShaderBase*     m_pShader;
	Matrix4x4       m_worldViewProj;
	Matrix4x4       m_world;
	Matrix4x4       m_worldInvTranspose;
	
	unsigned  int renderState = 1;
	MeshData        m_pBoxMeshData;

	Texture2D        m_pTex;
	Material         m_pMaterial;

	const int dirCount = 8;
	DirectionalLight m_pDirLight[8];  

	POINT           m_cameraPos;


	float           mTheta;
	float           mPhi;
	float           mRadius;
	Vector2         mLastMousePos;

	float           mMeshPhi_x;
	float           mMeshPhi_y;
};

