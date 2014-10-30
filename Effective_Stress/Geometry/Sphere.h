#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <cmath>
#include "_ahedron.h"

const double PI=3.1415926;

class Sphere:public _ahedron
{
protected:
	sMathLib::VectorX<double,3>  position;
	double radius;

public:

	Sphere();

	//Sphere(const Sphere& rhs);

	Sphere(double x0,double y0,double z0,double radius);

	virtual ~Sphere();

public:

	Sphere& operator=(Sphere & rhs);

public:

	bool Intersect( ImmersedSphere<double>& sphere);
	sMathLib::VectorX<double,3> GetPosition();
	void SetPosition(sMathLib::VectorX<double,3>&);
	double GetRadius();

	double Volume();

};

//Sphere::Sphere():x0(double()), y0(double()), z0(double()),position(x0,y0,z0)
Sphere::Sphere()
{

}

//Sphere::Sphere(const Sphere& rhs)
//{
//	this->x0 = rhs.x0;
//
//	this->y0 = rhs.y0;
//
//	this->z0 = rhs.z0;
//	
//	this->radius = rhs.radius;
//}

//Sphere::Sphere(const Sphere& rhs):position(rhs)
//{
//	
//}

Sphere::Sphere(double x0,double y0,double z0,double radius):position(x0,y0,z0)
{
	this->radius = radius;
}

Sphere::~Sphere()
{

}

Sphere& Sphere::operator=(Sphere & rhs)// why can't Sphere & const rhs ??
{
	position=rhs.GetPosition();//????????
	/*this->x0 = rhs.x0;

	this->y0 = rhs.y0;

	this->z0 = rhs.z0;*/

	this->radius=rhs.radius;
	return *this;
}

bool Sphere::Intersect( ImmersedSphere<double>& sphere)
{
	return (sphere.getPosition()-position).Abs()<=radius;
}

double Sphere::Volume()
{
	using namespace std;
	return ( (4.0/3)*PI*radius*radius*radius );
}

sMathLib::VectorX<double,3> Sphere::GetPosition()
{
	return position;
}

double Sphere::GetRadius()
{
	return radius;
}

void Sphere::SetPosition(sMathLib::VectorX<double,3>& _position)
{
	position=_position;
}

#endif // _BOX3D_H_
