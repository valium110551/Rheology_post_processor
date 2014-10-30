#ifndef _EFFECTIVE_STRESS_CALCULATOR_HPP_
#define _EFFECTIVE_STRESS_CALCULATOR_HPP_

#include <iterator>
#include <algorithm>
#include <cmath>

#include "EffectiveStressCalculator.h"
#include "VirialStressTensor.h"

using namespace std;

//#include "Geometry/PolygonHelper.h"

template<class ImplementType>
ParticleFluidProcessor<ImplementType>::ParticleFluidProcessor()
{
	count=0;
	particleMass=0.077928904;
	particleVolume=0.05647022;
	timeInterval=1e-6;
	//particleMass=13000;
	//initialize _kineticStressTensor
	for(unsigned alpha = 0; alpha != 3; ++alpha)
	{
		for(unsigned beta = 0; beta != 3; ++beta)
		{
			_kineticStressTensor[alpha][beta] = 0;
		}
	}
	//initialize _potentialStressTensor
	for(unsigned alpha = 0; alpha != 3; ++alpha)
	{
		for(unsigned beta = 0; beta != 3; ++beta)
		{
			_potentialStressTensor[alpha][beta] = 0;
		}
	}
	

}

template<class ImplementType>
ParticleFluidProcessor<ImplementType>::~ParticleFluidProcessor()
{

}

