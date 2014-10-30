#ifndef _AHEDRON_H
#define _AHEDRON_H
#include "Geometry/ImmersedSphere.h"
#include "VectorX/VectorX.h"
#include "VectorX/VectorX.hpp"

//template<typename Tp>
class _ahedron
{
	public:
		_ahedron(){};
		virtual ~_ahedron(){};
		virtual bool Intersect( ImmersedSphere<double>& sphere)=0;
		virtual double Volume()=0;
};
#endif

