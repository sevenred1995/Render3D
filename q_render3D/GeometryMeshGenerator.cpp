#include "GeometryMeshGenerator.h"

IGeometryMeshGenerator::IGeometryMeshGenerator()
{
}


IGeometryMeshGenerator::~IGeometryMeshGenerator()
{
}

bool IGeometryMeshGenerator::ImportFile_OBJ(std::string filePath, 
	std::vector<Vertex>& refVertexBuffer, 
	std::vector<UINT>& refIndexBuffer)
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
		tempCompleteV.QColor = QColor{0.5,1,1};
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

void IGeometryMeshGenerator::createPlane(float width, float depth, unsigned int rowCnt, unsigned int columnCount, std::vector<Vertex>& outVerticeList, std::vector<unsigned int>& outIndicesList)
{
	buildQuad(Vector3(width / 2, 0, depth / 2),
		Vector3(width / (float)(columnCount - 1), 0, 0),
		Vector3(0, 0, -depth / (float)(rowCnt - 1)),
		rowCnt, columnCount, 0, outVerticeList, outIndicesList);
}
void IGeometryMeshGenerator::createBox(float width, float height, float depth, unsigned int depthStep, unsigned int widthStep, unsigned int heightStep, std::vector<Vertex>& outVerticeList, std::vector<unsigned int>& outIndicesList)
{
	int tempIndex;
	//bottom;
	float tempStep1 = width / (float)(widthStep - 1);
	float tempStep2 = depth / (float)(depthStep - 1);
	tempIndex = 0;
	buildQuad(
		Vector3(width/2,-height/2,depth/2),
		Vector3(-tempStep1,0,0),
		Vector3(0,0,-tempStep2),
		widthStep,
		depthStep,
		tempIndex,
		outVerticeList,
		outIndicesList
	);

	//top;
	tempStep1 = width / (widthStep - 1);
	tempStep2 = depth / (depthStep - 1);
	tempIndex = outVerticeList.size();
	buildQuad(
		Vector3(width / 2, height / 2, depth / 2),
		Vector3(-tempStep1, 0, 0),
		Vector3(0, 0, -tempStep2),
		widthStep,
		depthStep,
		tempIndex,
		outVerticeList,
		outIndicesList
	);
	//left
	tempStep1 = height / (heightStep - 1);
	tempStep2 = depth / (depthStep - 1);
	tempIndex = outVerticeList.size();
	buildQuad(
		Vector3(width / 2, height / 2, depth / 2),
		Vector3(0, -tempStep1, 0),
		Vector3(0, 0, -tempStep2),
		depthStep,
		heightStep, tempIndex,
		outVerticeList,
		outIndicesList
	);

	//right
	tempStep1 = height / (heightStep - 1);
	tempStep2 = depth / (depthStep-1);
	tempIndex = outVerticeList.size();
	buildQuad(
		Vector3(-width / 2, height / 2, depth / 2),
		Vector3(0, -tempStep1, 0),
		Vector3(0, 0, -tempStep2),
		heightStep,
		depthStep, tempIndex,
		outVerticeList,
		outIndicesList
	);

	//front
	tempStep1 = width / (widthStep - 1);
	tempStep2 = height / (heightStep - 1);
	tempIndex = outVerticeList.size();
	buildQuad(
		Vector3(width / 2, -height / 2, depth / 2),
		Vector3(-tempStep1,0, 0),
		Vector3(0, tempStep2, 0),
		widthStep,
		heightStep, tempIndex,
		outVerticeList,
		outIndicesList
	);
	//back
	tempStep1 = width / (widthStep - 1);
	tempStep2 = height / (heightStep - 1);
	tempIndex = outVerticeList.size();
	buildQuad(
		Vector3(-width / 2, height / 2, -depth / 2),
		Vector3(tempStep1, 0, 0),
		Vector3(0, -tempStep2, 0),
		widthStep,
		heightStep, tempIndex,
		outVerticeList,
		outIndicesList
	);
}
void IGeometryMeshGenerator::creatSphere(float radius, unsigned int columnCnt,
	unsigned int iRingCnt,
	bool bInvertNormal,
	std::vector<Vertex>& outVerticeList,
	std::vector<unsigned int>& outIndicesList)
{
	Vector4* tempV;
	Vector2* tmpTexCoord;
	unsigned int tempVertexCnt = (columnCnt + 1)*iRingCnt+2;
	tempV = new Vector4[tempVertexCnt];
	tmpTexCoord = new Vector2[tempVertexCnt];
	tempV[tempVertexCnt - 2] = Vector4(0, radius, 0, 1);
	tempV[tempVertexCnt - 1] = Vector4(0, -radius, 0, 1);
	tmpTexCoord[tempVertexCnt - 2] = Vector2(0.5f, 0);	 //TOP vertex
	tmpTexCoord[tempVertexCnt - 1] = Vector2(0.5f, 1.0f);//BOTTOM vertex
	float	tmpX, tmpY, tmpZ, tmpRingRadius;

	//Calculate the Step length (步长)
	float	StepLength_AngleY = PI / (iRingCnt + 1); // distances between each level (ring)
	float   StepLength_AngleXZ = 2 * PI / columnCnt;

	UINT k = 0;//for iteration
			   //start to iterate
	for (UINT i = 0; i < iRingCnt; i++)
	{
		//Generate Vertices ring By ring ( from top to down )
		//the first column will be duplicated to achieve adequate texture mapping
		for (UINT j = 0; j < columnCnt + 1; j++)
		{
			//the Y coord of  current ring 
			tmpY = radius *sin(PI / 2 - (i + 1) *StepLength_AngleY);
			////Pythagoras theorem(勾股定理)
			tmpRingRadius = sqrtf(radius*radius - tmpY * tmpY);
			////trigonometric function(三角函数)
			tmpX = tmpRingRadius * cos(j*StepLength_AngleXZ);
			//...
			tmpZ = tmpRingRadius * sin(j*StepLength_AngleXZ);

			//...
			tempV[k] = Vector4(tmpX, tmpY, tmpZ,1.0f);

			//map the i,j to closed interval [0,1] respectively , to proceed a spheric texture wrapping
			tmpTexCoord[k] = Vector2((float)j / (columnCnt), (float)i / (iRingCnt - 1));

			k++;
		}
	}
	//add to Memory
	Vertex tmpCompleteV;
	for (UINT i = 0; i < tempVertexCnt; i++)
	{

		tmpCompleteV.pos = tempV[i];
		tmpCompleteV.normal = Vector3(tempV[i].x / radius, tempV[i].y / radius, tempV[i].z / radius);
		tmpCompleteV.QColor = Vector3(tempV[i].x / radius, tempV[i].y / radius, tempV[i].z / radius);
		tmpCompleteV.tex = tmpTexCoord[i];
		if (bInvertNormal == TRUE)tmpCompleteV.normal = tmpCompleteV.normal*(-1);
		outVerticeList.push_back(tmpCompleteV);
	}
	for (UINT i = 0; i<iRingCnt - 1; i++)
	{
		for (UINT j = 0; j<columnCnt; j++)
		{
			outIndicesList.push_back(i*			(columnCnt + 1) + j + 0);
			outIndicesList.push_back(i*			(columnCnt + 1) + j + 1);
			outIndicesList.push_back((i + 1)*	(columnCnt + 1) + j + 0);

			outIndicesList.push_back(i*			(columnCnt + 1) + j + 1);
			outIndicesList.push_back((i + 1)*	(columnCnt + 1) + j + 1);
			outIndicesList.push_back((i + 1)*	(columnCnt + 1) + j + 0);
		}
	}
	//deal with the TOP/BOTTOM
	for (UINT j = 0; j<columnCnt; j++)
	{
		outIndicesList.push_back(j + 1);
		outIndicesList.push_back(j);
		outIndicesList.push_back(tempVertexCnt - 2);	//index of top vertex

		outIndicesList.push_back((columnCnt + 1)* (iRingCnt - 1) + j);
		outIndicesList.push_back((columnCnt + 1) * (iRingCnt - 1) + j + 1);
		outIndicesList.push_back(tempVertexCnt - 1); //index of bottom vertex
	}
}

