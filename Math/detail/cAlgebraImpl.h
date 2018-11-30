#pragma once

#include "Math/detail/cSetInterface.h"
#include "Math/cMathTemplateHelper.h"
#include "Math/detail/cMathTraitsConceptHelper.h"
#include "Math/detail/cMathTags.h"

namespace yame
{
namespace math
{
namespace detail
{

template<group_traits_type Traits, typename FinalObject>
class cGroupImpl : virtual public ISet<typename Traits::set_traits>, public inherit_extended_structure<Traits>::type, public group_tag
{
protected:
    typedef typename Traits::set_traits set_traits;
public:
    typedef Traits traits;

	friend inline FinalObject operator+(const FinalObject& i_lhs, const FinalObject& i_rhs)
	{
        FinalObject res;

        Traits::sum(res.getValue(), i_lhs.getValue(), i_rhs.getValue());

        return res;
	}
	inline FinalObject& operator+=(const cGroupImpl<Traits,FinalObject>& other);
	friend inline FinalObject operator-(const FinalObject& i_lhs, const FinalObject& i_rhs)
	{
        FinalObject res;
        FinalObject subs;

        Traits::inv(subs.getValue(), i_rhs.getValue());
        Traits::sum(res.getValue(), i_lhs.getValue(),subs.getValue());

        return res;
	}
	friend inline FinalObject operator-(const FinalObject& i_lhs)
	{
        FinalObject res;

        Traits::inv(res.getValue(), i_lhs.getValue());

        return res;
	}
	inline FinalObject& operator-=(const cGroupImpl<Traits,FinalObject>& other);
    static inline FinalObject neutral_element();
	inline static constexpr bool isCommutative()
	{
        return Traits::is_commutative;
	}
};

template<ring_traits_type Traits, typename FinalObject>
class cRingImpl : public cGroupImpl<typename Traits::group_traits,FinalObject>, public inherit_extended_structure<Traits>::type, public ring_tag
{
protected:
    typedef typename cGroupImpl<typename Traits::group_traits,FinalObject>::set_traits set_traits;
public:
    typedef Traits traits;
    typedef cGroupImpl<typename Traits::group_traits,FinalObject> group;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::cGroupImpl;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator==;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator!=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator+=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator-=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::isCommutative;

    friend inline FinalObject operator*(const FinalObject& i_lhs, const FinalObject& i_rhs)
    {
        FinalObject res;

        Traits::prod(res.getValue(), i_lhs.getValue(), i_rhs.getValue());

        return res;
    }
	inline FinalObject& operator*=(const cRingImpl<Traits,FinalObject>& other);
    static inline FinalObject neutral_element();
	inline static constexpr bool isCommutative()
	{
        return Traits::is_commutative;
	}
};

template<division_ring_traits_type Traits, typename FinalObject>
class cDivisionRingImpl : public cRingImpl<typename Traits::ring_traits,FinalObject>, public inherit_extended_structure<Traits>::type, public division_ring_tag
{
protected:
    typedef typename cRingImpl<typename Traits::ring_traits,FinalObject>::set_traits set_traits;

public:
    typedef Traits traits;
    typedef cRingImpl<typename Traits::ring_traits,FinalObject> ring;

	using cRingImpl<typename Traits::ring_traits,FinalObject>::cRingImpl;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::operator=;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::operator==;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::operator!=;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::operator+=;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::operator-=;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::isCommutative;
	using cRingImpl<typename Traits::ring_traits,FinalObject>::operator*=;

