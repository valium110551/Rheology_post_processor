#ifndef _DO_INTERACTION_READER_H_
#define _DO_INTERACTION_READER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Geometry/InteractionData.h"
#include "Geometry/ImmersedSphere.h"

template<class ReaderType>
class DOInteractionReader
{
private:

	std::ifstream* _fin;

	std::string _filename;

	std::vector< InteractionData > _interactions;

	std::vector< ImmersedSphere<double> > _spheres;

public:

	DOInteractionReader();

	virtual ~DOInteractionReader();

public:
	
	void SetFileName(std::string filename);

	void ReadFile();

	std::vector< InteractionData >& GetInteractions();

	std::vector< ImmersedSphere<double> >& GetSpheres();

};

#endif // _DO_INTERACTION_READER_H_
