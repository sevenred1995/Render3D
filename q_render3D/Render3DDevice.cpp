#include "Render3DDevice.h"

Render3DDevice::Render3DDevice(int width, int height)
{
	m_width   = width;
	m_height  = height;
	m_zBuffer = new std::vector<float>(m_width*m_height);
}
Render3DDevice::~Render3DDevice()
{
	if (m_pFrameBuffer)
		delete m_pFrameBuffer;
	if (m_zBuffer)
		delete m_zBuffer;
}
void Render3DDevice::DrawPixel(int pixelX, int pixelY, QColor color)
{
	m_pFrameBuffer[m_width*pixelY + pixelX] = Math::ColorToUINT(color);
}

void Render3DDevice::ClearBuffer(QColor color)
{
	for (int x = 0; x < m_width; ++x)
	{
		for (int y = 0; y < m_height; ++y)
		{
			m_pFrameBuffer[m_width*y + x] = Math::ColorToUINT(color);
		}
	}
	m_zBuffer->clear();
	m_zBuffer = new std::vector<float>(m_width*m_height);
}
void Render3DDevice::SetZ(int x, int y, float oz)
{
	m_zBuffer->at(y*m_width + x) = oz;
}

float Render3DDevice::GetZ(int x, int y)
{
	return m_zBuffer->at(y*m_width+x);
}

