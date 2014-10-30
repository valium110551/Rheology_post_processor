//adding shear rate cal. 2013/01/06
//adding Khakhar stress 2012/11/27
//adding deviaion of virial stress 2012/12/17
//6d_center
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <fstream>

#include "StressTimeAverager.h"
#include "StressTimeAverager.hpp"
#include "EffectiveStressCalculator.hpp"
#include "EffectiveStressCalculator.h"
#include "LagrangianStrainTensor.h"
#include "KhakharStress.h"
#include "KhakharStress.hpp"

#include "VirialStressTensor.h"
#include "CundallStressTensor.h"

#include "FileReader/CSVReader.h"
#include "FileReader/ParameterReader.h"

//#include "Geometry/Box3D.h"
#include "Geometry/Sphere.h"
#include "Geometry/Rectangle.h"
#include "Geometry/LineSegment.h"

#include "VectorX/VectorX.hpp"
#include "Tensor.h"

#include "FluidFieldDataConverter/FluidFieldData.h"
#include "FluidFieldDataConverter/BoundaryType.h"
#include "PostProcessorBase.h"

#include "StringConverter/StringUtility.h"

#include "ShearRateTimeAverager.h"

using namespace std;
using namespace sMathLib;

typedef double T;
typedef sMathLib::VectorX<double, 3> AveragedVel;

void tensorToFile(Tensor<T>& tensor,fstream& offile);
void tensorToScreen(Tensor<T>& tensor);


