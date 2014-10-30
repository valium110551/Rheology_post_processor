#ifndef SHEARRATEPROCESSOR_H
#define SHEARRATEPROCESSOR_H

#include "PostProcessorBase.h"
#include "_ahedron.h"
#include "Geometry\box.h"
#include <string>
#include <map>
#include "VectorX/VectorX.hpp"
#include <vector>

typedef unsigned SphereIndex;
typedef sMathLib::VectorX<double, 3> AveragedVel;

using namespace std;

class ShearRateProcessor:public PostProcessorBase
{
	public:
		ShearRateProcessor(string vpfFile,Box box,double interval,unsigned numMovement);
		void DefineControlVolume(_ahedron& ahedron,map<unsigned,ImmersedSphere<double>>& _relativeSpheres);
		void Initialize(){};
		AveragedVel calAveragedVel(map<unsigned,ImmersedSphere<double>> _relativeSpheres);
		void calShearRate(double interval);
		vector<double> GetShearRate();
		vector<AveragedVel> GetAveragedVel();
	protected:
		vector<AveragedVel> averagedVels;
		vector<double> shearRates;
		double intreval;
		
		
};

ShearRateProcessor::ShearRateProcessor(string vpfFile,Box box,double interval,unsigned numStep):PostProcessorBase()
{	
	this->intreval=interval;
	this->SetRefinedVPFFileName(vpfFile);
	this->ReadRefinedFile();
	for(unsigned i=0;i!=numStep;i++)
	{
		map<unsigned,ImmersedSphere<double>>_relativeSpheres;
		this->DefineControlVolume(box,_relativeSpheres);
		AveragedVel averagedVel=this->calAveragedVel(_relativeSpheres);
		
		averagedVels.push_back(averagedVel);
		//Move the control volume downward
		sMathLib::VectorX<double,3> position(box.GetPosition().x(),box.GetPosition().y()-interval,box.GetPosition().z());
		box.SetPosition(position);
	}

	this->calShearRate(interval);
}

void ShearRateProcessor::DefineControlVolume(_ahedron& ahedron,map<unsigned,ImmersedSphere<double>>& _relativeSpheres)
{
	//Fill the "_relativeSpheres" map
	for(int index = 0; index != this->_vpfRefinedReader.GetSpheres().size(); ++index)
	{
		ImmersedSphere<double>& sphere_ = this->_vpfRefinedReader.GetSpheres()[index];

		if(sphere_.getRadius() < 0)
			continue;

		if(ahedron.Intersect(sphere_))
		{
			_relativeSpheres[(SphereIndex)index] = sphere_;
		}
	}
}

 AveragedVel ShearRateProcessor::calAveragedVel(map<unsigned,ImmersedSphere<double>> _relativeSpheres)
{

	
	auto iter=_relativeSpheres.begin();
	auto iterEnd=_relativeSpheres.end();
	unsigned numRelatedSphere=_relativeSpheres.size();
	double sumVel_X=0;
	double sumVel_Y=0;
	double sumVel_Z=0;

	for(iter;iter!=iterEnd;iter++)
	{
		sumVel_X+=iter->second.getVelocity().x();
		sumVel_Y+=iter->second.getVelocity().y();
		sumVel_Z+=iter->second.getVelocity().z();
	}

	sMathLib::VectorX<double, 3> tempAveVel(sumVel_X/numRelatedSphere,sumVel_Y/numRelatedSphere,sumVel_Z/numRelatedSphere);
	return tempAveVel;
	
}

 void ShearRateProcessor::calShearRate(double interval)
 {		
		for(unsigned i=2;i!=(averagedVels.size()-2);i++)
		{
			//double _shearRate=(averagedVels[i].x()-averagedVels[i+1].x())/(this->intreval);
			double _shearRate=-(averagedVels[i-2].x()-8.0*averagedVels[i-1].x()+8.0*averagedVels[i+1].x()-averagedVels[i+2].x())/12.0/(this->intreval);
			shearRates.push_back(_shearRate);
		}
 }

 vector<double> ShearRateProcessor::GetShearRate()
 {
	return shearRates;
 }

 vector<AveragedVel> ShearRateProcessor::GetAveragedVel()
 {
	return averagedVels;
 }

#endif