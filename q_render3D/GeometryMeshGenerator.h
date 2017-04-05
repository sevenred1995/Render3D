#pragma once
#include"Types.h"
#include<fstream>
#include<sstream>
class IGeometryMeshGenerator
{
public:
	static IGeometryMeshGenerator* GetInstance()
	{
		static IGeometryMeshGenerator instance;
		return &instance;
	}
	IGeometryMeshGenerator();
	~IGeometryMeshGenerator();
	bool ImportFile_OBJ(std::string filePath, std::vector<Vertex>& refVertexBuffer, std::vector<UINT>& refIndexBuffer);

	void createPlane(float width, float height, unsigned int rowCnt,unsigned int columnCount,
		std::vector<Vertex>& outVerticeList, 
		std::vector<unsigned int>& outIndicesList);
	void createBox(float width, float height, float depth, unsigned int depthStep, unsigned int widthStep, unsigned int heightStep,
		std::vector<Vertex>& outVerticeList,
		std::vector<unsigned int>& outIndicesList);
	void creatSphere(float radius, unsigned int columnCnt, unsigned int iRingCnt, bool bInvertNormal,
		std::vector<Vertex>& outVerticeList,
		std::vector<unsigned int>& outIndicesList);
	void createCylinder(float radius, float height, unsigned int columnCnt, unsigned int iRingCnt,
		std::vector<Vertex>& outVerticeList,
		std::vector<unsigned int>& outIndicesList);
private:
	void buildQuad(Vector3 originQPoint,Vector3 baseVector1, Vector3 baseVector2, unsigned int stepCount1, unsigned stepCount2, UINT iBaseIndex,
		std::vector<Vertex>& outVerticeList,
		std::vector<unsigned int>& outIndicesList);
};

