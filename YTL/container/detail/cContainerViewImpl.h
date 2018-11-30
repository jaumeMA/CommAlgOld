#pragma once

#include "YTL/types/smartPtr/lent_ptr.h"
#include "YTL/functional/cFunction.h"
#include "YTL/container/cAlgorithm.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename Traits>
class cContainerViewImplBase
{
    friend inline bool operator==(const cContainerViewImplBase& i_lhs, const cContainerViewImplBase& i_rhs)
    {
        return (i_lhs.getSize() != i_rhs.getSize()) ? false : equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend());
    }
    friend inline bool operator!=(const cContainerViewImplBase& i_lhs, const cContainerViewImplBase& i_rhs)
    {
        return (i_lhs.getSize() != i_rhs.getSize()) ? false : equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) == false;
    }

public:
    typedef typename Traits::iterable_type iterable_type;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer_type pointer_type;
    typedef typename Traits::const_pointer_type const_pointer_type;

    cContainerViewImplBase(iterable_type& i_iterable, const ytl::function<bool(const_reference)> i_filter = null_ptr);
    cContainerViewImplBase(const cContainerViewImplBase& other);
    cContainerViewImplBase(cContainerViewImplBase&& other);
    ~cContainerViewImplBase();

    cContainerViewImplBase& operator=(iterable_type& i_iterable);
    cContainerViewImplBase& operator=(const cContainerViewImplBase& other);
    cContainerViewImplBase& operator=(cContainerViewImplBase&& other);

    //usual api
    iterator_type begin(const ytl::function<bool(const_reference)>& filter = null_ptr);
    iterator_type end();
    const_iterator_type cbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    const_iterator_type cend() const;
    size_t getSize() const;
    size_t empty() const;
    cContainerViewImplBase<Traits> subView(const ytl::function<bool(const_reference)>& i_filter) const;

protected:
    ytl::lent_ref<iterable_type> m_iterable;
    ytl::function<bool(const_reference)> m_filter;
};

template<typename Iterable, typename Traits>
class cContainerViewImpl : public cContainerViewImplBase<Traits>
{
    typedef cContainerViewImplBase<Traits> Base;

public:
    typedef typename Base::iterator_type iterator_type;
    typedef typename Base::const_iterator_type const_iterator_type;
    typedef typename Base::reference reference;
    typedef typename Base::const_reference const_reference;
    typedef typename Base::pointer_type pointer_type;
    typedef typename Base::const_pointer_type const_pointer_type;

    using Base::Base;
    using Base::operator=;
    using Base::begin;
    using Base::end;
    using Base::cbegin;
    using Base::cend;
    using Base::subView;
    using Base::getSize;
    using Base::empty;
};

template<typename T, typename Traits>
class cContainerViewImpl<cConstBidirectionalIterable<T>,Traits> : protected cContainerViewImplBase<Traits>
{
    typedef cContainerViewImplBase<Traits> Base;
public:
    typedef typename Base::iterator_type iterator_type;
    typedef typename Base::const_iterator_type const_iterator_type;
    typedef typename Traits::reverse_iterator_type reverse_iterator_type;
    typedef typename Traits::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename Base::reference reference;
    typedef typename Base::const_reference const_reference;
    typedef typename Base::pointer_type pointer_type;
    typedef typename Base::const_pointer_type const_pointer_type;

    using Base::Base;
    using Base::begin;
    using Base::end;
    using Base::cbegin;
    using Base::cend;
    using Base::subView;

    reverse_iterator_type rbegin(const ytl::function<bool(const_reference)>& filter = null_ptr);
    reverse_iterator_type rend();
    const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    const_reverse_iterator_type crend() const;

protected:
    using Base::m_filter;
    using Base::m_iterable;
};

