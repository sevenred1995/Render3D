#include "FileLoader.h"



FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

bool FileLoader::ImportTextureFromFile(std::string filePath, unsigned int & outWidth, unsigned int & outHeight, std::vector<Vector3>& colorBuffer)
{
	std::ifstream fileIn(filePath, std::ios::binary);
	if (fileIn.good()==false)
	{
		return false;
	}
	unsigned int maxColorIntensity;
	fileIn.seekg(3);
	std::streampos filepos = fileIn.tellg();
	fileIn >> outWidth >> outHeight >> maxColorIntensity;

	fileIn.seekg(1,std::ios::cur);

	unsigned char* byteBuffer = new unsigned char[outHeight*outWidth * 3];
	fileIn.read((char*)byteBuffer, 3 * outWidth*outHeight);
	colorBuffer.resize(outHeight*outWidth);
	fileIn.close();

	for (int i = 0; i < outWidth*outHeight; i++)
	{
		Vector3 tempColor(byteBuffer[3*i]/255.0f,byteBuffer[3*i+1]/255.0f,byteBuffer[3*i+2]/255.0f);
		colorBuffer.at(i) = tempColor;
	}
	return true;
}