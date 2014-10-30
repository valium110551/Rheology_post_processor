#ifndef STRESSTIMEAVERAGER_HPP
#define STRESSTIMEAVERAGER_HPP
#include "StressTimeAverager.h"
#include "StringConverter/StringUtility.h"
#include <iterator>
#include <sstream>
#include <string>


template<class ImplementType>
StressTimeAverager<ImplementType>::StressTimeAverager(string filenameVPF,string filenameCSV,_ahedron& ahedron,unsigned averageNum)
{
	//Initialization of data member
	normalXXStressDeviation=0;
	normalYYStressDeviation=0;
	shearXYStressDeviation=0;
	NorXXPowDeSum=0;
	NorYYPowDeSum=0;
	SheaXYPowDeSum=0;

	vector<string> strDlmTmp = StringUtility::StringDelimiter(filenameVPF, "_");
	vector<string> strDlmTmp2 = StringUtility::StringDelimiter(strDlmTmp[1], ".");
	string VPFframe = strDlmTmp2[0];
	int _VPFframe=StringUtility::ToTargetType<int>(VPFframe);
	_VPFframe+=(-averageNum);

	vector<string> strDlmTmp3 = StringUtility::StringDelimiter(filenameCSV, "_");
	vector<string> strDlmTmp4 = StringUtility::StringDelimiter(strDlmTmp3[1], ".");
	string CSVframe = strDlmTmp4[0];
	int _CSVframe=StringUtility::ToTargetType<int>(CSVframe);
	_CSVframe+=(-averageNum);

	numTimeSteps=averageNum*2+1;
	
	for(int i=0;i!=numTimeSteps;i++)
	{
		

		std::stringstream sStream;
		sStream<<_VPFframe;
		string NewVPFframe;
		sStream >> NewVPFframe;


		std::stringstream sStream2;
		sStream2<<_CSVframe;
		string NewCSVframe;
		sStream2 >> NewCSVframe;

		//string NewVPFframe=StringUtility::ToTargetType<string>(_VPFframe);
		//string NewCSVframe=StringUtility::ToTargetType<string>(_CSVframe);

		ParticleFluidProcessor<VirialStressTensor> processor;
		//snare "00" just for temperary

	
		string csvFile=strDlmTmp3[0]+"_";
		for(unsigned i=0;i!=(CSVframe.size()-NewCSVframe.size());i++)
		{
			csvFile+="0";
		}
		csvFile+=NewCSVframe+"."+"csv";

		string vpfFile=strDlmTmp[0]+"_";
		for(unsigned i=0;i!=(VPFframe.size()-NewVPFframe.size());i++)
		{
			vpfFile+="0";
		}
		vpfFile+=NewVPFframe+"."+"vpf";
		
		
		processor.SetRefinedCSVFileName(csvFile.c_str());
		processor.SetRefinedVPFFileName(vpfFile.c_str());
		//processor.SetFluidFieldFileName(filenameDAT.c_str());
		processor.ReadRefinedFile();
		processor.Initialize();
		processor.DefineControlVolume( ahedron );
		interactionNums.push_back(processor.GetInteractionNum());
		processor.CalculateStressTensor();
		processor.Finalize();
		processors.push_back(processor);

		_VPFframe+=1;
		_CSVframe+=1;
	}
	vector<ParticleFluidProcessor<ImplementType>>::iterator iter;
	vector<ParticleFluidProcessor<ImplementType>>::iterator iterBegin=processors.begin();
	vector<ParticleFluidProcessor<ImplementType>>::iterator iterEnd=processors.end();
	
////Average stress tensor over each time step (processors) 
	//average potential stress tensors
	for(unsigned alpha = 0; alpha != 3; ++alpha)
	{
		for(unsigned beta = 0; beta != 3; ++beta)
		{
			AveragedPotentialStress[alpha][beta] = 0 ;		
		}
	}
	
	for(iter=iterBegin;iter!=iterEnd;iter++)
	{
		Tensor<T> potentialStress=iter->GetPotentialStressTensor();
		for(unsigned alpha = 0; alpha != 3; ++alpha)
		{
			for(unsigned beta = 0; beta != 3; ++beta)
			{
				AveragedPotentialStress[alpha][beta] += potentialStress[alpha][beta]/numTimeSteps/1.0;		
			}
		}
	}

	for(iter=iterBegin;iter!=iterEnd;iter++)
	{
		Tensor<T> potentialStress=iter->GetPotentialStressTensor();
		//Normal stress(xx) deviation calcilation
		NorXXPowDeSum+=pow(potentialStress[0][0]-AveragedPotentialStress[0][0],2);
		//Normal stress(yy) deviation calcilation
		NorYYPowDeSum+=pow(potentialStress[1][1]-AveragedPotentialStress[1][1],2);		
		//Shear stress(xy) deviation calcilation
		SheaXYPowDeSum+=pow(potentialStress[0][1]-AveragedPotentialStress[0][1],2);
		
	}
	//Normal stress deviation calcilation
	normalXXStressDeviation=sqrt(NorXXPowDeSum/numTimeSteps);
	//Normal stress deviation calcilation
	normalYYStressDeviation=sqrt(NorYYPowDeSum/numTimeSteps);
	//Shear stress deviation calcilation
	shearXYStressDeviation=sqrt(SheaXYPowDeSum/numTimeSteps);


	//average kinetic stress tensors
	for(unsigned alpha = 0; alpha != 3; ++alpha)
	{
		for(unsigned beta = 0; beta != 3; ++beta)
		{
			AveragedKineticStress[alpha][beta] = 0 ;		
		}
	}
	
	for(iter=iterBegin;iter!=iterEnd;iter++)
	{
		Tensor<T> kineticStress=iter->GetKineticStressTensor();
		for(unsigned alpha = 0; alpha != 3; ++alpha)
		{
			for(unsigned beta = 0; beta != 3; ++beta)
			{
				AveragedKineticStress[alpha][beta] += kineticStress[alpha][beta]/numTimeSteps/1.0;		
			}
		}
	}

	



}

