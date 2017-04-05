#include "MeshDemo.h"



MeshDemo::MeshDemo() :mTheta(1.5f * PI), mPhi(0.4*PI), mRadius(5.0f), mMeshPhi_x(0), mMeshPhi_y(0)
{
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	m_pDirLight[0].ambient = Vector3{ 0.2f, 0.2f, 0.2f };
	m_pDirLight[0].diffuse = Vector3{ 0.5f, 0.5f, 0.5f };
	m_pDirLight[0].direction = Vector3{ 0.57735f, 0.57735f, 0.57735f };
	m_pDirLight[0].specular = Vector3{ 0.5f, 0.5f, 0.5f };
	m_pDirLight[0].mSpeaularIntensity = 1.0f;
	m_pDirLight[0].isEnabled = true;
	m_pDirLight[0].mDiffuseInstensity = 1.0f;

	m_pDirLight[1].ambient = Vector3{ 0.0f, 0.0f, 0.0f };
	m_pDirLight[1].diffuse = Vector3{ 0.20f, 0.20f, 0.20f };
	m_pDirLight[1].direction = Vector3{ -0.57735f, 0.57735f, 0.57735f };
	m_pDirLight[1].specular = Vector3{ 0.25f, 0.25f, 0.25f };
	m_pDirLight[1].mSpeaularIntensity = 1.0f;
	m_pDirLight[1].isEnabled = true;
	m_pDirLight[1].mDiffuseInstensity = 1.0f;

	m_pDirLight[2].ambient = Vector3{ 0.0f, 0.0f, 0.0f };
	m_pDirLight[2].diffuse = Vector3{ 0.2f, 0.2f, 0.2f };
	m_pDirLight[2].direction = Vector3{ 0.0f, 0.707f, -0.707f };
	m_pDirLight[2].specular = Vector3{ 0.0f, 0.0f, 0.0f };
	m_pDirLight[2].mSpeaularIntensity = 1.0f;
	m_pDirLight[2].isEnabled = true;
	m_pDirLight[2].mDiffuseInstensity = 1.0f;


	m_pMaterial.ambient = { 0.8f, 0.8f, 0.8f };
	m_pMaterial.diffuse = { 0.8f, 0.8f, 0.8f };
	m_pMaterial.specular = { 0.8f, 0.8f, 0.8f };
	m_pMaterial.specularSmoothLev =6.0f;
}
MeshDemo::~MeshDemo()
{
	Clear();
}
#include<iostream>
using namespace std;
bool MeshDemo::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	m_width  = 800;
	m_height = 600;

	m_pDevice = new Render3DDevice(m_width, m_height);
	m_pImmediateContext = new Render3DDeviceContext();
	m_pImmediateContext->Init(m_pDevice);
	m_pShader = new ShaderBase();

	m_pBoxMeshData.pIndexBuffer  = new std::vector<unsigned int>();
	m_pBoxMeshData.pVertexBuffer = new std::vector<Vertex>();
	
	//IGeometryMeshGenerator::GetInstance()->createBox(3, 3, 3, 2, 2, 2, *m_pBoxMeshData.pVertexBuffer, *m_pBoxMeshData.pIndexBuffer);
	//m_pImmediateContext->SetIndexBuffer(*m_pBoxMeshData.pIndexBuffer);
	//m_pImmediateContext->SetVertexBuffer(*m_pBoxMeshData.pVertexBuffer);
	
	m_pImmediateContext->SetShader(m_pShader);
	
	m_pTex.loaddBit(L"Media/Wood.bmp");

	m_pShader->SetTexture(m_pTex);
	m_pShader->SetLightEnable(true);//允许进行光照
	m_pShader->SetDirLightArray(0,m_pDirLight[0]);
	m_pShader->SetDirLightArray(1, m_pDirLight[1]);
	m_pShader->SetDirLightArray(2, m_pDirLight[2]);

	m_pShader->SetMaterial(m_pMaterial);
	return true;
}

