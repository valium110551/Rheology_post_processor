#ifndef _FLUID_FIELD_CONVERTER_HPP_
#define _FLUID_FIELD_CONVERTER_HPP_

#include "FluidFieldDataConverter/FluidFieldConverter.h"

template<typename ProcessorType>
FluidFieldConverter<ProcessorType>::FluidFieldConverter(std::ifstream *fin)
: _fin(fin)
{
}

template<typename ProcessorType>
FluidFieldConverter<ProcessorType>::~FluidFieldConverter()
{
}


template<typename ProcessorType>
void FluidFieldConverter<ProcessorType>::converter(std::string out_filename)
{
	this->_fout = new std::ofstream(out_filename.c_str());
	ProcessorType::converImplement(this->_fin, this->_fout);
	//std::ifstream* fin = new std::ifstream("field_100000.dat");
	//ProcessorType::converImplement(fin, this->_fout);
}

template<typename ProcessorType>
FluidFieldConverter<ProcessorType> * FluidFieldConverter<ProcessorType>::CreateConverter(std::ifstream *fin)
{
	return new FluidFieldConverter<ProcessorType>(fin);
}

template<typename ProcessorType>
int FluidFieldConverter<ProcessorType>::getNx()
{
	return ProcessorType::Nx;
}

template<typename ProcessorType>
int FluidFieldConverter<ProcessorType>::getNy()
{
	return ProcessorType::Ny;
}

template<typename ProcessorType>
int FluidFieldConverter<ProcessorType>::getNz()
{
	return ProcessorType::Nz;
}

template<typename ProcessorType>
sEG::Cell<double, sEG::Descriptor::FluidFieldDescriptor2D>& FluidFieldConverter<ProcessorType>::get(int iX, int iY)
{
	return ProcessorType::_fluidCell[iX][iY];
}


#endif // _FLUID_FIELD_CONVERTER_HPP