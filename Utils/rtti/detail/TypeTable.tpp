
namespace yame
{
namespace rtti
{

template<typename T>
size_t TypeTable::addType()
{
    static const TypeId k_typeId = TypeInfo::getNextTypeId<T>();

    container::cUnorderedMap<TypeInfo,TypeId>::iterator_type itTypeTable = m_typeTable.search(k_typeId);

    ASSERT(itTypeTable == m_typeTable.end(), "You cannot register the same type twice!");

    m_typeTable.insert(k_typeId,TypeInfo());

    return k_typeId;
}
template<typename T,typename TT>
bool TypeTable::addTypeBinding()
{
    const TypeId k_lhsTypeId = TypeInfo::getNextTypeId<T>();
    const TypeId k_rhsTypeId = TypeInfo::getNextTypeId<TT>();

    container::cUnorderedMap<TypeInfo,TypeId>::iterator_type itTypeTable = m_typeTable.search(k_lhsTypeId);

    ASSERT(itTypeTable != m_typeTable.end(), "You cannot register the same type twice!");

    TypeInfo& currTypeInfo = itTypeTable->second;

    const TypeInfo::iterable_interface& bases = currTypeInfo.getBases();

    TypeInfo::const_iterator_type itBase = yame::find_if(bases.cbegin(),bases.cend(),[&k_rhsTypeId](const TypeInfo::TypeLayout& i_value){ return i_value.typeId == k_rhsTypeId; });

    ASSERT(itBase == bases.cend(), "You cannot register the same base against the same base twice!");

    //dont worry, we are ot accessig layout
    const T* lhsPtr = reinterpret_cast<T*>(0xDEAD);
    const TT* rhsPtr = lhsPtr;
    std::ptrdiff_t lhsRhsDiff = lhsPtr - rhsPtr;

    currTypeInfo.addBase(k_rhsTypeId,lhsRhsDiff);

    //back propagate
    const_iterator_type itBackTableBase = m_typeTable.search(k_rhsTypeId);
    if(itBackTableBase != m_typeTable.cend())
    {
        const TypeInfo& currBackTypeInfo = itBackTableBase->second;

        const TypeInfo::iterable_interface& backBases = currBackTypeInfo.getBases();
        TypeInfo::const_iterator_type itBackBase = backBases.cbegin();
        for(;itBackBase!=backBases.cend();++itBackBase)
        {
            const TypeInfo::TypeLayout& currBackTypeLayout = *itBackBase;

            currTypeInfo.addBase(k_rhsTypeId,lhsRhsDiff + currBackTypeLayout.offset);
        }
    }

    //forward propagate this new binding to its dependencies
    container::cUnorderedMap<TypeInfo,TypeId>::iterator_type itPropTypeTable = m_typeTable.begin();
    for(;itPropTypeTable!=m_typeTable.end();++itPropTypeTable)
    {
        TypeInfo& currPropTypeInfo = itPropTypeTable->second;

        const TypeInfo::iterable_interface& propBases = currPropTypeInfo.getBases();

        TypeInfo::const_iterator_type itPropBase = yame::find_if(propBases.cbegin(),propBases.cend(),[&k_lhsTypeId](const TypeInfo::TypeLayout& i_value){ return i_value.typeId == k_lhsTypeId; });

        if(itPropBase != propBases.cend())
        {
            const TypeInfo::TypeLayout& currPropTypeLayout = *itPropBase;

            currPropTypeInfo.addBase(k_rhsTypeId,currPropTypeLayout.offset + lhsRhsDiff);
        }
    }

    return true;
}

}
}
