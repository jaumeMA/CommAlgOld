
namespace yame
{
namespace rtti
{
namespace detail
{

inline TypeId _getNextTypeId()
{
    static TypeId s_currTypeId = 0;

    return s_currTypeId++;
}
template<typename T>
TypeId getNextTypeId()
{
    static TypeId s_currTypeId = _getNextTypeId();

    return s_currTypeId++;
}
template<typename T, typename TT>
const TypeInfo* __get_rtti_type_info(const T*,const TT&)
{
    static const TypeInfo builtinTypeInfo = TypeInfo(yame::container::string(typeid(T).name()),getNextTypeId<T>(), {}, {});

    return &builtinTypeInfo;
}
template<typename T>
const TypeInfo* _getTypeInfo(typename T::rtti_members*)
{
    return __get_rtti_type_info(static_cast<const T*>(NULL),__get_rtti_members(static_cast<const T*>(NULL)));
}
template<typename T>
const TypeInfo* _getTypeInfo(...)
{
    typedef yame::container::cArray<yame::container::cPair<yame::container::string,const yame::rtti::TypeInfo*>> __rtti_members;
    return __get_rtti_type_info(static_cast<const T*>(NULL),__rtti_members{});
}

}

template<typename TT, typename T>
TT& access(T* i_ptr, const yame::container::string& i_memberName)
{
    if(size_t T::* memberPtr = __get_rtti_member_address(static_cast<const T*>(NULL),i_memberName))
    {
        return i_ptr->*reinterpret_cast<TT T::*>(memberPtr);
    }
    else
    {
        return *reinterpret_cast<TT*>(0xDEAD);
    }
}
template<typename TT, typename T>
const TT& access(const T* i_ptr, const yame::container::string& i_memberName)
{
    if(size_t T::* memberPtr = __get_rtti_member_address(static_cast<const T*>(NULL),i_memberName))
    {
        return i_ptr->*reinterpret_cast<TT T::*>(memberPtr);
    }
    else
    {
        return *reinterpret_cast<const TT*>(0xDEAD);
    }
}
template<typename Return, typename T, typename ... Args>
Return execute(T* i_ptr, const yame::container::string& i_methodName, Args&& ... i_args)
{
    typedef Return (T::*rtti_func_pointer_type)(Args...); \

    if(rtti_func_pointer_type methodPtr = reinterpret_cast<rtti_func_pointer_type>(__get_rtti_method_address(static_cast<const T*>(NULL),i_methodName)))
    {
        return (i_ptr->*methodPtr)(mpl::forward<Args>(i_args)...);
    }
    else
    {
        return *reinterpret_cast<Return*>(0xDEAD);
    }
}
template<typename Return, typename T, typename ... Args>
Return execute(const T* i_ptr, const yame::container::string& i_methodName, Args&& ... i_args)
{
    typedef Return (T::*const rtti_func_pointer_type)(Args...); \

    if(rtti_func_pointer_type methodPtr = reinterpret_cast<rtti_func_pointer_type>(__get_rtti_method_address(static_cast<const T*>(NULL),i_methodName)))
    {
        return (i_ptr->*methodPtr)(mpl::forward<Args>(i_args)...);
    }
    else
    {
        return *reinterpret_cast<Return*>(0xDEAD);
    }
}
template<typename T>
TypeInfo getTypeInfo()
{
    static const TypeInfo* res = detail::_getTypeInfo<T>(NULL);

    return *res;
}

}
}

