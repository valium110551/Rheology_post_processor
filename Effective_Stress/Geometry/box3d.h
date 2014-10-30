#ifndef _BOX3D_H_
#define _BOX3D_H_

#include <cmath>
#include "_ahedron.h" 

//template<typename T>
class Box3D :public _ahedron
{
public:

	double x0, x1;
	double y0, y1;
	double z0, z1;

public:

	Box3D();

	Box3D(const Box3D& rhs);

	Box3D(double x0, double x1, double y0, double y1, double z0, double z1);

	void SetPosition(sMathLib::VectorX<double,3>& _position);

	virtual ~Box3D();

public:

	const Box3D& operator=(Box3D const& rhs);

public:

	
	bool Intersect(const ImmersedSphere<double>& sphere);
	

	double Volume();




};

//template<typename double>
Box3D::Box3D():x0(double()), x1(double()), y0(double()), y1(double()), z0(double()),z1(double())
{

}

//template<typename T>
Box3D::Box3D(const Box3D& rhs)
{
	this->x0 = rhs.x0;
	this->x1 = rhs.x1;

	this->y0 = rhs.y0;
	this->y1 = rhs.y1;

	this->z0 = rhs.z0;
	this->z1 = rhs.z1;
}

//template<typename T>
Box3D::Box3D(double x0, double x1, double y0, double y1, double z0, double z1)
{
	this->x0 = x0;
	this->x1 = x1;

	this->y0 = y0;
	this->y1 = y1;

	this->z0 = z0;
	this->z1 = z1;
}

//template<typename T>
Box3D::~Box3D()
{

}

//template<typename T>
const Box3D& Box3D::operator=(Box3D const& rhs)
{
	this->x0 = rhs.x0;
	this->x1 = rhs.x1;

	this->y0 = rhs.y0;
	this->y1 = rhs.y1;

	this->z0 = rhs.z0;
	this->z1 = rhs.z1;
	return *this;
}

//template<typename T>
//template<class PolygonType>
bool Box3D::Intersect(const ImmersedSphere<double>& sphere)
{
	bool xInside = sphere.getPosition().x() >= this->x0 && sphere.getPosition().x() <= this->x1;
	bool yInside = sphere.getPosition().y() >= this->y0 && sphere.getPosition().y() <= this->y1;
	bool zInside = sphere.getPosition().z() >= this->z0 && sphere.getPosition().z() <= this->z1;
	return xInside && yInside && zInside;
}

//template<typename T>
double Box3D::Volume()
{
	using namespace std;
	return ( abs(x1 - x0) * abs(y1 - y0) * abs(z1 - z0) );
}


#endif // _BOX3D_H_
