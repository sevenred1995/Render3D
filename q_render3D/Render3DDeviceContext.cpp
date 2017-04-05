#include "Render3DDeviceContext.h"



Render3DDeviceContext::Render3DDeviceContext()
{
}
Render3DDeviceContext::~Render3DDeviceContext()
{
}
void    Render3DDeviceContext::DrawTrangles(VertexShaderOut o1,VertexShaderOut o2,VertexShaderOut o3)
{
	//RasterizedFragment outVertex; //光栅化片段
	Trape trap[2];
	//得到三角形的三个点
	auto& v1 = o1;
	auto& v2 = o2;
	auto& v3 = o3;
	//透视除法+标准设备空间
	auto convertTopixelSpace = [&](const Vector4& v, Vector4& out)
	{
		//v.w='z'
		float rhw = (float)(1 / v.w);
		out.x = (v.x*rhw + 1.0f)*m_device->GetClientWidth()*0.5f;
		out.y = (1.0 - v.y*rhw) *m_device->GetClientHeight()*0.5f;
		out.z = v.z*rhw;
		out.w = 1;
	};

	Vector4 c1, c2, c3;
	convertTopixelSpace(v1.QPoint.pos, c1);
	convertTopixelSpace(v2.QPoint.pos, c2);
	convertTopixelSpace(v3.QPoint.pos, c3);

	Vertex p1, p2, p3;
	p1 = v1.QPoint; p2 = v2.QPoint; p3 = v3.QPoint;

	auto rhw_init = [&](Vertex& v, float zSpace)
	{
		float rhw = 1.0 / zSpace;//1/z
		v.rhw = rhw;
		v.tex *= rhw;
		v.QColor *= rhw;
		v.normal *= rhw;
	};
	p1.pos = c1;
	p2.pos = c2;
	p3.pos = c3;
	rhw_init(p1, v1.QPoint.pos.w);
	rhw_init(p2, v2.QPoint.pos.w);
	rhw_init(p3, v3.QPoint.pos.w);
	int n = trangleDevide(trap, p1, p2, p3);
	if (n >= 1)rasterizeTrangle(&trap[0]);
	if (n >= 2)rasterizeTrangle(&trap[1]);
	//}
}
int     Render3DDeviceContext::trangleDevide(Trape* trap, Vertex p1, Vertex p2, Vertex p3)
{
	float k, x;
	Vertex temp;
	if (p1.pos.y >  p2.pos.y)   temp = p1, p1 = p2, p2 = temp;
	if (p1.pos.y >  p3.pos.y)   temp = p1, p1 = p3, p3 = temp;
	if (p2.pos.y >  p3.pos.y)   temp = p2, p2 = p3, p3 = temp;
	if (p1.pos.y == p2.pos.y && p1.pos.y == p3.pos.y) return 0;
	if (p1.pos.x == p2.pos.x && p1.pos.x == p3.pos.x) return 0;
	if (p1.pos.y == p2.pos.y)
	{
		if (p1.pos.x > p2.pos.x)temp = p1, p1 = p2, p2 = temp;
		//Trape trap;
		trap[0].top = p1.pos.y;
		trap[0].bottom = p3.pos.y;
		trap[0].left.v1 = p1;
		trap[0].left.v2 = p3;
		trap[0].right.v1 = p2;
		trap[0].right.v2 = p3;
		return trap[0].top < trap[0].bottom ? 1 : 0;
	}
	if (p2.pos.y == p3.pos.y)
	{
		if (p2.pos.x > p3.pos.x) temp = p2, p2 = p3, p3 = temp;
		//Trape trap;
		trap[0].top = p1.pos.y;
		trap[0].bottom = p3.pos.y;
		trap[0].left.v1 = p1;
		trap[0].left.v2 = p2;
		trap[0].right.v1 = p1;
		trap[0].right.v2 = p3;
		return  trap[0].top < trap[0].bottom ? 1 : 0;
	}

	trap[0].top = p1.pos.y;
	trap[0].bottom = p2.pos.y;
	trap[1].top = p2.pos.y;
	trap[1].bottom = p3.pos.y;

	k = (p3.pos.y - p1.pos.y) / (p2.pos.y - p1.pos.y);
	x = p1.pos.x + (p2.pos.x - p1.pos.x) * k;

	if (x <= p3.pos.x) {		// triangle left
		trap[0].left.v1 = p1;
		trap[0].left.v2 = p2;
		trap[0].right.v1 = p1;
		trap[0].right.v2 = p3;
		trap[1].left.v1 = p2;
		trap[1].left.v2 = p3;
		trap[1].right.v1 = p1;
		trap[1].right.v2 = p3;
	}
	else {					// triangle right
		trap[0].left.v1 = p1;
		trap[0].left.v2 = p3;
		trap[0].right.v1 = p1;
		trap[0].right.v2 = p2;

		trap[1].left.v1 = p1;
		trap[1].left.v2 = p3;
		trap[1].right.v1 = p2;
		trap[1].right.v2 = p3;
	}
	return 2;
}
void    Render3DDeviceContext::rasterizeTrangle(Trape *trap)
{
	Scanline scaline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (j = top; j<bottom; ++j)
	{
		if (j >= 0 && j < m_device->GetClientHeight())
		{
			//y变化获取三角形的边上顶点
			trapedgeLerp(trap, (float)j + 0.5);
			trapinitScanline(trap, &scaline, j);
			int x = scaline.x;
			int y = j;
			int w = scaline.w;
			int _width = m_device->GetClientWidth();
			for (; w > 0; x++, w--)
			{
				if (x > 0 && x < _width)
				{
					float rhw = scaline.v.rhw;
					if (rhw >= m_device->GetZ(x,y))
					{
						//设置缓存
						//zBuffer->at(y*mBufferwidth + x) = rhw;
						m_device->SetZ(x, y, rhw);
						float r = scaline.v.QColor.x *(1 / rhw);
						float g = scaline.v.QColor.y *(1 / rhw);
						float b = scaline.v.QColor.z *(1 / rhw);

						VertexShaderOut out;
						out.QPoint.QColor = QColor(r, g, b);
						out.QPoint.normal = scaline.v.normal*(1 / rhw);// Vector3();
						out.QPoint.pos = scaline.v.pos*(1 / rhw);
						out.QPoint.tex = scaline.v.tex*(1 / rhw);
						m_device->DrawPixel(x,y,m_pShader->PS(out));
					}
				}
				//增加长度
				scaline.v.pos += scaline.step.pos;
				scaline.v.QColor += scaline.step.QColor;
				scaline.v.rhw += scaline.step.rhw;
				scaline.v.tex += scaline.step.tex;
				if (x >= _width)break;
			}
		}
		if (j >= m_device->GetClientHeight())break;
	}
}
void    Render3DDeviceContext::trapedgeLerp(Trape* trap, float y)
{
	float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
	float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
	float t1 = (y - trap->left.v1.pos.y) / s1;
	float t2 = (y - trap->right.v1.pos.y) / s2;

	vector4_lerp(&trap->left.v.pos, &trap->left.v1.pos, &trap->left.v2.pos, t1);//坐标插值
	Vector2_lerp(&trap->left.v.tex, &trap->left.v1.tex, &trap->left.v2.tex, t1);
	Vector3_lerp(&trap->left.v.QColor, &trap->left.v1.QColor, &trap->left.v2.QColor, t1);
	Vector3_lerp(&trap->left.v.normal, &trap->left.v1.normal, &trap->left.v2.normal, t1);
	trap->left.v.rhw = lerp(trap->left.v1.rhw, trap->left.v2.rhw, t1);
	vector4_lerp(&trap->right.v.pos, &trap->right.v1.pos, &trap->right.v2.pos, t2);//坐标插值
	Vector2_lerp(&trap->right.v.tex, &trap->right.v1.tex, &trap->right.v2.tex, t2);
	Vector3_lerp(&trap->right.v.QColor, &trap->right.v1.QColor, &trap->right.v2.QColor, t2);
	Vector3_lerp(&trap->right.v.normal, &trap->right.v1.normal, &trap->right.v2.normal, t2);
	trap->right.v.rhw = lerp(trap->right.v1.rhw, trap->right.v2.rhw, t2);
}
void    Render3DDeviceContext::trapinitScanline(Trape* trap, Scanline* scanline, int y)
{
	float width = trap->right.v.pos.x - trap->left.v.pos.x;
	scanline->x = (int)(trap->left.v.pos.x + 0.5f);
	scanline->w = (int)(trap->right.v.pos.x + 0.5) - scanline->x;
	scanline->y = y;
	scanline->v = trap->left.v;
	if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
	float inv = 1 / width;
	scanline->step.pos = (trap->right.v.pos - trap->left.v.pos)*inv;
	scanline->step.QColor = (trap->right.v.QColor - trap->left.v.QColor)*inv;
	scanline->step.tex = (trap->right.v.tex - trap->left.v.tex)*inv;
	scanline->step.rhw = (trap->right.v.rhw - trap->left.v.rhw)*inv;
}