/*
template<class ImplementType>
void EffectiveStressCalculator<ImplementType>::DefineControlVolume(Box3D<int> box3D)
{
	this->_controlVolume = (double)box3D.Volume();

	for(int index = 0; index != this->_vpfReader.GetSpheres().size(); ++index)
	{
		ImmersedSphere<double>& sphere_ = this->_vpfReader.GetSpheres()[index];

		if(sphere_.getRadius() < 0)
			continue;

		if(box3D.Intersect< ImmersedSphere<double> >(sphere_))
			this->_relativeSpheres[(unsigned)index] = sphere_;
	}
	
	using namespace std;

	map<SphereIndex, ImmersedSphere<double> >::iterator npIter;
	map<SphereIndex, ImmersedSphere<double> >::iterator npBegin = _relativeSpheres.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator npEnd = _relativeSpheres.end();

	unsigned InteractionsNumber = this->_csvReader.GetInteractions().size();
	SphereIndex sphereEnd = this->_vpfReader.GetSpheres().size();

	BranchVector rij = BranchVector();

	for(int interactionIntex = 0; interactionIntex != InteractionsNumber; ++interactionIntex)
	{
		InteractionData& interaction = this->_csvReader.GetInteractions()[interactionIntex];
		
		SphereIndex masterID = interaction.MasterID();
		SphereIndex slaveID = interaction.SlaveID();
		
		npIter = _relativeSpheres.find(masterID);
		if(npIter != npEnd)
		{
			rij = this->_vpfReader.GetSpheres()[slaveID].getPosition() - (*npIter).second.getPosition();
			_relativeInteractions.push_back( make_pair(interaction, rij) );
		}
	}	
}
*/

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::refineInputData(std::ofstream* outfile1 ,std::ofstream* outfile2,_ahedron& ahedron,double ro_angle)
{
	for(int index = 0; index != this->_vpfReader.GetSpheres().size(); ++index)
	{
		double _x=this->_vpfReader.GetSpheres()[index].getPosition().x();
		double _y=this->_vpfReader.GetSpheres()[index].getPosition().y();
		double _z=this->_vpfReader.GetSpheres()[index].getPosition().z();

		double reposeAngle=ro_angle/180*3.14159265;
		double _x_ro=_x*cos(reposeAngle)-_y*sin(reposeAngle);
		double _y_ro=_x*sin(reposeAngle)+_y*cos(reposeAngle);

		this->_vpfReader.GetSpheres()[index].setPosition(_x_ro,_y_ro,_z);

		double _Vx=this->_vpfReader.GetSpheres()[index].getVelocity().x();
		double _Vy=this->_vpfReader.GetSpheres()[index].getVelocity().y();
		double _Vz=this->_vpfReader.GetSpheres()[index].getVelocity().z();

		double _Vx_ro=_Vx*cos(reposeAngle)-_Vy*sin(reposeAngle);
		double _Vy_ro=_Vx*sin(reposeAngle)+_Vy*cos(reposeAngle);
		
		this->_vpfReader.GetSpheres()[index].setVelocity(_Vx_ro,_Vy_ro,_Vz);

	}

	for(int index = 0; index != this->_csvReader.GetInteractions().size(); ++index)
	{
		double reposeAngle=ro_angle/180*3.14159265;

		//Rotate force
		double _Fx=this->_csvReader.GetInteractions()[index].GetInteractiveForce().x();
		double _Fy=this->_csvReader.GetInteractions()[index].GetInteractiveForce().y();
		double _Fz=this->_csvReader.GetInteractions()[index].GetInteractiveForce().z();

		double _Fx_ro=_Fx*cos(reposeAngle)-_Fy*sin(reposeAngle);
		double _Fy_ro=_Fx*sin(reposeAngle)+_Fy*cos(reposeAngle);

		this->_csvReader.GetInteractions()[index].setInterForce(_Fx_ro,_Fy_ro,_Fz);

		//Rotate impact direction
		double _Dx=this->_csvReader.GetInteractions()[index].GetImpactDirection().x();
		double _Dy=this->_csvReader.GetInteractions()[index].GetImpactDirection().y();
		double _Dz=this->_csvReader.GetInteractions()[index].GetImpactDirection().z();

		double _Dx_ro=_Dx*cos(reposeAngle)-_Dy*sin(reposeAngle);
		double _Dy_ro=_Dx*sin(reposeAngle)+_Dy*cos(reposeAngle);

		this->_csvReader.GetInteractions()[index].setImpactDirection(_Dx_ro,_Dy_ro,_Dz);
	}

	/*for(int index = 0; index != this->_csvReader.GetInteractions.size(); ++index)
	{
		double reposeAngle=ro_angle/180*3.14159265;

		double _Fx=this->_csvReader.GetInteractions()[index].GetInteractiveForce().x();
		double _Fy=this->_csvReader.GetInteractions()[index].GetInteractiveForce().y();
		double _Fz=this->_csvReader.GetInteractions()[index].GetInteractiveForce().z();

		double _Fx_ro=_Fx*cos(reposeAngle)-_Fy*sin(reposeAngle);
		double _Fy_ro=_Fx*sin(reposeAngle)+_Fy*cos(reposeAngle);

		this->_csvReader.GetInteractions()[index].setInterForce(_Fx_ro,_Fy_ro,_Fz);
	}*/

	for(int index = 0; index != this->_vpfReader.GetSpheres().size(); ++index)
	{
		ImmersedSphere<double>& sphere_ = this->_vpfReader.GetSpheres()[index];

		if(sphere_.getRadius() < 0)
			continue;

		if(ahedron.Intersect(sphere_))
		{
			this->_relativeSpheres[(SphereIndex)index] = sphere_;
			this->_relativeSphereInteractions[(SphereIndex)index] = std::vector<InteractionData>();
		}
	}
	
	map<SphereIndex, ImmersedSphere<double> >::iterator npIter3;
	map<SphereIndex, ImmersedSphere<double> >::iterator npBegin3 = _relativeSpheres.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator npEnd3 = _relativeSpheres.end();
	///Output relative particles
	for(npIter3=npBegin3;npIter3!=npEnd3;npIter3++)
	{
		*outfile1<<"sphere"<<" "<<npIter3->first<<" "<<npIter3->second.getPosition().x()<<" "<<npIter3->second.getPosition().y()<<" "<<npIter3->second.getPosition().z()<<" "
			<<npIter3->second.getVelocity().x()<<" "<<npIter3->second.getVelocity().y()<<" "<<npIter3->second.getVelocity().z()<<" "<<"blank"<<" "<<"blank"<<" "<<"blank"<<" "<<npIter3->second.getRadius()<<endl;
	}

	map<SphereIndex, ImmersedSphere<double> >::iterator npIter;
	map<SphereIndex, ImmersedSphere<double> >::iterator npBegin = _relativeSpheres.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator npEnd = _relativeSpheres.end();
	map<SphereIndex, ImmersedSphere<double> >::iterator npIter2;



	unsigned InteractionsNumber = this->_csvReader.GetInteractions().size();
	SphereIndex sphereEnd = this->_vpfReader.GetSpheres().size();

	BranchVector rij = BranchVector();

	for(int interactionIntex = 0; interactionIntex != InteractionsNumber; ++interactionIntex)
	{
		InteractionData& interaction = this->_csvReader.GetInteractions()[interactionIntex];

		SphereIndex masterID = interaction.MasterID();
		SphereIndex slaveID = interaction.SlaveID();

		npIter = _relativeSpheres.find(masterID);
		npIter2 = _relativeSpheres.find(slaveID);
		//In virial stress, if the bond is not fully within the control volume, the interaction is not counted
		bool checkBoundardySphere=ImplementType::boundaryInteractionHandle(interaction,ahedron,_relativeSpheres);

		if((npIter != npEnd || npIter2!=npEnd) && checkBoundardySphere)
		{
			_relativeSphereInteractions[masterID].push_back(interaction);
		}
	}	
	std::map<SphereIndex, std::vector<InteractionData> >::iterator iter4;
	std::map<SphereIndex, std::vector<InteractionData> >::iterator iterBegin4=_relativeSphereInteractions.begin();
	std::map<SphereIndex, std::vector<InteractionData> >::iterator iterEnd4=_relativeSphereInteractions.end();
	///Output relative interactions
	for(iter4=iterBegin4;iter4!=iterEnd4;iter4++)
	{
		unsigned _size=iter4->second.size();
		for(unsigned i=0;i!=_size;i++)
		{
			*outfile2<<iter4->second[i].MasterID()<<", "<<iter4->second[i].SlaveID()<<", "<<iter4->second[i].GetInteractiveForce().x()<<", "
				<<iter4->second[i].GetInteractiveForce().y()<<", "<<iter4->second[i].GetInteractiveForce().z()<<", "<<-(iter4->second[i].GetInteractiveForce().x())
				<<", "<<-(iter4->second[i].GetInteractiveForce().y())<<", "<<-(iter4->second[i].GetInteractiveForce().z())<<", "<<" "<<", "<<" "<<", "<<" "<<", "<<" "<<", "<<" "<<", "<<" "<<", "<<" "<<", "<<iter4->second[i].GetImpactDirection().x()<<", "
				<<iter4->second[i].GetImpactDirection().y()<<", "<<iter4->second[i].GetImpactDirection().z()<<", "<<iter4->second[i].GetContactPoint().x()<<", "<<iter4->second[i].GetContactPoint().y()<<", "
				<<iter4->second[i].GetContactPoint().z()<<", "<<endl;
		}
	}


}

