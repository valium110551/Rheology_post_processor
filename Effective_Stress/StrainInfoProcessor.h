#ifndef STRAININFOPROCESSOR_H
#define STRAININFOPROCESSOR_H

#include "PostProcessorBase.h"
#include "VectorX/VectorX.hpp"
#include "Geometry/ImmersedSphere.h"
#include "Geometry\Sphere.h"
#include <string>
#include <vector>
typedef sMathLib::VectorX<double, 3> Velocity3;
typedef sMathLib::VectorX<double, 3> Position3;
typedef unsigned SphereIndex;

using namespace std;

class StrainInfoProcessor:public PostProcessorBase
{
	public:
	StrainInfoProcessor(){}
	~StrainInfoProcessor(){}
	StrainInfoProcessor(Sphere& _controlSphere,string filenameVPF);
	StrainInfoProcessor(map<SphereIndex,ImmersedSphere<double>> _relativeSpheres,string filenameVPF,int targetSphereIndex);
	map<SphereIndex,ImmersedSphere<double>>& GetRelativeSpheres();
	int GetIndexTargetImmersedSphere();
	vector<pair<unsigned,Velocity3>> GetRefRelativeVelocity();
	vector<pair<unsigned,Velocity3>> GetCurRelativeVelocity();
	vector<pair<unsigned,Position3>> GetRefRelativePosition();
	vector<pair<unsigned,Position3>>& GetCurRelativePosition();
	double nearestTargetImmersedSphereDistance(int targetSphereIndex);
	double GetRefNearDis();
	Sphere& GetControlVolume();
	void Initialize(){};

	protected:
		std::map<SphereIndex, ImmersedSphere<double> > _relativeSpheres;
		Sphere controlVolumeSphere ;
		vector<pair<unsigned,Velocity3>> refRelativeVelocity;
		vector<pair<unsigned,Velocity3>> curRelativeVelocity;
		vector<pair<unsigned,Position3>> refRelativePosition;
		vector<pair<unsigned,Position3>> curRelativePosition;
		ImmersedSphere<double> targetImmersedSphere;
		int indexTargetImmersedSphere;
		SphereIndex indexNearestTargetImmersedSphere;
		Position3 nearestImmersedSphere(Sphere& _controlSphere);
		Position3 spherePosition;
		double refNearDis;

};


StrainInfoProcessor::StrainInfoProcessor(Sphere& _controlSphere,string filenameVPF):PostProcessorBase()
{	
	this->SetVPFFileName(filenameVPF.c_str());
	this->ReadFile();

//////////////////////////////////////////////////////////////////////////////
	this->controlVolumeSphere = _controlSphere;
	
	controlVolumeSphere.SetPosition(this->nearestImmersedSphere(controlVolumeSphere));

	spherePosition=controlVolumeSphere.GetPosition();

	//Fill the "_relativeSpheres" map
	for(int index = 0; index != this->_vpfReader.GetSpheres().size(); ++index)
	{
		ImmersedSphere<double>& sphere_ = this->_vpfReader.GetSpheres()[index];

		if(sphere_.getRadius() < 0)
			continue;

		if(controlVolumeSphere.Intersect(sphere_))
		{
			this->_relativeSpheres[(SphereIndex)index] = sphere_;
		}
	}

	//Calculate refRelativeVelocity

	map<SphereIndex,ImmersedSphere<double>> _relativeSpheres_temp=_relativeSpheres;
	//Erase the tagetImmersedSphere from _relativeSpheres
	_relativeSpheres_temp.erase(_relativeSpheres_temp.find((SphereIndex)(this->indexTargetImmersedSphere)));
	
	map<SphereIndex, ImmersedSphere<double> >::iterator iter;
	map<SphereIndex, ImmersedSphere<double> >::iterator iterBegin=_relativeSpheres_temp.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator iterEnd=_relativeSpheres_temp.end();

	//Fill vector "refRelativeVelocity"
	unsigned j=0;
	for(iter=iterBegin;iter!=iterEnd;iter++)
	{
		Velocity3 relativeVelocity=(iter->second).getVelocity()-this->targetImmersedSphere.getVelocity();
		refRelativeVelocity.push_back(make_pair(j,relativeVelocity));
		Position3 relativePosition=(iter->second).getPosition()-this->targetImmersedSphere.getPosition();
		refRelativePosition.push_back(make_pair(j,relativePosition));
		j++;
	}
}

