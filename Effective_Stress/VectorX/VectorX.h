#ifndef VECTORX_H
#define VECTORX_H

// Ver. 3.5

namespace sMathLib
{

template<typename T, int dimension>
class VectorX
{

public:

	VectorX();

	VectorX(T x, T y, T z);

	VectorX(const VectorX &rhs);

	virtual ~VectorX(void);

public:
	T& operator[](const int index);
	
	const T& operator[](const int index) const;

	const VectorX& operator=(VectorX const &rhs);

	VectorX operator+(const VectorX& rhs) const;

	VectorX operator-(const VectorX& rhs) const;

	VectorX operator*(const T& rhs) const;

	VectorX Cross(const VectorX& rhs) const;

	virtual T NormSqr() const;

	virtual T Volume() const;

	virtual T Abs() const;
	
	virtual T x() const;
	
	virtual T y() const;
	
	virtual T z() const;

private:
	typedef T VectorData[dimension];
	VectorData vec;
};

//typedef VectorX<double, 3> Vector3;
//typedef VectorX<double, 2> Vector2;

} // sMathLib

#endif // VECTORX_H