void IGeometryMeshGenerator::createCylinder(float radius, float height, unsigned int columnCnt, unsigned int iRingCnt, std::vector<Vertex>& outVerticeList, std::vector<unsigned int>& outIndicesList)
{

}

inline void IGeometryMeshGenerator::buildQuad
       (
		   Vector3 originQPoint, Vector3 baseVector1, Vector3 baseVector2, 
		   unsigned int stepCount1, unsigned stepCount2, UINT iBaseIndex, 
		   std::vector<Vertex>& outVerticeList, 
		   std::vector<unsigned int>& outIndicesList
	   )
{
	Vector3 tempNormal;
	Vertex  tempCompleteV;
	vector3_cross(&tempNormal, baseVector1, baseVector2);//得到法线向量
	tempNormal.normalize();
	for (int i=0;i<stepCount1;i++)
	{
		for (int j=0;j<stepCount2;j++)
		{
			tempCompleteV.normal = tempNormal;
			Vector3 pos= Vector3(originQPoint + (float)i*baseVector1 + (float)j*baseVector2);
			tempCompleteV.pos   = Vector4(pos.x, pos.y, pos.z, 1.0f);
			tempCompleteV.QColor = Vector3(((float)i / stepCount1), ((float)j / stepCount2), 0.5f);
			tempCompleteV.tex   = Vector2((float)i / (stepCount1 - 1), ((float)j / stepCount2));
			outVerticeList.push_back(tempCompleteV);
		}
	}
	for (unsigned int i = 0; i < stepCount1 - 1; i++)
	{
		for (int j=0;j<stepCount2-1;j++)
		{
			outIndicesList.push_back(iBaseIndex + i *		stepCount2 + j);
			outIndicesList.push_back(iBaseIndex + (i + 1)*  stepCount2 + j);
			outIndicesList.push_back(iBaseIndex + i *		stepCount2 + j + 1);

			outIndicesList.push_back(iBaseIndex + i *		stepCount2 + j + 1);
			outIndicesList.push_back(iBaseIndex + (i + 1)  *stepCount2 + j);
			outIndicesList.push_back(iBaseIndex + (i + 1)  *stepCount2 + j + 1);
		}
	}
}
