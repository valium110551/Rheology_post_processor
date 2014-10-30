#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <vector>

namespace sMathLib
{

template<typename T>
class Tensor
{
protected:
	
	std::vector< std::vector< T > > val;

public:

	Tensor();

	Tensor(const Tensor& rhs);

	Tensor(double (*rhs)[3]);//??

	~Tensor();

public:

	const Tensor& operator=(const Tensor& rhs);

	std::vector<T>& operator[](const int index);

	const std::vector<T>& operator[](const int index) const;
};

template<typename T>
Tensor<T>::Tensor()
{
	val.assign(3, std::vector<double>( 3, T() ) );
	for(int i = 0; i != 3; ++i)
		for(int j = 0; j != 3; ++j)
			val[i][j] = T();
}

template<typename T>
Tensor<T>::Tensor(const Tensor& rhs)
{
	val.assign(3, std::vector<double>( 3, T() ) );
	for(int i = 0; i != 3; ++i)
		for(int j = 0; j != 3; ++j)
			this->val[i][j] = rhs[i][j];
}

template<typename T>
Tensor<T>::Tensor(double (*rhs)[3])
{
	val.assign(3, std::vector<double>( 3, T() ) );
	for(int i = 0; i != 3; ++i)
		for(int j = 0; j != 3; ++j)
			this->val[i][j] = rhs[i][j];
}

template<typename T>
Tensor<T>::~Tensor()
{

}

template<typename T>
const Tensor<T>& Tensor<T>::operator=(const Tensor& rhs)
{
	for(int i = 0; i != 3; ++i)
		for(int j = 0; j != 3; ++j)
			this->val[i][j] = rhs[i][j];
}

template<typename T>
std::vector<T>& Tensor<T>::operator[](const int index)
{
	return this->val[index];
}

template<typename T>
const std::vector<T>& Tensor<T>::operator[](const int index) const
{
	return this->val[index];
}

} // sMathLib
#endif // _TENSOR_H_