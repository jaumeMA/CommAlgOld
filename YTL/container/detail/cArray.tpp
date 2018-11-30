
#include "YTL/container/cAlgorithm.h"

namespace yame
{
namespace container
{

template<typename T, typename A>
typename cArray<T,A>::value_type* cArray<T,A>::acquireMem(size_t size)
{
    return (value_type*)m_alloc.allocate(size, sizeof(T));
}
template<typename T, typename A>
void cArray<T,A>::releaseMem(value_type* node)
{
    if(node)
    {
        //free memory
        m_alloc.deallocate((void *)node);
    }

    return;
}
template<typename T, typename A>
typename cArray<T,A>::value_type* cArray<T,A>::allocate(size_t size)
{
    static const pointer_type headNode = detail::receiveNodeFromIterator<pointer_type>(iterable_type::m_pHead);
    value_type* mem = (size > 0) ? acquireMem(size) : headNode;

    //after allocating we have to initialize the object
    if(mem != headNode)
    {
        value_type* pointer = mem;
        for(size_t i=0;i<size;i++)
        {
            if(pointer)
            {
                construct(pointer);
                pointer++;
            }
        }
    }
    return mem;
}
template<typename T, typename A>
typename cArray<T,A>::value_type* cArray<T,A>::reallocate(value_type* ptr, size_t oldSize, size_t newSize)
{
    //we dont really trust libc realloc method (specially with non-pod objects)
    static const pointer_type headNode = detail::receiveNodeFromIterator<pointer_type>(iterable_type::m_pHead);
    value_type* mem = (newSize > 0) ? acquireMem(newSize) : headNode;

    //after allocating we have to initialize the object
    if(mem != headNode)
    {
        value_type* pointer = mem;

        for(size_t i=0;i<newSize;i++)
        {
            if(pointer)
            {
                (i < oldSize) ? construct(pointer, mpl::move(ptr[i])) : construct(pointer);
                pointer++;
            }
        }

        //deallocate previous memory
        deallocate(ptr);
    }
    else
    {
        //error while reallocating
        ASSERT(false, "wtf");
        mem = ptr;
    }

    return mem;
}
template<typename T, typename A>
void cArray<T,A>::deallocate(value_type* node, bool destroy)
{
    static const pointer_type headNode = detail::receiveNodeFromIterator<pointer_type>(iterable_type::m_pHead);

    if(node != headNode)
    {
        ASSERT(isInBounds(node), "Trying to free unbounded node!");

        if(destroy)
        {
            destruct(node,getAddressAtIndex(m_size));
        }

        //free memory
        releaseMem(node);
        node = NULL;
    }

    return;
}
template<typename T, typename A>
bool cArray<T,A>::isInBounds(const value_type* address) const
{
    const pointer_type rootNode = detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());

