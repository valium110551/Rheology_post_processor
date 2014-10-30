#ifndef _LINE_SEGMENT_H_
#define _LINE_SEGMENT_H_

#include "VectorX/VectorX.hpp"

template<typename T>
class LineSegment
{
protected:

	sMathLib::VectorX<T, 3> _startPoint, _endPoint;

public:

	LineSegment();

	LineSegment(sMathLib::VectorX<T, 3> startPoint, sMathLib::VectorX<T, 3> endPoint);

	LineSegment(const LineSegment& rhs);

	~LineSegment();

public:


public:

	sMathLib::VectorX<T, 3>& getStartPoint();

	sMathLib::VectorX<T, 3>& getEndPoint();

private:
};

template<typename T>
LineSegment<T>::LineSegment():
_startPoint(T(), T(), T()), _endPoint(T(), T(), T())
{

}

template<typename T>
LineSegment<T>::LineSegment(sMathLib::VectorX<T, 3> startPoint, sMathLib::VectorX<T, 3> endPoint):
_startPoint(startPoint), _endPoint(endPoint)
{

}

template<typename T>
LineSegment<T>::LineSegment(const LineSegment& rhs)
{
	this->_startPoint = rhs._startPoint;
	this->_endPoint = rhs._endPoint;
}

template<typename T>
LineSegment<T>::~LineSegment()
{

}

template<typename T>
sMathLib::VectorX<T, 3>& LineSegment<T>::getStartPoint()
{
	return this->_startPoint;	
}

template<typename T>
sMathLib::VectorX<T, 3>& LineSegment<T>::getEndPoint()
{
	return this->_endPoint;
}

#endif // _LINE_SEGMENT_H_