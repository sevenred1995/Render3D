#include "IRenderPipelineDevice.h"

IRenderPipelineDevice::IRenderPipelineDevice()
{
	//m_vertex_homospace        = new std::vector<VertexShaderOut>;
	//m_vertex_homospace_cliped = new std::vector<VertexShaderOut>;
	//m_index_homospace_cliped  = new std::vector<unsigned int>;
	m_rasterized              = new std::vector<RasterizedFragment>;
	world.set_identity();
	view.set_identity();
	projection.set_identity();
	cameraPos = { 0,0,0,1 };
	mLightEnable = false;
	for (UINT i = 0; i < c_maxLightCount; ++i)memset(&mLight[i], 0, sizeof(DirectionalLight));
}
IRenderPipelineDevice::~IRenderPipelineDevice()
{
	//std::vector<VertexShaderOut>(*m_vertex_homospace).swap(*m_vertex_homospace);
	//std::vector<VertexShaderOut>(*m_vertex_homospace_cliped).swap(*m_vertex_homospace_cliped);
	//std::vector<unsigned int>(*m_index_homospace_cliped).swap(*m_index_homospace_cliped);
	std::vector<RasterizedFragment>(*m_rasterized).swap(*m_rasterized);
	//delete m_Texture;
	//delete m_vertex_homospace;
	//delete m_vertex_homospace_cliped;
	delete m_rasterized;
	//delete m_index_homospace_cliped;
	delete m_pOutQColorBuffer;
	delete zBuffer;
}
bool  IRenderPipelineDevice::init(renderInitData data)
{
	mBufferwidth  = data.bufferWidth;
	mBufferheight = data.bufferHeight;
	if (data.framebuffer != nullptr)
	{
		m_pOutQColorBuffer = data.framebuffer;
		m_pOutBuffer = data.mFrameBuffer;
	}
	else
	{
		//throw exception;
	}
	if (data.zBuffer != nullptr)
	{
		zBuffer = data.zBuffer;
	}
	else
	{
		//throw exception;
	}
	return true;
}
void  IRenderPipelineDevice::setWorldMatrix(const Matrix4x4& world)
{
	this->world = world;
}
void  IRenderPipelineDevice::setViewMatrix(const Matrix4x4& view)
{
	this->view = view;
}
void  IRenderPipelineDevice::setProjMatrix(const Matrix4x4& proj)
{
	this->projection = proj;
}
void  IRenderPipelineDevice::setCameraPos(const  Vector4& pos) 
{
	this->cameraPos = pos;
}
void  IRenderPipelineDevice::setLightingEnable(bool isEnable)
{
	mLightEnable = isEnable;
}
void  IRenderPipelineDevice::setLight(int index,const DirectionalLight& light)
{
	if (index < 8)
		mLight[index] = light;
}
void  IRenderPipelineDevice::setMaterial(const Material& material)
{
	mMaterial = material;
}
void  IRenderPipelineDevice::setTexture(Texture2D* mTex)
{
	//TODO
	m_Texture = mTex;
}
void  IRenderPipelineDevice::drawQPoint()
{

}
void  IRenderPipelineDevice::drawTriangle(DrawData inData,int *ptr)
{
	m_rasterized->clear();
	zBuffer->clear();
	zBuffer = new std::vector<float>(mBufferheight*mBufferwidth);
	m_rasterized      ->reserve(mBufferheight*mBufferwidth);
	//for (int i = 0; i < inData.vCount; i++)
	//{
	//	Vertex currentVertex = inData.pVertexBuffer->at(i);
	//	VertexShaderOut out=VS(currentVertex);//顶点着色器
	//}
	for (int i = 0; i < inData.pIndexBuffer->size()-2; i += 3) {
		unsigned int index1 = inData.pIndexBuffer->at(i);
		unsigned int index2 = inData.pIndexBuffer->at(i+1);
		unsigned int index3 = inData.pIndexBuffer->at(i + 2);
		Vertex v1 = inData.pVertexBuffer->at(index1);
		VertexShaderOut out1 = VS(v1);

		Vertex v2 = inData.pVertexBuffer->at(index2);
		VertexShaderOut out2 = VS(v2);

		Vertex v3 = inData.pVertexBuffer->at(index3);
		VertexShaderOut out3 = VS(v3);
		//进行裁剪
		if (homospaceCheckCvv(&out1.QPoint.pos) != 0 || homospaceCheckCvv(&out2.QPoint.pos) != 0 || homospaceCheckCvv(&out3.QPoint.pos) != 0)
			continue;
		RasterizeTrangles(out1,out2,out3);
	}
	//裁剪
	//homospaceClipedTriangle(inData.pIndexBuffer);
	//RasterizeTrangles();
	for (auto& rasterVertex:*m_rasterized)
	{
		pixelShader(rasterVertex,ptr);
	}
}
//顶点处理器-->得到裁空间顶点坐标
VertexShaderOut  IRenderPipelineDevice::VS(Vertex& inVertex)
{
	//QColor texSampleQColor = sampleTexture(inVertex.tex.x, inVertex.tex.y);
	VertexShaderOut out;//输出顶点
	Vector4 pos(inVertex.pos.x, inVertex.pos.y, inVertex.pos.z, inVertex.pos.w);
	Vector4 projPos;
	Matrix4x4 transform_,transform2;
	matrix4x4_mul(&transform_, &world, &view);
	matrix4x4_mul(&transform2, &transform_,&projection);
	vector4_matrix4x4(&projPos, pos, transform2);
	out.QPoint.pos = projPos;//---->(x,y,z,w)-->(w=z);
	Matrix4x4 worldmat_notrans = world;
	worldmat_notrans.setColumn(3, { 0.0f, 0.0f, 0.0f, 1.0f });
	Vector4 normW(inVertex.normal.x, inVertex.normal.y, inVertex.normal.z, 1.0f);
	Vector4 normal;

	vector4_matrix4x4(&normal, normW, matrixTranspose(worldmat_notrans));
	out.QPoint.normal = Vector3(normal.x,normal.y,normal.z);
	out.QPoint.tex     = inVertex.tex;
	out.QPoint.normal.normalize();
	QColor texCol = sampleTexture(out.QPoint.tex.x, out.QPoint.tex.y);
	if(!mLightEnable)
	  out.QPoint.QColor = inVertex.QColor;
	else
	{
		Vector3 A, D, S;
		vertexLighting(inVertex.pos, out.QPoint.normal, A, D, S);
		out.QPoint.QColor = (A + D) + S;
	}
	return out;
	//m_vertex_homospace->push_back(out);
}

