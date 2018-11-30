
#include "YTL/container/cAlgorithm.h"

namespace yame
{
namespace container
{

template<typename A>
void cString<A>::endString()
{
    if(this->m_size > 0)
    {
        cArray<char,A>::push_back('\0');

        this->m_size--;
    }

    return;
}
template<typename A>
size_t cString<A>::getLengthStringArray(const char *data) const
{
    size_t res = 0;
    char invalidValue = '\0';

    while(data+res && memcmp(data+res,&invalidValue,sizeof(char)) != 0)
    {
        res++;
    }

    return res;
}
template<typename A>
bool cString<A>::isStaticAddress(const char* address) const
{
    return address == m_string;
}
template<typename A>
bool cString<A>::isStaticAddress() const
{
    return this->template getTypedRootNode<value_type>() == m_string;
}
template<typename A>
bool cString<A>::isInBounds(const char* itPos) const
{
    const bool isBounded = isStaticAddress() || cArray<char,A>::isInBounds(itPos);

    ASSERT(isBounded, "Bad bounds!");

    return isBounded;
}
template<typename A>
char* cString<A>::acquireMem(size_t size)
{
    return (size <= NUM_STACK_CHARS) ? m_string : cArray<char,A>::acquireMem(size);
}
template<typename A>
void cString<A>::releaseMem(char* node)
{
    if(!isStaticAddress(node))
    {
        cArray<char,A>::releaseMem(node);
    }

    return;
}
template<typename A>
cString<A>::cString() : cArray<char,A>()
{
    memset(m_string,0,sizeof(char)*NUM_STACK_CHARS);
    this->m_size = 0;
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(m_string));
    this->m_capacity = NUM_STACK_CHARS;
}
template<typename A>
cString<A>::cString(const cArray<char,A>& other)
{
    this->m_size = other.getSize();
    pointer_type newRootNode = cArray<char,A>::allocate(other.getCapacity());
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(newRootNode));
    this->m_capacity = (isStaticAddress(newRootNode)) ? NUM_STACK_CHARS : other.getCapacity();

    //in this case since we handle chars no problem by memcpying
    memcpy(newRootNode,other.getArray(),sizeof(char)*other.getSize());

