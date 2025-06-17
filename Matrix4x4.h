#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Vector3D.h"
#include "Vector4D.h"

class Matrix4x4
{
	public:
		float m[4][4];

		Matrix4x4(){}

		void setIdentity()
		{
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m[i][j] = (i == j) ? 1.0f : 0.0f; // Identity matrix
		}

		void setTranslation(Vector3D translation)
		{
			setIdentity();
			m[3][0] = translation.x;
			m[3][1] = translation.y;
			m[3][2] = translation.z;
		}

		void setTranslation(Vector3D translation, bool setIdentityMatrix)
		{
			if (setIdentityMatrix)
				setIdentity();

			m[3][0] = translation.x;
			m[3][1] = translation.y;
			m[3][2] = translation.z;
		}

		void setScale(Vector3D scale)
		{
			setIdentity();
			m[0][0] = scale.x;
			m[1][1] = scale.y;
			m[2][2] = scale.z;
		}

		void setScale(Vector3D scale, bool setIdentityMatrix)
		{
			if (setIdentityMatrix)
				setIdentity();

			m[0][0] = scale.x;
			m[1][1] = scale.y;
			m[2][2] = scale.z;
		}

		void setRotationX(float x)
		{
			setIdentity();
			m[1][1] = cosf(x);
			m[1][2] = sinf(x);
			m[2][1] = -sinf(x);
			m[2][2] = cosf(x);
		}

		void setRotationX(float x, bool setIdentityMatrix)
		{
			if (setIdentityMatrix)
				setIdentity();

			m[1][1] = cosf(x);
			m[1][2] = sinf(x);
			m[2][1] = -sinf(x);
			m[2][2] = cosf(x);
		}

		void setRotationY(float y)
		{
			setIdentity();
			m[0][0] = cosf(y);
			m[0][2] = -sinf(y);
			m[2][0] = sinf(y);
			m[2][2] = cosf(y);
		}

		void setRotationY(float y, bool setIdentityMatrix)
		{
			if (setIdentityMatrix)
				setIdentity();

			setIdentity();
			m[0][0] = cosf(y);
			m[0][2] = -sinf(y);
			m[2][0] = sinf(y);
			m[2][2] = cosf(y);
		}

		void setRotationZ(float z)
		{
			setIdentity();
			m[0][0] = cosf(z);
			m[0][1] = sinf(z);
			m[1][0] = -sinf(z);
			m[1][1] = cosf(z);
		}

		void setRotationZ(float z, bool setIdentityMatrix)
		{

			if (setIdentityMatrix)
				setIdentity();

			m[0][0] = cosf(z);
			m[0][1] = sinf(z);
			m[1][0] = -sinf(z);
			m[1][1] = cosf(z);
		}

		float getDeterminant()
		{
			Vector4D minor, v1, v2, v3;
			float det;

			v1 = Vector4D(this->m[0][0], this->m[1][0], this->m[2][0], this->m[3][0]);
			v2 = Vector4D(this->m[0][1], this->m[1][1], this->m[2][1], this->m[3][1]);
			v3 = Vector4D(this->m[0][2], this->m[1][2], this->m[2][2], this->m[3][2]);


			minor.cross(v1, v2, v3);
			det = -(this->m[0][3] * minor.m_x + this->m[1][3] * minor.m_y + this->m[2][3] * minor.m_z +
				this->m[3][3] * minor.m_w);
			return det;
		}

		void inverse()
		{
			int a, i, j;
			Matrix4x4 out;
			Vector4D v, vec[3];
			float det = 0.0f;

			det = this->getDeterminant();
			if (!det) return;
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (j != i)
					{
						a = j;
						if (j > i) a = a - 1;
						vec[a].m_x = (this->m[j][0]);
						vec[a].m_y = (this->m[j][1]);
						vec[a].m_z = (this->m[j][2]);
						vec[a].m_w = (this->m[j][3]);
					}
				}
				v.cross(vec[0], vec[1], vec[2]);

				out.m[0][i] = pow(-1.0f, i) * v.m_x / det;
				out.m[1][i] = pow(-1.0f, i) * v.m_y / det;
				out.m[2][i] = pow(-1.0f, i) * v.m_z / det;
				out.m[3][i] = pow(-1.0f, i) * v.m_w / det;
			}

			this->setMatrix(out);
		}

		void operator*=(const Matrix4x4& other)
		{
			Matrix4x4 result;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					result.m[i][j] = m[i][0] * other.m[0][j] +
						m[i][1] * other.m[1][j] +
						m[i][2] * other.m[2][j] +
						m[i][3] * other.m[3][j];
				}
			}
			::memcpy(m, result.m, sizeof(float) * 16);
		}

		void setMatrix(const Matrix4x4& other)
		{
			::memcpy(m, other.m, sizeof(float) * 16);
		}

		Vector3D getZDirection()
		{
			return Vector3D(m[2][0], m[2][1], m[2][2]);
		}

		Vector3D getYDirection()
		{
			return Vector3D(m[1][0], m[1][1], m[1][2]);
		}

		Vector3D getXDirection()
		{
			return Vector3D(m[0][0], m[0][1], m[0][2]);
		}

		Vector3D getTranslation()
		{
			return Vector3D(m[3][0], m[3][1], m[3][2]);
		}

		void setPerspectiveFovLH(float fov, float aspect, float z_near, float z_far)
		{
			setIdentity();
			float y_scale = 1.0f / tanf(fov / 2.0f);
			float x_scale = y_scale / aspect;
			m[0][0] = x_scale;
			m[1][1] = y_scale;
			m[2][2] = z_far / (z_far - z_near);
			m[2][3] = 1.0f;
			m[3][2] = (-z_near * z_far) / (z_far - z_near);
		}

		void setOrthoLH(float width, float height, float near_plane, float far_plane)
		{
			setIdentity();
			m[0][0] = 2.0f / width;
			m[1][1] = 2.0f / height;
			m[2][2] = 1.0f / (far_plane - near_plane);
			m[3][2] = -(near_plane / (far_plane - near_plane));
		}

};