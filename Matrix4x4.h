#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Vector3D.h"

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

		void setScale(Vector3D scale)
		{
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

		void setRotationY(float y)
		{
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

		void setOrthoLH(float width, float height, float near_plane, float far_plane)
		{
			setIdentity();
			m[0][0] = 2.0f / width;
			m[1][1] = 2.0f / height;
			m[2][2] = 1.0f / (far_plane - near_plane);
			m[3][2] = -(near_plane / (far_plane - near_plane));
		}

		DirectX::XMMATRIX getInverse()
		{
			auto D11Matrix = DirectX::XMMATRIX(
				m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], m[3][2], m[3][3]
			);
			auto D11Inverse = DirectX::XMMatrixInverse(nullptr, D11Matrix);

			return D11Inverse;
		}

};