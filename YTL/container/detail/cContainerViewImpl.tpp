
#include "YTL/mpl/cTemplateHelper.h"
#include "YTL/functional/cFunctionOps.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename Traits>
cContainerViewImplBase<Traits>::cContainerViewImplBase(iterable_type& i_iterable, const ytl::function<bool(const_reference)> i_filter)
: m_iterable(ytl::lend(i_iterable))
, m_filter(i_filter)
{
}
template<typename Traits>
cContainerViewImplBase<Traits>::cContainerViewImplBase(const cContainerViewImplBase& other)
: m_iterable(other.m_iterable)
, m_filter(other.m_filter)
{
}
template<typename Traits>
cContainerViewImplBase<Traits>::cContainerViewImplBase(cContainerViewImplBase&& other)
: m_iterable(mpl::move(other.m_iterable))
, m_filter(mpl::move(other.m_filter))
{
}
template<typename Traits>
cContainerViewImplBase<Traits>::~cContainerViewImplBase()
{
}
template<typename Traits>
cContainerViewImplBase<Traits>& cContainerViewImplBase<Traits>::operator=(iterable_type& i_iterable)
{
    m_iterable = ytl::lend(i_iterable);
    m_filter = null_ptr;

    return *this;
}
template<typename Traits>
cContainerViewImplBase<Traits>& cContainerViewImplBase<Traits>::operator=(const cContainerViewImplBase& other)
{
    m_iterable = other.m_iterable;
    m_filter = other.m_filter;

    return *this;
}
template<typename Traits>
cContainerViewImplBase<Traits>& cContainerViewImplBase<Traits>::operator=(cContainerViewImplBase&& other)
{
    m_iterable = mpl::move(other.m_iterable);
    m_filter = mpl::move(other.m_filter);

    return *this;
}
template<typename Traits>
typename cContainerViewImplBase<Traits>::iterator_type cContainerViewImplBase<Traits>::begin(const ytl::function<bool(const_reference)>& filter)
{
    return m_iterable->begin(m_filter && filter);
}
template<typename Traits>
typename cContainerViewImplBase<Traits>::iterator_type cContainerViewImplBase<Traits>::end()
{
    return m_iterable->end();
}
template<typename Traits>
typename cContainerViewImplBase<Traits>::const_iterator_type cContainerViewImplBase<Traits>::cbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return m_iterable->cbegin(m_filter && filter);
}
template<typename Traits>
typename cContainerViewImplBase<Traits>::const_iterator_type cContainerViewImplBase<Traits>::cend() const
{
    return m_iterable->cend();
}
template<typename Traits>
size_t cContainerViewImplBase<Traits>::getSize() const
{
    return yame::distance(m_iterable->cbegin(m_filter), m_iterable->cend());
}
template<typename Traits>
size_t cContainerViewImplBase<Traits>::empty() const
{
    return m_iterable->cbegin(m_filter) == m_iterable->cend();
}
template<typename Traits>
cContainerViewImplBase<Traits> cContainerViewImplBase<Traits>::subView(const ytl::function<bool(const_reference)>& i_filter) const
{
    return cContainerViewImplBase<Traits>(m_iterable,m_filter && i_filter);
}