	friend inline FinalObject operator/(const FinalObject& i_lhs, const FinalObject& i_rhs)
	{
        FinalObject res;
        FinalObject inv;

        Traits::inv(inv.getValue(), i_rhs.getValue());

        Traits::ring_traits::prod(res.getValue(), i_lhs.getValue(), inv.getValue());

        return res;
	}
	inline FinalObject& operator/=(const cDivisionRingImpl<Traits,FinalObject>& other) const;
};

template<field_traits_type Traits, typename FinalObject>
class cFieldImpl : public cDivisionRingImpl<Traits,FinalObject>, public field_tag
{
protected:
    typedef typename cDivisionRingImpl<Traits,FinalObject>::set_traits set_traits;

public:
    typedef Traits traits;
    typedef cDivisionRingImpl<Traits,FinalObject> ring;
	using cDivisionRingImpl<Traits,FinalObject>::cDivisionRingImpl;
	using cDivisionRingImpl<Traits,FinalObject>::operator=;
	using cDivisionRingImpl<Traits,FinalObject>::operator==;
	using cDivisionRingImpl<Traits,FinalObject>::operator!=;
	using cDivisionRingImpl<Traits,FinalObject>::operator+=;
	using cDivisionRingImpl<Traits,FinalObject>::operator-=;
	using cDivisionRingImpl<Traits,FinalObject>::isCommutative;
	using cDivisionRingImpl<Traits,FinalObject>::operator*=;
};

template<module_traits_type Traits, typename FinalObject>
class cModuleImpl : public cGroupImpl<typename Traits::group_traits,FinalObject>, public inherit_extended_structure<Traits>::type, public module_tag
{
	typedef typename Traits::ring ring;
protected:
    typedef typename cGroupImpl<typename Traits::group_traits,FinalObject>::set_traits set_traits;
public:
    typedef Traits traits;
    typedef cGroupImpl<typename Traits::group_traits,FinalObject> group;
    using cGroupImpl<typename Traits::group_traits,FinalObject>::cGroupImpl;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator==;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator!=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator+=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::operator-=;
	using cGroupImpl<typename Traits::group_traits,FinalObject>::isCommutative;

	inline FinalObject prod(const ring& other) const;
	static constexpr inline bool isLeftModule()
	{
        return Traits::is_leftModule;
	}
	static constexpr inline bool isRightModule()
	{
        return Traits::is_rightModule;
	}
	static constexpr inline bool isBilateral()
	{
	    return Traits::is_leftModule && Traits::is_rightModule;
	}
};

template<algebra_traits_type Traits, typename FinalObject>
class cAlgebraImpl : public cModuleImpl<typename Traits::module_traits,FinalObject>, public inherit_extended_structure<Traits>::type, public algebra_tag
{
protected:
    typedef typename cModuleImpl<typename Traits::module_traits,FinalObject>::set_traits set_traits;
public:
    typedef Traits traits;
    typedef cModuleImpl<typename Traits::module_traits,FinalObject> module;
    using cModuleImpl<typename Traits::module_traits,FinalObject>::cModuleImpl;
	using cModuleImpl<typename Traits::module_traits,FinalObject>::operator=;
	using cModuleImpl<typename Traits::module_traits,FinalObject>::operator==;
	using cModuleImpl<typename Traits::module_traits,FinalObject>::operator!=;
	using cModuleImpl<typename Traits::module_traits,FinalObject>::operator+=;
	using cModuleImpl<typename Traits::module_traits,FinalObject>::operator-=;
	using cModuleImpl<typename Traits::module_traits,FinalObject>::isCommutative;

	friend inline FinalObject operator*(const FinalObject& i_lhs, const FinalObject& i_rhs)
	{
        FinalObject res;

        Traits::algProd(res.getValue(), i_lhs.getValue(), i_rhs.getValue());

        return res;
	}
	inline FinalObject& operator*=(const cAlgebraImpl<Traits,FinalObject>& other);
};

template<typename Traits, typename FinalObject>
FinalObject operator*(const cModuleImpl<Traits,FinalObject>& i_lhs, const typename Traits::ring& i_rhs);
template<typename Traits, typename FinalObject>
FinalObject operator*(const typename Traits::ring& i_lhs, const cModuleImpl<Traits,FinalObject>& i_rhs);
template<typename Traits, typename FinalObject>
const cModuleImpl<Traits,FinalObject>& operator*=(const cModuleImpl<Traits,FinalObject>& i_lhs, const typename Traits::ring& i_rhs);
template<typename Traits, typename FinalObject>
const cModuleImpl<Traits,FinalObject>& operator*=(const typename Traits::ring& i_lhs, const cModuleImpl<Traits,FinalObject>& i_rhs);

NESTED_STRUCTURE_ACQUIRER(cGroupImpl,group_traits_type)
NESTED_STRUCTURE_ACQUIRER(cRingImpl,ring_traits_type)
NESTED_STRUCTURE_ACQUIRER(cDivisionRingImpl,division_ring_traits_type)
NESTED_STRUCTURE_ACQUIRER(cFieldImpl,field_traits_type)
NESTED_STRUCTURE_ACQUIRER(cModuleImpl,module_traits_type)
NESTED_STRUCTURE_ACQUIRER(cAlgebraImpl,algebra_traits_type)

}

}

}


#include "Math/detail/cAlgebraImpl.tpp"
