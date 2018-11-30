#pragma once

#include "YTL/types/smartPtr/reference_counter.h"
#include "YTL/types/smartPtr/smart_deleter.h"
#include "YTL/types/taggedPtr/tagged_pointer.h"

namespace yame
{
namespace ytl
{

template<typename T>
class shared_ptr
{
	typedef shared_reference_counter* shared_reference_counter_ptr;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
	template<typename>
	friend class shared_ptr;
	template<typename TT, typename ... Args>
	friend shared_ptr<TT> make_shared_reference(Args&& ...);
	template<typename TT,typename TTT>
	friend shared_ptr<TTT> dynamic_shared_cast(const shared_ptr<TT>&);

public:
	shared_ptr();
	shared_ptr(T* i_data, ISmartPtrDeleter* i_refDeleter = NULL);
	shared_ptr(const shared_ptr& other);
	shared_ptr(shared_ptr&& other);
	template<typename TT>
	shared_ptr(const shared_ptr<TT>& other);
	template<typename TT>
	shared_ptr(shared_ptr<TT>&& other);
	~shared_ptr();
	shared_ptr& operator=(const std::nullptr_t&);
	shared_ptr& operator=(const shared_ptr& other);
	shared_ptr& operator=(shared_ptr&& other);
	template<typename TT>
	shared_ptr& operator=(const shared_ptr<TT>& other);
	template<typename TT>
	shared_ptr& operator=(shared_ptr<TT>&& other);
	bool operator==(std::nullptr_t) const;
	bool operator==(const shared_ptr<T>& other) const;
	bool operator!=(std::nullptr_t) const;
	bool operator!=(const shared_ptr<T>& other);
	T* operator->();
	const T* operator->() const;
	T& operator*();
	const T& operator*() const;
	operator bool() const;
	void clear();

private:
	shared_ptr(T* i_data, tagged_reference_counter i_refCounter);
	shared_ptr(T* i_data, shared_reference_counter* i_refCounter);
	void clearIfCounterVoid();

	tagged_reference_counter m_refCounter;
	T* m_data;
	ISmartPtrDeleter* m_deleter;
};

template<typename T, typename ... Args>
shared_ptr<T> make_shared_reference(Args&& ... i_args);

}
}

#include "YTL/types/smartPtr/detail/shared_ptr.tpp"
