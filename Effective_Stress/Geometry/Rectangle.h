#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "VectorX/VectorX.hpp"

#include "Geometry/LineSegment.h"

template<typename T>
class Rectangle
{
protected:

	sMathLib::VectorX<T, 3> _position;

	sMathLib::VectorX<T, 3> _normalDirection;

	T _width, _height;

	T a, b, c;

public:

	Rectangle();

	Rectangle(T x, T y, T z, T normX, T normY, T normZ, T width, T height);

	Rectangle(sMathLib::VectorX<T, 3> position, sMathLib::VectorX<T, 3> normalDirection, T width, T height);

public:

	sMathLib::VectorX<T, 3>& getPosition();

	T& getWidth();

	T& getHeight();

	template<class PolygonType>
	bool Intersect(PolygonType &polygon);

	bool Intersect(LineSegment<T>& line);

private:
};

template<typename T>
Rectangle<T>::Rectangle():
_position(T(), T(), T()), _width((T)1), _height((T)1)
{

}

template<typename T>
Rectangle<T>::Rectangle(T x, T y, T z, T normX, T normY, T normZ,T width, T height):
_position(x, y, z), _normalDirection(normX, normY, normZ), _width(width), _height(height)
{

}

template<typename T>
Rectangle<T>::Rectangle(sMathLib::VectorX<T, 3> position, sMathLib::VectorX<T, 3> normalDirection, T width, T height):
_position(position), _normalDirection(normalDirection),  _width(width), _height(height)
{

}

template<typename T>
sMathLib::VectorX<T, 3>& Rectangle<T>::getPosition()
{
	return this->_position;
}

template<typename T>
T& Rectangle<T>::getWidth()
{
	return this->_width;
}

template<typename T>
T& Rectangle<T>::getHeight()
{
	return this->_height;
}

template<typename T>
template<class PolygonType>
bool Rectangle<T>::Intersect(PolygonType& polygon)
{
	return false
}

template<typename T>
bool Rectangle<T>::Intersect(LineSegment<T>& line)
{
	T lhs = _normalDirection[0] * line.getStartPoint()[0] + _normalDirection[1] * line.getStartPoint()[1] + _normalDirection[2] * line.getStartPoint()[2];
	T rhs = _normalDirection[0] * line.getEndPoint()[0] + _normalDirection[1] * line.getEndPoint()[1] + _normalDirection[2] * line.getEndPoint()[2];

	bool case1 = lhs >= 0 && rhs <= 0;
	bool case2 = lhs <= 0 && rhs >= 0;

	return case1 || case2;
}

#endif // _RECTANGLE_H_