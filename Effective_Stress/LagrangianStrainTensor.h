#ifndef LAGRANGIANSTRAINTENSOR_H
#define LAGRANGIANSTRAINTENSOR_H

#include "PostProcessorBase.h"
#include "StrainInfoProcessor.h"
#include "VectorX/VectorX.hpp"//in package
#include "Geometry/ImmersedSphere.h"
#include "Geometry\Sphere.h"//in package
#include <string>
#include <vector>
#include "Tensor.h"//in package
typedef sMathLib::VectorX<double, 3> Velocity3;
typedef sMathLib::VectorX<double, 3> Position3;

typedef unsigned SphereIndex;


class LagrangianStrainTensor
{
	typedef double StrainTensor[3][3];

	public:
		LagrangianStrainTensor(Sphere& _controlSphere,string filenameVPF);
		void CalculateStrainTensor();
		void Initialize(){};
		sMathLib::Tensor<double> GetStrainTensor();
	
	protected:
		StrainInfoProcessor processor1;
		StrainInfoProcessor processor2;
		ImmersedSphere<double> targetImmersedSphere;
		Position3 nearestImmersedSphere(Sphere& _controlSphere);
		StrainTensor strainTensor;
};


////Create 2 processors to process 2 time step's particle infomation.
LagrangianStrainTensor::LagrangianStrainTensor(Sphere& _controlSphere,string filenameVPF)
{	
//Create 1st. processor
	StrainInfoProcessor processor1_temp(_controlSphere,filenameVPF);
	processor1=processor1_temp;

//Create 2nd. processor
	vector<string> strDlmTmp = StringUtility::StringDelimiter(filenameVPF, "_");//in package
	vector<string> strDlmTmp2 = StringUtility::StringDelimiter(strDlmTmp[1], ".");
	string VPFframe = strDlmTmp2[0];
	
	int _VPFframe=StringUtility::ToTargetType<int>(VPFframe);//in package
	//Create frame number of 2nd. processor
	_VPFframe+=1;

	std::stringstream sStream;
	sStream<<_VPFframe;
	string NewVPFframe;
	sStream >> NewVPFframe;

	string vpfFile=strDlmTmp[0]+"_";
	for(unsigned i=0;i!=(VPFframe.size()-NewVPFframe.size());i++)
	{
		vpfFile+="0";
	}
	vpfFile+=NewVPFframe+"."+"vpf";

	StrainInfoProcessor processor2_temp(processor1.GetRelativeSpheres(),vpfFile,processor1.GetIndexTargetImmersedSphere());
	processor2=processor2_temp;	
}

