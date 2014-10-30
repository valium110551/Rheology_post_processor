#ifndef _ImmersedSphere_H_
#define _ImmersedSphere_H_

#include "VectorX/VectorX.hpp"
#include <cmath>

class NJRvector3d;

template<typename T>
class ImmersedSphere
{
private:

	sMathLib::VectorX<T, 3> _position, _shiftedPosition;
	sMathLib::VectorX<T, 3> _velocity;
	T _radius;
	T _sphereIndex;

public:

	ImmersedSphere();

	ImmersedSphere(T& sphereIndex,sMathLib::VectorX<T, 3>& position, sMathLib::VectorX<T, 3>& velocity, T& radius);
	
	ImmersedSphere(sMathLib::VectorX<T, 3>& position, sMathLib::VectorX<T, 3>& velocity, T& radius);

	ImmersedSphere(NJRvector3d& position, NJRvector3d& velocity, T& radius);

	ImmersedSphere(T& px, T& py, T& pz, T& ux, T& uy, T& uz, T& radius);

	ImmersedSphere(const ImmersedSphere<T>& rhs);

public:

	sMathLib::VectorX<T, 3>& getPosition() ;

	sMathLib::VectorX<T, 3>& getShftedPosition();

	sMathLib::VectorX<T, 3>& getVelocity();

	T& getRadius();

	T getVolume();
	
	T getSphereIndex();

	void setPosition(T ,T ,T); //20120102 SANKAI
	void setVelocity(T ,T ,T); //20120102 SANKAI

};

template<typename T>
ImmersedSphere<T>::ImmersedSphere():_sphereIndex(0)
{
	
}


template<typename T>
ImmersedSphere<T>::ImmersedSphere(T& sphereIndex,sMathLib::VectorX<T, 3>& position, sMathLib::VectorX<T, 3>& velocity, T& radius)
	:_sphereIndex(sphereIndex),_position(position), _velocity(velocity), _radius(radius)
{
	
}

template<typename T>
ImmersedSphere<T>::ImmersedSphere(sMathLib::VectorX<T, 3>& position, sMathLib::VectorX<T, 3>& velocity, T& radius)
	:_sphereIndex(0),_position(position), _velocity(velocity), _radius(radius)
{

}

template<typename T>
ImmersedSphere<T>::ImmersedSphere(NJRvector3d& position, NJRvector3d& velocity, T& radius)
	:_sphereIndex(0),_radius(radius)
{
	_position[0] = position.x();
	_position[1] = position.y();
	_position[2] = position.z();

	_velocity[0] = velocity.x();
	_velocity[1] = velocity.y();
	_velocity[2] = velocity.z();
}

template<typename T>
ImmersedSphere<T>::ImmersedSphere(T& px, T& py, T& pz, T& ux, T& uy, T& uz, T& radius)
	:_sphereIndex(0),_radius(radius)
{
	_position[0] = px;
	_position[1] = py;
	_position[2] = pz;

	_velocity[0] = ux;
	_velocity[1] = uy;
	_velocity[2] = uz;
}

template<typename T>
ImmersedSphere<T>::ImmersedSphere(const ImmersedSphere<T>& rhs)
{
	this->_position = rhs._position;
	this->_velocity = rhs._velocity;
	this->_radius = rhs._radius;
	this->_sphereIndex = rhs._sphereIndex;
}

template<typename T>
sMathLib::VectorX<T, 3>& ImmersedSphere<T>::getPosition() 
{
	return this->_position;
}

template<typename T>
sMathLib::VectorX<T, 3>& ImmersedSphere<T>::getShftedPosition()
{
	return this->_shftedPosition;
}

template<typename T>
sMathLib::VectorX<T, 3>& ImmersedSphere<T>::getVelocity()
{
	return this->_velocity;
}

template<typename T>
T& ImmersedSphere<T>::getRadius()
{
	return this->_radius;
}

template<typename T>
T ImmersedSphere<T>::getVolume()
{	 
	return (4. / 3.) * (4 * std::atan(1.)) * (_radius * _radius * _radius);
}

template<typename T>
T ImmersedSphere<T>::getSphereIndex()
{	 
	return _sphereIndex;
}

template<typename T>
void ImmersedSphere<T>::setPosition(T _x,T _y ,T _z) //20120102 SANKAI
{
	sMathLib::VectorX<T, 3> position(_x,_y,_z);
	this->_position=position;
}

template<typename T>
void ImmersedSphere<T>::setVelocity(T _x,T _y ,T _z) //20120102 SANKAI
{
	sMathLib::VectorX<T, 3> velocity(_x,_y,_z);
	this->_velocity=velocity;
}


#endif //_ImmersedSphereW_H_
