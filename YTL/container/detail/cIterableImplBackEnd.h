#pragma once

#include "YTL/container/detail/cIteratorTag.h"
#include "YTL/types/smartPtr/lent_ptr.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename Iterable>
class cIterableImplBackEndInterface
{
public:
    virtual ~cIterableImplBackEndInterface() = default;
    virtual const Iterable& getPrivateIterable() const = 0;
    virtual Iterable& getPrivateIterable() = 0;
};

template<typename Iterable>
class cForwardIterableImplBackEndInterface : virtual public cIterableImplBackEndInterface<Iterable>
{
public:
    virtual const cForwardIterableImplBackEndInterface* getNextIterable() const = 0;
    virtual cForwardIterableImplBackEndInterface* getNextIterable() = 0;
};

template<typename Iterable>
class cBidirectionalIterableImplBackEndInterface : virtual public cIterableImplBackEndInterface<Iterable>
{
public:
    virtual const cBidirectionalIterableImplBackEndInterface* getNextIterable() const = 0;
    virtual cBidirectionalIterableImplBackEndInterface* getNextIterable() = 0;
    virtual const cBidirectionalIterableImplBackEndInterface* getPrevIterable() const = 0;
    virtual cBidirectionalIterableImplBackEndInterface* getPrevIterable() = 0;
};

template<typename IterableBackEnd, typename Traits>
class cForwardIterableImplBackEnd : virtual public cForwardIterableImplBackEndInterface<typename Traits::iterable_private_interface>
{
public:
    typedef typename Traits::iterable_private_interface iterable_private_interface;
    typedef cForwardIterableImplBackEndInterface<typename Traits::iterable_private_interface> nested_iterable_interface;

    cForwardIterableImplBackEnd() = default;
    cForwardIterableImplBackEnd(nested_iterable_interface& prevIterable);
private:
    const nested_iterable_interface* getNextIterable() const override;
    nested_iterable_interface* getNextIterable() override;
    template<typename T>
    inline const nested_iterable_interface* _getNextIterable(const T* iterableBackEnd, decltype(&T::m_nestedIterable) foo = NULL) const;
    inline const nested_iterable_interface* _getNextIterable(...) const;
    template<typename T>
    inline nested_iterable_interface* _getNextIterable(T* iterableBackEnd, decltype(&T::m_nestedIterable) foo = NULL);
    inline nested_iterable_interface* _getNextIterable(...);
};

template<typename IterableBackEnd, typename Traits>
class cBidirectionalIterableImplBackEnd : public cBidirectionalIterableImplBackEndInterface<typename Traits::iterable_private_interface>
{
public:
    typedef typename Traits::iterable_private_interface iterable_private_interface;
    typedef cBidirectionalIterableImplBackEndInterface<typename Traits::iterable_private_interface> nested_iterable_interface;

    cBidirectionalIterableImplBackEnd() = default;
    cBidirectionalIterableImplBackEnd(nested_iterable_interface& prevIterable);

private:
    const nested_iterable_interface* getNextIterable() const override;
    nested_iterable_interface* getNextIterable() override;
    const nested_iterable_interface* getPrevIterable() const override;
    nested_iterable_interface* getPrevIterable() override;
    template<typename T>
    inline const nested_iterable_interface* _getNextIterable(const T* iterableBackEnd, decltype(&T::m_nestedIterable) foo = NULL) const;
    inline const nested_iterable_interface* _getNextIterable(...) const;
    template<typename T>
    inline nested_iterable_interface* _getNextIterable(T* iterableBackEnd, decltype(&T::m_nestedIterable) foo = NULL);
    inline nested_iterable_interface* _getNextIterable(...);

    nested_iterable_interface* m_prevIterable = NULL;
};

template<size_t numIterables, typename Traits, size_t IterablePos>
class cIterableImplBackEnd;

