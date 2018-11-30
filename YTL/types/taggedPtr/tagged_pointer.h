#pragma once

#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace ytl
{

#define MASK_NUM_BITS 2

template<typename T>
class tagged_pointer
{
	static const short tag_mask = (1 << MASK_NUM_BITS) - 1;
	typedef T* interface_ptr;
	static const size_t bitMask = ((unsigned long long) (1) << ((unsigned long long) (sizeof(interface_ptr)) << 3)) - (tag_mask + 1); //in 64 bit systems we have to use extra large units so it does not overflow

	template<typename TT>
	friend class tagged_pointer;
	template<typename TT, typename TTT>
	friend tagged_pointer<TTT> dynamic_tagged_cast(const tagged_pointer<TT>&);

    friend inline tagged_pointer<T> operator<<(void* i_lhs, const tagged_pointer<T>& i_rhs)
    {
        memcpy(reinterpret_cast<char*>(i_lhs), &i_rhs.m_data, sizeof(interface_ptr));

        return i_rhs;
    }
    friend inline tagged_pointer<T> operator>>(const void* i_lhs, tagged_pointer<T>& i_rhs)
    {
        const tagged_pointer<T>* tmp = reinterpret_cast<const tagged_pointer<T>*>(i_lhs);

        if(tmp->empty() == false)
        {
            i_rhs = (tmp->is_tagged()) ? *tmp : reinterpret_cast<T*>(const_cast<void*>(i_lhs));
        }

        return i_rhs;
    }

public:
	typedef T raw_type;
    typedef typename mpl::add_pointer<T>::type value_pointer;
    typedef typename mpl::add_constness<value_pointer>::type value_const_pointer;
    typedef typename mpl::add_reference<T>::type value_reference;
    typedef typename mpl::add_constness<value_reference>::type value_const_reference;

	tagged_pointer();
	tagged_pointer(T* i_data);
	tagged_pointer(T* i_data, short i_tag);
	template<typename TT>
	tagged_pointer(tagged_pointer<TT> other);
	tagged_pointer& operator=(const tagged_pointer& other);
	template<typename TT>
	tagged_pointer& operator=(const tagged_pointer<TT>& other);
	short get_tag() const;
	bool is_tagged() const;
	value_pointer operator->();
	value_const_pointer operator->() const;
	value_reference operator*();
	value_const_reference operator*() const;
	value_pointer get_pointer();
	value_const_pointer get_pointer() const;
    tagged_pointer extract();
	value_pointer extract_pointer();
    bool empty() const;
    void destroy();
	bool operator==(const tagged_pointer& other) const;
	bool operator!=(const tagged_pointer& other) const;
    explicit operator bool() const;

private:
	value_pointer untagged_pointer();
	value_const_pointer untagged_pointer() const;

	T* m_data;
};

template<typename T>
tagged_pointer<T> make_tagged_pointer(short i_tag);
template<typename T, typename ... Args>
tagged_pointer<T> make_tagged_pointer(Args&& ... i_val, short i_tag);
template<typename TT, typename T>
tagged_pointer<TT> dynamic_tagged_cast(const tagged_pointer<T>& i_ptr);

}
}

#include "YTL/types/taggedPtr/detail/tagged_pointer.tpp"
