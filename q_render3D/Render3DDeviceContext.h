#pragma once
#include"Render3DDevice.h"
#include"ShaderBase.h"
class Render3DDeviceContext
{
public:
	Render3DDeviceContext();
	~Render3DDeviceContext();
private:
	void   DrawTrangles(VertexShaderOut o1,VertexShaderOut o2,VertexShaderOut o3);
	int    trangleDevide(Trape* trap, Vertex p1, Vertex p2, Vertex p3);
	void   rasterizeTrangle(Trape *trap);
	void   trapedgeLerp(Trape* trap, float y);
	void   trapinitScanline(Trape* trap, Scanline* scanline, int y);
public:
	void Init(Render3DDevice* device);
	void SetVertexBuffer(std::vector<Vertex> vectices);
	void SetIndexBuffer(std::vector<UINT> indices);
	void SetShader(ShaderBase* base);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT startVertexLocation);
private:
	//vector<float>*       zBuffer;
	ShaderBase*          m_pShader;
	std::vector<Vertex>  m_vertices;
	std::vector<UINT>    m_indices;
	Render3DDevice*      m_device;
private:
	//²Ã¼ô¶¥µã
	int     homospaceCheckCvv(const Vector4* pos)
	{
		float w = pos->w;
		int check = 0;
		if (pos->z < 0.0f) check |= 1; //-->check0001
		if (pos->z >  w)   check |= 2;   //-->
		if (pos->x < -w)   check |= 4;   //-->check0111
		if (pos->x >  w)   check |= 8;
		if (pos->y < -w)   check |= 16;
		if (pos->y >  w)   check |= 32;
		return check;
	}
};

