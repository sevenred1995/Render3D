#pragma once
#include "Types.h"
#include "Texture2D.h"
class Render3DDevice
{
public:
	Render3DDevice(int width,int height);
	~Render3DDevice();
	void DrawPixel(int pixelX, int pixelY, QColor color);
	inline UINT*& GetFrameBuffer()  { return m_pFrameBuffer;}
	inline int    GetClientWidth()  { return m_width; }
	inline int    GetClientHeight() { return m_height;}
	void          ClearBuffer(QColor color);
	void          SetZ(int x, int y, float oz);
	float         GetZ(int x, int y);
private:
	int m_width;
	int m_height;
	UINT* m_pFrameBuffer;
	std::vector<float>* m_zBuffer;
};