template<typename T, typename Traits>
class cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits> : protected cContainerViewImplBase<Traits>
{
    typedef cContainerViewImplBase<Traits> Base;
public:
    typedef typename Base::const_iterator_type const_iterator_type;
    typedef typename Traits::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename Base::const_reference const_reference;
    typedef typename Base::const_pointer_type const_pointer_type;

    using Base::Base;
    using Base::begin;
    using Base::end;
    using Base::cbegin;
    using Base::cend;
    using Base::subView;

    const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    const_reverse_iterator_type crend() const;

protected:
    using Base::m_filter;
    using Base::m_iterable;
};

template<typename T, typename Traits>
class cContainerViewImpl<cConstRandomAccessIterable<T>,Traits> : protected cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>
{
    typedef cContainerViewImpl<cConstBidirectionalIterable<T>,Traits> Base;
    using Base::m_filter;
    using Base::m_iterable;

public:
    typedef typename Base::iterator_type iterator_type;
    typedef typename Base::const_iterator_type const_iterator_type;
    typedef typename Base::reverse_iterator_type reverse_iterator_type;
    typedef typename Base::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename Base::reference reference;
    typedef typename Base::const_reference const_reference;
    typedef typename Base::pointer_type pointer_type;
    typedef typename Base::const_pointer_type const_pointer_type;

    using Base::begin;
    using Base::end;
    using Base::rbegin;
    using Base::rend;
    using Base::cbegin;
    using Base::cend;
    using Base::crbegin;
    using Base::crend;
    using Base::subView;

    cContainerViewImpl(cConstRandomAccessIterable<T>& i_iterable, const ytl::function<bool(const_reference)> i_filter = null_ptr);
    cContainerViewImpl(const cContainerViewImpl& other);
    cContainerViewImpl(cContainerViewImpl&& other);
    cContainerViewImpl<cConstRandomAccessIterable<T>,Traits> subView(size_t i_initPos, size_t i_length) const;
    cContainerViewImpl& operator=(const cContainerViewImpl& other);
    cContainerViewImpl& operator=(cContainerViewImpl&& other);
    reference operator[](size_t i_index);
    const_reference operator[](size_t i_index) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    iterator_type begin(const ytl::function<bool(const_reference)>& filter = null_ptr);
    iterator_type end();
    const_iterator_type cbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    const_iterator_type cend() const;
    size_t getSize() const;
    size_t empty() const;

private:
    cContainerViewImpl(const Base& i_base, size_t i_initPos, size_t i_length);

    size_t m_initOffset = 0;
    size_t m_endOffset = 0;
};

template<typename T, typename Traits>
class cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits> : protected cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits>
{
    typedef cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits> Base;
    using Base::m_filter;
    using Base::m_iterable;

public:
    typedef typename Base::iterator_type iterator_type;
    typedef typename Base::const_iterator_type const_iterator_type;
    typedef typename Base::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename Base::const_reference const_reference;
    typedef typename Base::const_pointer_type const_pointer_type;

    using Base::cbegin;
    using Base::cend;
    using Base::crbegin;
    using Base::crend;
    using Base::subView;

    cContainerViewImpl(const cConstRandomAccessIterable<T>& i_iterable, const ytl::function<bool(const_reference)> i_filter = null_ptr);
    cContainerViewImpl(const cContainerViewImpl& other);
    cContainerViewImpl(cContainerViewImpl&& other);
    cContainerViewImpl& operator=(const cContainerViewImpl& other);
    cContainerViewImpl& operator=(cContainerViewImpl&& other);
    cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits> subView(size_t i_initPos, size_t i_length) const;
    const_reference operator[](size_t i_index) const;
    const_reference front() const;
    const_reference back() const;
    const_iterator_type cbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    const_iterator_type cend() const;
    size_t getSize() const;
    size_t empty() const;

private:
    cContainerViewImpl(const Base& i_base, size_t i_initPos, size_t i_length);

    size_t m_initOffset = 0;
    size_t m_endOffset = 0;
};


}
}
}

#include "YTL/container/detail/cContainerViewImpl.tpp"
