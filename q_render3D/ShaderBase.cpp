#include "ShaderBase.h"
ShaderBase::ShaderBase()
{
}
ShaderBase::~ShaderBase()
{

}
void ShaderBase::SetWorldViewProj(const Matrix4x4 worldViewProj)
{
	m_worldViewProj = worldViewProj;
}
void ShaderBase::SetTexture(const Texture2D& tex)
{
	m_tex = tex;
}
void ShaderBase::SetEyePos(QPoint cameraPos)
{
	m_cameraPos = cameraPos;
}
void ShaderBase::SetWorld(const Matrix4x4& world)
{
	m_world = world;
}
void ShaderBase::SetWorldInvTranspose(const Matrix4x4& worldInvTrans)
{
	m_worldInvTranspose = worldInvTrans;
}
void ShaderBase::SetMaterial(const Material& material)
{
	m_material.ambient = material.ambient;
	m_material.diffuse = material.diffuse;
	m_material.specularSmoothLev = material.specularSmoothLev;
	m_material.specular = material.specular;
}
void ShaderBase::SetDirLight(const DirectionalLight& dirLight)
{
	m_dirLight.ambient = dirLight.ambient;
	m_dirLight.diffuse = dirLight.diffuse;
	m_dirLight.direction = dirLight.direction;
	m_dirLight.specular = dirLight.specular;
	m_dirLight.mDiffuseInstensity = dirLight.mDiffuseInstensity;
	m_dirLight.mSpeaularIntensity = dirLight.mSpeaularIntensity;
	m_dirLight.isEnabled = dirLight.isEnabled;
}

void ShaderBase::SetDirLightArray(int index, const DirectionalLight & dirLight)
{
	if (index < 8)
		m_Light[index] = dirLight;
}

VertexShaderOut ShaderBase::VS(Vertex & inVertex)
{
	VertexShaderOut out;//Êä³ö¶¥µã
	Vector4 pos(inVertex.pos.x, inVertex.pos.y, inVertex.pos.z, inVertex.pos.w);
	Vector4 projPos;
	vector4_matrix4x4(&projPos, pos, m_worldViewProj);
	out.QPoint.pos = projPos;//---->(x,y,z,w)-->(w=z);
	Vector4 normW(inVertex.normal.x, inVertex.normal.y, inVertex.normal.z, 1.0f);
	Vector4 normal;
	vector4_matrix4x4(&normal, normW,m_worldInvTranspose);
	out.QPoint.normal = Vector3(normal.x, normal.y, normal.z);
	out.QPoint.tex = inVertex.tex;
	out.QPoint.normal.normalize();
	QColor texCol = m_tex.sampleTexture(out.QPoint.tex.x, out.QPoint.tex.y);
	if (!m_LightEnable)
		out.QPoint.QColor = inVertex.QColor;
	else
	{
		Vector3 A, D, S;
		vertexLighting(inVertex.pos, out.QPoint.normal, A, D, S);
		out.QPoint.QColor = (A + D) + S;
	}
	return out;
}
QColor ShaderBase::PS(VertexShaderOut vin)
{
	return m_tex.sampleTexture(vin.QPoint.tex.x, vin.QPoint.tex.y)*vin.QPoint.QColor;
}
void ShaderBase::SetLightEnable(bool light)
{
	m_LightEnable = light;
}
QColor   ShaderBase::vertexLighting(const QPoint vPosw,
		const Vector3 vNormalw,
		Vector3& ambient,
		Vector3& diffuse,
		Vector3& spec
	)
	{

		QColor  outQColor = { 0,0,0 };
		if (m_LightEnable) {
			for (int i = 0; i < c_maxLightCount; ++i) {
				if (m_Light[i].isEnabled == true)
				{
					Vector3 lightDir = m_Light[i].direction;
					lightDir.normalize();
					Vector4 toEye4 = m_cameraPos - vPosw;
					Vector3 toEye(toEye4.x, toEye4.y, toEye4.z);
					toEye.normalize();
					Vector3 normal = vNormalw;
					normal.normalize();
					Vector3 currentAmbient = m_material.ambient*m_Light[i].ambient*m_material.diffuse;
					Vector3 currentDiffuse(0.0f, 0.0f, .0f);
					Vector3 currentSpecular(.0f, .0f, .0f);
					float   diffuseFactor = m_dirLight.mDiffuseInstensity*(max(0.0f, vector3_dot(lightDir, normal)));
					if (diffuseFactor > 0.0f)
					{
						currentDiffuse = diffuseFactor*m_Light[i].diffuse;
						currentDiffuse = currentDiffuse*m_material.diffuse;
						Vector3 reflectLightDir = vector3_reflect(lightDir, normal);
						float specFactor = m_Light[i].mSpeaularIntensity*
							pow(max(vector3_dot(reflectLightDir, toEye), 0.0f), m_material.specularSmoothLev);
						currentSpecular = specFactor* m_material.specular * m_Light[i].specular;
					}
					ambient += currentAmbient;
					diffuse += currentDiffuse;
					spec += currentSpecular;
					outQColor += (currentAmbient + currentDiffuse + currentSpecular);
				}
			}
		}
		return outQColor;
	}
