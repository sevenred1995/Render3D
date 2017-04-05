#pragma once
#include"Types.h"
#include"Texture2D.h"
class ShaderBase
{
protected:
	Matrix4x4   m_worldViewProj;//WVP  ÊÀ½çÔ‡ˆD
	Texture2D   m_tex;
	Matrix4x4   m_world;
	Matrix4x4   m_worldInvTranspose;
	Material    m_material;
	QPoint      m_cameraPos;
	
	static const UINT	c_maxLightCount = 8;
	DirectionalLight    m_Light[c_maxLightCount];
	
	DirectionalLight    m_dirLight;
	bool                m_LightEnable;
public:
	void SetWorldViewProj(const Matrix4x4 worldViewProj);
	void SetTexture(const Texture2D& tex);
	void SetWorld(const Matrix4x4& world);
	void SetWorldInvTranspose(const Matrix4x4& worldInvTrans);
	void SetMaterial(const Material& material);
	void SetDirLight(const DirectionalLight& dirLight);
	void SetDirLightArray(int index, const DirectionalLight& dirLight);
	void SetLightEnable(bool light = true);
	void SetEyePos(QPoint cameraPos);
private:
	QColor vertexLighting(const QPoint vPosw, const Vector3 vNormalw, Vector3 & ambient, Vector3 & diffuse, Vector3 & spec);
public:
	ShaderBase();
	virtual ~ShaderBase();
	virtual VertexShaderOut  VS(Vertex& inVertex);
	virtual QColor           PS(VertexShaderOut vin);
};