void Render3DDeviceContext::Init(Render3DDevice * device)
{
	m_device = device;
}

void Render3DDeviceContext::SetVertexBuffer(std::vector<Vertex> vertices)
{
	if (!m_vertices.empty())m_vertices.clear();
	m_vertices = vertices;
}

void Render3DDeviceContext::SetIndexBuffer(std::vector<UINT> indices)
{
	if (!m_indices.empty())m_indices.clear();
	m_indices = indices;
}

void Render3DDeviceContext::SetShader(ShaderBase * base)
{
	m_pShader = base;
}

void Render3DDeviceContext::DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT startVertexLocation)
{
	for (int i = startIndexLocation; i < indexCount/3; ++i) {
		unsigned int index1 = m_indices.at(3*i);
		unsigned int index2 = m_indices.at(3*i + 1);
		unsigned int index3 = m_indices.at(3*i + 2);

		Vertex v1 = m_vertices.at(index1);
		VertexShaderOut out1 = m_pShader->VS(v1);
		Vertex v2 = m_vertices.at(index2);
		VertexShaderOut out2 = m_pShader->VS(v2);
		Vertex v3 = m_vertices.at(index3);
		VertexShaderOut out3 = m_pShader->VS(v3);
		//进行裁剪
		if (homospaceCheckCvv(&out1.QPoint.pos) != 0 || homospaceCheckCvv(&out2.QPoint.pos) != 0 || homospaceCheckCvv(&out3.QPoint.pos) != 0)
			continue;
		DrawTrangles(out1, out2, out3);
	}
	////裁剪
	////homospaceClipedTriangle(inData.pIndexBuffer);
	////RasterizeTrangles();
	//for (auto& rasterVertex : *m_rasterized)
	//{
	//	pixelShader(rasterVertex, ptr);
	//}
}