/////////////////////Vertical sampling_Input:rd_002500.vpf interactions_002500.csv 25/////////////////////////
//int main(int argc, char* argv[])
//{
///////////////////////////// Parse input file name ///////////////////////////
//	string filenameVPF(argv[1]);
//	string filenameCSV(argv[2]);
//	string _angle(argv[3]);
//
//	istringstream convert(_angle);
//	double angle;
//	convert>>angle;
//	
//
//	//pattern_001212.vpf interactions_001212.csv field_110000.dat 
//	//Sphere sphere(2,-0.00605995,0,10);
//	//ibmtest_000040.vpf interactions_000040.csv field_110000.dat  Sphere sphere(125,25,160,14);
//	//ibmtest_000001.vpf interactions_000001.csv field_110000.dat
//	//three particle case:verify strain code:threeParticle_000007.vpf threeParticleInteraction_000007.csv field_110000.dat
//	//four particle case:verify strain code:four_000007.vpf FourInteraction.csv field_110000.dat Sphere sphere(0,0,0,10)
//	//Rotating Drum steady state:rd_004000.vpf RDinteraction_004000.csv field_110000.dat Sphere sphere(0,-12.5,0,2.5)
//	
///////Sphere parameters
//	double radius=2.5;
//	double depth=16.9;
//	//double positionY=radius;
//
///////Control volume parameters
//	double diameter=0.476;
//	double length=15*diameter;
//	double width=10*diameter;
//	double height=5*diameter;
//	double interval=0.2;
//	
//	
//	double firstPosition=height/2.0;
//	double depthConstVolFraction=6;	//Khakhar stress
//	double depthFirstVolFraction=height/4.0;//Khakhar stress
//	
//	int averagedStep=0;//number of time steps=averagedStep*2+1
//////Control volume parameters
//
//	string frame=filenameVPF.substr(3,6);
//	string strain="strain";
//	string stress="stress";
//	string kstress="kstress";
//	string shearRate_="shearRate";
//	string bottom="_";
//	string txt=".txt";
//
//	//ofstream outfile2(strain+bottom+frame+txt);
//	ofstream outfile(stress+bottom+frame+txt);
//	ofstream outfile3(kstress+bottom+frame+txt);
//	ofstream outfile6(shearRate_+bottom+frame+txt);
//	
//
///////Refine input data
//	
//	//input file name
//	string VPFfileName=filenameVPF;
//	string CSVfileName=filenameCSV;
//	//output file name
//	string VPFfileName2=filenameVPF.replace(2,1,"2_");
//	string CSVfileName2=filenameCSV.replace(11,1,"2");
//	CSVfileName2.replace(0,1,"RDi");
//	
//
//	string number[11]={"1","2","3","4","5","6","7","8","9","10","11"};
//	for(unsigned i=0;i!=1;i++)
//	{
//	//VPFfileName2.replace(9,1,number[i]);
//	//CSVfileName2.replace(20,1,number[i]);
//	//VPFfileName.replace(8,1,number[i]);
//	//CSVfileName.replace(18,1,number[i]);
//
//	ofstream* outfile4=new ofstream(VPFfileName2);
//	if(!*outfile4)
//	{
//	cerr<<"Can't open file"<<endl;
//	}
//	ofstream* outfile5=new ofstream(CSVfileName2);
//	if(!*outfile5)
//	{
//	cerr<<"Can't open file"<<endl;
//	}
//	std::string unrefinedCSV(CSVfileName);
//	std::string unrefinedVPF(VPFfileName);
//	double length2=16*diameter;
//	double width2=10*diameter;
//	double height2=10*diameter;
//	double positionY2=height2/2.0;
//	Box box2(length2,height2,width2,0,-10,0);
//	ParticleFluidProcessor<VirialStressTensor> processor;		
//	processor.SetCSVFileName(unrefinedCSV.c_str());
//	processor.SetVPFFileName(unrefinedVPF.c_str());
//	processor.ReadFile();
//	processor.Initialize();
//	processor.refineInputData(outfile4,outfile5,box2,angle);
//	outfile4->close();
//	outfile5->close();
//	}
///////Refine input data
//
///////Shear rate calculation
//	//double positionX=0;
//	//double positionY=-height/2.0;//cal shear rate
//	//double positionZ=0;
//
//	//Box box(length,height,width,positionX,positionY,positionZ);
//	////Box box(30,2,7.2,0,1.0,0);//test rotation
//	//unsigned numStep=32;
//	//ShearRateTimeAverager shearRateTimeAverager(filenameVPF,box,interval,numStep,averagedStep);
//	//vector<double> shearRate=shearRateTimeAverager.GetAveragedShearRates();
//	//vector<double> shearRateDevi=shearRateTimeAverager.GetShearRatesDevi();
//	//vector<double> averagedXVel=shearRateTimeAverager.GetTimeAveragedXVel();
//	//cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	//cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	//cout<<"positionX= " <<positionX<<" positionY= "<<positionY<<" positionZ= "<<positionZ<<endl;
//	//outfile6<<"positionX= " <<positionX<<" positionY= "<<positionY<<" positionZ= "<<positionZ<<endl;
//	//outfile6<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	//outfile6<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	//cout<<"depth"<<" shearRate"<<" SRdeviation"<<endl;
//	//outfile6<<"depth"<<" shearRate"<<" SRdeviation"<<endl;
//	//for(unsigned i=0;i!=shearRate.size();i++)
//	//{
//	//	/*cout<<"depth= "<<-positionY-interval*i-interval/2.0<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
//	//	outfile6<<"depth= "<<-positionY-interval*i-interval/2.0<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;*/
//	//	cout<<positionY-2*interval-interval*i<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
//	//	outfile6<<positionY-2*interval-interval*i<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
//	//}
//
//	//cout<<endl;
//	//outfile6<<endl;
//	////Output averaged velocityX
//	//cout<<"Averaged velocityX:"<<endl;
//	//outfile6<<"Averaged velocityX:"<<endl;
//	//cout<<"depth "<<"AveragedVelX"<<endl;
//	//outfile6<<"depth "<<"AveragedVelX"<<endl;
//
//	//for(unsigned i=0;i!=averagedXVel.size();i++)
//	//{
//	//	cout<<positionY-interval*i<<" "<<averagedXVel[i]<<endl;
//	//	outfile6<<positionY-interval*i<<" "<<averagedXVel[i]<<endl;
//	//}
///////Shear rate calculation
//
//  
///////KhakharStress/////
//	double positionY=-10;
//	//double positionY=-height/2.0;
//	//Khakhar stress output
//		cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//		cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//		outfile3<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//		outfile3<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	////Khakhar stress output
//	vector<double> volFractions;
//	vector<double> totalNormalStresses;
//	double firstTotalShearStress;
//	////Sphere sphere(0,-5,0,radius);
//	Box box1(length,height,width,0,-depthConstVolFraction,0);
//	StressTimeAverager<VirialStressTensor> stressTimeAveragerBaseVol(filenameVPF,filenameCSV,box1,averagedStep);
//	double baseVolFraction=stressTimeAveragerBaseVol.GetAveragedVolumeFraction();
//	
//	/*Box box3(length,height/2.0,width,0,-depthFirstVolFraction,0);
//	StressTimeAverager<VirialStressTensor> stressTimeAveragerFirstVol(filenameVPF,filenameCSV,box3,averagedStep);
//	double firstVolFraction=stressTimeAveragerFirstVol.GetAveragedVolumeFraction();*/
//
//	for(unsigned i=0;i!=50;i++)
//	{
//	///KhakharStress calculation///
//	
//	//Sphere sphere(0,-positionY,0,radius);	
//	
//	Box box2(length,height,width,0,positionY,0);
//	//
//	StressTimeAverager<VirialStressTensor> stressTimeAverager(filenameVPF,filenameCSV,box2,averagedStep);
//
//	double volumeFraction=stressTimeAverager.GetAveragedVolumeFraction();
//	volFractions.push_back(volumeFraction);
//	KhakharStress _KhakharStress(firstTotalShearStress,interval,totalNormalStresses,firstPosition,positionY,volFractions,baseVolFraction,height);
//	double totalNormalStress=_KhakharStress.GetTotalNormalStress();
//	
//	////output KhakharStress
//	cout<<"depth="<<" "<<positionY<<" "<<_KhakharStress.GetTotalNormalStress()<<" "<<_KhakharStress.GetTotalShearStress()<<" "<<"volume fraction= "<<volumeFraction<<" "<<_KhakharStress.GetFrictionForce()<<endl;
//
//	outfile3<<"depth="<<" "<<positionY<<" "<<_KhakharStress.GetTotalNormalStress()<<" "<<_KhakharStress.GetTotalShearStress()<<" "<<"volume fraction= "<<volumeFraction<<" "<<_KhakharStress.GetFrictionForce()<<endl;
//	positionY-=interval;
//	}
//	
//	///KhakharStress calculation///
//
///////KhakharStress/////
//
//
//
///////Strain calculation//////////
//	//for(unsigned i=0;i!=18;i++)
//	//{
//	///*LagrangianStrainTensor lagrangian(sphere,filenameVPF);
//	//lagrangian.CalculateStrainTensor();
//	//Tensor<T> strainTensor=lagrangian.GetStrainTensor();*/
//	//Output strain tensor to screen 
//	//cout<<"Lagrangian strain tensor:"<<endl;
//	//tensorToScreen(strainTensor);	
// //   Output strain tensor to file "strain.txt" 
//	//outfile2<<"Lagrangian strain tensor:"<<endl;
//	//tensorToFile(strainTensor,outfile2);
//	//}
///////Strain calculation//////////
//
///////virial stress calculation/////
////
//	///double positionY=1.2;//check whether rotated or not
////	double positionY=-height/2.0;//cal stress
////	///////virial stress output to screen 
////	cout<<"Averaged steps:"<<averagedStep*2+1<<endl;
////	cout<<"virial stress calculation";
////	cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
////	cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
////	cout<<"depth"<<" "<<"depth "<<"PotenNorStress"<<" "<<"PotenNorStress deviation"<<" "<<" "<<" "<<"PotenShearStress"<<" "<<"PotenShearStress devitaion"<<" "<<"KineNorStress"<<" "<<"KineSheaStess deviation"<<endl;
////
////	outfile<<"Averaged steps:"<<averagedStep*2+1<<endl;
////	outfile<<"virial stress calculation";
////	outfile<<"Averaged time steps:"<<averagedStep*2+1<<endl;
////	outfile<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
////	outfile<<"depth"<<" "<<"depth "<<"PotenNorXXStress"<<" "<<"PotenNorXXStressDeviation"<<" "<<"PotenNorYYStress"<<" "<<"PotenNorYYStressDeviation "<<" "<<"PotenNorZZStress"<<" "<<"PotenShearStress"<<" "<<"PotenShearStressDevitaion"<<" "<<"KineNorXXStress "<<"KineNorYYStress"<<" "<<"KineSheaXYStessDeviation"<<endl;
/////////////	virial stress infomation output	
////
////for(unsigned i=0;i!=50;i++)
////{
////	//Sphere sphere(0,positionY,0,radius);	
////	Box box2(length,height,width,0,positionY,0);
/////////////////	Box box2(length,height,width,0,-8.0,0);//test
////	StressTimeAverager<VirialStressTensor> stressTimeAverager(VPFfileName2,CSVfileName2,box2,averagedStep);
////	Tensor<T> averagedPotentialTensor=stressTimeAverager.GetAveragedPotentialStressTensor();
////	Tensor<T> averagedKineticTensor=stressTimeAverager.GetAveragedKineticStressTensor();
////	Tensor<T> averagedTotalStressTensor=stressTimeAverager.GetAveragedTotalStressTensor();	
////
///////////////////////////////Output virial stress Data/////////////////////////////
//////////Output stress to screen: only output stress11 stress12
////	cout<<"depth="<<" "<<positionY<<" "<<averagedPotentialTensor[0][0]<<" "<<stressTimeAverager.GetNorXXStressDevi()<<" "<<averagedPotentialTensor[1][1]<<" "<<stressTimeAverager.GetNorYYStressDevi()<<" "<<averagedPotentialTensor[2][2]<<" "<<averagedPotentialTensor[0][1]<<" "<<stressTimeAverager.GetSheaXYStressDevi()<<" "<<averagedKineticTensor[0][0]<<" "<<averagedKineticTensor[1][1]<<" "<<averagedKineticTensor[0][1]<<endl;
///////////////Output stress to file: only output stress11 stress12
////	outfile<<"depth="<<" "<<positionY<<" "<<averagedPotentialTensor[0][0]<<" "<<stressTimeAverager.GetNorXXStressDevi()<<" "<<averagedPotentialTensor[1][1]<<" "<<stressTimeAverager.GetNorYYStressDevi()<<" "<<averagedPotentialTensor[2][2]<<" "<<averagedPotentialTensor[0][1]<<" "<<stressTimeAverager.GetSheaXYStressDevi()<<" "<<averagedKineticTensor[0][0]<<" "<<averagedKineticTensor[1][1]<<" "<<averagedKineticTensor[0][1]<<endl;
////	positionY=positionY-interval;
////}
////Output stress to screen:Tensor form
//	/*cout<<"depth="<<positionY-interval<<endl;
//	cout<<"Averaged potential tensor:"<<endl;
//	tensorToScreen(averagedPotentialTensor);
//
//	cout<<"Averaged kinetic tensor:"<<endl;
//	tensorToScreen(averagedKineticTensor);
//
//	cout<<"Total stress tensor:"<<endl;
//	tensorToScreen(averagedTotalStressTensor);*/
//
////Output stress to file:Tensor form
//	
//	///*outfile<<"depth="<<positionY-interval<<endl;
//	//outfile<<"Averaged potential stress tensor:"<<endl;
//	//tensorToFile(averagedPotentialTensor,outfile);
//
//	//outfile<<"Averaged kinetic stress tensor:"<<endl;
//	//	tensorToFile(averagedKineticTensor,outfile);
//
//	//outfile<<"Total stress tensor:"<<endl;
//	//tensorToFile(averagedTotalStressTensor,outfile);*/
//
//
//	return 0;
//}
//
//
//void tensorToFile(Tensor<T>& tensor,fstream& offile)
//{
//	for(unsigned alpha = 0; alpha != 3; ++alpha)
//		{
//			for(unsigned beta = 0; beta != 3; ++beta)
//			{
//				offile << tensor[alpha][beta] << " ";
//			}
//			offile << endl;
//		}
//		offile << endl;
//}
//
//void tensorToScreen(Tensor<T>& tensor)
//{
//	for(unsigned alpha = 0; alpha != 3; ++alpha)
//		{
//			for(unsigned beta = 0; beta != 3; ++beta)
//			{
//				cout << tensor[alpha][beta] << " ";
//			}
//			cout << endl;
//		}
//		cout << endl;
//}

