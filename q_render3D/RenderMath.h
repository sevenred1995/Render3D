#pragma once
#include<math.h>

const float PI = 3.1415926;

namespace Math
{

	struct Vector2
	{
		Vector2() {
		};
		Vector2(float _x, float _y) :x(_x), y(_y) {};
		Vector2(const float param[2])
		{
			x = param[0];
			y = param[1];
		}
		float length() {
			return sqrt(x*x + y*y);
		}
		void normlize() {
			float len = length();
			if (len != 0)
			{
				x /= len; y /= len;
			}
			else
			{
				x = 0.0f; y = 0.0f;
			}
		}
		bool operator==(const Vector2& vec)
		{
			return ((vec.x == x) && (vec.y == y));
		}
		Vector2 operator+(const Vector2& vec)
		{
			return Vector2(x + vec.x, y + vec.y);
		}
		Vector2 operator-(const Vector2& vec)
		{
			return Vector2(x - vec.x, y - vec.y);
		}
		Vector2& operator+=(const Vector2& vec)
		{
			x += vec.x;
			y += vec.y;
			return *this;
		}
		Vector2& operator-=(const Vector2& vec)
		{
			x -= vec.x;
			y -= vec.y;
			return *this;
		}
		Vector2  operator*(const float scaleFac)
		{
			return Vector2(scaleFac*x, scaleFac*y);
		}
		Vector2& operator*=(const float scalFac)
		{
			x *= scalFac;
			y *= scalFac;
			return *this;
		}
		float x;
		float y;
	};
	struct Vector3
	{
		float x, y, z;
		Vector3() :x(0), y(0), z(0) {};
		Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
		Vector3(float param[3])
		{
			x = param[0]; y = param[1]; z = param[2];
		}
		float length() {
			return sqrt(x*x + y*y + z*z);
		}
		void normalize()
		{
			float len = length();
			if (len != 0)
			{
				x /= len; y /= len; z /= len;
			}
			else
			{
				x = y = z = 0.0f;
			}
		}
		bool     operator==(const Vector3& vec)
		{
			return((x == vec.x) && (y == vec.y) && (z == vec.z));
		}
		Vector3  operator+(const Vector3& vec)
		{
			return Vector3(x + vec.x, y + vec.y, z + vec.z);
		}
		Vector3  operator-(const Vector3& vec)
		{
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}
		Vector3& operator+=(const Vector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		Vector3  operator*(const float scaleFac)
		{
			return Vector3(scaleFac*x, scaleFac*y, scaleFac*z);
		}
		Vector3  operator*(const Vector3& vec)
		{
			return Vector3(x*vec.x, y*vec.y, z*vec.z);
		}
		friend Vector3 operator*(float scalFac, const Vector3& vec)
		{
			return Vector3(scalFac*vec.x, scalFac*vec.y, scalFac*vec.z);
		}
		Vector3& operator*=(const float scalFac)
		{
			x *= scalFac; y *= scalFac; z *= scalFac;
			return*this;
		}
	};
	struct Vector4
	{
		float x, y, z, w;
		Vector4() :x(0), y(0), z(0), w(0) {};
		Vector4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
		Vector4(float param[4])
		{
			x = param[0]; y = param[1]; z = param[2]; w = param[3];
		}
		float length() {
			return (float)sqrt(x*x + y*y + z*z);
		}
		void normalize()
		{
			float len = length();
			if (len != 0)
			{
				x /= len; y /= len; z /= len;
			}
			else
			{
				x = y = z = 0.0f; w = 1.0f;
			}
		}
		bool operator==(const Vector4& vec)
		{
			return((x == vec.x) && (y == vec.y) && (z == vec.z) && (w = vec.w));
		}
		Vector4& operator=(const Vector4& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
			return *this;
		}
		Vector4 operator+(const Vector4& vec)
		{
			return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
		Vector4 operator-  (const Vector4& vec)
		{
			return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}
		Vector4 operator-  (Vector4& vec)
		{
			return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}
		Vector4& operator+=(const Vector4& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			w += vec.w;
			return *this;
		}
		Vector4& operator-=(const Vector4& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;
			return *this;
		}
		Vector4 operator*(const float scaleFac)
		{
			return Vector4(scaleFac*x, scaleFac*y, scaleFac*z, scaleFac*w);
		}
		Vector4 operator*(const Vector4& vec)
		{
			return Vector4(x*vec.x, y*vec.y, z*vec.z, w*vec.z);
		}
		friend Vector4 operator*(float scalFac, const Vector4& vec)
		{
			return Vector4(scalFac*vec.x, scalFac*vec.y, scalFac*vec.z, scalFac*vec.w);
		}
		Vector4& operator*=(const float scalFac)
		{
			x *= scalFac; y *= scalFac; z *= scalFac; w *= scalFac;
			return*this;
		}
	};
	struct Matrix4x4
	{
		Matrix4x4()
		{
			set_identity();
		}
		inline void	setRow(unsigned int row, float newRow[4])
		{
			if (row < 4)
			{
				for (unsigned int i = 0; i < 4; i++)
					m[row][i] = newRow[i];
			}
		};