    endString();
}
template<typename A>
cString<A>::cString(cArray<char,A>&& other) : cArray<char,A>(mpl::move(other))
{
    endString();
}
template<typename A>
cString<A>::cString(const cString<A>& other)
{
    this->m_size = other.getSize();
    pointer_type newRootNode = cArray<char,A>::allocate(other.getCapacity());
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(newRootNode));
    this->m_capacity = (isStaticAddress(newRootNode)) ? NUM_STACK_CHARS : other.getCapacity();

    //in this case since we handle chars no problem by memcpying
    memcpy(newRootNode,other.getStr(),sizeof(char)*other.getSize());

    endString();
}
template<typename A>
cString<A>::cString(cString<A>&& other)
: cArray<char,A>()
{
    //be careful here, could happen that passes us an address pointer to other m_string and so we cannot move this
    if(other.isStaticAddress())
    {
        //pass it as normal reference
        this->m_size = other.getSize();
        pointer_type newRootNode = cArray<char,A>::allocate(other.getCapacity());
        this->setRootNode(newRootNode);
        this->m_capacity = (isStaticAddress(newRootNode)) ? NUM_STACK_CHARS : other.getCapacity();

        //in this case since we handle chars no problem by memcpying
        memcpy(newRootNode,other.getStr(),sizeof(char)*other.getSize());

        endString();

        //reset other data
        other.setRootNode(this->getHead());
        other.m_capacity = 0;
        other.m_size = 0;
    }
    else
    {
        cArray<char,A>::operator=(mpl::move(other));
    }
}
template<typename A>
cString<A>::cString(const const_string_view& str)
: cArray<char,A>(str)
{
}
template<typename A>
cString<A>::cString(const string_view& str)
: cArray<char,A>(str)
{
}
template<typename A>
cString<A>::cString(const char *data)
{
    u32 dataSize = getLengthStringArray(data);
    this->m_size = dataSize;
    pointer_type rootNode = acquireMem(dataSize+1);
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(rootNode));
    this->m_capacity = (isStaticAddress(rootNode)) ? NUM_STACK_CHARS : dataSize+1;

    //in this case since we handle chars no problem by memcpying
    memcpy(rootNode,data,sizeof(char)*dataSize);

    endString();
}
template<typename A>
cString<A>::cString(const char *data, u32 length)
{
    this->m_size = length;
    pointer_type rootNode = acquireMem(length+1);
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(rootNode));
    this->m_capacity = (isStaticAddress(rootNode)) ? NUM_STACK_CHARS : length+1;

    //in this case since we handle chars no problem by memcpying
    memcpy(rootNode,data,sizeof(char)*length);

    endString();
}
template<typename A>
cString<A>::cString(char value, u16 length)
{
    this->m_size = length;
    this->m_capacity = length+1;
    pointer_type rootNode = acquireMem(length+1);
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(rootNode));

    pointer_type currChar = detail::receiveNodeFromIterator<pointer_type>(this->getFirstElem());

    for(u32 i=0;i<this->m_size;i++)
    {
        *(currChar + i) = value;
    }

    endString();
}
template<typename A>
cString<A>::cString(const std::string& str)
{
    u32 strSize = str.size();
    this->m_size = strSize;
    this->m_capacity = strSize+1;
    pointer_type rootNode = acquireMem(strSize+1);
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(rootNode));

    //in this case since we handle chars no problem by memcpying
    memcpy(rootNode,str.c_str(),sizeof(char)*strSize);

    endString();
}
template<typename A>
cString<A>::~cString()
{
    if(isStaticAddress(detail::receiveNodeFromIterator<pointer_type>(this->getRootNode())))
    {
        //in case we are handling stacked data do not free a shit
        this->setRootNode(this->getHead());
    }
}
template<typename A>
void cString<A>::resize(u16 size, u16 initPos)
{
    cArray<char,A>::resize(size);

    endString();

    return;
}
template<typename A>
const char* cString<A>::getStr() const
{
    return cArray<char,A>::getArray();
}
template<typename A>
char* cString<A>::getStr()
{
    return cArray<char,A>::getArray();
}
template<typename A>
cString<A> cString<A>::subStr(const u32 initPos, const u32 length) const
{
    return cArray<char,A>::subArr(initPos,length);
}
template<typename A>
cString<A> cString<A>::subStr(const u32 initPos) const
{
    return cArray<char,A>::subArr(initPos,cArray<char,A>::getSize() - initPos);
}
template<typename A>
std::string cString<A>::getStdString() const
{
    std::string res(this->getRootNode());

    return res;
}
template<typename A>
template<typename ... T>
void cString<A>::Scan(const char* format, T&& ... args) const
{
    if(format)
    {
        sscanf(this->getRootNode(),format,args...);
    }

    return;
}
template<typename A>
cString<A> cString<A>::Format(const char *format, ... )
{
    char entry[64];
    va_list args;
    va_start(args, format);
    bool formatting = false;
    cString<A> resFormat = cString<A>();

    while (*format != '\0')
    {
        memset(entry,0,sizeof(char)*64);
        if(!formatting)
        {
            if (*format == '%')
            {
                formatting = true;
            }
            else
            {
                resFormat.push_back(*format);
            }
        }
        else
        {
            if (*format == 'd')
            {
                int i = va_arg(args, int);
                sprintf(entry,"%d",i);
                resFormat += entry;
            }
            else if (*format == 'u')
            {
                int i = va_arg(args, int);
                sprintf(entry,"%u",i);
                resFormat += entry;
            }
            else if (*format == 'c')
            {
                // note automatic conversion to integral type
                int c = va_arg(args, int);
                sprintf(entry,"%c",c);
                resFormat += entry;
            }
            else if (*format == 'f')
            {
                double d = va_arg(args, double);
                sprintf(entry,"%f",d);
                resFormat += entry;
            }
            else if (*format == 'g')
            {
                double d = va_arg(args, double);
                sprintf(entry,"%g",d);
                resFormat += entry;
            }
            else if(*format == 's')
            {
                char *pData = va_arg(args, char *);

                while(pData && *pData != 0)
                {
                    sprintf(entry,"%c",*pData);
                    resFormat += entry;
                    pData++;
                }
            }
            else
            {
                formatting = false;
                resFormat.push_back(*format);
            }
        }
        ++format;
    }

    va_end(args);

    return resFormat;
}
template<typename A>
cArray<cString<A>> cString<A>::tokenize(const cString<A>& separator) const
{
    cArray<cString<A>> res;
    cArray<cArray<char,A>> resLocal = cArray<char,A>::tokenize(separator);

    //convert lists into strings
    for(u32 i=0;i<resLocal.getSize();i++)
    {
        res.push_back(mpl::move(resLocal[i]));
    }

    return res;
}
template<typename A>
cString<A> cString<A>::subArr(const u32 initPos, const u32 length) const
{
    cString<A> res = cArray<char,A>::subArr(initPos,length);

    res.endString();

    return res;
}
template<typename A>
bool cString<A>::equal(const cString<A>& stringA, const cString<A>& stringB)
{
    return stringA.equal(stringB);
}
template<typename A>
bool cString<A>::equal(const cString<A>& other) const
{
    return cArray<char,A>::equal(other);
}
template<typename A>
cString<A>& cString<A>::operator=(const cString<A>& other)
{
    cArray<char,A>::operator=(other);

    return *this;
}
template<typename A>
cString<A>& cString<A>::operator=(cString<A>&& other)
{
    //be careful here, could happen that passes us an address pointer to other m_string and so we cannot move this
    if(!other.isStaticAddress())
    {
        cArray<char,A>::operator=(mpl::move(other));
    }
    else
    {
        cArray<char,A>::operator=(other);
    }

    return *this;
}
template<typename A>
cString<A>& cString<A>::operator+=(const char& append)
{
    this->_insert(this->m_size,append);

    return *this;
}
template<typename A>
cString<A>& cString<A>::operator+=(const cString<A>& append)
{
    *this = *this + append;

    return *this;
}
template<typename A>
cString<A>& cString<A>::operator<<(const cString<A>& append)
{
    cArray<char,A>::operator<<(append);

    return *this;
}
template<typename A>
cArray<char,A>& cString<A>::operator()()
{
    return *this;
}
}
}

