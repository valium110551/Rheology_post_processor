#ifndef _EFFECTIVE_STRESS_CALCULATOR_H_
#define _EFFECTIVE_STRESS_CALCULATOR_H_

#include <vector>
#include <utility>
#include <map>
#include "VectorX/VectorX.hpp"
#include "FileReader/DOInteractionReader.hpp"
#include "Geometry/InteractionData.h"
#include "_ahedron.h"
#include "Geometry/box.h"

#include "PostProcessorBase.h"


#include "Tensor.h"

typedef double T;
typedef unsigned SphereIndex;
typedef unsigned InteractionIndex;
typedef sMathLib::VectorX<double, 3> BranchVector;
typedef sMathLib::VectorX<double, 3> Force;

template<class ImplementType>
class ParticleFluidProcessor: public PostProcessorBase
{
	typedef T StressTensor[3][3]; 

protected:	

	std::vector< std::pair<InteractionData, BranchVector> > _relativeInteractions;

	std::map<SphereIndex, ImmersedSphere<double> > _relativeSpheres;

	std::map<SphereIndex, std::vector<InteractionData> > _relativeSphereInteractions;

	StressTensor _kineticStressTensor;

	StressTensor _potentialStressTensor;

	StressTensor _totalStressTensor;

	sMathLib::VectorX<double, 3> CMvelocity; //center of gravity ,center of mass

	double _controlVolume;

	double particleMass;

	double particleVolume;

	double timeInterval;
	
	void calculateCMvelocity();

	int count;


public:

	ParticleFluidProcessor();

	virtual ~ParticleFluidProcessor();

public:

	void Initialize();

	void DefineControlVolume(_ahedron& adedron);

	//eliminate the particles that's not related to the middle region(calculated area)
	void refineInputData(std::ofstream* outfile1 ,std::ofstream* outfile2,_ahedron& adedron,double angle);

	void potentialPartUpdate(const T* force, const T* rij);

	void kineticPartUpdate(const double particleMass, const sMathLib::VectorX<double, 3> velocity ,const sMathLib::VectorX<double, 3> CMvelocity);

	void potentialPartUpdate(Force& force, BranchVector& rij);

	void CalculateStressTensor();

	void CalculateVericalStress();

	void Finalize();

	sMathLib::Tensor<T> GetPotentialStressTensor();	
	
	sMathLib::Tensor<T> GetKineticStressTensor();	
	
	void DrawVolumeFractionFunction();

	double GetVolumeFraction();

	int GetInteractionNum();
};

#endif // _EFFECTIVE_STRESS_CALCULATOR_H_