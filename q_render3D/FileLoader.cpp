#include "FileLoader.h"


IFileLoader::IFileLoader()
{
}


IFileLoader::~IFileLoader()
{
}

bool IFileLoader::ImportTextureFromFile(std::string filePath, unsigned int & outWidth, unsigned int & outHeight, std::vector<Vector3>& QColorBuffer)
{
	std::ifstream fileIn(filePath, std::ios::binary);
	if (fileIn.good()==false)
	{
		return false;
	}
	unsigned int maxQColorIntensity;
	fileIn.seekg(3);
	std::streampos filepos = fileIn.tellg();
	fileIn >> outWidth >> outHeight >> maxQColorIntensity;

	fileIn.seekg(1,std::ios::cur);

	unsigned char* byteBuffer = new unsigned char[outHeight*outWidth * 3];
	fileIn.read((char*)byteBuffer, 3 * outWidth*outHeight);
	QColorBuffer.resize(outHeight*outWidth);
	fileIn.close();

	for (int i = 0; i < outWidth*outHeight; i++)
	{
		Vector3 tempQColor(byteBuffer[3*i]/255.0f,byteBuffer[3*i+1]/255.0f,byteBuffer[3*i+2]/255.0f);
		QColorBuffer.at(i) = tempQColor;
	}
	return true;
}
bool IFileLoader::ImportFile_OBJ(std::string filePath, std::vector<Vertex>& refVertexBuffer, std::vector<UINT>& refIndexBuffer)
{
	std::ifstream fin(filePath);
	if (!fin)return false;
	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;
	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;
	Vector3 tempNormal;
	Vertex  tempCompleteV;
	for (UINT i = 0; i < vcount; i++) {
		fin >> tempCompleteV.pos.x >> tempCompleteV.pos.y >> tempCompleteV.pos.z;
		tempCompleteV.pos.w = 1;
		fin >> tempNormal.x >> tempNormal.y >> tempNormal.z;
		tempCompleteV.normal = tempNormal;
		tempCompleteV.QColor = QColor{ 0.5,1,1 };
		refVertexBuffer.push_back(tempCompleteV);
	}
	fin >> ignore;
	fin >> ignore;
	fin >> ignore;
	UINT IndexCount = 3 * tcount;
	for (int i = 0; i < tcount; i++)
	{
		int a, b, c;
		fin >> a >> b >> c;
		refIndexBuffer.push_back(a);
		refIndexBuffer.push_back(b);
		refIndexBuffer.push_back(c);
	}
	fin.close();
	return true;
}

bool IFileLoader::ImportFile_PPM(std::string filePath, UINT & outWidth, UINT & outHeight, std::vector<QColor>& outQColorBuffer)
{
	std::ifstream fileIn(filePath, std::ios::binary);
	if (fileIn.good() == false)
	{
		return false;
	}
	UINT maxQColorIntensity;
	//skip the file Format "P6/n"
	fileIn.seekg(3);
	std::streampos filePos = fileIn.tellg();
	fileIn >> outWidth >> outHeight >> maxQColorIntensity;
	//skip the '/r'
	fileIn.seekg(1, std::ios::cur);
	//read the main QColor chunk (binary)
	unsigned char* byteBuff = new unsigned char[outWidth*outHeight * 3];
	fileIn.read((char*)byteBuff, 3 * outWidth*outHeight);
	outQColorBuffer.resize(outWidth*outHeight);
	fileIn.close();
	//output
	for (UINT i = 0; i < outWidth*outHeight; ++i)
	{
		QColor tmpQColor;
		tmpQColor = { byteBuff[3 * i] / 255.0f,byteBuff[3 * i + 1] / 255.0f,byteBuff[3 * i + 2] / 255.0f };
		outQColorBuffer.at(i) = tmpQColor;
	}

	return true;
}

bool IFileLoader::ExportFile_PPM(std::string filePath, UINT width, UINT height, const std::vector<QColor>& QColorBuffer, bool bOverrideExistedFile)
{
	return false;
}

bool IFileLoader::ImportFile_Bimp(std::wstring filePath, 
	unsigned int & outWidth, 
	unsigned int & outHeight, 
	std::vector<QColor>& outQColorBuffer)
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, nullptr);
	Bitmap* bmp = new Bitmap(filePath.c_str());

	if (!bmp)
	{
		MessageBox(nullptr, L"error", L"picture path is null!", MB_OK);
		delete bmp;
		GdiplusShutdown(gdiplustoken);
		return false;
		//return Texture2D(0, 0);
	}
	else
	{
		outHeight = bmp->GetHeight();
		outWidth  = bmp->GetWidth();
		//≥ı ºªØTexture2D
		//Texture2D texture(width, height);
		outQColorBuffer.resize(outWidth*outHeight);
		Color color;
		for (int x = 0; x < outWidth; x++)
			for (int y = 0; y < outHeight; y++)
			{
				bmp->GetPixel(x, y, &color);
				QColor tempCol = QColor(
					color.GetRed() / 255.f,
					color.GetGreen() / 255.f,
					color.GetBlue() / 255.f);
				outQColorBuffer.at(x*outWidth + y) = tempCol;
			}
		delete bmp;
		GdiplusShutdown(gdiplustoken);
	}
	return true;
}