    return address >= rootNode && address - rootNode <= m_size;
}
template<typename T, typename A>
void cArray<T,A>::construct(const value_type* address)
{
    static const pointer_type headNode = detail::receiveNodeFromIterator<pointer_type>(iterable_type::m_pHead);

    //TODO: as soon as we have new gcc so there is available is_default_constructible, use construct method from template helper
    if(address != headNode)
    {
        mpl::construct(address);
    }
}
template<typename T, typename A>
template<typename TT, typename ... TTs>
void cArray<T,A>::construct(const value_type* address, TT&& i_arg, TTs&& ... i_args)
{
    static const pointer_type headNode = detail::receiveNodeFromIterator<pointer_type>(iterable_type::m_pHead);

    if(address != headNode)
    {
        new(const_cast<typename mpl::drop_constness<value_type>::type*>(address)) T(mpl::forward<TT>(i_arg), mpl::forward<TTs>(i_args)...);
    }
}
template<typename T, typename A>
void cArray<T,A>::destruct(value_type* addressInit, value_type* addressEnd)
{
    ASSERT(addressInit && addressEnd && addressInit <= addressEnd, "Unconsistent address range");

    //deconstruct the bunch of elements
    value_type* currAddress = addressInit;

    for(;currAddress!=addressEnd;currAddress++)
    {
        (*currAddress).~T();
    }

    return;
}
template<typename T, typename A>
template<typename TT>
void cArray<T,A>::assign(TT&& val, value_type* address)
{
    ASSERT(isInBounds(address), "Out of bounds!");

    *address = mpl::forward<TT>(val);
}
template<typename T, typename A>
template<typename ... TT>
typename cArray<T,A>::lreference cArray<T,A>::_insert(size_t pos, TT&& ... i_args)
{
    cArray data;

    if(pos > m_size)
    {
        pos = m_size;
    }

    if(m_size > pos)
    {
        //in this case we can use memcpy since its a local copy
        data.reserve(m_size-pos+1);
        for(size_t i=0;i < m_size - pos;i++)
        {
            data.construct(data.getAddressAtIndex(i), mpl::move(*getAddressAtIndex(pos+i)));
        }
        data.m_size = m_size - pos;
    }

    if(m_size + 1 >= m_capacity)
    {//primer de tot realocatem en cas que haguem arribat al tope de capacitat
        static const pointer_type headNode = reinterpret_cast<const pointer_type>(iterable_type::m_pHead);
        pointer_type rootNode = this->template getTypedRootNode<value_type>();
        size_t newCapacity = 2*(m_size + 1);
        this->setRootNode((rootNode == headNode) ? allocate(newCapacity) : reallocate(rootNode,m_capacity,newCapacity));
        m_capacity = newCapacity;
    }

    construct(getAddressAtIndex(pos), mpl::forward<TT>(i_args)...);

    m_size++;

    if(data.empty() == false)
    {
            //recuperem info
        for(size_t i=0;i<m_size-pos-1;i++)
        {
            construct(getAddressAtIndex(pos+1+i), mpl::move(*data.getAddressAtIndex(i)));
        }
    }

    return *getAddressAtIndex(pos);
}
template<typename T, typename A>
cArray<T,A>::cArray()
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_capacity = 0;
}
template<typename T, typename A>
cArray<T,A>::cArray(const cArray& other)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_capacity = other.getCapacity();
    m_size = other.getSize();
    this->setRootNode(allocate(m_capacity));

    for(size_t i=0;i<m_size;i++)
    {
        construct(getAddressAtIndex(i), *other.getAddressAtIndex(i));
    }
}
template<typename T, typename A>
cArray<T,A>::cArray(cArray&& other)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_capacity = other.m_capacity;
    m_size = other.getSize();
    this->setRootNode(other.getRootNode());

    other.setRootNode(iterable_type::m_pHead);
    other.m_capacity = 0;
    other.m_size = 0;
}
template<typename T, typename A>
cArray<T,A>::cArray(const std::initializer_list<T>& i_initList)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    yame::assign(*this, i_initList);
}
template<typename T, typename A>
cArray<T,A>::cArray(const_lreference value, size_t length)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_size = length;
    m_capacity = 2*length;

    if(length > 0)
    {
        this->setRootNode(allocate(m_capacity));

        for(size_t i=0;i<length;i++)
        {
            construct(getAddressAtIndex(i), value);
        }
    }
}
template<typename T, typename A>
cArray<T,A>::cArray(rreference value, size_t length)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_size = length;
    m_capacity = 2*length;

    if(length > 0)
    {
        this->setRootNode(allocate(m_capacity));

        for(size_t i=0;i<length;i++)
        {
            construct(getAddressAtIndex(i), mpl::move(value));
        }
    }
}
template<typename T, typename A>
cArray<T,A>::cArray(const value_type* data, size_t length)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_size = length;
    m_capacity = 2*length;

    if(length > 0)
    {
        this->setRootNode(allocate(m_capacity));

        for(size_t i=0;i<length;i++)
        {
            construct(getAddressAtIndex(i), data[i]);
        }
    }
}
template<typename T, typename A>
cArray<T,A>::cArray(const std::vector<T>& other)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    m_size = other.size();
    m_capacity = 2*other.size();

    if(other.empty() == false)
    {
        this->setRootNode(allocate(m_capacity));

        for(size_t i=0;i<m_size;i++)
        {
            construct(getAddressAtIndex(i), other[i]);
        }
    }
}
template<typename T, typename A>
cArray<T,A>::cArray(const array_view<T>& i_arrayView)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    yame::assign(*this, i_arrayView.cbegin(),i_arrayView.cend());
}
template<typename T, typename A>
cArray<T,A>::cArray(const const_array_view<T>& i_arrayView)
: detail::cRandomAccessIterableBaseImpl<T&>()
, m_capacity(0)
, m_size(0)
, m_alloc(memory::cMemoryProvider<A>::acquire())
{
    yame::assign(*this, i_arrayView.cbegin(),i_arrayView.cend());
}
template<typename T, typename A>
cArray<T,A>::~cArray()
{
    //a veure, es sumament perillos fer destruccions de punters aqui (ja que els operadors reben referencies!!), pèr tant millor ferho dsd un metode segur
    pointer_type rootNode = detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());
    deallocate(rootNode);
    this->setRootNode(iterable_type::m_pHead);
    m_capacity = 0;
    m_size = 0;
}
template<typename T, typename A>
cArray<T,A>& cArray<T,A>::operator=(const cArray& other)
{
    size_t newCap = other.m_capacity;

    pointer_type rootNode = detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());

    deallocate(rootNode);

    pointer_type newRootNode = allocate(newCap);
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(newRootNode));

    m_capacity = newCap;
    m_size = other.getSize();

    for(size_t i=0;i<m_size;i++)
    {
        assign(*other.getAddressAtIndex(i), getAddressAtIndex(i));
    }

    return *this;
}
template<typename T, typename A>
cArray<T,A>& cArray<T,A>::operator=(cArray&& other)
{
    pointer_type rootNode = detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());

    deallocate(rootNode);

    m_capacity = other.m_capacity;
    m_size = other.getSize();
    this->setRootNode(other.getRootNode());

    other.setRootNode(iterable_type::m_pHead);
    other.m_capacity = 0;
    other.m_size = 0;

    return *this;
}
template<typename T, typename A>
cArray<T,A>& cArray<T,A>::operator+=(const cArray<T,A>& other)
{
    *this = *this + other;

    return *this;
}
template<typename T, typename A>
typename cArray<T,A>::const_lreference cArray<T,A>::operator[](const size_t index) const
{
    ASSERT(index >= 0 && index < m_size, "Index out of bounds");

    return *getAddressAtIndex(index);
}
template<typename T, typename A>
typename cArray<T,A>::lreference cArray<T,A>::operator[](const size_t index)
{
    //how to put a mutex over this reference??
    ASSERT(index >= 0 && index < m_size, "Index out of bounds");

    return *getAddressAtIndex(index);
}
template<typename T, typename A>
typename cArray<T,A>::lreference cArray<T,A>::at(const size_t index)
{
    //how to put a mutex over this reference??
    ASSERT(index >= 0 && index < m_size, "Index out of bounds");

    return *getAddressAtIndex(index);
}
template<typename T, typename A>
cArray<T,A> cArray<T,A>::subArr(const size_t initPos, const size_t length) const
{
    cArray res = cArray();
    res.reserve(length);

    for(size_t i=0;i<length && (initPos+i)<m_size;i++)
    {
        res.push_back(*getAddressAtIndex(initPos+i));
    }

    return res;
}
template<typename T, typename A>
const typename cArray<T,A>::value_type* cArray<T,A>::getArray() const
{
    return detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());
}
template<typename T, typename A>
typename cArray<T,A>::value_type* cArray<T,A>::getArray()
{
    return detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());
}
template<typename T, typename A>
void cArray<T,A>::reset(T value)
{
    for(size_t i=0;i<m_size;i++)
    {
        assign(value, getAddressAtIndex(i));
    }

    return;
}
template<typename T, typename A>
void cArray<T,A>::reset(const T& value)
{
    for(size_t i=0;i<m_size;i++)
    {
        assign(value,getAddressAtIndex(i));
    }

    return;
}
template<typename T, typename A>
void cArray<T,A>::reset(T&& value)
{
    for(size_t i=0;i<m_size;i++)
    {
        assign(mpl::move(value),getAddressAtIndex(i));
    }

    return;
}
template<typename T, typename A>
typename cArray<T,A>::iterator_type cArray<T,A>::insert(iterator_type itPos, const_iterator_type itOtherInit, const_iterator_type itOtherEnd)
{
    size_t localPos = (itPos != this->end()) ? itPos - this->constructIteratorAtNode(this->getRootNode()) : m_size;

    ASSERT(localPos <= m_size, "Inserting out of bounds");

    //in the end, the iterator over arrays is just an array
    size_t otherSize = itOtherEnd - itOtherInit;

    reserve(2*(m_size+otherSize+1));

    for(const_iterator_type itOther=itOtherInit;itOther!=itOtherEnd;++itOther)
    {
        _insert(localPos,*itOther);
        localPos++;
    }

    return this->constructIteratorAtNode(this->template getTypedRootNode<value_type>() + localPos);
}
template<typename T, typename A>
template<typename ... TT>
typename cArray<T,A>::iterator_type cArray<T,A>::insert(iterator_type itPos, TT&& ... i_args)
{
    size_t localPos = (itPos != this->end()) ? itPos - this->constructIteratorAtNode(this->getRootNode()) : m_size;

    ASSERT(localPos <= m_size, "Inserting out of bounds");

    _insert(localPos,mpl::forward<TT>(i_args)...);

    return this->constructIteratorAtNode(this->template getTypedRootNode<value_type>() + localPos);
}
template<typename T, typename A>
template<typename ... TT>
typename cArray<T,A>::lreference cArray<T,A>::push_back(TT&& ... i_args)
{
    return _insert(m_size,mpl::forward<TT>(i_args)...);
}
template<typename T, typename A>
template<typename ... TT>
typename cArray<T,A>::lreference cArray<T,A>::push_front(TT&& ... i_args)
{
    return _insert(0,mpl::forward<TT>(i_args)...);
}
template<typename T, typename A>
typename cArray<T,A>::lreference cArray<T,A>::front()
{
    return *getAddressAtIndex(0);
}
template<typename T, typename A>
typename cArray<T,A>::lreference cArray<T,A>::back()
{
    return *getAddressAtIndex(m_size - 1);
}
template<typename T, typename A>
typename cArray<T,A>::const_lreference cArray<T,A>::front() const
{
    return *getAddressAtIndex(0);
}
template<typename T, typename A>
typename cArray<T,A>::const_lreference cArray<T,A>::back() const
{
    return *getAddressAtIndex(m_size - 1);
}
template<typename T, typename A>
void cArray<T,A>::resize(size_t size)
{
    if(size > m_capacity)
    {
        size_t prevCap = m_capacity;
        m_capacity = 2*size;
        node_pointer_type rootNode = this->getRootNode();

        if(rootNode != iterable_type::m_pHead)
        {
            //JAUME: abans era prevCap+1
            this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(reallocate(detail::receiveNodeFromIterator<T*>(rootNode),m_size,m_capacity)));
        }
        else
        {
            this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(allocate(m_capacity)));
        }
    }
    else if(size < m_size)
    {
        destruct(getAddressAtIndex(size),getAddressAtIndex(m_size));
    }

    m_size = size;

    return;
}
template<typename T, typename A>
void cArray<T,A>::pop()
{//per definicio ens carreguem lelement mes antic
    if(this->getRootNode() != iterable_type::m_pHead)
    {
        resize(m_size-1);
    }
}
template<typename T, typename A>
void cArray<T,A>::clear()
{
    if(this->getRootNode() != iterable_type::m_pHead)
    {
        resize(0);
    }

    return;
}
template<typename T, typename A>
typename cArray<T,A>::iterator_type cArray<T,A>::erase(size_t initPos, size_t length)
{
    if(initPos < m_size)
    {
        if(initPos + length > m_size)
        {
            length = m_size - initPos;
        }

        if(this->getRootNode() != iterable_type::m_pHead)
        {
            for(size_t i=initPos;i<m_size-length;i++)
            {
                assign(mpl::move(*getAddressAtIndex(i+length)),getAddressAtIndex(i));
            }

            for(size_t i=m_size-length;i<m_size;i++)
            {
                construct(getAddressAtIndex(i));
            }
        }

        m_size -= length;

        return this->constructIteratorAtNode(this->template getTypedRootNode<value_type>() + initPos);
    }

    return this->constructIteratorAtNode(iterator_type::m_pHead);
}
template<typename T, typename A>
size_t cArray<T,A>::getCapacity() const
{
    return m_capacity;
}
template<typename T, typename A>
size_t cArray<T,A>::getSize() const
{
    return m_size;
}
template<typename T, typename A>
void cArray<T,A>::reserve(size_t newCap)
{
    static const pointer_type headNode = detail::receiveNodeFromIterator<pointer_type>(iterable_type::m_pHead);
    pointer_type rootNode = this->template getTypedRootNode<value_type>();

    this->setRootNode((rootNode == headNode) ? allocate(newCap) : reallocate(rootNode,m_capacity,newCap));

    m_capacity = newCap;

    return;
}
template<typename T, typename A>
cArray<size_t> cArray<T,A>::find(const cArray<T>& other) const
{
    //NOTE: this string search algorithm implements the KMP search algorithm (for knuth-morris-pratt)
    size_t thisSize = m_size;
    size_t otherSize = other.getSize();
    bool precond = thisSize>0 && otherSize>0 && thisSize>=otherSize;
    cArray<size_t> res;
    size_t numOk = 0;
    size_t counterThis, counterOther;
    counterOther = 0;
    const value_type* block = other.getArray();
    size_t blockSize = otherSize;
    s8 shift;

    if(precond)
    {
        counterThis = 0;
        shift = 1;

        //build the look-up table
        cArray<short> kmpTable;

        kmpTable.reserve(otherSize);
        //by definition 0 and 1 are -1 and 0 respectively
        kmpTable.push_back(-1);
        kmpTable.push_back(0);

        size_t kmpPos = 2;
        size_t kmpCond = 0;

        while(kmpPos < otherSize)
        {
            if(other[kmpPos-1] == other[kmpCond])
            {
                kmpCond++;
                kmpTable.push_back(kmpCond);
                kmpPos++;
            }
            else if(kmpCond > 0)
            {
                kmpCond = kmpTable[kmpCond];
            }
            else
            {
                kmpTable.push_back(0);
                kmpPos++;
            }
        }

        //look for other using kmp table
        while(counterThis+counterOther < thisSize)
        {
            if(*getAddressAtIndex(counterThis+counterOther) == other[counterOther])
            {
                counterOther++;
                if(counterOther == otherSize)
                {
                    numOk++;
                    res.push_back(counterThis);
                    counterThis += counterOther;
                    counterOther = 0;
                }
            }
            else
            {
                if(kmpTable[counterOther] > 0)
                {
                    counterThis += shift*(counterOther - kmpTable[counterOther]);
                    counterOther = kmpTable[counterOther];
                }
                else if(kmpTable[counterOther] != -1)
                {
                    counterThis += shift*(counterOther + 1);
                    counterOther = 0;
                }
                else
                {
                    counterOther = 0;
                    counterThis += shift;
                }
            }
        }

    }

    return res;
}
template<typename T, typename A>
size_t cArray<T,A>::find_first_of(const_lreference other) const
{
    cArray<size_t> resFind = find(other);
    size_t res = (resFind.getSize() > 0) ? resFind[0] : cArray<size_t>::npos;

    return res;
}
template<typename T, typename A>
size_t cArray<T,A>::find_last_of(const_lreference other) const
{
    cArray<size_t> resFind = find(other);
    size_t res = (resFind.getSize() > 0) ? resFind[resFind.getSize()-1] : cArray<size_t>::npos;

    return res;
}
template<typename T, typename A>
void cArray<T,A>::replace(size_t prevPos, size_t newPos)
{
    s8 sign = (prevPos > newPos) ? -1 : 1;
    size_t thisSize = m_size;
    size_t counter = 0;

    if(prevPos < thisSize && newPos < thisSize)
    {
        value_type tmp = *getAddressAtIndex(prevPos);

        while((newPos-prevPos)*sign > counter)
        {
            *getAddressAtIndex(prevPos+counter*sign) = *getAddressAtIndex(prevPos+(counter+1)*sign);
            counter++;
        }

        *getAddressAtIndex(newPos) = tmp;
    }

    return;
}
template<typename T, typename A>
cArray<cArray<T>> cArray<T,A>::tokenize(const cArray<T>& separator) const
{
    cArray<cArray<T> > res;
    cArray<size_t> tokens = this->find(separator);

    if(tokens.getSize() > 0)
    {
        size_t counter = 0;
        size_t initBound = 0;

        while(counter < tokens.getSize())
        {
            size_t endBound = tokens[counter];
            res.push_back(subArr(initBound,endBound-initBound));
            initBound = endBound+1;
            counter++;
        }

        //last entry
        res.push_back(this->subArr(initBound,this->getSize()-initBound));
    }
    else
    {
        res.push_back(*this);
    }

    return res;
}
template<typename T, typename A>
bool cArray<T,A>::equal(const cArray& arrayA, const cArray& arrayB)
{
    bool res = true;
    size_t counter = 0;
    const value_type* dataA = arrayA.getArray();
    const value_type* dataB = arrayB.getArray();
    size_t sizeA = arrayA.getSize();
    size_t sizeB = arrayB.getSize();

    size_t maxSize = (sizeA > sizeB) ? sizeA : sizeB;

    while(maxSize > counter)
    {
        if(*dataA != *dataB)
        {
            res = false;
            break;
        }
        counter++;
        dataA++;
        dataB++;
    }

    return res;
}
template<typename T, typename A>
bool cArray<T,A>::equal(const cArray& other) const
{
    bool res = equal(*this,other);

    return res;
}
template<typename T, typename A>
size_t cArray<T,A>::copyBlock(const value_type* source, value_type* dest, size_t numBits, u8 offset, u8 endianess)
{//aquesta funcio es similar a memcpy pero a nivell de bits i amb la possibilitat dindicar lordre dels bits (endianess)
    size_t numBytes = numBits>>3;
    size_t numRemBits = numBits - numBytes*8;
    size_t res = numBytes*8 + numRemBits;
    u8 sizeBlock = sizeof(T)/sizeof(u8);

    //primer llegim loffset
    if(offset && numBits)
    {
        numRemBits = numBits - (8-offset);
        numBytes = numRemBits>>3;
        numRemBits = numRemBits - numBytes*8;
        if(endianess)
        {
            *((u8 *)dest) = (RBMS(*((u8 *)source)))&(~(1<<(8-offset)));
        }
        else
        {
            *((u8 *)dest) = *((u8 *)source)&(~(1<<(8-offset)));
        }
    }

    //a coninuacio els bytes
    for(size_t i=0;i<numBytes;i++)
    {
        for(u8 j=0;j<sizeBlock;j++)
        {
            if(endianess)
            {
                *((u8 *)(dest)+(offset>0)+i*sizeBlock+j) = RBMS(*((u8 *)(source)+(offset>0)+i*sizeBlock+j));
            }
            else
            {
                *((u8 *)(dest)+(offset>0)+i*sizeBlock+j) = *((u8 *)(source)+(offset>0)+i*sizeBlock+j);
            }
        }
    }
    //finalment el bits restants
    if(numRemBits)
    {
        if(endianess)
        {
            *((u8 *)(dest)+(offset>0)+sizeBlock*numBytes) = (RBMS(*((u8 *)(source)+(offset>0)+sizeBlock*numBytes)))&((1<<numRemBits));
        }
        else
        {
            *((u8 *)(dest)+(offset>0)+sizeBlock*numBytes) = *((u8 *)(source)+(offset>0)+sizeBlock*numBytes)&((1<<numRemBits));
        }
    }

    return res;
}
template<typename T, typename A>
typename cArray<T,A>::value_type* cArray<T,A>::getAddressAtIndex(size_t index) const
{
    ASSERT(index >= 0 && index <= m_capacity, "Index out of bounds");

    return detail::receiveNodeFromIterator<pointer_type>(this->getRootNode()) + index;
}
template<typename T, typename A>
typename cArray<T,A>::node_pointer_type cArray<T,A>::getFirstElem() const
{
    return (m_size > 0) ? this->getRootNode() : iterable_type::m_pHead;
}
template<typename T, typename A>
typename cArray<T,A>::node_pointer_type cArray<T,A>::getLastElem() const
{
    return (m_size > 0) ? detail::sendNodeToIterator<node_pointer_type>(this->template getTypedRootNode<value_type>() + m_size - 1) : NULL;
}
template<typename T, typename A>
typename cArray<T,A>::node_pointer_type cArray<T,A>::getNextElem(node_pointer_type i_currNode) const
{
    if(m_size > 0)
    {
        const pointer_type currNode = detail::receiveNodeFromIterator<pointer_type>(i_currNode);
        const pointer_type rootNode = this->template getTypedRootNode<value_type>();

        if(currNode + 1 - rootNode < m_size)
        {
            return detail::sendNodeToIterator<node_pointer_type>(currNode + 1);
        }
        else
        {
            return iterable_type::m_pHead;
        }
    }
    else
    {
        return iterable_type::m_pHead;
    }
}
template<typename T, typename A>
typename cArray<T,A>::node_pointer_type cArray<T,A>::getPrevElem(node_pointer_type i_currNode) const
{
    if(m_size > 0)
    {
        const pointer_type currNode = detail::receiveNodeFromIterator<pointer_type>(i_currNode);
        const pointer_type rootNode = this->template getTypedRootNode<value_type>();

        if(currNode > rootNode)
        {
            return detail::sendNodeToIterator<node_pointer_type>(currNode - 1);
        }
        else
        {
            return iterable_type::m_pHead;
        }
    }
    else
    {
        return iterable_type::m_pHead;
    }
}
template<typename T, typename A>
typename cArray<T,A>::node_pointer_type cArray<T,A>::shiftNodeByIndex(node_pointer_type i_currNode, int index) const
{
    const pointer_type currNode = detail::receiveNodeFromIterator<pointer_type>(i_currNode);
    const pointer_type rootNode = this->template getTypedRootNode<value_type>();
    const int finalIndex = (currNode - rootNode) + index;

    return (finalIndex >= 0 && finalIndex < m_size) ? detail::sendNodeToIterator<node_pointer_type>(currNode + static_cast<size_t>(index)) : iterable_type::m_pHead;
}
template<typename T, typename A>
size_t cArray<T,A>::getIndexOfNode(node_pointer_type i_node) const
{
    static const pointer_type headNode = reinterpret_cast<const pointer_type>(iterable_type::m_pHead);
    const pointer_type rootNode = this->template getTypedRootNode<value_type>();
    const pointer_type node = detail::receiveNodeFromIterator<pointer_type>(i_node);

    if(node != headNode)
    {
        ASSERT(isInBounds(node), "Trying to free unbounded node!");

        return node - rootNode;
    }
    else
    {
        return m_size;
    }
}
template<typename T, typename A>
typename cArray<T,A>::const_lreference cArray<T,A>::getValue(node_pointer_type i_currNode) const
{
    const pointer_type currMapNode = detail::receiveNodeFromIterator<pointer_type>(i_currNode);

    return *currMapNode;
}
template<typename T, typename A>
typename cArray<T,A>::lreference cArray<T,A>::getValue(node_pointer_type i_currNode)
{
    pointer_type currMapNode = detail::receiveNodeFromIterator<pointer_type>(i_currNode);

    return *currMapNode;
}
template<typename T, typename A>
void cArray<T,A>::eraseNode(node_pointer_type i_valueNode)
{
    const pointer_type rootNode = detail::receiveNodeFromIterator<pointer_type>(this->getRootNode());
    const pointer_type valueNode = detail::receiveNodeFromIterator<pointer_type>(i_valueNode);

    const size_t localPos = valueNode - rootNode;

    ASSERT(localPos >= 0 && localPos < m_size, "Inserting out of bounds");

    erase(localPos);

    return;
}
template<typename T, typename A>
typename cArray<T,A>::node_pointer_type cArray<T,A>::addNode(add_type i_value)
{
    _insert(m_size,mpl::forward<add_type>(i_value));

    return getLastElem();
}

}
}