template<class ImplementType>
//void ParticleFluidProcessor<ImplementType>::DefineControlVolume(Box3D<int>& box3D)
void ParticleFluidProcessor<ImplementType>::DefineControlVolume(_ahedron& ahedron)
{
	this->_controlVolume = (double)ahedron.Volume();

	for(int index = 0; index != this->_vpfRefinedReader.GetSpheres().size(); ++index)
	{
		ImmersedSphere<double>& sphere_ = this->_vpfRefinedReader.GetSpheres()[index];

		if(sphere_.getRadius() < 0)
			continue;

		if(ahedron.Intersect(sphere_))
		{
			unsigned index2=sphere_.getSphereIndex();
			this->_relativeSpheres[(SphereIndex)index2] = sphere_;
			//first, give each particle an empty vetor ,than add interaction info into it 
			this->_relativeSphereInteractions[(SphereIndex)index2] = std::vector<InteractionData>();
		}
	}
	//calculate the velocity of mass center
	calculateCMvelocity();

	using namespace std;

	map<SphereIndex, ImmersedSphere<double> >::iterator npIter;
	map<SphereIndex, ImmersedSphere<double> >::iterator npBegin = _relativeSpheres.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator npEnd = _relativeSpheres.end();
	map<SphereIndex, ImmersedSphere<double> >::iterator npIter2;

	unsigned InteractionsNumber = this->_csvRefinedReader.GetInteractions().size();
	SphereIndex sphereEnd = this->_vpfRefinedReader.GetSpheres().size();

	BranchVector rij = BranchVector();

	for(int interactionIntex = 0; interactionIntex != InteractionsNumber; ++interactionIntex)
	{
		InteractionData& interaction = this->_csvRefinedReader.GetInteractions()[interactionIntex];

		SphereIndex masterID = interaction.MasterID();
		SphereIndex slaveID = interaction.SlaveID();

		npIter = _relativeSpheres.find(masterID);
		npIter2 = _relativeSpheres.find(slaveID);
		//In virial stress, if the bond is not fully within the control volume, the interaction is not counted
		bool checkBoundardySphere=ImplementType::boundaryInteractionHandle(interaction,ahedron,_relativeSpheres);

		if((npIter != npEnd || npIter2!=npEnd) && checkBoundardySphere)
		{
			_relativeSphereInteractions[masterID].push_back(interaction);
			/*rij = interaction.GetContactPoint()  - npIter->second.getPosition();
			T abs = rij.Abs();
			rij = interaction.GetImpactDirection() * abs;

			_relativeInteractions.push_back( make_pair(interaction, rij) );*/
		}
	}	
}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::Initialize()
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			this->_potentialStressTensor[alpha][beta] = 0.;

	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			this->_kineticStressTensor[alpha][beta] = 0.;

	this->_relativeInteractions.clear();
	this->_relativeSpheres.clear();
}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::potentialPartUpdate(const T* force, const T* rij)
{
	ImplementType::potentialPartUpdate(this->__potentialStressTensor,timeInterval,force, rij);
	//ImplementType::kineticPartUpdate(this->_potentialStressTensor, force, rij);
}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::kineticPartUpdate(const double particleMass, const sMathLib::VectorX<double, 3> velocity ,const sMathLib::VectorX<double, 3> CMvelocity)
{
	ImplementType::kineticPartUpdate(this->_kineticStressTensor, particleMass, velocity, CMvelocity);
}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::potentialPartUpdate(Force& force, BranchVector& rij)
{
	ImplementType::potentialPartUpdate(this->_potentialStressTensor,timeInterval,force, rij);
}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::CalculateStressTensor()
{
	using namespace std;

	SphereIndex SphereNumber = this->_relativeSpheres.size();

	map<SphereIndex, ImmersedSphere<double> >::iterator npIter;
	map<SphereIndex, ImmersedSphere<double> >::iterator npBegin = _relativeSpheres.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator npEnd = _relativeSpheres.end();

	for(npIter = npBegin; npIter != npEnd; ++npIter)
	{
		SphereIndex spherID = npIter->first;

		double sphereVolume = npIter->second.getVolume();

		InteractionIndex interactionSize = this->_relativeSphereInteractions[spherID].size();
		//Get parameters that "kineticPartUpdate need" and pass into kineticPartUpdate
		this->kineticPartUpdate(particleMass,npIter->second.getVelocity(),CMvelocity);

		//sMathLib::VectorX<double, 3> kk=npIter->second.getVelocity();
	
		//Get parameters that "potentialPartUpdate need" and pass into potentialPartUpdate
		for(InteractionIndex index = 0; index != interactionSize; ++index)
		{
			//Vector3 contactPoint = this->_relativeSphereInteractions[spherID][index].GetContactPoint();
			//Get the other sphere's position
			unsigned theOtherSphereIndex;
			if(spherID==this->_relativeSphereInteractions[spherID][index].SlaveID())
			{
				count++;
				theOtherSphereIndex=this->_relativeSphereInteractions[spherID][index].MasterID();
			}
			else
			{		
				theOtherSphereIndex=this->_relativeSphereInteractions[spherID][index].SlaveID();
			}
			Vector3 theOtherSpherePosition = this->_relativeSpheres[theOtherSphereIndex].getPosition();

			Vector3 impactDirection = this->_relativeSphereInteractions[spherID][index].GetImpactDirection();
			
			//Vector3 rij = contactPoint -  npIter->second.getPosition();
			Vector3 rij = theOtherSpherePosition -  npIter->second.getPosition();
			Force force = this->_relativeSphereInteractions[spherID][index].GetInteractiveForce();
			rij = impactDirection * rij.Abs();
			double check=rij.Abs();
			/*if(check==0)
			{
				count++;
			}*/

			this->potentialPartUpdate(force, rij);
		}
		
	}

}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::CalculateVericalStress()
{

}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::Finalize()
{
	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			this->_potentialStressTensor[alpha][beta] /= this->_controlVolume;

	for(unsigned alpha = 0; alpha != 3; ++alpha)
		for(unsigned beta = 0; beta != 3; ++beta)
			this->_kineticStressTensor[alpha][beta] /= this->_controlVolume;
}