		inline void	setRow(unsigned int row, float(&&newRow)[4])
		{
			if (row < 4)
			{
				for (unsigned int i = 0; i < 4; i++)
					m[row][i] = newRow[i];
			}
		};

		inline void	setColumn(unsigned int col, float newCol[4])
		{
			if (col < 4)
			{
				for (unsigned int i = 0; i < 4; ++i)
					m[i][col] = newCol[i];
			}
		};

		inline void	setColumn(unsigned int col, float(&&newCol)[4])
		{
			if (col < 4)
			{
				for (unsigned int i = 0; i < 4; ++i)
					m[i][col] = newCol[i];
			}
		};

		inline void setMatrix(Vector4(&&row)[4])
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				m[i][0] = row[i].x;
				m[i][1] = row[i].y;
				m[i][2] = row[i].z;
				m[i][3] = row[i].w;
			}
		};
		void set_identity()
		{
			setRow(0, { 1,0,0,0 });
			setRow(1, { 0,1,0,0 });
			setRow(2, { 0,0,1,0 });
			setRow(3, { 0,0,0,1 });
		};
		void set_zero()
		{
				setRow(0, { 0,0,0,0 });
				setRow(1, { 0,0,0,0 });
				setRow(2, { 0,0,0,0 });
				setRow(3, { 0,0,0,0 });
		}
		float m[4][4];
	};
	//添加我们需要的一些数学运算
	inline	void  vector4_cross(Vector4* result, const Vector4& m, const Vector4& n)
	{
		result->w = 1.0f;
		result->x = m.y*n.z - m.z*n.y;
		result->y = m.z*n.x - m.x*n.z;
		result->z = m.x*n.y - m.y*n.x;
	}
	inline  float vector3_dot(const Vector3& m, const Vector3& n)
	{
		return m.x*n.x + m.y*n.y + m.z*n.z;
	}
	inline  Vector3 vector3_reflect(Vector3 lightDir, Vector3 normal)
	{
		Vector3 In = lightDir;
		Vector3 Out = (-1)*In + 2 * (In - vector3_dot(In, normal) / In.length()*normal);
		return Out;
	}
	inline	float vector4_dot(const Vector4& m, const Vector4& n)
	{
		return m.x*n.x + m.y*n.y + m.z*n.z;
	}
	inline	void  matrix4x4_add(Matrix4x4* re, const Matrix4x4& a, const Matrix4x4& b)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				re->m[i][j] = a.m[i][j] + b.m[i][j];
			}
		}
	}
	inline	void  matrix4x4_sub(Matrix4x4* re, const Matrix4x4& a, const Matrix4x4& b)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				re->m[i][j] = a.m[i][j] - b.m[i][j];
			}
		}
	}
	inline	void  matrix4x4_mul(Matrix4x4* re, const Matrix4x4* a, const Matrix4x4* b)
	{
		Matrix4x4 z;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
					(a->m[j][1] * b->m[1][i]) +
					(a->m[j][2] * b->m[2][i]) +
					(a->m[j][3] * b->m[3][i]);
			}
		}
		re[0] = z;
	}
	//行向量x矩阵  类似direct3d变换
	inline	void  vector4_matrix4x4(Vector4* re, const Vector4& vec, const Matrix4x4& matrix)
	{
		float x = vec.x, y = vec.y, z = vec.z, w = vec.w;
		re->x = x*matrix.m[0][0] + y*matrix.m[1][0] + z*matrix.m[2][0] + w*matrix.m[3][0];
		re->y = x*matrix.m[0][1] + y*matrix.m[1][1] + z*matrix.m[2][1] + w*matrix.m[3][1];
		re->z = x*matrix.m[0][2] + y*matrix.m[1][2] + z*matrix.m[2][2] + w*matrix.m[3][2];
		re->w = x*matrix.m[0][3] + y*matrix.m[1][3] + z*matrix.m[2][3] + w*matrix.m[3][3];

	}


	//光栅化插值	
	inline	float lerp(float x1,float x2,float t)
	{
		return x1+(x2-x1)*t;
	}

	inline	Vector2 Vector2_lerp(Vector2* z, const Vector2* t1, const Vector2* t2, float t)
	{
		z->x = lerp(t1->x, t2->x, t);
		z->y = lerp(t1->y, t2->y, t);
		return Vector2(z->x, z->y);
	}
	inline Vector3 Vector3_lerp(Vector3* z, const Vector3* t1, const Vector3* t2, float t)
	{
		z->x = lerp(t1->x, t2->x, t);
		z->y = lerp(t1->y, t2->y, t);
		z->z = lerp(t1->z, t2->z, t);
		return Vector3(z->x, z->y, z->z);
	}
	inline	Vector4 vector4_lerp(Vector4* z, const Vector4* t1, const Vector4* t2, float t)
	{
		z->x = lerp(t1->x, t2->x, t);
		z->y = lerp(t1->y, t2->y, t);
		z->z = lerp(t1->z, t2->z, t);
		z->w = 1.0f;
		return Vector4(z->x, z->y, z->z, z->w);
	}

	//变换矩阵设置
	// 平移变换  --三种变换得到由自身转为世界坐标
	inline	void matrix_set_translate(Matrix4x4 *m, float x, float y, float z) {
		m->set_identity();
		m->m[3][0] = x;
		m->m[3][1] = y;
		m->m[3][2] = z;
	}
	// 缩放变换
	inline	void matrix_set_scale(Matrix4x4 *m, float x, float y, float z) {
		m->set_identity();
		m->m[0][0] = x;
		m->m[1][1] = y;
		m->m[2][2] = z;
	}
	// 旋转矩阵
	inline	void matrix_set_rotate(Matrix4x4 *m, float x, float y, float z, float theta) {
		float qsin = (float)sin(theta * 0.5f);
		float qcos = (float)cos(theta * 0.5f);
		Vector4 vec = { x, y, z, 1.0f };
		float w = qcos;
		vec.normalize();
		x = vec.x * qsin;
		y = vec.y * qsin;
		z = vec.z * qsin;
		m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
		m->m[1][0] = 2 * x * y - 2 * w * z;
		m->m[2][0] = 2 * x * z + 2 * w * y;
		m->m[0][1] = 2 * x * y + 2 * w * z;
		m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
		m->m[2][1] = 2 * y * z - 2 * w * x;
		m->m[0][2] = 2 * x * z - 2 * w * y;
		m->m[1][2] = 2 * y * z + 2 * w * x;
		m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
		m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
		m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
		m->m[3][3] = 1.0f;
	}
	inline	void vector_sub(Vector4 *z, const Vector4& x, const Vector4& y) {
		z->x = x.x - y.x;
		z->y = x.y - y.y;
		z->z = x.z - y.z;
		z->w = 1.0;
	}
	// 设置摄像机 一个正交矩阵 得到view
	inline	void matrix_set_lookat(Matrix4x4 *m, const Vector4& eye, const Vector4& at, const Vector4& up) {
		Vector4 xaxis, yaxis, zaxis;
		vector_sub(&zaxis, at, eye);
		zaxis.normalize();
		vector4_cross(&xaxis, up, zaxis);
		xaxis.normalize();
		vector4_cross(&yaxis, zaxis, xaxis);

		m->m[0][0] = xaxis.x;
		m->m[1][0] = xaxis.y;
		m->m[2][0] = xaxis.z;
		m->m[3][0] = -vector4_dot(xaxis, eye);

		m->m[0][1] = yaxis.x;
		m->m[1][1] = yaxis.y;
		m->m[2][1] = yaxis.z;
		m->m[3][1] = -vector4_dot(yaxis, eye);

		m->m[0][2] = zaxis.x;
		m->m[1][2] = zaxis.y;
		m->m[2][2] = zaxis.z;
		m->m[3][2] = -vector4_dot(zaxis, eye);

		m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
		m->m[3][3] = 1.0f;
	}
	// D3DXMatrixPerspectiveFovLH 设置投影矩阵  -->需要了解投影矩阵的推到过程
	//得到的是proj
inline	void matrix_set_perspective(Matrix4x4 *m, float fovy, float aspect, float zn, float zf) {
		float fax = 1.0f / (float)tan(fovy * 0.5f);
		m->set_zero();
		m->m[0][0] = (float)(fax / aspect);
		m->m[1][1] = (float)(fax);
		m->m[2][2] = zf / (zf - zn);
		m->m[2][3] = 1;
		m->m[3][2] = -zn * zf / (zf - zn);
	}
}