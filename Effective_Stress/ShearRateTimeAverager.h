//Use shearRateProcessor to cal. space averaged velocity, then use ShearRate to cal time averaged vel.
//Finally, cal. shear rate in ShearRate

#ifndef SHEARRATETIMEAVERAGER_H
#define SHEARRATETIMEAVERAGER_H

#include <string>
#include "_ahedron.h"
#include "StringConverter/StringUtility.h"
#include "shearRateProcessor.h"
#include <vector>
#include "Geometry\box.h"

typedef sMathLib::VectorX<double, 3> AveragedVel;


using namespace std;


class ShearRateTimeAverager
{
	public:
		ShearRateTimeAverager(string& filenameVPF,Box& box,double interval,unsigned numMovement,unsigned averageNum);
		vector<double> GetAveragedShearRates();
		vector<double> GetShearRatesDevi();
		vector<double> GetTimeAveragedXVel();
	protected:
		int numTimeSteps;
		vector<ShearRateProcessor> processors;
		vector<double> averagedShearRate;
		vector<double> sumShearRate;
		vector<double> ShearRateDeviation;
		vector<double> averagedXVel;
		vector<AveragedVel> sumAveragedVel;


};

ShearRateTimeAverager::ShearRateTimeAverager(string& filenameVPF,Box& box,double interval,unsigned numStep,unsigned averageNum)
{
	///initialization of paremeters to cal. averaged shearRates and deviations
	vector<double> tempVector(numStep-4,0.0);
	vector<double> tempShearVector(numStep-4,0.0);
	vector<AveragedVel> tempAveragedVel();
	this->averagedShearRate=tempShearVector;
	this->sumShearRate=tempVector;
	this->ShearRateDeviation=tempVector;	
	for(unsigned i=0;i!=numStep;i++)
	{
		AveragedVel tempVector(0.0,0.0,0.0);
		this->sumAveragedVel.push_back(tempVector);
	}
	this->averagedXVel=tempVector;
	vector<double> ShearRatePowDeSum=tempVector;

	vector<string> strDlmTmp = StringUtility::StringDelimiter(filenameVPF, "_");
	vector<string> strDlmTmp2 = StringUtility::StringDelimiter(strDlmTmp[1], ".");
	string VPFframe = strDlmTmp2[0];
	int _VPFframe=StringUtility::ToTargetType<int>(VPFframe);
	_VPFframe+=(-averageNum);

	numTimeSteps=averageNum*2+1;
	
	for(int i=0;i!=numTimeSteps;i++)
	{
		std::stringstream sStream;
		sStream<<_VPFframe;
		string NewVPFframe;
		sStream >> NewVPFframe;

		//snare "00" just for temperary

		//add "0" to filename
		string vpfFile=strDlmTmp[0]+"_";
		for(unsigned i=0;i!=(VPFframe.size()-NewVPFframe.size());i++)
		{
			vpfFile+="0";//use size() to decide how many "0" should add
		}
		vpfFile+=NewVPFframe+"."+"vpf";
				
		ShearRateProcessor processor(vpfFile.c_str(),box,interval,numStep);
		//processor.SetRefinedVPFFileName();
		//processor.ReadRefinedFile();
		//processor.Initialize();
		//processor.DefineControlVolume( box );
		//processor.calAveragedVel(interval,numMovement);
		processors.push_back(processor);

		_VPFframe+=1;
	}
	
////Average shear rate over each time step (processors)

	auto iter=processors.begin();
	auto iterEnd=processors.end();
	
	for(iter; iter != iterEnd; ++iter)
	{	
		vector<double> shearRates=iter->GetShearRate();
			for(unsigned i=0;i!=(shearRates.size());i++)
			{
				sumShearRate[i]+=shearRates[i];
			}
	}
	
	for(unsigned i=0;i!=(averagedShearRate.size());i++)
	{
		averagedShearRate[i]=sumShearRate[i]/numTimeSteps;
	}

//Cal. deviation of shear rates //iter should set to zero
	auto iter2=processors.begin();
	auto iterEnd2=processors.end();

	for(iter2;iter2!=iterEnd2;iter2++)
	{
		vector<double> shearRates=iter2->GetShearRate();
		for(unsigned i=0;i!=(numStep-4);i++)
		{
			ShearRatePowDeSum[i]+=pow(shearRates[i]-averagedShearRate[i],2);
		}
	}
	//Normal stress deviation calcilation

	for(unsigned i=0;i!=(numStep-4);i++)
	{
		ShearRateDeviation[i]=sqrt(ShearRatePowDeSum[i]/numTimeSteps);
	}	

//Cal. averaged vel.
	auto iter3=processors.begin();
	auto iterEnd3=processors.end();
	
	for(iter3; iter3 != iterEnd3; ++iter3)
	{	
		vector<AveragedVel> averagedVel=iter3->GetAveragedVel();
			for(unsigned i=0;i!=(averagedVel.size());i++)
			{
				sumAveragedVel[i]=sumAveragedVel[i]+averagedVel[i];
			}
	}
	
	for(unsigned i=0;i!=(averagedXVel.size());i++)
	{
		averagedXVel[i]=sumAveragedVel[i].x()/numTimeSteps;
	}
}

vector<double> ShearRateTimeAverager::GetAveragedShearRates()
{
	return averagedShearRate;
}

vector<double> ShearRateTimeAverager::GetShearRatesDevi()
{
	return ShearRateDeviation;
}

vector<double> ShearRateTimeAverager::GetTimeAveragedXVel()
{
	return averagedXVel;
}


#endif