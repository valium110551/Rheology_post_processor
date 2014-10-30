#ifndef LB_PARAMETER_READER_H
#define LB_PARAMETER_READER_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>

template <class IntType, class DecType>
class ParameterReader
{
public:
	static ParameterReader<IntType, DecType>* CreateFromFile(const char* filename);

	const DecType GetFluidDensity() const 
	{ return this->lbParams.rho_fluid; }

	const DecType GetPhysicalDynamicViscosity() const 
	{ return this->lbParams.mu_phy; }

	const DecType GetPhysicalReferenceLength() const 
	{ return this->lbParams.L0_phy; }

	const DecType GetPhysicalReferenceTime() const 
	{ return this->lbParams.T0_phy; }

	const DecType GetPhysicalLengthX() const
	{ return this->lbParams.Lx; }

	const DecType GetPhysicalLengthY() const
	{ return this->lbParams.Ly; }

	const DecType GetPhysicalLengthZ() const
	{ return this->lbParams.Lz; }

	const DecType GetDeltaX() const
	{ return this->lbParams.deltaX; }

	const DecType GetDeltaT()const
	{ return this->lbParams.deltaT; }

	const DecType GetTau() const
	{ return this->lbParams.tau; }	

	const DecType GetGravity() const
	{ return this->lbParams.gravity; }

	const IntType GetVTKSaveTimeSteps() const
	{ return this->lbParams.vtkSaveSteps; }

	bool EnableVTKSave() const
	{ return this->lbParams.vtkSaveSteps > 0; }

	const DecType GetBottomPressure() const
	{ return this->lbParams.bottomPressure; }

	const DecType GetModelHeight() const
	{ return this->lbParams.modelHeight; }

	std::map< std::string, std::pair<std::string, DecType> > GetBoundingSides() const
	{ return this->lbParams.BoundingSides; }

private:	
	template <class T>
	static bool From_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&));

	template <class T>
	static bool ReadDataField(std::ifstream& inf, T& data);

	template <class T>
	static bool ReadBoundaryConditions(std::ifstream& inf, T& data);

	typedef struct LBParams
	{
		DecType rho_fluid, mu_phy, L0_phy, T0_phy, deltaX, deltaT, tau, Lx, Ly, Lz, bottomPressure, gravity, modelHeight;
		IntType vtkSaveSteps;
		std::map< std::string, std::pair<std::string, DecType> > BoundingSides;
	} LBParams;

	ParameterReader<IntType, DecType> (const LBParams& _params) : lbParams(_params) {}
	LBParams lbParams;
};

template <class IntType, class DecType>
ParameterReader<IntType, DecType>* ParameterReader<IntType, DecType>::CreateFromFile(const char* filename)
{
	std::ifstream inf;
	inf.open(filename);
	if(inf.is_open())
	{
		typename ParameterReader<IntType, DecType>::LBParams _lbParams;
		bool ok = true;
		ok &= ReadDataField<DecType>(inf, _lbParams.rho_fluid);
		ok &= ReadDataField<DecType>(inf, _lbParams.mu_phy);
		ok &= ReadDataField<DecType>(inf, _lbParams.L0_phy);
		ok &= ReadDataField<DecType>(inf, _lbParams.T0_phy);
		ok &= ReadDataField<DecType>(inf, _lbParams.deltaX);
		ok &= ReadDataField<DecType>(inf, _lbParams.deltaT);
		ok &= ReadDataField<DecType>(inf, _lbParams.tau);
		ok &= ReadDataField<DecType>(inf, _lbParams.Lx);
		ok &= ReadDataField<DecType>(inf, _lbParams.Ly);
		ok &= ReadDataField<DecType>(inf, _lbParams.Lz);
		ok &= ReadDataField<IntType>(inf, _lbParams.vtkSaveSteps);
		ok &= ReadDataField<DecType>(inf, _lbParams.modelHeight);


		////// Temp
		ok &= ReadBoundaryConditions< std::map< std::string, std::pair<std::string, DecType> > >(inf, _lbParams.BoundingSides);	

		inf.close();

		if (ok) 
			return new ParameterReader<IntType, DecType>(_lbParams);
		else 
			std::cerr << "Wrong LB parameter file format." << std::endl;
	}
	else
		std::cerr << "Unable to open LB parameter file." << std::endl;

	return 0;
}

template <class IntType, class DecType>
template <class T>
bool ParameterReader<IntType, DecType>::From_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

template <class IntType, class DecType>
template <class T>
bool ParameterReader<IntType, DecType>::ReadDataField(std::ifstream& inf, T& data)
{
	std::string line;
	while(!inf.eof())
	{
		line.clear();
		std::getline(inf, line);
		if(line.length() == 0) return false;
		if(line.c_str()[0] == '#') continue;
		return From_string<T>(data, line, std::dec); 
	};
	return false;
}

template <class IntType, class DecType>
template <class T>
bool ParameterReader<IntType, DecType>::ReadBoundaryConditions(std::ifstream& inf, T& data)
{
	std::string line;
	while(!inf.eof())
	{
		line.clear();
		std::getline(inf, line);

		std::cout << line << std::endl;

		if(line.length() == 0) break;
		if(line.c_str()[0] == '#') continue;

		std::istringstream isstream(line);
		std::string tmp1, tmp2, tmp3;
		isstream >> tmp1 >> tmp2 >> tmp3;		
		data[tmp1].first = tmp2;
		bool readfile = From_string<DecType>(data[tmp1].second, tmp3, std::dec);	
	};
	return true;
}


#endif // LB_PARAMETER_READER_H
