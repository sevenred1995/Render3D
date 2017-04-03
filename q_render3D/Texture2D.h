#pragma once
#include "FileLoader.h"
class Texture2D :
	public IFileLoader
{
public:
	bool loadPPM(std::string ppmFilePath);
	bool loaddBit(std::wstring bitmpFilePath);
public:
	Texture2D();
	~Texture2D();
	unsigned int getWidth();
	unsigned int getHeight();
	void   setPixel(unsigned int x,unsigned int y,const QColor& QColor);
	QColor getPixel(unsigned int x,unsigned int y);
private:
	std::vector<QColor>* m_pQColorBuffer;
	unsigned int mWidth;
	unsigned int mHeight;
};