template<typename T, typename A>
inline yame::container::cArray<T,A> operator+(const yame::container::cArray<T,A>& i_lhs, const yame::container::cArray<T,A>& i_rhs)
{
    yame::container::cArray<T,A> res = i_lhs;

    const size_t lhsSize = i_lhs.getSize();
    const size_t rhsSize = i_lhs.getSize();

    //avoid step-by-step allocations
    res.resize(lhsSize+rhsSize);

    //insert i_rhs data
    if(rhsSize > 0)
    {
        res.insert(res.end(),i_rhs.cbegin(),i_rhs.cend());
    }

    return res;
}
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator<<(yame::container::cArray<T,A>& o_array, const yame::container::cArray<T,A>& i_array)
{
    o_array.insert(o_array.cend(), i_array.cbegin(), i_array.cend());

    return o_array;
}
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator>>(const yame::container::cArray<T,A>& i_array, yame::container::cArray<T,A>& o_array)
{
    o_array.insert(o_array.cbegin(), i_array.cbegin(), i_array.cend());

    return o_array;
}
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator<<(yame::container::cArray<T,A>& o_array, const typename yame::container::cArray<T,A>::particle& i_value)
{
    o_array.push_back(i_value);

    return o_array;
}
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator>>(const typename yame::container::cArray<T,A>::particle& i_value, yame::container::cArray<T,A>& o_array)
{
    o_array.push_front(i_value);

    return o_array;
}