QColor   IRenderPipelineDevice::vertexLighting(
	const QPoint vPosw, 
	const Vector3 vNormalw,
	Vector3& ambient,
	Vector3& diffuse,
	Vector3& spec
  )
{
	QColor  outQColor = {0,0,0};
	for (int i = 0; i < c_maxLightCount; ++i)
	{
		if (mLight[i].isEnabled = true)
		{
			Vector3 lightDir = mLight[i].direction;
			lightDir.normalize();
			Vector4 toEye4 = cameraPos - vPosw;
			Vector3 toEye(toEye4.x, toEye4.y, toEye4.z);
			toEye.normalize();
			Vector3 normal=vNormalw;
			normal.normalize();
			Vector3 currentAmbient = mMaterial.ambient*mLight[i].ambient*mMaterial.diffuse;
			Vector3 currentDiffuse(0.0f, 0.0f, .0f);
			Vector3 currentSpecular(.0f,.0f,.0f);
			float   diffuseFactor = mLight[i].mDiffuseInstensity*(max(0.0f,vector3_dot(lightDir, normal)));
			if (diffuseFactor > 0.0f)
			{
				currentDiffuse = diffuseFactor*mLight[i].diffuse;
				currentDiffuse = currentDiffuse*mMaterial.diffuse;	
				Vector3 reflectLightDir = vector3_reflect(lightDir, normal);
				float specFactor = mLight[i].mSpeaularIntensity*
					pow(max(vector3_dot(reflectLightDir, toEye), 0.0f), mMaterial.specularSmoothLev);
				currentSpecular = specFactor* mMaterial.specular * mLight[i].specular;
			}
			ambient += currentAmbient;
			diffuse += currentDiffuse;
			spec    += currentSpecular;
			outQColor += (currentAmbient + currentDiffuse + currentSpecular);
		}
	}
	return outQColor;
}
Vector3 IRenderPipelineDevice::sampleTexture(float x, float y)
{
	if (m_Texture == nullptr)
		return Vector3(1.0f,1.0f,1.0f);
	unsigned int width  = m_Texture->getWidth();
	unsigned int height = m_Texture->getHeight();
	float pixelX = abs(width*(float(x - UINT(x))));
	float pixelY = abs(height*(float(y - UINT(y))));
	return m_Texture->getPixel(UINT(pixelX),UINT(pixelY));
}
void    IRenderPipelineDevice::pixelShader(RasterizedFragment& inVertex, int *ptr)
{
	int outQColor;
	outQColor = inVertex.QColor;
	int m = inVertex.pixelY*mBufferwidth + inVertex.pixelX;
	ptr[m] = outQColor;
}
void    IRenderPipelineDevice::Present(int *ptr)
{
	//ptr = m_pOutQColorBuffer;
}
void    IRenderPipelineDevice::RasterizeTrangles(
	VertexShaderOut o1,
	VertexShaderOut o2,
	VertexShaderOut o3)
{
	//每次处理三个顶点 即处理一个三角形
	//for (int i = 0; i < m_index_homospace_cliped->size() - 2; i += 3)
	//{
		//unsigned int index1 = m_index_homospace_cliped->at(i);
		//unsigned int index2 = m_index_homospace_cliped->at(i + 1);
		//unsigned int index3 = m_index_homospace_cliped->at(i + 2);
		RasterizedFragment outVertex; //光栅化片段
		Trape trap[2];

		//得到三角形的三个点
		auto& v1 =o1;
		auto& v2 =o2;
		auto& v3 =o3;

		//透视除法+标准设备空间
		auto convertTopixelSpace = [&](const Vector4& v,Vector4& out)
		{
			//v.w='z'
			float rhw = (float)(1/v.w);
			out.x = (v.x*rhw + 1.0f)*mBufferwidth*0.5f;
			out.y = (1.0 - v.y*rhw) *mBufferheight*0.5f;
			out.z = v.z*rhw;
			out.w = 1;
		};

		Vector4 c1, c2, c3;
		convertTopixelSpace(v1.QPoint.pos, c1);
		convertTopixelSpace(v2.QPoint.pos, c2);
		convertTopixelSpace(v3.QPoint.pos, c3);

		Vertex p1, p2, p3;
		p1 = v1.QPoint; p2 = v2.QPoint; p3 = v3.QPoint;

		auto rhw_init = [&](Vertex& v,float zSpace) 
		{
			float rhw  = 1.0/zSpace;//1/z
			v.rhw      = rhw;
			v.tex     *= rhw;
			v.QColor  *= rhw;
			v.normal  *= rhw;
		};
		p1.pos = c1; 
		p2.pos = c2; 
		p3.pos = c3;
		rhw_init(p1,v1.QPoint.pos.w);
		rhw_init(p2,v2.QPoint.pos.w);
		rhw_init(p3,v3.QPoint.pos.w);
		int n=trangleDevide(trap, p1, p2, p3);
		//printf("%f--%f\n", trap[0].right.v2.pos.x,trap[1].right.v1.pos.x);
		if (n >= 1)rasterizeTrangle(&trap[0]);
	    if (n >= 2)rasterizeTrangle(&trap[1]);

	//}
}
void    IRenderPipelineDevice::RasterizeQPoint()
{
	//for (int i = 0; i < m_index_homospace_cliped->size; i++) {
	//	RasterizedFragment outVertex;
	//	int idx = m_index_homospace_cliped->at(i);
	//	const auto& v1 = m_vertex_homospace_cliped->at(idx);
	//	auto convertToPixelSpace = [&](const Vector4& v, Vector4& outV)
	//	{
	//		float rhw = (float)(1 / v.w);
	//		outV.x = (v.x*rhw + 1.0f)*mBufferwidth*0.5f;
	//		outV.y = (1.0 - v.y*rhw) *mBufferheight*0.5f;
	//		outV.z = v.z*rhw;
	//		outV.w = 1;
	//	};

	//	Vector4 c;
	//	convertToPixelSpace(v1.QPoint.pos,c);
	//	Vertex p = v1.QPoint;
	//	auto rhw_init = [&](Vertex& v, float zSpace)
	//	{
	//		float rhw = 1.0 / zSpace;//1/z
	//		v.rhw = rhw;
	//		v.tex *= rhw;
	//		v.QColor *= rhw;
	//	};
	//	p.pos = c;
	//}
}
int     IRenderPipelineDevice::trangleDevide(Trape* trap,Vertex p1,Vertex p2,Vertex p3)
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
		trap[0].left.v1  = p1;
		trap[0].left.v2  = p2;
		trap[0].right.v1 = p1;
		trap[0].right.v2 = p3;
		trap[1].left.v1  = p2;
		trap[1].left.v2  = p3;
		trap[1].right.v1 = p1;
		trap[1].right.v2 = p3;
	}
	else {					// triangle right
		trap[0].left.v1  = p1;
		trap[0].left.v2  = p3;
		trap[0].right.v1 = p1;
		trap[0].right.v2 = p2;

		trap[1].left.v1  = p1;
		trap[1].left.v2  = p3;
		trap[1].right.v1 = p2;
		trap[1].right.v2 = p3;
	}
	return 2;
}
void    IRenderPipelineDevice::rasterizeTrangle(Trape *trap)
{
	Scanline scaline;
	int j, top, bottom;
	top    = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (j=top;j<bottom;++j)
	{
		if (j >= 0 && j < mBufferheight)
		{
			//y变化获取三角形的边上顶点
			trapedgeLerp(trap, (float)j + 0.5);
			//扫描线的初始化
			//printf("%d----%d\n", trap->right.v.pos.y, trap->left.v.pos.y);
			trapinitScanline(trap, &scaline, j);
			//绘制扫描线
			int x = scaline.x;
			//printf("%d\n", x);
			int y = j;
			int w = scaline.w;
			int _width = mBufferwidth;
			for (; w > 0; x++, w--)
			{
				if (x > 0 && x < _width)
				{
					float rhw = scaline.v.rhw;
				   if (rhw >= zBuffer->at(y*mBufferwidth + x))
				   {
						//设置缓存
						zBuffer->at(y*mBufferwidth + x) = rhw;
						float r = scaline.v.QColor.x *(1 / rhw);
						float g = scaline.v.QColor.y *(1 / rhw);
						float b = scaline.v.QColor.z *(1 / rhw);
						
						VertexShaderOut out;
						out.QPoint.QColor  = QColor(r,g,b);
						out.QPoint.normal = scaline.v.normal*(1/rhw);// Vector3();
						out.QPoint.pos    = scaline.v.pos*(1/rhw);
						out.QPoint.tex    = scaline.v.tex*(1/rhw);
						QColor outCol = this->PS(out);
						int R = (int)(outCol.x * 255.0f);
						int G = (int)(outCol.y * 255.0f);
						int B = (int)(outCol.z * 255.0f);
						if (R < 0)R = 0; if (R > 255)R = 255;
						if (G < 0)R = 0; if (G > 255)G = 255;
						if (B < 0)R = 0; if (B > 255)B = 255;
						RasterizedFragment outVertex;
						outVertex.pixelX = x;
						outVertex.pixelY = y;
						outVertex.QColor = (R << 16) | (G << 8) | B;
						//我们在这里进行像素着色...
						m_rasterized->push_back(outVertex);
					}
				}
				//增加长度
				scaline.v.pos    += scaline.step.pos;
				scaline.v.QColor += scaline.step.QColor;
				scaline.v.rhw    += scaline.step.rhw;
				scaline.v.tex    += scaline.step.tex;
				if (x >= _width)break;
			}
		}
		if (j >= mBufferheight)break;
	}
}
void    IRenderPipelineDevice::trapedgeLerp(Trape* trap, float y)
{
	float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
	float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
	float t1 = (y - trap->left.v1.pos.y) / s1;
	float t2 = (y - trap->right.v1.pos.y) / s2;

	vector4_lerp(&trap->left.v.pos,    &trap->left.v1.pos, &trap->left.v2.pos, t1);//坐标插值
	Vector2_lerp(&trap->left.v.tex,    &trap->left.v1.tex, &trap->left.v2.tex, t1);
	Vector3_lerp(&trap->left.v.QColor,  &trap->left.v1.QColor, &trap->left.v2.QColor, t1);
	Vector3_lerp(&trap->left.v.normal, &trap->left.v1.normal, &trap->left.v2.normal, t1);
	trap->left.v.rhw = lerp(trap->left.v1.rhw, trap->left.v2.rhw, t1);
	vector4_lerp(&trap->right.v.pos,    &trap->right.v1.pos,    &trap->right.v2.pos, t2);//坐标插值
	Vector2_lerp(&trap->right.v.tex,    &trap->right.v1.tex,    &trap->right.v2.tex, t2);
	Vector3_lerp(&trap->right.v.QColor,  &trap->right.v1.QColor,  &trap->right.v2.QColor,t2);
	Vector3_lerp(&trap->right.v.normal, &trap->right.v1.normal, &trap->right.v2.normal,t2);
	trap->right.v.rhw = lerp(trap->right.v1.rhw, trap->right.v2.rhw, t2);
}
void    IRenderPipelineDevice::trapinitScanline(Trape* trap, Scanline* scanline, int y)
{
	float width = trap->right.v.pos.x - trap->left.v.pos.x;
	scanline->x = (int)(trap->left.v.pos.x + 0.5f);
	scanline->w = (int)(trap->right.v.pos.x + 0.5) - scanline->x;
	scanline->y = y;
	scanline->v = trap->left.v;
	if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
	float inv = 1 / width;
	scanline->step.pos   =    (trap->right.v.pos - trap->left.v.pos)*inv;
	scanline->step.QColor =    (trap->right.v.QColor - trap->left.v.QColor)*inv;
	scanline->step.tex   =    (trap->right.v.tex - trap->left.v.tex)*inv;
	scanline->step.rhw   =    (trap->right.v.rhw - trap->left.v.rhw)*inv;
}

QColor IRenderPipelineDevice::PS(VertexShaderOut vin) {
	return sampleTexture(vin.QPoint.tex.x,vin.QPoint.tex.y)*vin.QPoint.QColor;
}