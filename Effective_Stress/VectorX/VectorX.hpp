#ifndef VECTORX_HPP
#define VECTORX_HPP

// Ver. 3

#include "VectorX/VectorX.h"
#include <cmath>

namespace sMathLib
{

template<typename T, int dimension>
VectorX<T, dimension>::VectorX()
{
	for(int index = 0; index != dimension; ++index)
	{
		vec[index] = T();
	}
}


template<typename T, int dimension>
VectorX<T, dimension>::VectorX(T x, T y, T z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

template<typename T, int dimension>
VectorX<T, dimension>::VectorX(const VectorX &rhs)
{
    for(int index = 0; index != dimension; ++index)
	{
		vec[index] = rhs.vec[index];
	}
}

template<typename T, int dimension>
VectorX<T, dimension>::~VectorX()
{

}

template<typename T, int dimension>
T& VectorX<T, dimension>::operator[](const int index)
{
	return this->vec[index];
}

template<typename T, int dimension>
const T& VectorX<T, dimension>::operator[](const int index) const
{
	return this->vec[index];
}


template<typename T, int dimension>
const VectorX<T, dimension> & VectorX<T, dimension>::operator=(const VectorX &rhs)
{
   for(int index = 0; index != dimension; ++index)
	{
		vec[index] = rhs.vec[index];
	}

    return *this;
}

template<typename T, int dimension>
VectorX<T, dimension> VectorX<T, dimension>::operator+(const VectorX& rhs) const
{
	return VectorX<T, dimension>(this->vec[0] + rhs.vec[0], this->vec[1] + rhs.vec[1], this->vec[2] + rhs.vec[2]);
}

template<typename T, int dimension>
VectorX<T, dimension> VectorX<T, dimension>::operator-(const VectorX& rhs) const
{
	return VectorX(vec[0] - rhs.vec[0], vec[1] - rhs.vec[1], vec[2] - rhs.vec[2]);
}

template<typename T, int dimension>
VectorX<T, dimension> VectorX<T, dimension>::operator*(const T& rhs) const
{
	VectorX<T, dimension> out;
	for(int index = 0; index != dimension; ++index)
		out[index] = vec[index] * rhs;

	return out;
}

template<typename T, int dimension>
VectorX<T, dimension> VectorX<T, dimension>::Cross(const VectorX& rhs) const
{
	VectorX<T, dimension> tmp( (vec[1] * rhs[2] - vec[2] * rhs[1]),
							  -(vec[0] * rhs[2] - vec[2] * rhs[0]),
							   (vec[0] * rhs[1] - vec[1] * rhs[0])
							  );
	return tmp;
}

/// NormSqr
template<typename T, int dimension>
T VectorX<T, dimension>::NormSqr() const
{	
	return T();
}

template<>
double VectorX<double, 3>::NormSqr() const
{
   double value = double();

	for(int index = 0; index != 3; ++index)
	{
		value += vec[index] * vec[index];
	}
	
	return value;
}

/// Volume
template<typename T, int dimension>
T VectorX<T, dimension>::Volume() const
{
	/*
	if(dimension == 3)
	{
		return vec[0] * vec[1] * vec[2];
	}
	else
	{
		return -1;
	}
	*/
	return T();
}

template<>
double VectorX<double, 3>::Volume() const
{
	return vec[0] * vec[1] * vec[2];
}

template<typename T, int dimension>
T VectorX<T, dimension>::Abs() const
{
	T tmp = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	return std::sqrt((double)tmp);
}

enum BoundaryType{ VelocityBoundary = 0, PressureBoundary = 1 };
enum Direction{ XN = 0, XP = 1, YN = 2, YP = 3, ZN = 4, ZP = 5};
template<>
BoundaryType VectorX<BoundaryType, 6>::Abs() const
{
	return VelocityBoundary;
}
/// read-only get

template<typename T, int dimension>
T VectorX<T, dimension>::x() const
{
	return vec[0];
}

template<typename T, int dimension>
T VectorX<T, dimension>::y() const
{
	return vec[1];
}

template<typename T, int dimension>
T VectorX<T, dimension>::z() const
{
	return vec[2];
}

} // sMathLib

#endif // VECTORX_HPP

