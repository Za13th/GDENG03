#pragma once

class Vector3D
{
	public:
		float x, y, z;
		Vector3D() : x(0), y(0), z(0) {}
		Vector3D(float xyz) : x(xyz), y(xyz), z(xyz) {}
		Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3D operator+(const Vector3D& other) const {
			return Vector3D(x + other.x, y + other.y, z + other.z);
		}
		Vector3D operator-(const Vector3D& other) const {
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}
		Vector3D operator*(float scalar) const {
			return Vector3D(x * scalar, y * scalar, z * scalar);
		}
		Vector3D operator/(float scalar) const {
			return Vector3D(x / scalar, y / scalar, z / scalar);
		}
		float dot(const Vector3D& other) const {
			return x * other.x + y * other.y + z * other.z;
		}
		Vector3D cross(const Vector3D& other) const {
			return Vector3D(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);
		}

		static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta) {

			Vector3D v;
			v.x = start.x * (1.0f - delta) + end.x * (delta);
			v.y = start.y * (1.0f - delta) + end.y * (delta);
			v.z = start.z * (1.0f - delta) + end.z * (delta);


			return v;
		}


};

