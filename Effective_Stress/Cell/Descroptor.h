#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

namespace sEG
{
namespace Descriptor
{

template<typename T>
struct ScalarDescriptorBase
{
	typedef ScalarDescriptorBase<T> BaseDescriptor;

	enum{ d = 2, q = 1 };
};

template<typename T>
struct ScalarDescriptor: public ScalarDescriptorBase<T>
{
};

template<typename T>
struct FluidFieldDescriptor2DBase
{
	typedef FluidFieldDescriptor2DBase<T> BaseDescriptor;

	enum{ d = 2, q = 4};
};

template<typename T>
struct FluidFieldDescriptor2D: public FluidFieldDescriptor2DBase<T>
{

};

} // Descriptor
} // sEG

#endif // _DESCRIPTOR_H_