/////////////Vertical sampling_Input Size:rd_000609.vpf interactions_000609.csv 28.01 3 6 5//////////////////////////////
//

int main(int argc, char* argv[])
{
/////////////////////////// Parse input file name ///////////////////////////
	string filenameVPF(argv[1]);
	string filenameCSV(argv[2]);
	string _angle(argv[3]);
	string length_str(argv[4]);
	string width_str(argv[5]);
	string height_str(argv[6]);

	double length_num;
	double width_num;
	double height_num;

	StringUtility::ToTargetType<double>(length_str,length_num);
	StringUtility::ToTargetType<double>(width_str,width_num);
	StringUtility::ToTargetType<double>(height_str,height_num);

	istringstream convert(_angle);
	double angle;
	convert>>angle;
	

	//pattern_001212.vpf interactions_001212.csv field_110000.dat 
	//Sphere sphere(2,-0.00605995,0,10);
	//ibmtest_000040.vpf interactions_000040.csv field_110000.dat  Sphere sphere(125,25,160,14);
	//ibmtest_000001.vpf interactions_000001.csv field_110000.dat
	//three particle case:verify strain code:threeParticle_000007.vpf threeParticleInteraction_000007.csv field_110000.dat
	//four particle case:verify strain code:four_000007.vpf FourInteraction.csv field_110000.dat Sphere sphere(0,0,0,10)
	//Rotating Drum steady state:rd_004000.vpf RDinteraction_004000.csv field_110000.dat Sphere sphere(0,-12.5,0,2.5)
	
/////Sphere parameters
	double radius=2.5;
	double depth=16.9;
	//double positionY=radius;

/////Control volume parameters
	double diameter=0.476;
	double length=length_num*diameter;
	double width=width_num*diameter;
	double height=height_num*diameter;
	/*double length=3*diameter;
	double width=6*diameter;
	double height=5*diameter;*/
	double interval=0.2;
	
	
	double firstPosition=height/2.0;
	double depthConstVolFraction=6;	//Khakhar stress
	double depthFirstVolFraction=height/4.0;//Khakhar stress
	
	int averagedStep=0;//number of time steps=averagedStep*2+1
////Control volume parameters

	string frame=filenameVPF.substr(3,6);
	//string strain="strain";
	//string stress="stress";
	//string kstress="kstress";
	//string shearRate_="shearRate";
	//string bottom="_";
	//string txt=".txt";

	//ofstream outfile2(strain+bottom+frame+txt);
	//ofstream outfile(stress+bottom+frame+txt);
	//ofstream outfile3(kstress+bottom+frame+txt);
	//ofstream outfile6(shearRate_+bottom+frame+txt);

	string stress_="stress_";
	string kstress_="kstress_";
	string shearRate_="shearRate_";
	string txt_=".txt";
	string bottom="_";

	//ofstream outfile2("strain.txt");
	ofstream outfile(stress_+frame+bottom+length_str+bottom+width_str+bottom+height_str+txt_);
	ofstream outfile3(kstress_+frame+bottom+length_str+bottom+width_str+bottom+height_str+txt_);
	ofstream outfile6(shearRate_+frame+bottom+length_str+bottom+width_str+bottom+height_str+txt_);
	

/////Refine input data
	
	//input file name
	string VPFfileName=filenameVPF;
	string CSVfileName=filenameCSV;
	//output file name
	string VPFfileName2=filenameVPF.replace(2,1,"2_");
	string CSVfileName2=filenameCSV.replace(11,1,"2");
	CSVfileName2.replace(0,1,"RDi");
	

	string number[11]={"1","2","3","4","5","6","7","8","9","10","11"};
	for(unsigned i=0;i!=1;i++)
	{
	//VPFfileName2.replace(9,1,number[i]);
	//CSVfileName2.replace(20,1,number[i]);
	//VPFfileName.replace(8,1,number[i]);
	//CSVfileName.replace(18,1,number[i]);

	ofstream* outfile4=new ofstream(VPFfileName2);
	if(!*outfile4)
	{
	cerr<<"Can't open file"<<endl;
	}
	ofstream* outfile5=new ofstream(CSVfileName2);
	if(!*outfile5)
	{
	cerr<<"Can't open file"<<endl;
	}
	std::string unrefinedCSV(CSVfileName);
	std::string unrefinedVPF(VPFfileName);
	double length2=20*diameter;
	double width2=15*diameter;
	double height2=80*diameter;
	double positionY2=height2/2.0;
	Box box2(length2,height2,width2,0,0,0);
	ParticleFluidProcessor<VirialStressTensor> processor;		
	processor.SetCSVFileName(unrefinedCSV.c_str());
	processor.SetVPFFileName(unrefinedVPF.c_str());
	processor.ReadFile();
	processor.Initialize();
	processor.refineInputData(outfile4,outfile5,box2,angle);
	outfile4->close();
	outfile5->close();
	}
/////Refine input data

/////Shear rate calculation
	//double positionX=0;
	//double positionY=-height/2.0;//cal shear rate
	//double positionZ=0;

	//Box box(length,height,width,positionX,positionY,positionZ);
	////Box box(30,2,7.2,0,1.0,0);//test rotation
	//unsigned numStep=40;
	//ShearRateTimeAverager shearRateTimeAverager(filenameVPF,box,interval,numStep,averagedStep);
	//vector<double> shearRate=shearRateTimeAverager.GetAveragedShearRates();
	//vector<double> shearRateDevi=shearRateTimeAverager.GetShearRatesDevi();
	//vector<double> averagedXVel=shearRateTimeAverager.GetTimeAveragedXVel();
	//cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
	//cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
	//cout<<"positionX= " <<positionX<<" positionY= "<<positionY<<" positionZ= "<<positionZ<<endl;
	//outfile6<<"positionX= " <<positionX<<" positionY= "<<positionY<<" positionZ= "<<positionZ<<endl;
	//outfile6<<"Averaged time steps:"<<averagedStep*2+1<<endl;
	//outfile6<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
	//cout<<"depth"<<" shearRate"<<" SRdeviation"<<endl;
	//outfile6<<"depth"<<" shearRate"<<" SRdeviation"<<endl;
	//for(unsigned i=0;i!=shearRate.size();i++)
	//{
	//	/*cout<<"depth= "<<-positionY-interval*i-interval/2.0<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
	//	outfile6<<"depth= "<<-positionY-interval*i-interval/2.0<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;*/
	//	cout<<positionY-0.5*interval*((i+1)*2-1)<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
	//	outfile6<<positionY-0.5*interval*((i+1)*2-1)<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
	//}

	//cout<<endl;
	//outfile6<<endl;
	////Output averaged velocityX
	//cout<<"Averaged velocityX:"<<endl;
	//outfile6<<"Averaged velocityX:"<<endl;
	//cout<<"depth "<<"AveragedVelX"<<endl;
	//outfile6<<"depth "<<"AveragedVelX"<<endl;

	//for(unsigned i=0;i!=averagedXVel.size();i++)
	//{
	//	cout<<positionY-interval*i<<" "<<averagedXVel[i]<<endl;
	//	outfile6<<positionY-interval*i<<" "<<averagedXVel[i]<<endl;
	//}
/////Shear rate calculation

  
/////KhakharStress/////
	//positionY=-height/2.0;
	////Khakhar stress output
	//	cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
	//	cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
	//	outfile3<<"Averaged time steps:"<<averagedStep*2+1<<endl;
	//	outfile3<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
	//////Khakhar stress output
	//vector<double> volFractions;
	//vector<double> totalNormalStresses;
	//double firstTotalShearStress;
	//////Sphere sphere(0,-5,0,radius);
	//Box box1(length,height,width,0,-depthConstVolFraction,0);
	//StressTimeAverager<VirialStressTensor> stressTimeAveragerBaseVol(filenameVPF,filenameCSV,box1,averagedStep);
	//double baseVolFraction=stressTimeAveragerBaseVol.GetAveragedVolumeFraction();
	//
	///*Box box3(length,height/2.0,width,0,-depthFirstVolFraction,0);
	//StressTimeAverager<VirialStressTensor> stressTimeAveragerFirstVol(filenameVPF,filenameCSV,box3,averagedStep);
	//double firstVolFraction=stressTimeAveragerFirstVol.GetAveragedVolumeFraction();*/

	//for(unsigned i=0;i!=40;i++)
	//{
	/////KhakharStress calculation///
	//
	////Sphere sphere(0,-positionY,0,radius);	
	//
	//Box box2(length,height,width,0,positionY,0);
	////
	//StressTimeAverager<VirialStressTensor> stressTimeAverager(filenameVPF,filenameCSV,box2,averagedStep);

	//double volumeFraction=stressTimeAverager.GetAveragedVolumeFraction();
	//volFractions.push_back(volumeFraction);
	//KhakharStress _KhakharStress(firstTotalShearStress,interval,totalNormalStresses,firstPosition,positionY,volFractions,baseVolFraction,height);
	//double totalNormalStress=_KhakharStress.GetTotalNormalStress();
	//
	//////output KhakharStress
	//cout<<"depth="<<" "<<positionY<<" "<<_KhakharStress.GetTotalNormalStress()<<" "<<_KhakharStress.GetTotalShearStress()<<" "<<"volume fraction= "<<volumeFraction<<" "<<_KhakharStress.GetFrictionForce()<<endl;

	//outfile3<<"depth="<<" "<<positionY<<" "<<_KhakharStress.GetTotalNormalStress()<<" "<<_KhakharStress.GetTotalShearStress()<<" "<<"volume fraction= "<<volumeFraction<<" "<<_KhakharStress.GetFrictionForce()<<endl;
	//positionY-=interval;
	//}
	
	///KhakharStress calculation///

/////KhakharStress/////



/////Strain calculation//////////
	//for(unsigned i=0;i!=18;i++)
	//{
	///*LagrangianStrainTensor lagrangian(sphere,filenameVPF);
	//lagrangian.CalculateStrainTensor();
	//Tensor<T> strainTensor=lagrangian.GetStrainTensor();*/
	//Output strain tensor to screen 
	//cout<<"Lagrangian strain tensor:"<<endl;
	//tensorToScreen(strainTensor);	
 //   Output strain tensor to file "strain.txt" 
	//outfile2<<"Lagrangian strain tensor:"<<endl;
	//tensorToFile(strainTensor,outfile2);
	//}
/////Strain calculation//////////

/////virial stress calculation/////
//
	/////double positionY=1.2;//check whether rotated or not
	//double positionY=-11.09;
	double positionY=-height/2.0;//cal stress
	///////virial stress output to screen 
	cout<<"Averaged steps:"<<averagedStep*2+1<<endl;
	cout<<"virial stress calculation";
	cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
	cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
	cout<<"depth"<<" "<<"depth "<<"PotenNorXXStress"<<" "<<"PotenNorXXStressDeviation"<<" "<<"PotenNorYYStress"<<" "<<"PotenNorYYStressDeviation "<<" "<<"PotenNorZZStress"<<" "<<"PotenShearStress"<<" "<<"PotenShearStressDevitaion"<<" "<<"KineNorXXStress "<<"KineNorYYStress"<<" "<<"KineSheaXYStessDeviation"<<" "<<"Contact number"<<endl;

	outfile<<"Averaged steps:"<<averagedStep*2+1<<endl;
	outfile<<"virial stress calculation";
	outfile<<"Averaged time steps:"<<averagedStep*2+1<<endl;
	outfile<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
	outfile<<"depth"<<" "<<"depth "<<"PotenNorXXStress"<<" "<<"PotenNorXXStressDeviation"<<" "<<"PotenNorYYStress"<<" "<<"PotenNorYYStressDeviation "<<" "<<"PotenNorZZStress"<<" "<<"PotenShearStress"<<" "<<"PotenShearStressDevitaion"<<" "<<"KineNorXXStress "<<"KineNorYYStress"<<" "<<"KineSheaXYStessDeviation"<<" "<<"Contact number"<<endl;
/////////	virial stress infomation output	
	vector<int> interactionNums;
for(unsigned i=0;i!=40;i++)
{
	//Sphere sphere(0,positionY,0,radius);	
	Box box2(length,height,width,0,positionY,0);
/////////////	Box box2(length,height,width,0,-8.0,0);//test
	StressTimeAverager<VirialStressTensor> stressTimeAverager(VPFfileName2,CSVfileName2,box2,averagedStep);
	Tensor<T> averagedPotentialTensor=stressTimeAverager.GetAveragedPotentialStressTensor();
	Tensor<T> averagedKineticTensor=stressTimeAverager.GetAveragedKineticStressTensor();
	Tensor<T> averagedTotalStressTensor=stressTimeAverager.GetAveragedTotalStressTensor();	
	//get interaction number
	interactionNums=stressTimeAverager.GetInteractionNums();
	//cout<<"InteractionNum:"<<interactionNums[0]<<endl;
///////////////////////////Output virial stress Data/////////////////////////////
//////Output stress to screen: only output stress11 stress12
	cout<<"depth="<<" "<<positionY<<" "<<averagedPotentialTensor[0][0]<<" "<<stressTimeAverager.GetNorXXStressDevi()<<" "<<averagedPotentialTensor[1][1]<<" "<<stressTimeAverager.GetNorYYStressDevi()<<" "<<averagedPotentialTensor[2][2]<<" "<<averagedPotentialTensor[0][1]<<" "<<stressTimeAverager.GetSheaXYStressDevi()<<" "<<averagedKineticTensor[0][0]<<" "<<averagedKineticTensor[1][1]<<" "<<averagedKineticTensor[0][1]<<" "<<interactionNums[0]<<endl;
///////////Output stress to file: only output stress11 stress12
	outfile<<"depth="<<" "<<positionY<<" "<<averagedPotentialTensor[0][0]<<" "<<stressTimeAverager.GetNorXXStressDevi()<<" "<<averagedPotentialTensor[1][1]<<" "<<stressTimeAverager.GetNorYYStressDevi()<<" "<<averagedPotentialTensor[2][2]<<" "<<averagedPotentialTensor[0][1]<<" "<<stressTimeAverager.GetSheaXYStressDevi()<<" "<<averagedKineticTensor[0][0]<<" "<<averagedKineticTensor[1][1]<<" "<<averagedKineticTensor[0][1]<<" "<<interactionNums[0]<<endl;
	positionY=positionY-interval;
}
//Output stress to screen:Tensor form
	/*cout<<"depth="<<positionY-interval<<endl;
	cout<<"Averaged potential tensor:"<<endl;
	tensorToScreen(averagedPotentialTensor);

	cout<<"Averaged kinetic tensor:"<<endl;
	tensorToScreen(averagedKineticTensor);

	cout<<"Total stress tensor:"<<endl;
	tensorToScreen(averagedTotalStressTensor);*/

//Output stress to file:Tensor form
	
	///*outfile<<"depth="<<positionY-interval<<endl;
	//outfile<<"Averaged potential stress tensor:"<<endl;
	//tensorToFile(averagedPotentialTensor,outfile);

	//outfile<<"Averaged kinetic stress tensor:"<<endl;
	//	tensorToFile(averagedKineticTensor,outfile);

	//outfile<<"Total stress tensor:"<<endl;
	//tensorToFile(averagedTotalStressTensor,outfile);*/


	return 0;
}


