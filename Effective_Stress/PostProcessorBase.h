#ifndef _POST_PROCESSOR_BASE_H_
#define _POST_PROCESSOR_BASE_H_

#include <string>
#include <fstream>

#include "FileReader/CSVReader.h"
#include "FileReader/VPFReader.h"
#include "FileReader/CSVRefinedReader.h"
#include "FileReader/VPFRefinedReader.h"
#include "FileReader/DOInteractionReader.hpp"

#include "FluidFieldDataConverter/FluidFieldConverter.hpp"
#include "FluidFieldDataConverter/FluidFieldData.h"


class PostProcessorBase
{
protected:

	PostProcessorBase();

	DOInteractionReader<CSVReader> _csvReader;

	DOInteractionReader<VPFReader> _vpfReader;

	DOInteractionReader<CSVRefinedReader> _csvRefinedReader;

	DOInteractionReader<VPFRefinedReader> _vpfRefinedReader;

	FluidFieldConverter<FluidFieldData>* _fluidField;

	std::string _fluidFieldFileName;

	std::ifstream* _fIn;

public:

	virtual void SetCSVFileName(std::string filename);

	virtual void SetVPFFileName(std::string filename);

	virtual void SetRefinedCSVFileName(std::string filename);
	
	virtual void SetRefinedVPFFileName(std::string filename);
	
	virtual void SetFluidFieldFileName(std::string filename);

	virtual void ReadFile();

	virtual void ReadRefinedFile();

	virtual void Initialize() = 0;

public:

	virtual ~PostProcessorBase();

};

PostProcessorBase::PostProcessorBase()
{

}

PostProcessorBase::~PostProcessorBase()
{

}

void PostProcessorBase::SetCSVFileName(std::string filename)
{
	this->_csvReader.SetFileName(filename);
}

void PostProcessorBase::SetVPFFileName(std::string filename)
{
	this->_vpfReader.SetFileName(filename);
}

void PostProcessorBase::SetRefinedCSVFileName(std::string filename)
{
	this->_csvRefinedReader.SetFileName(filename);
}

void PostProcessorBase::SetRefinedVPFFileName(std::string filename)
{
	this->_vpfRefinedReader.SetFileName(filename);
}

void PostProcessorBase::SetFluidFieldFileName(std::string filename)
{
	this->_fluidFieldFileName = filename;
	this->_fIn = new std::ifstream(filename.c_str(), std::ios::in | std::ios::binary);
}

void PostProcessorBase::ReadFile()
{
	this->_csvReader.ReadFile();
	this->_vpfReader.ReadFile();
	//this->_fluidField = FluidFieldConverter<FluidFieldData>::CreateConverter(this->_fIn);
	//this->_fluidField->converter("f_" + this->_fluidFieldFileName);
}

void PostProcessorBase::ReadRefinedFile()
{
	this->_csvRefinedReader.ReadFile();
	this->_vpfRefinedReader.ReadFile();
}

#endif // _POST_PROCESSOR_BASE_H_