void LagrangianStrainTensor::CalculateStrainTensor()
{
	double D[3][3]={0.};
	double A[3][3]={0.};
	double F[3][3]={0.};
	double tf[3][3]={0.};
	double Lstrain[3][3]={0.};

	vector<pair<unsigned,Velocity3>> refRelativeVector=processor1.GetRefRelativeVelocity();
	vector<pair<unsigned,Velocity3>> curRelativeVector=processor2.GetCurRelativeVelocity();
	vector<pair<unsigned,Position3>>& refRelativePosition=processor1.GetRefRelativePosition();
	vector<pair<unsigned,Position3>>& curRelativePosition=processor2.GetCurRelativePosition();	
	
	for(int j = 0; j != refRelativeVector.size(); ++j)
			{
				double x1 = (refRelativePosition[j]).second.x();
				double y1 = (refRelativePosition[j]).second.y();
				double z1 = (refRelativePosition[j]).second.z();
					
				double x2 = (curRelativePosition[j]).second.x();
				double y2 = (curRelativePosition[j]).second.y();
				double z2 = (curRelativePosition[j]).second.z();
				/*double x1 = (refRelativeVector[j]).second.x();
				double y1 = (refRelativeVector[j]).second.y();
				double z1 = (refRelativeVector[j]).second.z();
					
				double x2 = (curRelativeVector[j]).second.x();
				double y2 = (curRelativeVector[j]).second.y();
				double z2 = (curRelativeVector[j]).second.z();*/
								
				double refNearDis=processor2.GetRefNearDis();
				double radius=this->processor1.GetControlVolume().GetRadius();

				double relativeDistance=curRelativePosition[j].second.Abs();
				double r = (relativeDistance - refNearDis)/radius;

				double weightFactor;
				if(r<=0.5) weightFactor = 1-6*r*r+6*r*r*r;
				else if(r>0.5 && r<1.) weightFactor = 2-6*r+6*r*r-2*r*r*r;
				else weightFactor = 0;
	
				D[0][0] += x1*x1*weightFactor;D[0][1] += x1*y1*weightFactor; D[0][2] += x1*z1*weightFactor;
				D[1][0] += y1*x1*weightFactor;D[1][1] += y1*y1*weightFactor; D[1][2] += y1*z1*weightFactor;
				D[2][0] += z1*x1*weightFactor;D[2][1] += z1*y1*weightFactor; D[2][2] += z1*z1*weightFactor;
				
				A[0][0] += x2*x1*weightFactor;A[0][1] += x2*y1*weightFactor;A[0][2] += x2*z1*weightFactor;
				A[1][0] += y2*x1*weightFactor;A[1][1] += y2*y1*weightFactor;A[1][2] += y2*z1*weightFactor;
				A[2][0] += z2*x1*weightFactor;A[2][1] += z2*y1*weightFactor;A[2][2] += z2*z1*weightFactor;
			}
	double matrix[9];
	matrix[0] = D[0][0];
	matrix[1] = D[0][1];
	matrix[2] = D[0][2];
	matrix[3] = D[1][0];
	matrix[4] = D[1][1];
	matrix[5] = D[1][2];
	matrix[6] = D[2][0];
	matrix[7] = D[2][1];
	matrix[8] = D[2][2];
	
	double determine=matrix[0]*matrix[4]*matrix[8]+matrix[1]*matrix[5]*matrix[6]+matrix[2]*matrix[3]*matrix[7]-matrix[2]*matrix[4]*matrix[6]-matrix[5]*matrix[7]*matrix[0]-matrix[1]*matrix[3]*matrix[8];
	double _determine = 1./determine;

	double inverse[3][3];
		inverse[0][0] = _determine * ( matrix[4]*matrix[8]-matrix[5]*matrix[7]);
		inverse[0][1] = _determine * (-matrix[1]*matrix[8]+matrix[2]*matrix[7]);
		inverse[0][2] = _determine * ( matrix[1]*matrix[5]-matrix[2]*matrix[4]);
		inverse[1][0] = _determine * (-matrix[3]*matrix[8]+matrix[5]*matrix[6]);
		inverse[1][1] = _determine * ( matrix[0]*matrix[8]-matrix[2]*matrix[6]);
		inverse[1][2] = _determine * (-matrix[0]*matrix[5]+matrix[2]*matrix[3]);
		inverse[2][0] = _determine * ( matrix[3]*matrix[7]-matrix[4]*matrix[6]);
		inverse[2][1] = _determine * (-matrix[0]*matrix[7]+matrix[1]*matrix[6]);
		inverse[2][2] = _determine * ( matrix[0]*matrix[4]-matrix[1]*matrix[3]);

		for(int p = 0; p != 3; ++p)
			{
				for(int q = 0; q != 3; ++q)
				{
					for(int r = 0; r != 3; ++r)
					{
						F[p][q] += A[p][r]*inverse[r][q];
					}
				}
				
			}

			 tf[0][0]=F[0][0];
		     tf[0][1]=F[1][0];
             tf[0][2]=F[2][0];
             tf[1][0]=F[0][1];
             tf[1][1]=F[1][1];
             tf[1][2]=F[2][1];
             tf[2][0]=F[0][2];
             tf[2][1]=F[1][2];
             tf[2][2]=F[2][2];
	     

		for(int p = 0; p != 3; ++p)
			{
				for(int q = 0; q != 3; ++q)
				{
					for(int r = 0; r != 3; ++r)
					{
						Lstrain[p][q] += tf[p][r]*F[r][q];
					}
				}
			}	 


			 Lstrain[0][0]-=1;
			 Lstrain[1][1]-=1;
			 Lstrain[2][2]-=1;


			 for(int p = 0; p != 3; ++p)
			{
				for(int q = 0; q != 3; ++q)
				{
						Lstrain[p][q] = 0.5*Lstrain[p][q];
				}
			}
			//Fill Tensor "strainTensor"
			 for(int p = 0; p != 3; ++p)
			{
				for(int q = 0; q != 3; ++q)
				{
						strainTensor[p][q] = Lstrain[p][q];
				}
			}

}


sMathLib::Tensor<double> LagrangianStrainTensor::GetStrainTensor()
{
	return sMathLib::Tensor<double>(this->strainTensor);	
}
		
#endif