void tensorToFile(Tensor<T>& tensor,fstream& offile)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		{
			for(unsigned beta = 0; beta != 3; ++beta)
			{
				offile << tensor[alpha][beta] << " ";
			}
			offile << endl;
		}
		offile << endl;
}

void tensorToScreen(Tensor<T>& tensor)
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		{
			for(unsigned beta = 0; beta != 3; ++beta)
			{
				cout << tensor[alpha][beta] << " ";
			}
			cout << endl;
		}
		cout << endl;
}




/////////////////////////////////////Horizontal sampling//////////////////////////////////////////////////////

//int main(int argc, char* argv[])
//{
///////////////////////////// Parse input file name ///////////////////////////
//	string filenameVPF(argv[1]);
//	string filenameCSV(argv[2]);
//	string _angle(argv[3]);
//
//	istringstream convert(_angle);
//	double angle;
//	convert>>angle;
//	
//
//	//pattern_001212.vpf interactions_001212.csv field_110000.dat 
//	//Sphere sphere(2,-0.00605995,0,10);
//	//ibmtest_000040.vpf interactions_000040.csv field_110000.dat  Sphere sphere(125,25,160,14);
//	//ibmtest_000001.vpf interactions_000001.csv field_110000.dat
//	//three particle case:verify strain code:threeParticle_000007.vpf threeParticleInteraction_000007.csv field_110000.dat
//	//four particle case:verify strain code:four_000007.vpf FourInteraction.csv field_110000.dat Sphere sphere(0,0,0,10)
//	//Rotating Drum steady state:rd_004000.vpf RDinteraction_004000.csv field_110000.dat Sphere sphere(0,-12.5,0,2.5)
//	
///////Sphere parameters
//	double radius=2.5;
//	double depth=16.9;
//	//double positionY=radius;
//
///////Control volume parameters
//	double diameter=0.476;
//	double length=15*diameter;
//	double width=10*diameter;
//	double height=5*diameter;
//	double interval=0.5;
//	
//	
//	double firstPosition=height/2.0;
//	double depthConstVolFraction=6;	//Khakhar stress
//	double depthFirstVolFraction=height/4.0;//Khakhar stress
//	
//	int averagedStep=0;//number of time steps=averagedStep*2+1
//////Control volume parameters
//
//	string frame=filenameVPF.substr(3,6);
//	string strain="strain";
//	string stress="stress";
//	string kstress="kstress";
//	string shearRate_="shearRate";
//	string bottom="_";
//	string txt=".txt";
//
//	//ofstream outfile2(strain+bottom+frame+txt);
//	ofstream outfile(stress+bottom+frame+txt);
//	ofstream outfile3(kstress+bottom+frame+txt);
//	ofstream outfile6(shearRate_+bottom+frame+txt);
//	
//
///////Refine input data
//	
//	//input file name
//	string VPFfileName=filenameVPF;
//	string CSVfileName=filenameCSV;
//	//output file name
//	string VPFfileName2=filenameVPF.replace(2,1,"2_");
//	string CSVfileName2=filenameCSV.replace(11,1,"2");
//	CSVfileName2.replace(0,1,"RDi");
//	
//
//	string number[11]={"1","2","3","4","5","6","7","8","9","10","11"};
//	for(unsigned i=0;i!=1;i++)
//	{
//	//VPFfileName2.replace(9,1,number[i]);
//	//CSVfileName2.replace(20,1,number[i]);
//	//VPFfileName.replace(8,1,number[i]);
//	//CSVfileName.replace(18,1,number[i]);
//
//	ofstream* outfile4=new ofstream(VPFfileName2);
//	if(!*outfile4)
//	{
//	cerr<<"Can't open file"<<endl;
//	}
//	ofstream* outfile5=new ofstream(CSVfileName2);
//	if(!*outfile5)
//	{
//	cerr<<"Can't open file"<<endl;
//	}
//	std::string unrefinedCSV(CSVfileName);
//	std::string unrefinedVPF(VPFfileName);
//	double length2=71*diameter;
//	double width2=11*diameter;
//	double height2=6*diameter;
//	double positionY2=height2/2.0;
//	Box box2(length2,height2,width2,0,-2.375,0);
//	ParticleFluidProcessor<VirialStressTensor> processor;		
//	processor.SetCSVFileName(unrefinedCSV.c_str());
//	processor.SetVPFFileName(unrefinedVPF.c_str());
//	processor.ReadFile();
//	processor.Initialize();
//	processor.refineInputData(outfile4,outfile5,box2,angle);
//	outfile4->close();
//	outfile5->close();
//	}
///////Refine input data
//
///////Shear rate calculation
//	//double positionX=0;
//	//double positionY=-height/2.0;//cal shear rate
//	//double positionZ=0;
//
//	//Box box(length,height,width,positionX,positionY,positionZ);
//	////Box box(30,2,7.2,0,1.0,0);//test rotation
//	//unsigned numStep=50;
//	//ShearRateTimeAverager shearRateTimeAverager(filenameVPF,box,interval,numStep,averagedStep);
//	//vector<double> shearRate=shearRateTimeAverager.GetAveragedShearRates();
//	//vector<double> shearRateDevi=shearRateTimeAverager.GetShearRatesDevi();
//	//vector<double> averagedXVel=shearRateTimeAverager.GetTimeAveragedXVel();
//	//cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	//cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	//cout<<"positionX= " <<positionX<<" positionY= "<<positionY<<" positionZ= "<<positionZ<<endl;
//	//outfile6<<"positionX= " <<positionX<<" positionY= "<<positionY<<" positionZ= "<<positionZ<<endl;
//	//outfile6<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	//outfile6<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	//cout<<"depth"<<" shearRate"<<" SRdeviation"<<endl;
//	//outfile6<<"depth"<<" shearRate"<<" SRdeviation"<<endl;
//	//for(unsigned i=0;i!=shearRate.size();i++)
//	//{
//	//	/*cout<<"depth= "<<-positionY-interval*i-interval/2.0<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
//	//	outfile6<<"depth= "<<-positionY-interval*i-interval/2.0<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;*/
//	//	cout<<positionY-2*interval-interval*i<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
//	//	outfile6<<positionY-2*interval-interval*i<<" "<<shearRate[i]<<" "<<shearRateDevi[i]<<endl;
//	//}
//
//	//cout<<endl;
//	//outfile6<<endl;
//	////Output averaged velocityX
//	//cout<<"Averaged velocityX:"<<endl;
//	//outfile6<<"Averaged velocityX:"<<endl;
//	//cout<<"depth "<<"AveragedVelX"<<endl;
//	//outfile6<<"depth "<<"AveragedVelX"<<endl;
//
//	//for(unsigned i=0;i!=averagedXVel.size();i++)
//	//{
//	//	cout<<positionY-interval*i<<" "<<averagedXVel[i]<<endl;
//	//	outfile6<<positionY-interval*i<<" "<<averagedXVel[i]<<endl;
//	//}
///////Shear rate calculation
//
//  
///////KhakharStress/////
//	//positionY=-height/2.0;
//	////Khakhar stress output
//	//	cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	//	cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	//	outfile3<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	//	outfile3<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	//////Khakhar stress output
//	//vector<double> volFractions;
//	//vector<double> totalNormalStresses;
//	//double firstTotalShearStress;
//	//////Sphere sphere(0,-5,0,radius);
//	//Box box1(length,height,width,0,-depthConstVolFraction,0);
//	//StressTimeAverager<VirialStressTensor> stressTimeAveragerBaseVol(filenameVPF,filenameCSV,box1,averagedStep);
//	//double baseVolFraction=stressTimeAveragerBaseVol.GetAveragedVolumeFraction();
//	//
//	///*Box box3(length,height/2.0,width,0,-depthFirstVolFraction,0);
//	//StressTimeAverager<VirialStressTensor> stressTimeAveragerFirstVol(filenameVPF,filenameCSV,box3,averagedStep);
//	//double firstVolFraction=stressTimeAveragerFirstVol.GetAveragedVolumeFraction();*/
//
//	//for(unsigned i=0;i!=32;i++)
//	//{
//	/////KhakharStress calculation///
//	//
//	////Sphere sphere(0,-positionY,0,radius);	
//	//
//	//Box box2(length,height,width,0,positionY,0);
//	////
//	//StressTimeAverager<VirialStressTensor> stressTimeAverager(filenameVPF,filenameCSV,box2,averagedStep);
//
//	//double volumeFraction=stressTimeAverager.GetAveragedVolumeFraction();
//	//volFractions.push_back(volumeFraction);
//	//KhakharStress _KhakharStress(firstTotalShearStress,interval,totalNormalStresses,firstPosition,positionY,volFractions,baseVolFraction,height);
//	//double totalNormalStress=_KhakharStress.GetTotalNormalStress();
//	//
//	//////output KhakharStress
//	//cout<<"depth="<<" "<<positionY<<" "<<_KhakharStress.GetTotalNormalStress()<<" "<<_KhakharStress.GetTotalShearStress()<<" "<<"volume fraction= "<<volumeFraction<<" "<<_KhakharStress.GetFrictionForce()<<endl;
//
//	//outfile3<<"depth="<<" "<<positionY<<" "<<_KhakharStress.GetTotalNormalStress()<<" "<<_KhakharStress.GetTotalShearStress()<<" "<<"volume fraction= "<<volumeFraction<<" "<<_KhakharStress.GetFrictionForce()<<endl;
//	//positionY-=interval;
//	//}
//	
//	///KhakharStress calculation///
//
///////KhakharStress/////
//
//
//
///////Strain calculation//////////
//	//for(unsigned i=0;i!=18;i++)
//	//{
//	///*LagrangianStrainTensor lagrangian(sphere,filenameVPF);
//	//lagrangian.CalculateStrainTensor();
//	//Tensor<T> strainTensor=lagrangian.GetStrainTensor();*/
//	//Output strain tensor to screen 
//	//cout<<"Lagrangian strain tensor:"<<endl;
//	//tensorToScreen(strainTensor);	
// //   Output strain tensor to file "strain.txt" 
//	//outfile2<<"Lagrangian strain tensor:"<<endl;
//	//tensorToFile(strainTensor,outfile2);
//	//}
///////Strain calculation//////////
//
///////virial stress calculation/////
////
//	///double positionY=1.2;//check whether rotated or not
//	double positionY=-2.375;//cal stress
//	double positionX=-12.5;
//	///////virial stress output to screen 
//	cout<<"Averaged steps:"<<averagedStep*2+1<<endl;
//	cout<<"virial stress calculation";
//	cout<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	cout<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	cout<<"depth"<<" "<<"depth "<<"PotenNorStress"<<" "<<"PotenNorStress deviation"<<" "<<" "<<" "<<"PotenShearStress"<<" "<<"PotenShearStress devitaion"<<" "<<"KineNorStress"<<" "<<"KineSheaStess deviation"<<endl;
//
//	outfile<<"Averaged steps:"<<averagedStep*2+1<<endl;
//	outfile<<"virial stress calculation";
//	outfile<<"Averaged time steps:"<<averagedStep*2+1<<endl;
//	outfile<<"Control vol: "<<"length= " <<length<<" width= "<<width<<" height= "<<height<<endl;
//	outfile<<"depth"<<" "<<"depth "<<"PotenNorXXStress"<<" "<<"PotenNorXXStressDeviation"<<" "<<"PotenNorYYStress"<<" "<<"PotenNorYYStressDeviation "<<" "<<"PotenNorZZStress"<<" "<<"PotenShearStress"<<" "<<"PotenShearStressDevitaion"<<" "<<"KineNorXXStress "<<"KineNorYYStress"<<" "<<"KineSheaXYStessDeviation"<<endl;
///////////	virial stress infomation output	
//
//for(unsigned i=0;i!=51;i++)
//{
//	//Sphere sphere(0,positionY,0,radius);	
//	Box box2(length,height,width,positionX,positionY,0);
///////////////	Box box2(length,height,width,0,-8.0,0);//test
//	StressTimeAverager<VirialStressTensor> stressTimeAverager(VPFfileName2,CSVfileName2,box2,averagedStep);
//	Tensor<T> averagedPotentialTensor=stressTimeAverager.GetAveragedPotentialStressTensor();
//	Tensor<T> averagedKineticTensor=stressTimeAverager.GetAveragedKineticStressTensor();
//	Tensor<T> averagedTotalStressTensor=stressTimeAverager.GetAveragedTotalStressTensor();	
//
/////////////////////////////Output virial stress Data/////////////////////////////
////////Output stress to screen: only output stress11 stress12
//	cout<<"x="<<" "<<positionX<<" "<<averagedPotentialTensor[0][0]<<" "<<stressTimeAverager.GetNorXXStressDevi()<<" "<<averagedPotentialTensor[1][1]<<" "<<stressTimeAverager.GetNorYYStressDevi()<<" "<<averagedPotentialTensor[2][2]<<" "<<averagedPotentialTensor[0][1]<<" "<<stressTimeAverager.GetSheaXYStressDevi()<<" "<<averagedKineticTensor[0][0]<<" "<<averagedKineticTensor[1][1]<<" "<<averagedKineticTensor[0][1]<<endl;
/////////////Output stress to file: only output stress11 stress12
//	outfile<<"x="<<" "<<positionX<<" "<<averagedPotentialTensor[0][0]<<" "<<stressTimeAverager.GetNorXXStressDevi()<<" "<<averagedPotentialTensor[1][1]<<" "<<stressTimeAverager.GetNorYYStressDevi()<<" "<<averagedPotentialTensor[2][2]<<" "<<averagedPotentialTensor[0][1]<<" "<<stressTimeAverager.GetSheaXYStressDevi()<<" "<<averagedKineticTensor[0][0]<<" "<<averagedKineticTensor[1][1]<<" "<<averagedKineticTensor[0][1]<<endl;
//	//positionY=positionY-interval;
//	positionX=positionX+interval;
//}
////Output stress to screen:Tensor form
//	/*cout<<"depth="<<positionY-interval<<endl;
//	cout<<"Averaged potential tensor:"<<endl;
//	tensorToScreen(averagedPotentialTensor);
//
//	cout<<"Averaged kinetic tensor:"<<endl;
//	tensorToScreen(averagedKineticTensor);
//
//	cout<<"Total stress tensor:"<<endl;
//	tensorToScreen(averagedTotalStressTensor);*/
//
////Output stress to file:Tensor form
//	
//	///*outfile<<"depth="<<positionY-interval<<endl;
//	//outfile<<"Averaged potential stress tensor:"<<endl;
//	//tensorToFile(averagedPotentialTensor,outfile);
//
//	//outfile<<"Averaged kinetic stress tensor:"<<endl;
//	//	tensorToFile(averagedKineticTensor,outfile);
//
//	//outfile<<"Total stress tensor:"<<endl;
//	//tensorToFile(averagedTotalStressTensor,outfile);*/
//
//
//	return 0;
//}
//
//
//void tensorToFile(Tensor<T>& tensor,fstream& offile)
//{
//	for(unsigned alpha = 0; alpha != 3; ++alpha)
//		{
//			for(unsigned beta = 0; beta != 3; ++beta)
//			{
//				offile << tensor[alpha][beta] << " ";
//			}
//			offile << endl;
//		}
//		offile << endl;
//}
//
//void tensorToScreen(Tensor<T>& tensor)
//{
//	for(unsigned alpha = 0; alpha != 3; ++alpha)
//		{
//			for(unsigned beta = 0; beta != 3; ++beta)
//			{
//				cout << tensor[alpha][beta] << " ";
//			}
//			cout << endl;
//		}
//		cout << endl;
//}