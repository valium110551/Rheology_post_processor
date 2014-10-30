#ifndef _BOUNDARY_TYPE_H_
#define _BOUNDARY_TYPE_H_

#include <string>
#include <map>
#include <utility>


template<typename T>
class BoundaryInput
{
typedef T T;
private:

	std::map< std::string, std::pair<std::string, T> > _bcData;

public:

	BoundaryInput();

	void setBoundary(std::string Side, std::string type, T value);

	T& getBoundaryValue(std::string side);

	std::pair<std::string, T>& getBoundary(std::string side);

};

template<typename T>
BoundaryInput<T>::BoundaryInput()
{

}

template<typename T>
void BoundaryInput<T>::setBoundary(std::string direction, std::string type, T value)
{	
	_bcData[direction] = std::make_pair(type, value);
}

template<typename T>
T& BoundaryInput<T>::getBoundaryValue(std::string side)
{
	return _bcData[side].second;
}

template<typename T>
std::pair<std::string, T>& BoundaryInput<T>::getBoundary(std::string side)
{
	return _bcData[side];
}
#endif // _BOUNDARY_TYPE_H_