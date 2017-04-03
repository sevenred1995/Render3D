#include "Mesh.h"



IMesh::IMesh()
{
	indata.pVertexBuffer = new vector<Vertex>();
	indata.pIndexBuffer  = new vector<unsigned int>();
	indata.vCount = 0;
}


IMesh::~IMesh()
{
	delete indata.pIndexBuffer;
	delete indata.pVertexBuffer;
}
void IMesh::setType()
{
}
void IMesh::createMeshData(string filePath)
{
	if (type == DRAWFILE)
	{
		IFileLoader::ImportFile_OBJ(filePath, *indata.pVertexBuffer, *indata.pIndexBuffer);
	}
	else if (type == SPHERE) {
		IGeometryMeshGenerator::creatSphere(2, 50, 50, false, *indata.pVertexBuffer, *indata.pIndexBuffer);
	}
	else if (type == BOX) {
		IGeometryMeshGenerator::createBox(3, 3, 3, 2, 2, 2, *indata.pVertexBuffer, *indata.pIndexBuffer);
	}
}
bool IMesh::render()
{

	return false;
}
