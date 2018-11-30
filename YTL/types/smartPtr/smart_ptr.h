#pragma once

#include "shared_reference_wrapper.h"
#include "unique_reference_wrapper.h"
#include "lent_reference_wrapper.h"

namespace ewas
{

template<typename TT, typename T>
shared_reference_wrapper<TT> dynamic_shared_cast(const shared_reference_wrapper<T>& i_sharedRef)
{
	static_assert(std::is_base_of<T,TT>::value || std::is_base_of<TT,T>::value, "Trying to cast unrelated classes");

	if (TT* tData = dynamic_cast<TT*>(i_sharedRef.m_data))
	{
		return shared_reference_wrapper<TT>(tData,i_sharedRef.m_refCounter);
	}
	else
	{
		return nullptr;
	}
}

template<typename TT, typename T>
unique_reference_wrapper<TT> dynamic_unique_cast(unique_reference_wrapper<T>&& i_uniqueRef)
{
	static_assert(std::is_base_of<T,TT>::value || std::is_base_of<TT,T>::value, "Trying to cast unrelated classes");

	unique_reference_wrapper<TT> res;

	if (TT* tData = dynamic_cast<TT*>(i_sharedRef.m_data))
	{
		res = unique_reference_wrapper<TT>(tData,i_sharedRef.m_refCounter);

		i_uniqueRef.m_data = NULL;
		i_uniqueRef.m_refCounter = NULL;
	}
	else
	{
		res = nullptr;
	}

	return res;
}

template<typename TT, typename T>
lent_reference_wrapper<TT> dynamic_lent_cast(const lent_reference_wrapper<T>& i_lentRef)
{
	static_assert(std::is_base_of<T,TT>::value || std::is_base_of<TT,T>::value, "Trying to cast unrelated classes");

	if (TT* tData = dynamic_cast<TT*>(i_sharedRef.m_data))
	{
		return lent_reference_wrapper<TT>(tData,i_sharedRef.m_refCounter);
	}
	else
	{
		return nullptr;
	}
}

}