#ifndef _FLUID_FIELD_CONVERTER_H_
#define _FLUID_FIELD_CONVERTER_H_

#include <fstream>
#include <string>
#include <vector>

#include "Cell/Cell.hpp"
#include "Cell/Descroptor.h"

template<typename ProcessorType>
class FluidFieldConverter : public ProcessorType
{
public:

	virtual ~FluidFieldConverter();

	static FluidFieldConverter* CreateConverter(std::ifstream *fin);

	void converter(std::string out_filename);

	sEG::Cell<double, sEG::Descriptor::FluidFieldDescriptor2D>& get(int iX, int iY);

	int getNx();

	int getNy();

	int getNz();

private:

	FluidFieldConverter(std::ifstream *fin);

private:

	std::string _filename;

	std::ifstream *_fin;
	std::ofstream *_fout;

};

#endif // _FLUID_FIELD_CONVERTER_H_