template<class ImplementType>
sMathLib::Tensor<T> ParticleFluidProcessor<ImplementType>::GetPotentialStressTensor()
{
	return sMathLib::Tensor<T>(this->_potentialStressTensor);	
}

template<class ImplementType> 
sMathLib::Tensor<T> ParticleFluidProcessor<ImplementType>::GetKineticStressTensor()
{
	return sMathLib::Tensor<T>(this->_kineticStressTensor);	
}

template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::DrawVolumeFractionFunction()
{
	using namespace sMathLib;

	SphereIndex npNumber = this->_vpfReader.GetSpheres().size();

	VectorX<int, 3> fDomain;

	fDomain[0] = this->_fluidField->getNx();
	fDomain[1] = this->_fluidField->getNy();
	fDomain[2] = this->_fluidField->getNz();
	
	for(SphereIndex np = 0; np != npNumber; ++np)
	{
		ImmersedSphere<double>& sphere = this->_vpfReader.GetSpheres()[np];

		if(sphere.getRadius() < 0)
			continue;
		
		Vector3 bPosition = sphere.getPosition();

		int r_extend = (int)(sphere.getRadius() +0.5) + 1;
		int d_extend = 2 * r_extend;

		int x0 = std::max((int)bPosition.x() - r_extend, 0);
		int x1 = std::min(x0 + d_extend, fDomain.x() - 1);

		int y0 = std::max((int)bPosition.y() - r_extend, 0);
		int y1 = std::min(y0 + d_extend, fDomain.y() - 1);

		int z0 = std::max((int)bPosition.z() - r_extend, 0);
		int z1 = std::min(z0 + d_extend, fDomain.z() - 1);

		for(int iX = x0; iX != x1; ++iX)
		{
			for(int iY = y0; iY != y1; ++iY)
			{
				for(int iZ = z0; iZ != z1; ++iZ)
				{
					bool tmpY = iY >= (int)(bPosition.y() - r_extend) && iY <= (int)(bPosition.y() + r_extend);
					if(!tmpY)
						continue;

					double dx = (double)iX - bPosition.x();
					double dy = (double)iY - bPosition.y();
					double dz = (double)iZ - bPosition.z();

					double r2 = dx * dx + dy * dy + dz * dz;
					double r = sqrt(r2);

					double R = sphere.getRadius();

					double dist = r - R;

					double drift = -0.5;

					double phi = dist < -drift ? 1.0 : (dist < drift ? 1.-(dist + drift)/(drift + drift) : 0.0);

					this->_fluidField->get(iX, iZ).Vff[np] = phi;

				}
			}
		}
		
	}

	/*
	for(SphereIndex np = 0; np != npNumber; ++np)
	{
		ImmersedSphere<double>& sphere = this->_vpfReader.GetSpheres()[np];

		if(sphere.getRadius() < 0)
			continue;

		Vector3 bPosition = sphere.getPosition();

		int r_extend = (int)(sphere.getRadius() +0.5) + 1;
		int d_extend = 2 * r_extend;

		int x0 = std::max((int)bPosition.x() - r_extend, 0);
		int x1 = std::min(x0 + d_extend, fDomain.x() - 1);

		int y0 = std::max((int)bPosition.y() - r_extend, 0);
		int y1 = std::min(y0 + d_extend, fDomain.y() - 1);

		int z0 = std::max((int)bPosition.z() - r_extend, 0);
		int z1 = std::min(z0 + d_extend, fDomain.z() - 1);

		for(int iX = x0; iX != x1; ++iX)
		{
			for(int iY = y0; iY != y1; ++iY)
			{
				for(int iZ = z0; iZ != z1; ++iZ)
				{
					double phi = this->_fluidField->get(iX, iZ).Vff[np]


				}
			}
		}
	}
	*/

	std::ofstream test("vff.dat");
	test << "variables = X, Z, u, v, w, pressure , vff" << endl;
	test << "zone i = " << fDomain.x() << ", j = " << fDomain.z() << " , f = point" << std::endl;
	
	std::map<int, double>::iterator vffIter;

	for(int iZ = 0; iZ != fDomain[2]; ++iZ)
	{
		for(int iX = 0; iX != fDomain[0]; ++iX)	
		{
			double vfftmp = 0;
			for(vffIter = _fluidField->get(iX, iZ).Vff.begin(); vffIter != _fluidField->get(iX, iZ).Vff.end(); ++vffIter)
			{
				vfftmp = vffIter->second;
			}

			if(vfftmp > 0)
			{
				test << iX << " " << iZ << " "
					<< 0. << " "
					<< 0. << " "
					<< 0. << " "
					<< 0. << " "
					<< vfftmp << endl;
			}	
			else
			{
				test << iX << " " << iZ << " "
					<< _fluidField->get(iX, iZ)[0] << " "
					<< _fluidField->get(iX, iZ)[1] << " "
					<< _fluidField->get(iX, iZ)[2] << " "
					<< _fluidField->get(iX, iZ)[3] << " "
					<< vfftmp << endl;
			}
		}
	}

	test.close();
}


