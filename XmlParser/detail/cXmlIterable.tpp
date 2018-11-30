
namespace yame
{
namespace xml
{

template<template<typename> class A>
cXmlIterable<A>::cXmlIterable(const cXmlTree<A>& i_xmlTree)
: container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>(i_xmlTree,&cXmlIterable<A>::fromXmlValueDataToXmlData)
{
}
template<template<typename> class A>
cXmlData cXmlIterable<A>::fromXmlValueDataToXmlData(container::cPair<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type>& i_val)
{
    size_t totalDepth = 0;
    const MapNode* currMapNode = &(static_cast<const MapNode&>(i_val));

    while(currMapNode != NULL)
    {
        totalDepth++;

        if(const typename inner_value_type::second_type* parentNode = currMapNode->second.getHolder())
        {
            currMapNode = parentNode->getNodeLocation();
        }
        else
        {
            break;
        }
    }

    return cXmlData(i_val.second.getTag(),i_val.second.getProperties(),i_val.second.getContent(),totalDepth);
}

}
}
