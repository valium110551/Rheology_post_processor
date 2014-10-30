#ifndef _VPF_READER_H_
#define _VPF_READER_H_

#include <string>
#include "VectorX/VectorX.hpp"
#include "Geometry/ImmersedSphere.h"
#include "StringConverter/StringUtility.h"


struct VPFReader
{
	static void ReadFile(std::ifstream* fin, std::vector< ImmersedSphere<double> >& spheres);
};

void VPFReader::ReadFile(std::ifstream* fin, std::vector< ImmersedSphere<double> >& spheres)
{
	using namespace sMathLib;

	std::string word, row;

	// Erase the first four lines. //For unrefined data, it should be uncomment
	for(int conlumnIndex = 0; conlumnIndex != 4; ++conlumnIndex)
		std::getline(*fin, row);

	VectorX<double, 3> position, velocity;
	double radius;

	while(std::getline(*fin, row))
	{
		std::string shapeType;

		std::istringstream rowStream(row);
		
		// Get DO type.
		rowStream >> word;

		if(word == "plate")
		{
			position = VectorX<double, 3>();
			velocity = VectorX<double, 3>();

			for(int i = 0; i != 3; ++i)
			{
				position[i] = -999999;
				velocity[i] = -999999;
			}

			radius = -1.;
			spheres.push_back( ImmersedSphere<double>(position, velocity, radius) );
			continue;
		}

		if(word != "sphere")
			continue;

		// Erase the DO name.
		rowStream >> word;

		for(int column = 2; column != 12; ++column)
		{
			rowStream >> word;

			if(column == 2)
				StringUtility::ToTargetType<double>(word, position[0]);

			if(column == 3)
				StringUtility::ToTargetType<double>(word, position[1]);

			if(column == 4)
				StringUtility::ToTargetType<double>(word, position[2]);

			if(column == 5)
				StringUtility::ToTargetType<double>(word, velocity[0]);

			if(column == 6)
				StringUtility::ToTargetType<double>(word, velocity[1]);

			if(column == 7)
				StringUtility::ToTargetType<double>(word, velocity[2]);

			if(column == 11)
				StringUtility::ToTargetType<double>(word, radius);
		}

		spheres.push_back(ImmersedSphere<double>(position, velocity, radius));
	}

}

#endif // _VPF_READER_H_
