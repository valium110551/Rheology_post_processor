#ifndef _FLUID_FIELD_DATA_H_
#define _FLUID_FIELD_DATA_H_

#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include "VectorX/VectorX.hpp"
#include "Cell/Cell.hpp"
#include "Cell/Descroptor.h"

class FluidFieldData
{
protected:

typedef sEG::Cell<double, sEG::Descriptor::FluidFieldDescriptor2D > FluidCell2D;
typedef std::vector< std::vector<FluidCell2D> > Block2D;

protected:
	
	void converImplement(std::ifstream *fin, std::ofstream *fout);

	void converImplement(std::ifstream *fin, Block2D& fluidCell);

protected:

	FluidFieldData();

	~FluidFieldData();

	std::map<int, std::map<int, sMathLib::VectorX<double, 3> > > Velocity;
	std::map<int, std::map<int, double> > Density;

	Block2D _fluidCell;

	int Nx;
	int Ny;
	int Nz;
};

FluidFieldData::FluidFieldData()
{
	
}

FluidFieldData::~FluidFieldData()
{

}

void FluidFieldData::converImplement(std::ifstream *fin, std::ofstream *fout)
{
	using namespace std;


	fin->read( (char*) &Nx, sizeof(int) );
	fin->read( (char*) &Ny, sizeof(int) );
	fin->read( (char*) &Nz, sizeof(int) );

	this->_fluidCell.assign( Nx, vector<FluidCell2D>( Nz, FluidCell2D() ) );

	for(int iX = 0; iX != Nx; ++iX)	
	{			
		for(int iY = 0; iY != Nz; ++iY)
		{
			double vel[3] = {0., 0., 0.};

			fin->read( (char*) &vel, sizeof(double[3]) );

			double pre = 0.;

			fin->read( (char*) &pre, sizeof(pre) );

			this->_fluidCell[iX][iY][0] = vel[0];
			this->_fluidCell[iX][iY][1] = vel[1];
			this->_fluidCell[iX][iY][2] = vel[2];
			this->_fluidCell[iX][iY][3] = pre;

		}

	}	

}

#endif // _FLUID_FIELD_DATA_H_