template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : false;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::cString<A>& i_lhs, const char* i_rhs)
{
    const size_t rhsSize = strlen(i_rhs);

    return (i_lhs.getSize() == rhsSize) ? yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs,i_rhs + rhsSize) : false;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const char* i_lhs, const yame::container::cString<A>& i_rhs)
{
    const size_t lhsSize = strlen(i_lhs);

    return (lhsSize == i_rhs.getSize()) ? yame::equal(i_lhs,i_lhs + lhsSize,i_rhs.cbegin(),i_rhs.cend()) : false;
}
inline bool operator==(const yame::container::const_string_view& i_lhs, const yame::container::const_string_view& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : false;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::const_string_view& i_lhs, const yame::container::cString<A>& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : false;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::cString<A>& i_lhs, const yame::container::const_string_view& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : false;
}
inline bool operator==(const yame::container::const_string_view& i_lhs, const char* i_rhs)
{
    const size_t rhsSize = strlen(i_rhs);

    return (i_lhs.getSize() == rhsSize) ? yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs,i_rhs + rhsSize) : false;
}
inline bool operator==(const char* i_lhs, const yame::container::const_string_view& i_rhs)
{
    const size_t lhsSize = strlen(i_lhs);

    return (lhsSize == i_rhs.getSize()) ? yame::equal(i_lhs,i_lhs + lhsSize,i_rhs.cbegin(),i_rhs.cend()) : false;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? !yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : true;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::cString<A>& i_lhs, const char* i_rhs)
{
    const size_t rhsSize = strlen(i_rhs);

    return (i_lhs.getSize() == rhsSize) ? !yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs,i_rhs + rhsSize) : true;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const char* i_lhs, const yame::container::cString<A>& i_rhs)
{
    const size_t lhsSize = strlen(i_lhs);

    return (lhsSize == i_rhs.getSize()) ? !yame::equal(i_lhs,i_lhs + lhsSize,i_rhs.cbegin(),i_rhs.cend()) : true;
}
inline bool operator!=(const yame::container::const_string_view& i_lhs, const yame::container::const_string_view& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? !yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : true;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::const_string_view& i_lhs, const yame::container::cString<A>& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? !yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : true;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::cString<A>& i_lhs, const yame::container::const_string_view& i_rhs)
{
    return (i_lhs.getSize() == i_rhs.getSize()) ? !yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs.cbegin(),i_rhs.cend()) : true;
}
inline bool operator!=(const yame::container::const_string_view& i_lhs, const char* i_rhs)
{
    const size_t rhsSize = strlen(i_rhs);

    return (i_lhs.getSize () == rhsSize) ? !yame::equal(i_lhs.cbegin(),i_lhs.cend(),i_rhs,i_rhs + rhsSize) : true;
}
inline bool operator!=(const char* i_lhs, const yame::container::const_string_view& i_rhs)
{
    const size_t lhsSize = strlen(i_lhs);

    return (lhsSize == i_rhs.getSize()) ? !yame::equal(i_lhs,i_lhs + lhsSize,i_rhs.cbegin(),i_rhs.cend()) : true;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
yame::container::cString<A> operator+(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs)
{
    yame::container::cString<A> res = i_lhs;

    const size_t lhsSize = i_lhs.getSize();
    const size_t rhsSize = i_rhs.getSize();

    //avoid step-by-step allocations
    res.reserve(lhsSize+rhsSize);

    //insert i_rhs data
    if(rhsSize > 0)
    {
        res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());
    }

    return res;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
yame::container::cString<A> operator+(const yame::container::cString<A>& i_lhs, const char* i_rhs)
{
    yame::container::cString<A> res = i_lhs;
    yame::container::cString<A> rhs = i_rhs;

    const size_t lhsSize = i_lhs.getSize();
    const size_t rhsSize = rhs.getSize();

    //avoid step-by-step allocations
    res.reserve(lhsSize+rhsSize);

    //insert i_rhs data
    if(rhsSize > 0)
    {
        res.insert(res.end(),rhs.cbegin(),rhs.cend());
    }

    return res;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
yame::container::cString<A> operator+(const char* i_lhs, const yame::container::cString<A>& i_rhs)
{
    yame::container::cString<A> res = i_lhs;

    const size_t lhsSize = res.getSize();
    const size_t rhsSize = i_rhs.getSize();

    //avoid step-by-step allocations
    res.reserve(lhsSize+rhsSize);

    //insert i_rhs data
    if(rhsSize > 0)
    {
        res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());
    }

    return res;
}
//template<typename A = yame::memory::cTypedSystemAllocator<char> >
//inline yame::container::string operator+(const yame::container::cString<A>& i_lhs, const yame::container::const_string_view& i_rhs)
//{
//    yame::container::string res = i_lhs;
//
//    res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());
//
//    return res;
//}
//inline yame::container::string operator+(const yame::container::const_string_view& i_lhs, const yame::container::string& i_rhs)
//{
//    yame::container::string res = i_lhs;
//
//    res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());
//
//    return res;
//}
inline yame::container::string operator+(const yame::container::const_string_view& i_lhs, const yame::container::const_string_view& i_rhs)
{
    yame::container::string res = i_lhs;

    res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());

    return res;
}
inline yame::container::string operator+(const yame::container::const_string_view& i_lhs, const char* i_rhs)
{
    yame::container::string res = i_rhs;

    res.insert(res.begin(),i_lhs.cbegin(),i_lhs.cend());

    return res;
}
inline yame::container::string operator+(const char* i_lhs, const yame::container::const_string_view& i_rhs)
{
    yame::container::string res = i_lhs;

    res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());

    return res;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator<(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs)
{
    bool res = true;

    const size_t lhsSize = i_lhs.getSize();
    const size_t rhsSize = i_rhs.getSize();
    bool diff = true;

    typename yame::container::cString<A>::const_iterator_type itThis = i_lhs.cbegin();
    typename yame::container::cString<A>::const_iterator_type itOther = i_rhs.cbegin();
    typename yame::container::cString<A>::const_iterator_type  endStr = i_lhs.cend();

    for(;itThis!=endStr&&itOther!=endStr;++itThis,++itOther)
    {
        if(*itThis!=*itOther)
        {
            if(*itThis>*itOther)
            {
                res = false;
            }
            diff = false;
            break;
        }
    }

    if(lhsSize == rhsSize && diff)
    {
        res = false;
    }

    return res;
}
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator>(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs)
{
    return !(i_lhs < i_rhs);
}
