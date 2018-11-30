#pragma once

#include "YTL/types/smartPtr/lent_ptr.h"
#include "YTL/types/smartPtr/unique_ptr.h"
#include "YTL/types/smartPtr/shared_ptr.h"
#include "YTL/container/cString.h"

namespace yame
{
namespace rtti
{

template<typename T, typename TT>
T& access(ytl::unique_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName);
template<typename T, typename TT>
const T& access(const ytl::unique_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName);
template<typename Return, typename T, typename ... Args>
Return execute(ytl::unique_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args);
template<typename Return, typename T, typename ... Args>
Return execute(const ytl::unique_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args);

#ifdef LENT_WITH_LOGIC

template<typename T, typename TT>
T& access(ytl::lent_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName);
template<typename T, typename TT>
const T& access(const ytl::lent_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName);
template<typename Return, typename T, typename ... Args>
Return execute(ytl::lent_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args);
template<typename Return, typename T, typename ... Args>
Return execute(const ytl::lent_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args);

#endif

template<typename T, typename TT>
T& access(ytl::shared_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName);
template<typename T, typename TT>
const T& access(const ytl::shared_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName);
template<typename Return, typename T, typename ... Args>
Return execute(ytl::shared_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args);
template<typename Return, typename T, typename ... Args>
Return execute(const ytl::shared_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args);

}
}

#include "YTL/types/smartPtr/detail/ptr_utils.tpp"
