#ifndef _DO_INTERACTION_READER_HPP_
#define _DO_INTERACTION_READER_HPP_

#include "FileReader/DoInteractionReader.h"//"DOInteractionReader.h"

#include "FileReader/CSVReader.h"
#include "FileReader/VPFReader.h"
#include "FileReader/CSVRefinedReader.h"
#include "FileReader/VPFRefinedReader.h"

template<class ReaderType>
DOInteractionReader<ReaderType>::DOInteractionReader()
{

}

template<class ReaderType>
DOInteractionReader<ReaderType>::~DOInteractionReader()
{

}

template<class ReaderType>
void DOInteractionReader<ReaderType>::SetFileName(std::string filename)
{
	this->_filename = filename;
}

template<class ReaderType>
void DOInteractionReader<ReaderType>::ReadFile()
{
	this->_fin = new std::ifstream(this->_filename.c_str());
	ReaderType::ReadFile(this->_fin, this->_interactions);
}

template<>
void DOInteractionReader<CSVRefinedReader>::ReadFile()
{
	this->_fin = new std::ifstream(this->_filename.c_str());
	CSVRefinedReader::ReadFile(this->_fin, this->_interactions);
}

template<>
void DOInteractionReader<VPFRefinedReader>::ReadFile()
{
	this->_fin = new std::ifstream(this->_filename.c_str());
	VPFRefinedReader::ReadFile(this->_fin, this->_spheres);
}

template<>
void DOInteractionReader<CSVReader>::ReadFile()
{
	this->_fin = new std::ifstream(this->_filename.c_str());
	CSVReader::ReadFile(this->_fin, this->_interactions);
}

template<>
void DOInteractionReader<VPFReader>::ReadFile()
{
	this->_fin = new std::ifstream(this->_filename.c_str());
	VPFReader::ReadFile(this->_fin, this->_spheres);
}

template<class ReaderType>
std::vector< InteractionData >& DOInteractionReader<ReaderType>::GetInteractions()
{
	return this->_interactions;
}

template<class ReaderType>
std::vector< ImmersedSphere<double> >& DOInteractionReader<ReaderType>::GetSpheres()
{
	return this->_spheres;
}

#endif // _DO_INTERACTION_READER_HPP_