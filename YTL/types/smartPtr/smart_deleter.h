#pragma once

namespace yame
{
namespace ytl
{

class ISmartPtrDeleter
{
public:
	virtual ~ISmartPtrDeleter(){};
	virtual void Deallocate(void* i_object) = 0;
};

}
}