//Construct the next step's information obj for calculating the strain rate 
StrainInfoProcessor::StrainInfoProcessor(map<SphereIndex,ImmersedSphere<double>> RelativeSpheres,string filenameVPF,int targetSphereIndex)
{
	this->SetVPFFileName(filenameVPF.c_str());
	this->ReadFile();

	map<SphereIndex,ImmersedSphere<double>>::iterator iter;
	map<SphereIndex,ImmersedSphere<double>>::iterator iterBegin=RelativeSpheres.begin();
	map<SphereIndex,ImmersedSphere<double>>::iterator iterEnd=RelativeSpheres.end();
	//ImmersedSphere<double> ex;
	for(iter=iterBegin;iter!=iterEnd;iter++)
	{
		this->_relativeSpheres[iter->first]=this->_vpfReader.GetSpheres()[(int)(iter->first)];
	}

	//Fill vector "curRelativeVelocity"

	map<SphereIndex,ImmersedSphere<double>> _relativeSpheres_temp=_relativeSpheres;
	//Erase the tagetImmersedSphere from _relativeSpheres 
	_relativeSpheres_temp.erase(_relativeSpheres_temp.find((SphereIndex)(targetSphereIndex)));
	map<SphereIndex,ImmersedSphere<double>>::iterator iter2;
	map<SphereIndex,ImmersedSphere<double>>::iterator iterBegin2=_relativeSpheres_temp.begin();
	map<SphereIndex,ImmersedSphere<double>>::iterator iterEnd2=_relativeSpheres_temp.end();
		unsigned j=0;
		for(iter2=iterBegin2;iter2!=iterEnd2;iter2++)
		{
			//Velocity3 relativeVelocity=iter2->second.getVelocity();
			Velocity3 relativeVelocity=iter2->second.getVelocity()-this->_relativeSpheres[(SphereIndex)(targetSphereIndex)].getVelocity();
			curRelativeVelocity.push_back(make_pair(j,relativeVelocity));
			Position3 relativePosition=(iter2->second).getPosition()-this->_relativeSpheres[(SphereIndex)(targetSphereIndex)].getPosition();
			curRelativePosition.push_back(make_pair(j,relativePosition));
			j++;
		}

	//Calculate the nearest distance from nearest immersedsphere to the target immersedsphere
		refNearDis=this->nearestTargetImmersedSphereDistance(targetSphereIndex);


}

map<SphereIndex,ImmersedSphere<double>>& StrainInfoProcessor::GetRelativeSpheres()
{
	return this->_relativeSpheres;
}



double StrainInfoProcessor::nearestTargetImmersedSphereDistance(int targetSphereIndex)
{
	
	ImmersedSphere<double>  targetSphere=this->_relativeSpheres[(SphereIndex)targetSphereIndex];
	//ImmersedSphere<double>& sphere_ = this->_vpfReader.GetSpheres()[0];
	map<SphereIndex,ImmersedSphere<double>> _relativeSpheres_temp=_relativeSpheres;
	//Erase the tagetImmersedSphere from _relativeSpheres 
	_relativeSpheres_temp.erase(_relativeSpheres_temp.find((SphereIndex)(targetSphereIndex)));
	std::map<SphereIndex, ImmersedSphere<double> >::iterator iterBegin=_relativeSpheres_temp.begin();
	std::map<SphereIndex, ImmersedSphere<double> >::iterator iterEnd=_relativeSpheres_temp.end();
	std::map<SphereIndex, ImmersedSphere<double> >::iterator iter;

	
	double minDistance=(iterBegin->second.getPosition()-targetSphere.getPosition()).Abs();
	
	for(iter=iterBegin; iter !=iterEnd ; ++iter)
	{
		double minDistance2=(iter->second.getPosition()-targetSphere.getPosition()).Abs();
		if(minDistance2<=minDistance)
		{
			minDistance=minDistance2;
			indexNearestTargetImmersedSphere=iter->first;
			//this->_relativeSpheres[(SphereIndex)index] = sphere_;
		}
	}
	
	return minDistance;
}

Position3 StrainInfoProcessor::nearestImmersedSphere(Sphere& _controlSphere)
{
	
	ImmersedSphere<double>& sphere_ = this->_vpfReader.GetSpheres()[0];
	double minDistance=(_controlSphere.GetPosition()-sphere_.getPosition()).Abs();
	
	for(int index = 0; index != this->_vpfReader.GetSpheres().size(); ++index)
	{
		ImmersedSphere<double>& sphere_ = this->_vpfReader.GetSpheres()[index];

		if(sphere_.getRadius() < 0)
			continue;
		
		double minDistance2=(_controlSphere.GetPosition()-sphere_.getPosition()).Abs();
		if(minDistance2<=minDistance)
		{
			minDistance=minDistance2;
			indexTargetImmersedSphere=index;
			//this->_relativeSpheres[(SphereIndex)index] = sphere_;
		}
	}
	//ImmersedSphere<double>& sphere_temp=this->_vpfReader.GetSpheres()[indexOfNearestSphere];
	this->targetImmersedSphere=this->_vpfReader.GetSpheres()[indexTargetImmersedSphere];
	//sphere_ = this->_vpfReader.GetSpheres()[indexOfNearestSphere];
	//return sphere_.getPosition();
	return targetImmersedSphere.getPosition();
}

int StrainInfoProcessor::GetIndexTargetImmersedSphere()
{
	return this->indexTargetImmersedSphere;
}

vector<pair<unsigned,Velocity3>> StrainInfoProcessor::GetRefRelativeVelocity()
{
	return this->refRelativeVelocity;
}

vector<pair<unsigned,Velocity3>> StrainInfoProcessor::GetCurRelativeVelocity()
{
	return this->curRelativeVelocity;
}

double StrainInfoProcessor::GetRefNearDis()
{
	return this->refNearDis;
}

Sphere& StrainInfoProcessor::GetControlVolume()
{
	return controlVolumeSphere;
}

vector<pair<unsigned,Position3>> StrainInfoProcessor::GetRefRelativePosition()
{
	return this->refRelativePosition;
}

vector<pair<unsigned,Position3>>& StrainInfoProcessor::GetCurRelativePosition()
{
	return this->curRelativePosition;
}

#endif