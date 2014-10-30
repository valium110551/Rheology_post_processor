#ifndef _VIRIAL_STRESS_TENSOR_H_
#define _VIRIAL_STRESS_TENSOR_H_

#include <vector>
#include "VectorX/VectorX.hpp"
typedef sMathLib::VectorX<T, 3> Vector3;

typedef double T;



struct VirialStressTensor
{
	static void potentialPartUpdate(double (*stressTensor)[3], const double* force, const double* rij);
	//Only this will be used
	static void potentialPartUpdate(double (*stressTensor)[3],double timeInterval,const sMathLib::VectorX<T, 3> force, const sMathLib::VectorX<T, 3> rij);

	static void potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const double* force, const double* rij);
	
	static void potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const sMathLib::VectorX<T, 3> force, const sMathLib::VectorX<T, 3> rij);

	static void kineticPartUpdate(double (*stressTensor)[3], const double particleMass, const sMathLib::VectorX<T, 3> velocity ,const sMathLib::VectorX<T, 3> CMvelocity);

	static bool boundaryInteractionHandle(InteractionData& _interaction,_ahedron& adhedron, std::map<SphereIndex, ImmersedSphere<double> >& relativeSpheres);

};



void VirialStressTensor::potentialPartUpdate(double (*stressTensor)[3], const double* force, const double* rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * rij[beta];
}
//Only this will be used
void VirialStressTensor::potentialPartUpdate(double (*stressTensor)[3],double timeInterval,const sMathLib::VectorX<T, 3> force, const sMathLib::VectorX<T, 3> rij)
{
	double length=rij.Abs();
	double forceAbs=force.Abs();
	std::vector<double> _force;
	_force.push_back(force.x());
	_force.push_back(force.y());
	_force.push_back(force.z());

	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			if(length!=0)
			{
				stressTensor[alpha][beta] += -forceAbs/timeInterval *((rij[alpha]*rij[beta])/length);
				//stressTensor[alpha][beta] += rij[alpha]*_force[beta]/timeInterval*0.5;
			}
			else
				continue;
}

void VirialStressTensor::potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const double* force, const double* rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * rij[beta];
}

void VirialStressTensor::potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const sMathLib::VectorX<T, 3> force, const sMathLib::VectorX<T, 3> rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * (rij[alpha]*rij[beta])/rij.Abs()*0.5;
}


void VirialStressTensor::kineticPartUpdate(double (*stressTensor)[3] , const double particleMass, const sMathLib::VectorX<T, 3> velocity ,const sMathLib::VectorX<T, 3> CMvelocity)
{
	sMathLib::VectorX<T, 3> relativeVelocity=velocity-CMvelocity;
	for(unsigned alpha = 0; alpha != 3; ++alpha)
	{
		for(unsigned beta = 0; beta != 3; ++beta)
		{
			stressTensor[alpha][beta] += -particleMass*relativeVelocity[alpha] * relativeVelocity[beta];
		}

	}
}

bool VirialStressTensor::boundaryInteractionHandle(InteractionData& _interaction,_ahedron& adhedron,std::map<SphereIndex,  ImmersedSphere<double> >& relativeSpheres)
{
	unsigned master=_interaction.MasterID();
	unsigned slave=_interaction.SlaveID();
	return (relativeSpheres.end()!=relativeSpheres.find(master))&&(relativeSpheres.end()!=relativeSpheres.find(slave));
	//return adhedron.Intersect(relativeSpheres[master])&&adhedron.Intersect(relativeSpheres[slave]);
}

#endif // _VIRIAL_STRESS_TENSOR_H_