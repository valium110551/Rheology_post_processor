#ifndef _CUNDALL_STRESS_TENSOR_H_
#define _CUNDALL_STRESS_TENSOR_H_

#include <vector>
#include "VectorX/VectorX.hpp"

typedef double T;
typedef sMathLib::VectorX<T, 3> Vector3;

struct CundallStressTensor
{
	
	static void potentialPartUpdate(double (*stressTensor)[3], const double* force, const double* rij);

	static void potentialPartUpdate(T (*stressTensor)[3], const Vector3& force, const Vector3& rij);

	static void potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const double* force, const double* rij);//只有這個會用到
	
	static void potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const sMathLib::VectorX<T, 3>& force, const sMathLib::VectorX<T, 3>& rij);
};



void CundallStressTensor::potentialPartUpdate(double (*stressTensor)[3], const double* force, const double* rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * rij[beta];
}

void CundallStressTensor::potentialPartUpdate(T (*stressTensor)[3], const Vector3& force, const Vector3& rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * rij[beta];
}

void CundallStressTensor::potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const double* force, const double* rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * rij[beta];
}

void CundallStressTensor::potentialPartUpdate(std::vector< std::vector< T > >& stressTensor, const sMathLib::VectorX<T, 3>& force, const sMathLib::VectorX<T, 3>& rij)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			stressTensor[alpha][beta] += force[alpha] * rij[beta];
}

#endif // _CUNDALL_STRESS_TENSOR_H_