#ifndef _CSV_READER_H_
#define _CSV_READER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "VectorX/VectorX.hpp"
#include "Geometry/InteractionData.h"
#include "StringConverter/StringUtility.h"

struct CSVReader
{
	static void ReadFile(std::ifstream* fin, std::vector< InteractionData >& interactions);
};

void CSVReader::ReadFile(std::ifstream* fin, std::vector< InteractionData >& interactions)
{	
	std::string word, row;

	// Erase the first line which contains the column description
	std::getline(*fin, row); //For unrefined data, it should be uncomment
	while(std::getline(*fin, row))
	{
		unsigned masterID, slaveID;
		sMathLib::VectorX<double, 3> intForce, intForce2, contactPoint, impactDirection;

		std::istringstream rowStream(row);

		// Get the masterID
		rowStream >> word;
		if( word[word.size()-1] == ',')
			word.erase(word.size() - 1, 1);
		StringUtility::ToTargetType<unsigned>(word, masterID);
		
		// Skip the wall elements
		/*if (masterID <= 5)
			continue;*/
		
		for(int column = 1; column != 28; ++column)
		{
			rowStream >> word;
			
			if( word[word.size()-1] == ',')
				word.erase(word.size() - 1, 1);

			if(column == 1)
				StringUtility::ToTargetType<unsigned>(word, slaveID);

			if(column == 2)
				StringUtility::ToTargetType<double>(word, intForce[0]);

			if(column == 3)
				StringUtility::ToTargetType<double>(word, intForce[1]);

			if(column == 4)
				StringUtility::ToTargetType<double>(word, intForce[2]);

			if(column == 5)
				StringUtility::ToTargetType<double>(word, intForce2[0]);

			if(column == 6)
				StringUtility::ToTargetType<double>(word, intForce2[1]);

			if(column == 7)
				StringUtility::ToTargetType<double>(word, intForce2[2]);

			if(column == 15)
				StringUtility::ToTargetType<double>(word, impactDirection[0]);

			if(column == 16)
				StringUtility::ToTargetType<double>(word, impactDirection[1]);

			if(column == 17)
				StringUtility::ToTargetType<double>(word, impactDirection[2]);

			if(column == 18)
				StringUtility::ToTargetType<double>(word, contactPoint[0]);

			if(column == 19)
				StringUtility::ToTargetType<double>(word, contactPoint[1]);

			if(column == 20)
				StringUtility::ToTargetType<double>(word, contactPoint[2]);

		}
		//InteractionData intDataMaster(masterID, slaveID, intForce);
		//InteractionData intDataSlave(slaveID, masterID, intForce);
		//InteractionData intDataMaster(masterID, slaveID, intForce, contactPoint);
		/*if(slaveID <= 5)
			continue;*/
		interactions.push_back(InteractionData(masterID, slaveID, intForce, contactPoint, impactDirection));
		//interactions.push_back(intDataSlave);
	}
}

#endif // _CSV_READER_H_