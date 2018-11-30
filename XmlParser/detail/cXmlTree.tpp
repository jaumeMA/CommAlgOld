
namespace yame
{
namespace xml
{

template<template<typename> class A>
cXmlValue<A>::cXmlValue()
: MapValue(detail::xmlValueData())
{
}
template<template<typename> class A>
cXmlValue<A>::cXmlValue(container::string tag, container::cArray<cProperty> properties)
: MapValue(detail::xmlValueData())
{
    detail::xmlValueData& xmlData = this->getValue();

    xmlData.m_tag = tag;
    xmlData.m_properties = properties;
}
template<template<typename> class A>
bool cXmlValue<A>::operator==(const cXmlValue& other) const
{
    const detail::xmlValueData& xmlData = this->getValue();
    const detail::xmlValueData& otherXmlData = other.getValue();

    return (xmlData.m_tag == otherXmlData.m_tag) &&
            (xmlData.m_properties == otherXmlData.m_properties) &&
            (xmlData.m_content == otherXmlData.m_content);
}
template<template<typename> class A>
const container::string& cXmlValue<A>::getTag() const
{
    const detail::xmlValueData& xmlData = this->getValue();

    return xmlData.m_tag;
}
template<template<typename> class A>
const container::string& cXmlValue<A>::getContent() const
{
    const detail::xmlValueData& xmlData = this->getValue();

    return xmlData.m_content;
}
template<template<typename> class A>
void cXmlValue<A>::append(container::string content)
{
    detail::xmlValueData& xmlData = this->getValue();

    xmlData.m_content += content;

    return;
}
template<template<typename> class A>
const cProperty& cXmlValue<A>::getProperty(size_t i) const
{
    const detail::xmlValueData& xmlData = this->getValue();

    return xmlData.m_properties[i];
}
template<template<typename> class A>
const container::cArray<cProperty>& cXmlValue<A>::getProperties() const
{
    const detail::xmlValueData& xmlData = this->getValue();

    return xmlData.m_properties;
}
template<template<typename> class A>
size_t cXmlValue<A>::getNumProperties() const
{
    const detail::xmlValueData& xmlData = this->getValue();

    return xmlData.m_properties.getSize();
}

}
}
