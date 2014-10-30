#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <string>
#include <sstream>
#include <vector>

struct StringUtility
{

template<class TargetType>
static TargetType& ToTargetType(std::string& inputString, TargetType& targetType);

template<class TargetType>
static TargetType ToTargetType(std::string inputString);

//template<class TargetType>
//static unsigned& ToTargetType(std::string& inputString,unsigned& inputInt);

static std::vector< std::string > StringDelimiter(std::string inputString, std::string dlm);

};

template<class TargetType>
TargetType& StringUtility::ToTargetType(std::string& inputString, TargetType& targetType)
{
	std::istringstream sStream(inputString);

	sStream >> targetType;

	return targetType;
}

template<class TargetType>
TargetType StringUtility::ToTargetType(std::string inputString)
{
	std::istringstream sStream(inputString);

	TargetType targetType;
	sStream >> targetType;

	return targetType;
}

std::vector< std::string > StringUtility::StringDelimiter(std::string inputString, std::string dlm)
{
	std::vector< std::string > outputVector;
	std::size_t pos = 0;
	while(pos != std::string::npos)
	{
		pos = inputString.find(dlm);
		std::string tmp = inputString.substr(0, pos);
		outputVector.push_back(tmp);
		inputString = inputString.substr(pos + dlm.size(), inputString.size());
	}

	return outputVector;
}

//template<class TargetType>
//unsigned& StringUtility::ToTargetType(std::string& inputString,unsigned& inputInt)
//{
//	std::stringstream sStream;
//	sStream<<inputInt;
//	unsigned targetType;
//	sStream >> targetType;
//
//	return targetType;
//}

#endif _STRING_UTILITY_H_