#pragma once
#include "GeometryMeshGenerator.h"
#include "FileLoader.h"
#include "IRenderPipelineDevice.h"
#include "Types.h"
class IMesh :
	public IGeometryMeshGenerator,IFileLoader
{
public:
	IMesh();
	~IMesh();
	void setType();
	void createMeshData(string filePath);
	bool render();
	//bool renderStandard();
private:
	IRenderPipelineDevice renderDevice;
	DrawData indata;
private:
	drawType type;
};

