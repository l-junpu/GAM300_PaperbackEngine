#pragma once
#ifndef MATH_H
#define MATH_H

namespace tools
{
	/****************************************************/
	/*!                    Vector2D
	/****************************************************/
	template <typename T>
	struct Vector2D
	{
		using ValueType = typename T;

#pragma warning(disable : 4201)
		union
		{
			struct { T x, y; };
			T m[2];
		};
#pragma warning(default : 4201)

		Vector2D() : x{ }, y{ } { }

		Vector2D(const T& X, const T& Y) : x{ X }, y{ Y } { }

		Vector2D& operator = (const Vector2D& rhs) {
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		Vector2D& operator += (const Vector2D& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2D& operator -= (const Vector2D& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		template <typename U>
		Vector2D& operator *= (const U& val) {
			x *= val;
			y *= val;
			return *this;
		}

		template <typename U>
		Vector2D& operator /= (const U& val) {
			x /= val;
			y /= val;
			return *this;
		}

		bool operator == (const Vector2D& rhs) {
			return ((x != rhs.x) || (y != rhs.y)) ? false : true;
		}

		Vector2D operator - () const {
			return { -x, -y };
		}
	};

	template <typename T>
	typename Vector2D<T> operator + (const Vector2D<T>& lhs, const Vector2D<T>& rhs) {
		return Vector2D<T>(lhs) += rhs;
	}

	template <typename T>
	typename Vector2D<T> operator - (const Vector2D<T>& lhs, const Vector2D<T>& rhs) {
		return Vector2D<T>(lhs) -= rhs;
	}

	template <typename T, typename U>
	typename Vector2D<T> operator * (const Vector2D<T>& lhs, const U& val) {
		return Vector2D<T>(lhs) *= val;
	}

	template <typename T, typename U>
	typename Vector2D<T> operator * (const U& val, const Vector2D<T>& rhs) {
		return Vector2D<T>(rhs) *= val;
	}

	template <typename T, typename U>
	typename Vector2D<T> operator / (const Vector2D<T>& lhs, const U& val) {
		return Vector2D<T>(lhs) /= val;
	}

	template <typename T>
	T Vector2DDotProduct(const Vector2D<T>& vec1, const Vector2D<T>& vec2) {
		return (vec1.x * vec2.x) + (vec1.y * vec2.y);
	}

	template <typename T>
	T Vector2DLength(const Vector2D<T>& vec1) {
		return sqrt(Vector2DDotProduct(vec1, vec1));
	}

	template <typename T>
	void Vector2DNormalize(Vector2D<T>& result, const Vector2D<T>& vec) {
		result = vec / Vector2DLength(vec);
	}



	/****************************************************/
	/*!                    Vector3D
	/****************************************************/
	template <typename T>
	struct Vector3D
	{
		using ValueType = typename T;

#pragma warning(disable : 4201)
		union
		{
			struct { T x, y, z; };
			T m[3];
		};
#pragma warning(default : 4201)

		Vector3D() : x{ }, y{ }, z{ } { }

		Vector3D(const T& X, const T& Y, const T& Z) : x{ X }, y{ Y }, z{ Z } { }

		Vector3D& operator = (const Vector3D& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}

		Vector3D& operator += (const Vector3D& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3D& operator -= (const Vector3D& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		template <typename U>
		Vector3D& operator *= (const U& val) {
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}

		template <typename U>
		Vector3D& operator /= (const U& val) {
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}

		bool operator == (const Vector3D& rhs) {
			return ((x != rhs.x) || (y != rhs.y) || (z != rhs.z)) ? false : true;
		}

		Vector3D operator - () const {
			return { -x, -y, -z };
		}
	};

	template <typename T>
	typename Vector3D<T> operator + (const Vector3D<T>& lhs, const Vector3D<T>& rhs) {
		return Vector3D<T>(lhs) += rhs;
	}

	template <typename T>
	typename Vector3D<T> operator - (const Vector3D<T>& lhs, const Vector3D<T>& rhs) {
		return Vector3D<T>(lhs) -= rhs;
	}

	template <typename T, typename U>
	typename Vector3D<T> operator * (const Vector3D<T>& lhs, const U& val) {
		return Vector3D<T>(lhs) *= val;
	}

	template <typename T, typename U>
	typename Vector3D<T> operator * (const U& val, const Vector3D<T>& rhs) {
		return Vector3D<T>(rhs) *= val;
	}

	template <typename T, typename U>
	typename Vector3D<T> operator / (const Vector3D<T>& lhs, const U& val) {
		return Vector3D<T>(lhs) /= val;
	}

	template <typename T>
	T Vector3DDotProduct(const Vector3D<T>& vec1, const Vector3D<T>& vec2) {
		return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
	}

	template <typename T>
	T Vector3DLength(const Vector3D<T>& vec1) {
		return sqrt(Vector3DDotProduct(vec1, vec1));
	}

	template <typename T>
	void Vector3DNormalize(Vector3D<T>& result, const Vector3D<T>& vec) {
		result = vec / Vector3DLength(vec);
	}

	template <typename T>
	T Vector3DDistance(const Vector3D<T>& vec1, const Vector3D<T>& vec2) {
		return Vector3DLength(vec2 - vec1);
	}



	/****************************************************/
	/*!                    Vector4D
	/****************************************************/
	template <typename T>
	struct Vector4D
	{
		using ValueType = typename T;

#pragma warning(disable : 4201)
		union
		{
			struct { T x, y, z, w; };
			T m[4];
		};
#pragma warning(default : 4201)

		Vector4D() : x{ }, y{ }, z{ }, w{ } { }

		Vector4D(const T& X, const T& Y, const T& Z, const T& W) : x{ X }, y{ Y }, z{ Z }, w{ W } { }

		Vector4D& operator = (const Vector4D& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		Vector4D& operator += (const Vector4D& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		Vector4D& operator -= (const Vector4D& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		template <typename U>
		Vector4D& operator *= (const U& val) {
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}

		template <typename U>
		Vector4D& operator /= (const U& val) {
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}

		bool operator == (const Vector4D& rhs) {
			return ((x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w)) ? false : true;
		}

		Vector4D operator - () const {
			return { -x, -y, -z, -w };
		}
	};

	template <typename T>
	typename Vector4D<T> operator + (const Vector4D<T>& lhs, const Vector4D<T>& rhs) {
		return Vector4D<T>(lhs) += rhs;
	}

	template <typename T>
	typename Vector4D<T> operator - (const Vector4D<T>& lhs, const Vector4D<T>& rhs) {
		return Vector4D<T>(lhs) -= rhs;
	}

	template <typename T, typename U>
	typename Vector4D<T> operator * (const Vector4D<T>& lhs, const U& val) {
		return Vector4D<T>(lhs) *= val;
	}

	template <typename T, typename U>
	typename Vector4D<T> operator * (const U& val, const Vector4D<T>& rhs) {
		return Vector4D<T>(rhs) *= val;
	}

	template <typename T, typename U>
	typename Vector4D<T> operator / (const Vector4D<T>& lhs, const U& val) {
		return Vector4D<T>(lhs) /= val;
	}

	template <typename T>
	T Vector4DDotProduct(const Vector4D<T>& vec1, const Vector4D<T>& vec2) {
		return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z) + (vec1.w * vec2.w);
	}

	template <typename T>
	T Vector4DLength(const Vector4D<T>& vec1) {
		return sqrt(Vector4DDotProduct(vec1, vec1));
	}

	template <typename T>
	void Vector4DNormalize(Vector4D<T>& result, const Vector4D<T>& vec) {
		result = vec / Vector4DLength(vec);
	}

	template <typename T>
	T Vector4DDistance(const Vector4D<T>& vec1, const Vector4D<T>& vec2) {
		return Vector4DLength(vec2 - vec1);
	}

	// Vector2D
	using vec2_i = Vector2D<int>;
	using vec2_f = Vector2D<float>;
	using vec2_d = Vector2D<double>;
	using vec2_u = Vector2D<unsigned>;

	// Vector3D
	using vec3_i = Vector3D<int>;
	using vec3_f = Vector3D<float>;
	using vec3_d = Vector3D<double>;
	using vec3_u = Vector3D<unsigned>;

	// Vector4D
	using vec4_i = Vector4D<int>;
	using vec4_f = Vector4D<float>;
	using vec4_d = Vector4D<double>;
	using vec4_u = Vector4D<unsigned>;
}

#endif