void MeshDemo::Update(float dt)
{

	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius;// *sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	Matrix4x4 world;
	Matrix4x4 scale;
	Matrix4x4 rotate;
	matrix_set_translate(&world, 0, 0, -4.5);//position
	matrix_set_scale (&scale, 1.0, 1.0, 1.0);//scal
	matrix_set_rotate(&rotate, mMeshPhi_y, mMeshPhi_x, 0, mMeshPhi_x);//rotation
	Matrix4x4 World;//旋转和位置
	matrix4x4_mul(&World, &rotate, &world);//C=TR
	m_world = World;
	Vector4 eye = {x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 1, 0, 1 };
	Matrix4x4 View;
	matrix_set_lookat(&View, eye, at, up);
	Matrix4x4 Proj;
	float aspect = m_width / m_height;
	matrix_set_perspective(&Proj, PI*0.5f, aspect, 1.0f, 500.0f);
	Matrix4x4 transform_;
	matrix4x4_mul(&transform_, &World, &View);
	matrix4x4_mul(&m_worldViewProj, &transform_, &Proj);
	//m_worldViewProj = World*View*Proj;
	m_worldInvTranspose = Math::matrixTranspose(world);
	m_pShader->SetEyePos(eye);
}
void MeshDemo::render(int state)
{
	if (state == 1)
	{
		m_pBoxMeshData.pIndexBuffer->clear();
		m_pBoxMeshData.pVertexBuffer->clear();
		m_pBoxMeshData.pIndexBuffer = new std::vector<unsigned int>();
		m_pBoxMeshData.pVertexBuffer = new std::vector<Vertex>();
		IGeometryMeshGenerator::GetInstance()->createBox(3, 3, 3, 2, 2, 2, *m_pBoxMeshData.pVertexBuffer, *m_pBoxMeshData.pIndexBuffer);
		m_pImmediateContext->SetIndexBuffer(*m_pBoxMeshData.pIndexBuffer);
		m_pImmediateContext->SetVertexBuffer(*m_pBoxMeshData.pVertexBuffer);
	}
	else if(state==2)
	{
		m_pBoxMeshData.pIndexBuffer->clear();
		m_pBoxMeshData.pVertexBuffer->clear();
		m_pBoxMeshData.pIndexBuffer = new std::vector<unsigned int>();
		m_pBoxMeshData.pVertexBuffer = new std::vector<Vertex>();
		IGeometryMeshGenerator::GetInstance()->creatSphere(2, 50, 50, true, *m_pBoxMeshData.pVertexBuffer, *m_pBoxMeshData.pIndexBuffer);
		m_pImmediateContext->SetIndexBuffer(*m_pBoxMeshData.pIndexBuffer);
		m_pImmediateContext->SetVertexBuffer(*m_pBoxMeshData.pVertexBuffer);
	}
	else if (state == 3) {
		m_pBoxMeshData.pIndexBuffer->clear();
		m_pBoxMeshData.pVertexBuffer->clear();
		m_pBoxMeshData.pIndexBuffer = new std::vector<unsigned int>();
		m_pBoxMeshData.pVertexBuffer = new std::vector<Vertex>();
		IGeometryMeshGenerator::GetInstance()->ImportFile_OBJ("Media/car.txt", *m_pBoxMeshData.pVertexBuffer, *m_pBoxMeshData.pIndexBuffer);
		m_pImmediateContext->SetIndexBuffer(*m_pBoxMeshData.pIndexBuffer);
		m_pImmediateContext->SetVertexBuffer(*m_pBoxMeshData.pVertexBuffer);
	}
}
void MeshDemo::Render()
{
	render(renderState);
	m_pDevice->ClearBuffer(QColor{0.15f,0.15f,0.75f});
	m_pShader->SetWorldViewProj(m_worldViewProj);
	m_pShader->SetWorldInvTranspose(m_worldInvTranspose);
	m_pShader->SetWorld(m_world);
	m_pImmediateContext->DrawIndexed(m_pBoxMeshData.pIndexBuffer->size(),0,0);
}

void MeshDemo::Clear()
{
	if (m_pDevice)
		delete m_pDevice;
	if (m_pImmediateContext)
		delete m_pImmediateContext;
	if (m_pShader)
		delete m_pShader;
	delete[] m_pBoxMeshData.pIndexBuffer;
	delete[] m_pBoxMeshData.pVertexBuffer;
}

void MeshDemo::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(m_hWnd);
}

void MeshDemo::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void MeshDemo::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = Math::convertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = Math::convertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = Math::Clamp(mPhi, 0.1f, 2*PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.01f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.01f*static_cast<float>(y - mLastMousePos.y);
		// Update the camera radius based on input.
		mRadius += dx - dy;
		// Restrict the radius.
		mRadius = Math::Clamp(mRadius, 0.0f, 10.0f);
	}
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void MeshDemo::OnKeyDown(WPARAM keyState)
{
	switch (keyState)
	{
	case VK_UP:
		mMeshPhi_y += 0.1f;
		break;
	case VK_DOWN:
		mMeshPhi_y -= 0.1f;
		break;
	case VK_LEFT:
		mMeshPhi_x += 0.1f;
		break;
	case VK_RIGHT:
		mMeshPhi_x -= 0.1f;
		break;
	case 0x31://按下数字键盘1
		renderState = 1;
		break;
	case 0x32:
		renderState = 2;
		
		break;
	case 0x33:
		renderState = 3;
		break;
	default:
		break;
	}
}

void MeshDemo::OnMenuLightSwitch(bool& State)
{
	m_pShader->SetLightEnable(State);//允许进行光照
	State = !State;
}

void MeshDemo::OnLoadBit()
{
	m_pTex.loaddBit(L"Media/Wood.bmp");
	m_pShader->SetTexture(m_pTex);
}

void MeshDemo::OnRemoveBit()
{
	m_pTex.removeBit();
	m_pShader->SetTexture(m_pTex);
}


