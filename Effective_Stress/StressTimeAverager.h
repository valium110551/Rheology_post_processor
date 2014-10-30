#ifndef STRESSTIMEAVERAGER_H
#define STRESSTIMEAVERAGER_H
#include "EffectiveStressCalculator.h"
#include "VirialStressTensor.h"
#include "_ahedron.h"
#include "Tensor.h"
#include <vector>
using namespace std;

template<class ImplementType>
class StressTimeAverager
{
	typedef T StressTensor[3][3];
	public:
		StressTimeAverager(string filenameVPF,string filenameCSV,_ahedron& ahedron,unsigned averageNum);
		sMathLib::Tensor<T> GetAveragedPotentialStressTensor();	
		sMathLib::Tensor<T> GetAveragedKineticStressTensor();
		sMathLib::Tensor<T> GetAveragedTotalStressTensor();
		double GetAveragedVolumeFraction();
		double GetNorXXStressDevi();
		double GetNorYYStressDevi();
		double GetSheaXYStressDevi();
		vector<int> GetInteractionNums();
		
	protected:
		vector<ParticleFluidProcessor<ImplementType>> processors;
		//vector<EffectiveStressCalculator> processors;
		StressTensor AveragedPotentialStress;
		StressTensor AveragedKineticStress;
		StressTensor AveragedTotalStress;
		int numTimeSteps;
		double normalXXStressDeviation;
		double normalYYStressDeviation;
		double shearXYStressDeviation;
		double NorXXPowDeSum;
		double NorYYPowDeSum;
		double SheaXYPowDeSum;
		vector<int> interactionNums;


};
#endif