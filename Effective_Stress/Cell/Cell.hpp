#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "Cell.h"

namespace sEG
{

template<typename T, template<typename U> class MeshDescriptor>
Cell<T, MeshDescriptor>::Cell()
{
	for(int iPop = 0; iPop != MeshDescriptor<T>::q; ++iPop)
	{
		this->_data[iPop] = T();
	}
}

template<typename T, template<typename U> class MeshDescriptor>
Cell<T, MeshDescriptor>::Cell(const Cell &rhs)
{
	for(int iPop = 0; iPop != MeshDescriptor<T>::q; ++iPop)
	{
		this->_data[iPop] = rhs._data[iPop];
	}

	this->Vff = rhs.Vff;
}

template<typename T, template<typename U> class MeshDescriptor>
Cell<T, MeshDescriptor>::~Cell()
{
}

// Operator overlord
template<typename T, template<typename U> class MeshDescriptor>
T& Cell<T, MeshDescriptor>::operator[](int iPop)
{ 
	return _data[iPop];
}

template<typename T, template<typename U> class MeshDescriptor>
T const& Cell<T, MeshDescriptor>::operator[](int iPop) const 
{ 
	return _data[iPop];
}



} // sEG
#endif // _CELL_HPP_