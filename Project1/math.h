/*By: Daniel Domingo (djdomingo@csu.fullertin.edu), 
	  Jordan Harvey (jpharvey@csu.fullerton.edu), 
	  Patrick Myers (pmyers@csu.fullerton.edu),
	  Kim-Lan Hoang (khoangl@csu.fullerton.edu)

	  Program is a class used by 486_proj1_Source.cpp 
	  to handle Vectors and Vector math more easily while 
	  keeping the code cleaner.
*/

#pragma once

#include <cassert>
#include <ostream>

namespace gfx
{
	class Vector
	{
	public:

		double x, y, z, w;

		Vector()
			: x(0)
			, y(0)
			, z(0)
			, w(0)
		{}

		Vector(const Vector& vec)
			: x(vec.x)
			, y(vec.y)
			, z(vec.z)
			, w(vec.w)
		{}

		Vector(double sc)
			: x(sc)
			, y(sc)
			, z(sc)
		{}

		Vector(double inX, double inY, double inZ)
			: x(inX)
			, y(inY)
			, z(inZ)
			, w(0)
		{}
		Vector(double inX, double inY, double inZ, double inW)
			: x(inX)
			, y(inY)
			, z(inZ)
			, w(inW)
		{}
		Vector& operator=(const Vector& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;

			return *this;
		}

		double& operator[](const int i)
		{
			assert(i >= 0 && i <= 2);

			if (i == 0)
				return x;
			else if (i == 1)
				return y;
			else if (i == 2)
				return z;
			else
				return w;
		}

		double Length() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		double LengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		void Normalize()
		{
			const double len = Length();
			x /= len;
			y /= len;
			z /= len;
		}

		// TODO: implement NearlyEqual method

		double Dot(const Vector& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z + vec.w*w;
		}

		static double Dot(const Vector& vec1, const Vector& vec2)
		{
			return vec1.Dot(vec2);
		}

		Vector Cross(const Vector& vec) const
		{
			return Vector(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
		}

		static Vector Cross(const Vector& vec1, const Vector& vec2)
		{
			return vec1.Cross(vec2);
		}

		friend Vector operator+(const Vector& vec1, const Vector& vec2)
		{
			return Vector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
		}

		friend Vector operator-(const Vector& vec1, const Vector& vec2)
		{
			return Vector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
		}

		friend Vector operator*(const Vector& vec1, const Vector& vec2)
		{
			return Vector(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
		}

		friend Vector operator*(const Vector& vec, const double sc)
		{
			return Vector(vec.x * sc, vec.y * sc, vec.z * sc);
		}

		Vector operator-() const
		{
			return Vector(-x, -y, -z);
		}

		friend bool operator==(const Vector& vec1, const Vector& vec2)
		{
			return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z;
		}

		friend bool operator!=(const Vector& vec1, const Vector& vec2)
		{
			return vec1.x != vec2.x || vec1.y != vec2.y || vec1.z != vec2.z;
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector& vec)
		{
			os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
			return os;
		}
	};
}