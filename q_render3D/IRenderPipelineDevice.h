#pragma once
#include "Types.h"
#include "Texture2D.h"
class IRenderPipelineDevice
{
public:
	IRenderPipelineDevice();
	~IRenderPipelineDevice();
	bool init(renderInitData data);
	void setWorldMatrix(const Matrix4x4& world);
	void setViewMatrix(const Matrix4x4& view);
	void setProjMatrix(const Matrix4x4& proj);
	void setCameraPos(const  Vector4& pos);
	void setLightingEnable(bool isEnable);
	void setLight(int Index,const DirectionalLight& light);
	void setMaterial(const Material& material);
	void setTexture(Texture2D* mTex);
	void Present(int *ptr);
public:
	void drawQPoint();      //绘制点
	void drawTriangle(DrawData inData,int* ptr);   //绘制三角形
private:
	VertexShaderOut  VS(Vertex& inVertex);//进行顶点变换
	QColor vertexLighting(const QPoint vPosw, const Vector3 vNormalw,	Vector3& ambient,
		Vector3& diffuse,
		Vector3& spec);//处理顶点光照
	void  pixelShader(RasterizedFragment& inVertex, int *ptr);
	QColor PS(VertexShaderOut vin);
	void  RasterizeTrangles(VertexShaderOut o1,
		VertexShaderOut o2,
		VertexShaderOut o3);
	void  RasterizeQPoint();
	//三角形划分
	int   trangleDevide(Trape* trap,Vertex p1,Vertex p2,Vertex p3);
	void  rasterizeTrangle(Trape *trap);
	void  trapedgeLerp(Trape* trap, float y);
	void  trapinitScanline(Trape* trap,Scanline* scanline,int y);
private:
	float mBufferwidth;
	float mBufferheight;
	std::vector<float>* zBuffer;
	std::vector<unsigned int>* m_pOutQColorBuffer;//最终输出的像素颜色
private:
	float** mZbuffer;
	unsigned int** m_pOutBuffer;
	unsigned int** m_textureBuffer;
private:
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Matrix4x4 transform;
	Vector4   cameraPos;
	bool                mLightEnable;
	Material            mMaterial;
	static const UINT	c_maxLightCount = 8;
	DirectionalLight    mLight[c_maxLightCount];
	//纹理
	Texture2D* m_Texture;
	std::vector<RasterizedFragment>* m_rasterized;
private:
	//裁剪顶点
	int  homospaceCheckCvv(const Vector4* pos)
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
	Vector3 sampleTexture(float x, float y);
};

