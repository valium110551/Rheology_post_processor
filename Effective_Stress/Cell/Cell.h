#ifndef _CELL_H_
#define _CELL_H_

#include <map>
#include "Cell/Descroptor.h"

namespace sEG
{

template<typename T, template<typename U> class MeshDescriptor>
class Cell
{
public:

	typedef T dataPop[MeshDescriptor<T>::q];

public:

	Cell();

	Cell(const Cell &rhs);

	virtual ~Cell();

public:

	T& operator[](int iPop);

	T const& operator[](int iPop) const;

private:

	void IniPop();


private:

	dataPop _data;

public:

	std::map<int, double> Vff;


};

} // sEG

#endif // _CELL_H_