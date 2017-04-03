#include "Texture2D.h"



bool Texture2D::loadPPM(std::string ppmFilePath)
{
	bool yes = IFileLoader::ImportFile_PPM(ppmFilePath,mWidth,mHeight,*m_pQColorBuffer);
	if (yes==true)return true;
	else {
		mWidth = 0;
		mHeight = 0;
		return false;
	}
}

bool Texture2D::loaddBit(std::wstring bitmpFilePath)
{
	bool yes = IFileLoader::ImportFile_Bimp(bitmpFilePath,mWidth,mHeight,*m_pQColorBuffer);
	if (yes == true)return true;
	else {
		mWidth = 0;
		mHeight = 0;
		return false;
	}
	return false;
}

Texture2D::Texture2D()
{
	m_pQColorBuffer = new std::vector<QColor>;
}


Texture2D::~Texture2D()
{
	delete m_pQColorBuffer;
}

unsigned int Texture2D::getWidth()
{
	return mWidth;
}

unsigned int Texture2D::getHeight()
{
	return mHeight;
}

void Texture2D::setPixel(unsigned int x, unsigned int y, const QColor & QColor)
{
	if (x<mWidth && y<mHeight)
	{
		unsigned int index = y*mWidth + x;
		m_pQColorBuffer->at(index) = QColor;
	}
}

QColor Texture2D::getPixel(unsigned int x, unsigned int y)
{
	if (x<mWidth && y<mHeight)
	{
		UINT index = y*mWidth + x;
		return m_pQColorBuffer->at(index);
	}
	return QColor(255, 0, 0);
}
