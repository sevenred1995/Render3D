#pragma once
//#include "RenderMath.h"
#include "Types.h"
#include <fstream>


class FileLoader
{
public:
	FileLoader();
	~FileLoader();
	bool ImportTextureFromFile(std::string filePath,unsigned int& outWidth,unsigned int& outHeight,std::vector<Vector3>& colorBuffer);

};

