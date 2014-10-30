#ifndef BOX_H
#define BOX_H

#include <cmath>
#include "_ahedron.h"

class Box:public _ahedron
{
protected:
	sMathLib::VectorX<double,3>  position;
	double height;
	double length;
	double width;

public:

	Box();

	Box(double length,double width,double height,double x0,double y0,double z0);

	virtual ~Box();

public:

	Box& operator=(Box & rhs);

public:

	bool Intersect( ImmersedSphere<double>& sphere);
	sMathLib::VectorX<double,3> GetPosition();
	void SetPosition(sMathLib::VectorX<double,3>&);

	double Volume();

};


Box::Box()
{

}


Box::Box(double length,double width,double height,double x0,double y0,double z0):position(x0,y0,z0)
{
	this->length=length;
	this->width=width;
	this->height=height;
}

Box::~Box()
{

}

Box& Box::operator=(Box & rhs)// why can't Sphere & const rhs ??
{
	position=rhs.GetPosition();//????????
	this->length=rhs.length;
	this->width=rhs.width;
	this->height=rhs.height;

	return *this;
}

bool Box::Intersect( ImmersedSphere<double>& sphere)
{
	bool xInside = sphere.getPosition().x() >= (this->position.x()-0.5*length) && sphere.getPosition().x() <= (this->position.x()+0.5*length);
	bool yInside = sphere.getPosition().y() >= (this->position.y()-0.5*width) && sphere.getPosition().y() <=  (this->position.y()+0.5*width);
	bool zInside = sphere.getPosition().z() >= (this->position.z()-0.5*height) && sphere.getPosition().z() <= (this->position.z()+0.5*height);
	return xInside && yInside && zInside;
}

double Box::Volume()
{
	using namespace std;
	return length*width*height;
}

sMathLib::VectorX<double,3> Box::GetPosition()
{
	return position;
}


void Box::SetPosition(sMathLib::VectorX<double,3>& _position)
{
	position=_position;
}

#endif // _BOX3D_H_