template<class ImplementType>
sMathLib::Tensor<T> StressTimeAverager<ImplementType>::GetAveragedPotentialStressTensor()
{
	return sMathLib::Tensor<T>(this->AveragedPotentialStress);	
}

template<class ImplementType>
sMathLib::Tensor<T> StressTimeAverager<ImplementType>::GetAveragedKineticStressTensor()
{
	return sMathLib::Tensor<T>(this->AveragedKineticStress);
}

template<class ImplementType>
sMathLib::Tensor<T> StressTimeAverager<ImplementType>::GetAveragedTotalStressTensor()
{
	for(int i = 0; i != 3; ++i)
		for(int j = 0; j != 3; ++j)
			this->AveragedTotalStress[i][j] = AveragedKineticStress[i][j]+AveragedPotentialStress[i][j];
	
	return sMathLib::Tensor<T>(this->AveragedTotalStress);
}

template<class ImplementType>
double StressTimeAverager<ImplementType>::GetAveragedVolumeFraction()
{
	double totalNumParticles=0;
	for(unsigned i=0;i!=numTimeSteps;i++)
	{
		totalNumParticles+=this->processors[i].GetVolumeFraction();
	}
	return totalNumParticles/numTimeSteps;
}

template<class ImplementType>
double StressTimeAverager<ImplementType>::GetNorXXStressDevi()
{
	return normalXXStressDeviation;
}

template<class ImplementType>
double StressTimeAverager<ImplementType>::GetNorYYStressDevi()
{
	return normalYYStressDeviation;
}
template<class ImplementType>
double StressTimeAverager<ImplementType>::GetSheaXYStressDevi()
{
	return shearXYStressDeviation;
}

template<class ImplementType>
vector<int> StressTimeAverager<ImplementType>::GetInteractionNums()
{
	return interactionNums;
}



#endif