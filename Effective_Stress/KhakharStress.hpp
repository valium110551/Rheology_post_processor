#ifndef KHAKHARSTRESS_HPP
#define KHAKHARSTRESS_HPP

#include "KhakharStress.h"
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

KhakharStress::KhakharStress(double& firstTotalShearStress,double interval,vector<double>& totalNormalStresses,double firstPosition,double positionY,vector<double> volFraction,double baseVolFraction,double height)
{
	totalNormalStress=0;
	totalShearStress=0;
	RDwidth=7.25;
	bulkDensity=2700/(0.5*3.1415926*16.9*16.9*RDwidth)*980;
	reposeAngle=25;
	WallFriction=0.3;
	double cosine=cos(reposeAngle/180*3.14159265);

	if(volFraction.size()==1)
	{
			totalNormalStress=0;
			totalShearStress=0;
			/*totalNormalStress=bulkDensity*cos(reposeAngle/180*3.14159265)/baseVolFraction*firstVolFraction*height/4.0;	
			totalShearStress=bulkDensity*sin(reposeAngle/180*3.14159265)/baseVolFraction*firstVolFraction*height/4.0-2*WallFriction/RDwidth*totalNormalStress*height/4.0;
			totalNormalStresses.push_back(totalNormalStress);
			firstTotalShearStress=totalShearStress;*/
			totalNormalStresses.push_back(totalNormalStress);
	}
	else
	{
		double sumVolFraction1=accumulate(volFraction.begin(),volFraction.end(),0.0);
		double sumFirstFinalVolFraction=(volFraction[0]+volFraction[volFraction.size()-1]);
		double sumTotalNormalStress=std::accumulate(totalNormalStresses.begin(),totalNormalStresses.end(),0.0);
		double sumFirstFinalTotalNormalStress=(totalNormalStresses[0]+totalNormalStresses[totalNormalStresses.size()-1.0]);

		totalNormalStress=980*1.38*cos(reposeAngle/180*3.14159265)*interval*(sumVolFraction1-sumFirstFinalVolFraction/2.0);//+totalNormalStresses[0];
		//adjust lost term in analytical normal stress sol
		//totalNormalStress=totalNormalStress-2*WallFriction/RDwidth*(interval*(sumTotalNormalStress-sumFirstFinalTotalNormalStress/2.0));
		totalNormalStresses.push_back(totalNormalStress);
		totalShearStress=980*1.38*sin(reposeAngle/180*3.14159265)*interval*(sumVolFraction1-sumFirstFinalVolFraction/2.0)
			-2*WallFriction/RDwidth*(interval*(sumTotalNormalStress-sumFirstFinalTotalNormalStress/2.0));//+firstTotalShearStress;
	double wallForce=2*WallFriction/RDwidth*(interval*(sumTotalNormalStress-sumFirstFinalTotalNormalStress/2.0));
	frictionForce=wallForce;
	//double sumVolFraction1=accumulate(volFraction.begin(),volFraction.end(),0.0);
	//double sumFirstFinalVolFraction=(volFraction[0]+volFraction[volFraction.size()-1]);
	//double sumTotalNormalStress=std::accumulate(totalNormalStresses.begin(),totalNormalStresses.end(),0.0);
	//double sumFirstFinalTotalNormalStress=(totalNormalStresses[0]+totalNormalStresses[totalNormalStresses.size()-1.0]);

	//totalNormalStress=bulkDensity*cos(reposeAngle/180*3.14159265)/baseVolFraction*interval*(sumVolFraction1-sumFirstFinalVolFraction/2.0);//+totalNormalStresses[0];
	//totalNormalStresses.push_back(totalNormalStress);
	//totalShearStress=bulkDensity*sin(reposeAngle/180*3.14159265)/baseVolFraction*interval*(sumVolFraction1-sumFirstFinalVolFraction/2.0)
	//	-2*WallFriction/RDwidth*(interval*(sumTotalNormalStress-sumFirstFinalTotalNormalStress/2.0));//+firstTotalShearStress;
	}

	/*for(unsigned i=0;i!=volFraction.size();i++)
	{
		totalNormalStress+=bulkDensity*cos(reposeAngle/180*3.14159265)*volFraction[i]/baseVolFraction*height;	
		totalShearStress+=bulkDensity*sin(reposeAngle/180*3.14159265)*volFraction[i]/baseVolFraction*height-2*WallFriction/RDwidth*totalNormalStress*height;
	}*/
}

double KhakharStress::GetTotalNormalStress()
{
	return totalNormalStress;
}
double KhakharStress::GetTotalShearStress()
{
	return totalShearStress;
}

double KhakharStress::GetFrictionForce()
{
	return frictionForce;
}
		


#endif