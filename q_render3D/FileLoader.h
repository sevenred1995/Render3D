#pragma once
//#include "RenderMath.h"
#include "Types.h"
#include <fstream>

#include <windows.h>
#include <gdiplus.h>
#include <windows.h>
#include <sstream>

#pragma comment(lib, "gdiplus.lib")
using namespace std;
using namespace Gdiplus;

class IFileLoader
{
public:
	IFileLoader();
	~IFileLoader();
	bool ImportTextureFromFile(std::string filePath,unsigned int& outWidth,unsigned int& outHeight,std::vector<Vector3>& QColorBuffer);
	bool ImportFile_OBJ(std::string filePath, std::vector<Vertex>& refVertexBuffer, std::vector<UINT>& refIndexBuffer);
	bool ImportFile_PPM(std::string filePath, UINT& outWidth, UINT& outHeight, std::vector<QColor>& outQColorBuffer);
	bool ExportFile_PPM(std::string filePath, UINT width, UINT height, const std::vector<QColor>& QColorBuffer, bool bOverrideExistedFile);
	bool ImportFile_Bimp(std::wstring filePath,unsigned int& outWidth,unsigned int& outHeight, std::vector<QColor>& outQColorBuffer);
};