template<size_t NumIterables, typename Traits>
class cIterableImplBackEnd<NumIterables,Traits,0> : public mpl::static_if<impl::is_reversable_iterator<typename Traits::iterator_type::tag>::value,cBidirectionalIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,0>,Traits>,cForwardIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,0>,Traits>>::type
{
    typedef typename mpl::static_if<impl::is_reversable_iterator<typename Traits::iterator_type::tag>::value,cBidirectionalIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,0>,Traits>,cForwardIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,0>,Traits>>::type backend_base;
    friend backend_base;
protected:
	typedef typename Traits::iterable_private_interface iterable_private_interface;
	typedef typename Traits::forwarded_iterable_private_interface forwarded_iterable_private_interface;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::forwarded_iterator_type forwarded_iterator_type;
    typedef typename Traits::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::forwarded_type forwarded_type;
	typedef typename Traits::forwarded_reference forwarded_reference;
	typedef typename Traits::const_forwarded_reference const_forwarded_reference;
    typedef typename backend_base::nested_iterable_interface nested_iterable_interface;

public:
    static const size_t iterable_pos = 0;
    static const size_t num_iterables = NumIterables;

    cIterableImplBackEnd(ytl::lent_ref<iterable_private_interface> i_iterable);
    cIterableImplBackEnd(nested_iterable_interface& i_prevIterable, ytl::lent_ref<iterable_private_interface> i_iterable);
    template<int IterablePosition>
    inline const nested_iterable_interface& getNthIterable() const;
    template<int IterablePosition>
    inline nested_iterable_interface& getNthIterable();

private:
    const iterable_private_interface& getPrivateIterable() const override;
    iterable_private_interface& getPrivateIterable() override;

    ytl::lent_ref<iterable_private_interface> m_privateIterable;
};

template<size_t NumIterables, typename Traits, size_t IterablePos>
class cIterableImplBackEnd : public mpl::static_if<impl::is_reversable_iterator<typename Traits::iterator_type::tag>::value,cBidirectionalIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,IterablePos>,Traits>,cForwardIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,IterablePos>,Traits>>::type
{
    typedef typename mpl::static_if<impl::is_reversable_iterator<typename Traits::iterator_type::tag>::value,cBidirectionalIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,IterablePos>,Traits>,cForwardIterableImplBackEnd<cIterableImplBackEnd<NumIterables,Traits,IterablePos>,Traits>>::type backend_base;
    friend backend_base;
protected:
	typedef typename Traits::iterable_private_interface iterable_private_interface;
	typedef typename Traits::forwarded_iterable_private_interface forwarded_iterable_private_interface;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::forwarded_iterator_type forwarded_iterator_type;
    typedef typename Traits::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::forwarded_type forwarded_type;
	typedef typename Traits::forwarded_reference forwarded_reference;
	typedef typename Traits::const_forwarded_reference const_forwarded_reference;
    typedef typename backend_base::nested_iterable_interface nested_iterable_interface;

public:
    static const size_t iterable_pos = IterablePos;
    static const size_t num_iterables = NumIterables;

    template<typename ... Iterables>
    cIterableImplBackEnd(ytl::lent_ref<iterable_private_interface> i_privateIterable, Iterables&& ... i_privateIterables);
    template<typename ... Iterables>
    cIterableImplBackEnd(nested_iterable_interface& i_prevIterable, ytl::lent_ref<iterable_private_interface> i_privateIterable, Iterables&& ... i_privateIterables);
    template<int IterablePosition>
    inline const nested_iterable_interface& getNthIterable() const;
    template<int IterablePosition>
    inline nested_iterable_interface& getNthIterable();

private:
    const iterable_private_interface& getPrivateIterable() const override;
    iterable_private_interface& getPrivateIterable() override;

    ytl::lent_ref<iterable_private_interface> m_privateIterable;
    cIterableImplBackEnd<NumIterables,Traits,IterablePos-1> m_nestedIterable;
};

}
}
}

#include "YTL/container/detail/cIterableImplBackEnd.tpp"
