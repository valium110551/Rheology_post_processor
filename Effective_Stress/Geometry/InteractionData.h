#ifndef _INTERACTION_DATA_H_
#define _INTERACTION_DATA_H_

#include <utility>
#include "VectorX/VectorX.hpp"

 class InteractionData
{
protected:
	
	std::pair<unsigned, unsigned> _interactionPair;

	sMathLib::VectorX<double, 3> _forceActOnMaster;

	sMathLib::VectorX<double, 3>  _contactPoint, _impactDirection;

	InteractionData();
public:

	virtual unsigned MasterID() const;

	virtual unsigned SlaveID() const;

	virtual sMathLib::VectorX<double, 3> GetInteractiveForce() const;

	virtual sMathLib::VectorX<double, 3> GetContactPoint() const;

	virtual sMathLib::VectorX<double, 3> GetImpactDirection() const;

	void setInterForce(double ,double ,double);//20130121 SANKAI

	void setImpactDirection(double ,double ,double);//20130124 SANKAI

public:

	InteractionData
		(
		unsigned masterID, unsigned slaveID, 
		sMathLib::VectorX<double, 3> forceActOnMaster, 
		sMathLib::VectorX<double, 3> contactPoint,
		sMathLib::VectorX<double, 3> impactDirection
		);

	InteractionData(unsigned masterID, unsigned slaveID, sMathLib::VectorX<double, 3> forceActOnMaster);

	InteractionData(const InteractionData& rhs);

	virtual ~InteractionData();
};

InteractionData::InteractionData
	(
	unsigned masterID, unsigned slaveID, 
	sMathLib::VectorX<double, 3> forceActOnMaster, 
	sMathLib::VectorX<double, 3> contactPoint,
	sMathLib::VectorX<double, 3> impactDirection
	)
{
	this->_interactionPair.first = masterID;
	this->_interactionPair.second = slaveID;

	this->_forceActOnMaster = forceActOnMaster;
	this->_contactPoint = contactPoint;
	this->_impactDirection = impactDirection;
}

InteractionData::InteractionData()
{

}

InteractionData::InteractionData(unsigned masterID, unsigned slaveID, sMathLib::VectorX<double, 3> forceActOnMaster)
{
	this->_interactionPair.first = masterID;
	this->_interactionPair.second = slaveID;

	this->_forceActOnMaster = forceActOnMaster;
}

InteractionData::InteractionData(const InteractionData& rhs)
{
	this->_interactionPair = rhs._interactionPair;

	this->_forceActOnMaster = rhs._forceActOnMaster;

	this->_contactPoint = rhs._contactPoint;

	this->_impactDirection = rhs._impactDirection;
}

InteractionData::~InteractionData()
{

}

unsigned InteractionData::MasterID() const
{
	return this->_interactionPair.first;
}

unsigned InteractionData::SlaveID() const
{
	return this->_interactionPair.second;
}

sMathLib::VectorX<double, 3> InteractionData::GetInteractiveForce() const
{
	return this->_forceActOnMaster;
}

sMathLib::VectorX<double, 3> InteractionData::GetContactPoint() const
{
	return this->_contactPoint;
}

sMathLib::VectorX<double, 3> InteractionData::GetImpactDirection() const
{
	return this->_impactDirection;
}

//20130121 SANKAI
void InteractionData::setInterForce(double _x,double _y,double _z) 
{
	sMathLib::VectorX<double, 3> forceActOnMaster(_x,_y,_z);
	this->_forceActOnMaster=forceActOnMaster;
}


void InteractionData::setImpactDirection(double _x,double _y,double _z)
{
	sMathLib::VectorX<double, 3> impactDirection(_x,_y,_z);
	this->_impactDirection=impactDirection;
}




#endif // _INTERACTION_DATA_H_
