
#include "Utils/rtti/TypeAccess.h"

namespace yame
{
namespace rtti
{

template<typename T, typename TT>
T& access(ytl::unique_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName)
{
    return rtti::access<T>(i_smartPtr.getData(),i_memberName);
}
template<typename T, typename TT>
const T& access(const ytl::unique_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName)
{
    return rtti::access<T>(i_smartPtr.getData(),i_memberName);
}
template<typename Return, typename T, typename ... Args>
Return execute(ytl::unique_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args)
{
    return rtti::execute<Return>(i_smartPtr.getData(),i_memberName,mpl::forward<Args>(i_args)...);
}
template<typename Return, typename T, typename ... Args>
Return execute(const ytl::unique_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args)
{
    return rtti::execute<Return>(i_smartPtr.getData(),i_memberName,mpl::forward<Args>(i_args)...);
}

#ifdef LENT_WITH_LOGIC

template<typename T, typename TT>
T& access(ytl::lent_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName)
{
    return rtti::access<T>(i_smartPtr.getData(),i_memberName);
}
template<typename T, typename TT>
const T& access(const ytl::lent_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName)
{
    return rtti::access<T>(i_smartPtr.getData(),i_memberName);
}
template<typename Return, typename T, typename ... Args>
Return execute(ytl::lent_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args)
{
    return rtti::execute<Return>(i_smartPtr.getData(),i_memberName,mpl::forward<Args>(i_args)...);
}
template<typename Return, typename T, typename ... Args>
Return execute(const ytl::lent_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args)
{
    return rtti::execute<Return>(i_smartPtr.getData(),i_memberName,mpl::forward<Args>(i_args)...);
}

#endif

template<typename T, typename TT>
T& access(ytl::shared_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName)
{
    return rtti::access<T>(i_smartPtr.getData(),i_memberName);
}
template<typename T, typename TT>
const T& access(const ytl::shared_ptr<TT>& i_smartPtr, const yame::container::string& i_memberName)
{
    return rtti::access<T>(i_smartPtr.getData(),i_memberName);
}
template<typename Return, typename T, typename ... Args>
Return execute(ytl::shared_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args)
{
    return rtti::execute<Return>(i_smartPtr.getData(),i_memberName,mpl::forward<Args>(i_args)...);
}
template<typename Return, typename T, typename ... Args>
Return execute(const ytl::shared_ptr<T>& i_smartPtr, const yame::container::string& i_memberName, Args&& ... i_args)
{
    return rtti::execute<Return>(i_smartPtr.getData(),i_memberName,mpl::forward<Args>(i_args)...);
}

}
}
