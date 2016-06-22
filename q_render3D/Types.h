#pragma once
#include"RenderMath.h"
#include<windows.h>
#include<vector>

using namespace Math;
typedef Vector4 Point;
typedef Vector2 Texcoord;
typedef Vector3 Color;

struct DirectionalLight
{
public:
	DirectionalLight()
	{
		ZeroMemory(this, sizeof(*this));
		mSpeaularIntensity = 1.0f;
		direction = Vector3(1.0f, 0.0, 0.0);
		mDiffuseInstensity = 0.5;
		isEnabled = true;
	}
	BOOL operator==(DirectionalLight& light)
	{
		if (memcmp(this, &light, sizeof(light)) == 0)
		{
			return TRUE;
		}
		return FALSE;
	}
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 direction;
	float mSpeaularIntensity;
	float mDiffuseInstensity;
	bool isEnabled;
};
struct Material 
{
	Material()
	{
		ambient = Vector3{ 0,0,0 };
		diffuse = { 1.0f,0,0 };
		specular = { 1.0,0,0 };
		specularSmoothLev = 1.0f;
	}
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	unsigned int specularSmoothLev;
};
struct Vertex 
{ 
	Point    pos; 
	Vector3  normal; 
	Texcoord tex; 
	Color    color; 
	float    rhw; 
};

//用于支持光栅化
typedef struct 
{ 
	Vertex v, v1, v2; 
} Edge;
typedef struct 
{ 
	float top, bottom; 
	Edge left, right; 
} Trape;
typedef struct
{ 
	Vertex v, step; 
	int x, y, w; 
} Scanline;
struct renderInitData 
{
	float bufferWidth, bufferHeight;
	float** mZbuffer;
	unsigned int** mFrameBuffer;
	std::vector<float>*         zBuffer;
	std::vector<unsigned int>*  framebuffer;
};
struct VertexShaderOut
{
	Vertex point;
};
struct RasterizedFragment
{
	int pixelX, pixelY;
	int color;
	Texcoord tex;
};
struct DrawData
{
	int vCount;
	std::vector<Vertex>* pVertexBuffer;
	std::vector<unsigned int>* pIndexBuffer;
};