#pragma once

#include "Math/detail/cSetInterface.h"
#include "Math/cMathTemplateHelper.h"
#include "Math/detail/cMathTags.h"

namespace yame
{
namespace math
{

template<typename Traits>
class cSet : virtual public detail::ISet<Traits>, public inherit_extended_structure<Traits>::type, public detail::set_tag
{
    template<typename>
    friend class cSet;
    typedef typename inherit_extended_structure<Traits>::type extended_set;
public:
	typedef typename Traits::underlying_type underlying_type;
    using extended_set::extended_set;
    using extended_set::operator=;

	cSet(const underlying_type& i_value);
	cSet(const cSet<Traits>& other);
	cSet(cSet<Traits>&& other);
    template<typename TTraits>
    requires (mpl::is_base_of<Traits, TTraits>::value)
    cSet(const cSet<TTraits>& other);
    template<typename ... Args>
    cSet(const Args& ... i_args);
    ~cSet();

    cSet<Traits>& operator=(const cSet<Traits>& other) override;
    cSet<Traits>& operator=(cSet<Traits>&& other) override;
    cSet<Traits>& operator=(const underlying_type& i_value) override;
    cSet<Traits>& operator=(underlying_type&& i_value) override;
	inline bool operator==(const cSet<Traits>& other) const override;
	inline bool operator!=(const cSet<Traits>& other) const override;
    inline const underlying_type& getValue() const override;
    inline underlying_type& getValue() override;

protected:
    inline void setValue(const underlying_type& i_value) override;
    inline underlying_type&& extractValue();

private:
	underlying_type m_value;
};

}

}

#include "Math/detail/cSet.tpp"
