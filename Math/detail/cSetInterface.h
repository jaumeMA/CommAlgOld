#pragma once

namespace yame
{
namespace math
{

template<typename Traits>
class cSet;

namespace detail
{

template<typename Traits>
class ISet
{
    friend cSet<Traits>;

public:
	typedef typename Traits::underlying_type underlying_type;

    virtual cSet<Traits>& operator=(const cSet<Traits>& other) = 0;
    virtual cSet<Traits>& operator=(cSet<Traits>&& other) = 0;
    virtual cSet<Traits>& operator=(const underlying_type& i_value) = 0;
    virtual cSet<Traits>& operator=(underlying_type&& i_value) = 0;
	virtual bool operator==(const cSet<Traits>& other) const = 0;
	virtual bool operator!=(const cSet<Traits>& other) const = 0;
    virtual const underlying_type& getValue() const = 0;

protected:
    virtual underlying_type& getValue() = 0;
    virtual void setValue(const underlying_type& i_value) = 0;
};

}
}
}