template<class ImplementType>
void ParticleFluidProcessor<ImplementType>::calculateCMvelocity()
{
	map<SphereIndex, ImmersedSphere<double> >::iterator npIter;
	map<SphereIndex, ImmersedSphere<double> >::iterator npBegin = _relativeSpheres.begin();
	map<SphereIndex, ImmersedSphere<double> >::iterator npEnd = _relativeSpheres.end();
	sMathLib::VectorX<T, 3> tempVector;
	for(npIter=npBegin;npIter!=npEnd;npIter++)
	{
		tempVector=tempVector+(*npIter).second.getVelocity()*particleMass;
	}
	double Vx=tempVector.x();
	double Vy=tempVector.y();
	double Vz=tempVector.z();
	double totalParticleMass=particleMass*_relativeSpheres.size();

	CMvelocity=VectorX<double, 3>(Vx/totalParticleMass,Vy/totalParticleMass,Vz/totalParticleMass);
}

template<class ImplementType>
double ParticleFluidProcessor<ImplementType>::GetVolumeFraction()
{
	return _relativeSpheres.size()*particleVolume/_controlVolume;
}

template<class ImplementType>
int ParticleFluidProcessor<ImplementType>::GetInteractionNum()
{
	return this->_relativeSphereInteractions.size();
}

#endif // _EFFECTIVE_STRESS_CALCULATOR_HPP_