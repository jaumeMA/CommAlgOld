#pragma once

#include "YTL/types/smartPtr/reference_counter.h"
#include "YTL/types/smartPtr/enable_ref_from_this.h"
#include "YTL/types/taggedPtr/tagged_pointer.h"

namespace yame
{
namespace ytl
{

#ifdef LENT_WITH_LOGIC

template<typename>
class lent_ref;
template<typename>
class unique_ptr;
template<typename>
class unique_ref;

template<typename T>
class lent_ptr
{
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
    template<typename TT>
    friend class lent_ptr;
    template<typename TT>
    friend lent_ref<TT> promote_to_ref(const lent_ptr<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ptr<TTT> dynamic_lent_cast(const lent_ptr<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ptr<TTT> static_lent_cast(const lent_ptr<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ptr<TTT> dynamic_lent_cast(const lent_ref<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ref<TTT> static_lent_cast(const lent_ref<TT>& other);
    template<typename TT>
    friend lent_ptr<TT> const_lent_cast(const lent_ptr<const TT>& other);
    template<typename TT>
    friend lent_ref<TT> const_lent_cast(const lent_ref<const TT>& other);

public:
	lent_ptr();
	lent_ptr(const lent_ptr& other);
	lent_ptr(lent_ptr&& other);
	template<typename TT>
	lent_ptr(const lent_ptr<TT>& other);
	template<typename TT>
	lent_ptr(lent_ptr<TT>&& other);
	template<typename TT>
	lent_ptr(const unique_ptr<TT>& other);
	~lent_ptr();
	lent_ptr& operator=(const mpl::null_ptr_type&);
	lent_ptr& operator=(const lent_ptr& other);
	lent_ptr& operator=(lent_ptr&& other);
	template<typename TT>
	lent_ptr& operator=(const lent_ptr<TT>& other);
	template<typename TT>
	lent_ptr& operator=(lent_ptr<TT>&& other);
	template<typename TT>
	lent_ptr<T>& operator=(const unique_ptr<TT>& other);
	bool operator==(mpl::null_ptr_type) const;
	bool operator==(const lent_ptr<T>& other) const;
	bool operator!=(mpl::null_ptr_type) const;
	bool operator!=(const lent_ptr<T>& other);
	T* operator->();
	const T* operator->() const;
	T& operator*();
	const T& operator*() const;
	operator bool() const;
	void clear();
    const T* getData() const;
    T* getData();
    tagged_reference_counter getReferenceCounter() const;

protected:
	lent_ptr(T* i_data, tagged_reference_counter i_refCounter);

private:

	tagged_reference_counter m_refCounter;
	T* m_data = NULL;
};

template<typename T>
class lent_ref : public lent_ptr<T>
{
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
    template<typename TT>
    friend lent_ref<TT> promote_to_ref(const lent_ptr<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ptr<TTT> dynamic_lent_cast(const lent_ptr<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ptr<TTT> static_lent_cast(const lent_ptr<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ptr<TTT> dynamic_lent_cast(const lent_ref<TT>& other);
    template<typename TTT, typename TT>
    friend lent_ref<TTT> static_lent_cast(const lent_ref<TT>& other);
    template<typename TT>
    friend lent_ptr<TT> const_lent_cast(const lent_ptr<const TT>& other);
    template<typename TT>
    friend lent_ref<TT> const_lent_cast(const lent_ref<const TT>& other);

	lent_ref(T* i_data, tagged_reference_counter i_refCounter);

public:
    using lent_ptr<T>::getData;
    using lent_ptr<T>::getReferenceCounter;
    using lent_ptr<T>::operator==;
    using lent_ptr<T>::operator!=;
    using lent_ptr<T>::operator*;
    using lent_ptr<T>::operator->;
    using lent_ptr<T>::operator=;

    template<typename TT>
    lent_ref(const lent_ref<TT>& other);
    template<typename TT>
    lent_ref(lent_ref<TT>&& other);
    template<typename TT>
    lent_ref(const unique_ref<TT>& other);
    lent_ref(T *pData, enable_ref_from_this& referenceCounter);
};

template<typename T>
inline lent_ref<T> promote_to_ref(const lent_ptr<T>& other);
template<typename TT, typename T>
lent_ptr<TT> dynamic_lent_cast(const lent_ptr<T>& other);
template<typename TT, typename T>
lent_ptr<TT> static_lent_cast(const lent_ptr<T>& other);
template<typename TT, typename T>
lent_ptr<TT> dynamic_lent_cast(const lent_ref<T>& other);
template<typename TT, typename T>
lent_ref<TT> static_lent_cast(const lent_ref<T>& other);
template<typename T>
lent_ptr<T> const_lent_cast(const lent_ptr<const T>& other);
template<typename T>
lent_ref<T> const_lent_cast(const lent_ref<const T>& other);
template<typename T>
lent_ref<T> lend(T& other, const typename T::lendable_tag* _foo = NULL);

#else

template<typename T>
using lent_ptr = T*;

template<typename T>
using lent_ref = T*;

template<typename>
class unique_ptr;
template<typename>
class unique_ref;

template<typename T>
lent_ref<T> promote_to_ref(const lent_ptr<T>& other);
template<typename TT, typename T>
lent_ptr<TT> dynamic_lent_cast(const lent_ptr<T>& other);
template<typename TT, typename T>
lent_ptr<TT> static_lent_cast(const lent_ptr<T>& other);
template<typename T>
lent_ptr<T> const_lent_cast(const lent_ptr<const T>& other);
template<typename T>
lent_ref<T> lend(T& other, const typename T::lendable_tag* _foo = NULL);

#endif

}
}

#include "YTL/types/smartPtr/detail/lent_ptr.tpp"