template<typename T, typename Traits>
typename cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::reverse_iterator_type cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::rbegin(const ytl::function<bool(const_reference)>& filter)
{
    return m_iterable->rbegin(m_filter && filter);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::reverse_iterator_type cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::rend()
{
    return m_iterable->rend();
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::const_reverse_iterator_type cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::crbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return m_iterable->crbegin(m_filter && filter);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::const_reverse_iterator_type cContainerViewImpl<cConstBidirectionalIterable<T>,Traits>::crend() const
{
    return m_iterable->crend();
}

template<typename T, typename Traits>
typename cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits>::const_reverse_iterator_type cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits>::crbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return m_iterable->crbegin(m_filter && filter);
}
template<typename T, typename Traits>
typename cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits>::const_reverse_iterator_type cContainerViewImpl<const cConstBidirectionalIterable<T>,Traits>::crend() const
{
    return m_iterable->crend();
}

template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(cConstRandomAccessIterable<T>& i_iterable, const ytl::function<bool(const_reference)> i_filter)
: Base(i_iterable,i_filter)
, m_initOffset(0)
, m_endOffset(0)
{
}
template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(const Base& i_base, size_t i_initPos, size_t i_length)
: Base(i_base)
, m_initOffset(i_initPos)
, m_endOffset(Base::getSize() - (i_initPos + i_length))
{
}
template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(const cContainerViewImpl& other)
: Base(other)
, m_initOffset(other.m_initOffset)
, m_endOffset(other.m_endOffset)
{
}
template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(cContainerViewImpl&& other)
: Base(mpl::move(other))
, m_initOffset(other.m_initOffset)
, m_endOffset(other.m_endOffset)
{
}
template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>& cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::operator=(const cContainerViewImpl& other)
{
    Base::operator=(other);

    m_initOffset = other.m_initOffset;
    m_endOffset = other.m_endOffset;

    return *this;
}
template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>& cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::operator=(cContainerViewImpl&& other)
{
    Base::operator=(mpl::move(other));

    m_initOffset = other.m_initOffset;
    m_endOffset = other.m_endOffset;

    return *this;
}
template<typename T, typename Traits>
cContainerViewImpl<cConstRandomAccessIterable<T>,Traits> cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::subView(size_t i_initPos, size_t i_length) const
{
    return cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>(*this,m_initOffset+i_initPos,i_length);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::reference cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::operator[](size_t i_index)
{
    return *(this->m_iterable->begin(m_filter) + m_initOffset + i_index);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::const_reference cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::operator[](size_t i_index) const
{
    return *(this->m_iterable->begin(m_filter) + m_initOffset + i_index);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::reference cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::front()
{
    return *(this->m_iterable->begin(m_filter) + m_initOffset);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::const_reference cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::front() const
{
    return *(this->m_iterable->begin(m_filter) + m_initOffset);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::reference cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::back()
{
    return *(this->m_iterable->rbegin(m_filter) + m_endOffset);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::const_reference cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::back() const
{
    return *(this->m_iterable->crbegin(m_filter) + m_endOffset);
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::iterator_type cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::begin(const ytl::function<bool(const_reference)>& filter)
{
    return this->m_iterable->begin(m_filter) + m_initOffset;
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::iterator_type cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::end()
{
    return (m_endOffset == 0) ? this->m_iterable->end() : iterator_type(this->m_iterable->rbegin(m_filter) + (m_endOffset-1));
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::const_iterator_type cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::cbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return this->m_iterable->cbegin(m_filter) + m_initOffset;
}
template<typename T, typename Traits>
typename cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::const_iterator_type cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::cend() const
{
    return (m_endOffset == 0) ? this->m_iterable->cend() : const_iterator_type(this->m_iterable->crbegin(m_filter) + (m_endOffset-1));
}
template<typename T, typename Traits>
size_t cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::getSize() const
{
    const const_iterator_type initIterator = this->m_iterable->cbegin(m_filter) + m_initOffset;
    const const_iterator_type endIterator = (m_endOffset == 0) ? this->m_iterable->cend() : const_iterator_type(this->m_iterable->crbegin(m_filter) + (m_endOffset-1));

    return yame::distance(initialize,endIterator);
}
template<typename T, typename Traits>
size_t cContainerViewImpl<cConstRandomAccessIterable<T>,Traits>::empty() const
{
    const const_iterator_type initIterator = this->m_iterable->cbegin(m_filter) + m_initOffset;
    const const_iterator_type endIterator = (m_endOffset == 0) ? this->m_iterable->cend() : const_iterator_type(this->m_iterable->crbegin(m_filter) + (m_endOffset-1));

    return initIterator == endIterator;
}

template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(const cConstRandomAccessIterable<T>& i_iterable, const ytl::function<bool(const_reference)> i_filter)
: Base(i_iterable,i_filter)
, m_initOffset(0)
, m_endOffset(0)
{
}
template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(const Base& i_base, size_t i_initPos, size_t i_length)
: Base(i_base)
, m_initOffset(i_initPos)
, m_endOffset(Base::getSize() - (i_initPos + i_length))
{
}
template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(const cContainerViewImpl& other)
: Base(other)
, m_initOffset(other.m_initOffset)
, m_endOffset(other.m_endOffset)
{
}
template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::cContainerViewImpl(cContainerViewImpl&& other)
: Base(mpl::move(other))
, m_initOffset(other.m_initOffset)
, m_endOffset(other.m_endOffset)
{
}
template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>& cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::operator=(const cContainerViewImpl& other)
{
    cContainerViewImplBase<Traits>::operator=(other);

    m_initOffset = other.m_initOffset;
    m_endOffset = other.m_endOffset;

    return *this;
}
template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>& cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::operator=(cContainerViewImpl&& other)
{
    cContainerViewImplBase<Traits>::operator=(mpl::move(other));

    m_initOffset = other.m_initOffset;
    m_endOffset = other.m_endOffset;

    return *this;
}
template<typename T, typename Traits>
cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits> cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::subView(size_t i_initPos, size_t i_length) const
{
    return cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>(*this,m_initOffset+i_initPos,i_length);
}
template<typename T, typename Traits>
typename cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::const_reference cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::operator[](size_t i_index) const
{
    return *(this->m_iterable->cbegin(m_filter) + m_initOffset + i_index);
}
template<typename T, typename Traits>
typename cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::const_reference cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::front() const
{
    return *(this->m_iterable->cbegin(m_filter) + m_initOffset);
}
template<typename T, typename Traits>
typename cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::const_reference cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::back() const
{
    return *(this->m_iterable->crbegin(m_filter) + m_endOffset);
}
template<typename T, typename Traits>
typename cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::const_iterator_type cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::cbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return this->m_iterable->cbegin(m_filter) + m_initOffset;
}
template<typename T, typename Traits>
typename cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::const_iterator_type cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::cend() const
{
    return (m_endOffset == 0) ? this->m_iterable->cend() : const_iterator_type(this->m_iterable->crbegin(m_filter) + (m_endOffset-1));
}
template<typename T, typename Traits>
size_t cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::getSize() const
{
    const const_iterator_type initIterator = this->m_iterable->cbegin(m_filter) + m_initOffset;
    const const_iterator_type endIterator = (m_endOffset == 0) ? this->m_iterable->cend() : const_iterator_type(this->m_iterable->crbegin(m_filter) + (m_endOffset-1));

    return yame::distance(initIterator,endIterator);
}
template<typename T, typename Traits>
size_t cContainerViewImpl<const cConstRandomAccessIterable<T>,Traits>::empty() const
{
    const const_iterator_type initIterator = this->m_iterable->cbegin(m_filter) + m_initOffset;
    const const_iterator_type endIterator = (m_endOffset == 0) ? this->m_iterable->cend() : const_iterator_type(this->m_iterable->crbegin(m_filter) + (m_endOffset-1));

    return initIterator == endIterator;
}

}
}
}
