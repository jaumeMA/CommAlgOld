
#define CHECK_LOAD_FACTOR \
if(float(m_numItems) / float(m_records.getSize()) > m_maxLoadFactor) \
{ \
    m_maxLoadFactor = 2 * m_numItems; \
    reHash(); \
}

namespace yame
{
namespace container
{
namespace detail
{

template<typename R,typename T>
cUnorderedNode<R,T>::cUnorderedNode(const R& key, const T& value)
{
    m_bucket.push_back(cPair<R,T>(key,value));
}
template<typename R,typename T>
void cUnorderedNode<R,T>::push(const R& key, const T& value)
{
    m_bucket.push_back(cPair<R,T>(key,value));
}
template<typename R,typename T>
void cUnorderedNode<R,T>::push_front(const R& key, const T& value)
{
    m_bucket.push_front(cPair<R,T>(key,value));
}
template<typename R,typename T>
void cUnorderedNode<R,T>::pop()
{
    m_bucket.pop();
}
template<typename R,typename T>
void cUnorderedNode<R,T>::pop_back()
{
    m_bucket.pop_back();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::lvaluereference cUnorderedNode<R,T>::front()
{
    return m_bucket.front();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::lvaluereference cUnorderedNode<R,T>::back()
{
    return m_bucket.back();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::const_lvaluereference cUnorderedNode<R,T>::front() const
{
    return m_bucket.front();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::const_lvaluereference cUnorderedNode<R,T>::back() const
{
    return m_bucket.back();
}
template<typename R,typename T>
size_t cUnorderedNode<R,T>::getSize() const
{
    return m_bucket.getSize();
}
template<typename R,typename T>
bool cUnorderedNode<R,T>::empty() const
{
    return m_bucket.empty();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::iterator_type cUnorderedNode<R,T>::erase(iterator_type it)
{
    if(it != m_bucket.end())
    {
        return m_bucket.erase(it);
    }

    return it;
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::iterator_type cUnorderedNode<R,T>::erase(cPair<R,T>* i_node)
{
    return m_bucket.erase(i_node - &m_bucket.front());
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::const_iterator_type cUnorderedNode<R,T>::find(const R& key) const
{
    const_iterator_type itPair = m_bucket.cbegin();

    for(;itPair!=m_bucket.cend();++itPair)
    {
        if(itPair->first == key)
        {
            break;
        }
    }

    return itPair;
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::iterator_type cUnorderedNode<R,T>::find(const R& key)
{
    iterator_type itPair = m_bucket.begin();

    for(;itPair!=m_bucket.end();++itPair)
    {
        if(itPair->first == key)
        {
            break;
        }
    }

    return itPair;
}
template<typename R,typename T>
cPair<R,T>* cUnorderedNode<R,T>::getFirstElem() const
{
    return (m_bucket.empty() == false) ? const_cast<cPair<R,T>*>(&m_bucket.front()) : NULL;
}
template<typename R,typename T>
cPair<R,T>* cUnorderedNode<R,T>::getLastElem() const
{
    return (m_bucket.empty() == false) ? const_cast<cPair<R,T>*>(&m_bucket.back()) : NULL;
}
template<typename R,typename T>
cPair<R,T>* cUnorderedNode<R,T>::getNextElem(cPair<R,T>* currNode) const
{
    if(currNode == &m_bucket.back())
    {
        return NULL;
    }

    return ++currNode;
}
template<typename R,typename T>
cPair<R,T>* cUnorderedNode<R,T>::getPrevElem(cPair<R,T>* currNode) const
{
    if(currNode == &m_bucket.front())
    {
        return NULL;
    }

    return --currNode;
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::iterator_type cUnorderedNode<R,T>::begin()
{
    return m_bucket.begin();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::const_iterator_type cUnorderedNode<R,T>::begin() const
{
    return m_bucket.cbegin();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::iterator_type cUnorderedNode<R,T>::end()
{
    return m_bucket.end();
}
template<typename R,typename T>
typename cUnorderedNode<R,T>::const_iterator_type cUnorderedNode<R,T>::cend() const
{
    return m_bucket.cend();
}


template<typename T, typename R, typename N, typename hash, template<typename> class A>
cUnorderedMapImpl<T,R,N,hash,A>::cUnorderedMapImpl()
: m_maxNumBuckets(MAX_SIZE_TABLE)
, m_maxLoadFactor(1.f)
{
    m_records.resize(m_maxNumBuckets);

    m_numItems = 0;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
cUnorderedMapImpl<T,R,N,hash,A>::cUnorderedMapImpl(const_lkeyreference key, const_lvaluereference value)
: m_maxNumBuckets(MAX_SIZE_TABLE)
, m_maxLoadFactor(1.f)
{
    m_records.reserve(m_maxNumBuckets);

    for(u16 i=0;i<m_maxNumBuckets;i++)
    {
        m_records.push_back(cUnorderedNode<R,T>());
    }

    insert(key, value);
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
void cUnorderedMapImpl<T,R,N,hash,A>::clear()
{
    m_records.clear();

    m_numItems = 0;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::lvaluereference cUnorderedMapImpl<T,R,N,hash,A>::insert(const_lkeyreference key, const_lvaluereference val)
{
    //compute the hash
    size_t hashKey = m_hash(key) % m_maxNumBuckets;

    cUnorderedNode<R,T>& node = m_records[hashKey];

    node.push(key,val);

    m_numItems++;

    CHECK_LOAD_FACTOR

    return node.back()->second;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::lvaluereference cUnorderedMapImpl<T,R,N,hash,A>::insert(const_lkeyreference key, rvaluereference val)
{
    //compute the hash
    size_t hashKey = m_hash(key) % m_maxNumBuckets;

    cUnorderedNode<R,T>& node = m_records[hashKey];

    node.push(key, mpl::move(val));

    m_numItems++;

    CHECK_LOAD_FACTOR

    return node.back().second;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
size_t cUnorderedMapImpl<T,R,N,hash,A>::erase(const_lkeyreference key)
{
    size_t hashKey = m_hash(key) % m_maxNumBuckets;

    cUnorderedNode<R,T>& hashNode = m_records[hashKey];

    typename cUnorderedNode<R,T>::iterator_type itPair = hashNode.find(key);

    if(itPair != hashNode.end())
    {
        m_records[hashKey].erase(itPair);

        m_numItems--;
    }

    return m_numItems;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::lvaluereference cUnorderedMapImpl<T,R,N,hash,A>::operator[](const_lkeyreference key)
{
    size_t hashKey = m_hash(key) % m_maxNumBuckets;

    cUnorderedNode<R,T>& hashNode = m_records[hashKey];

    typename cUnorderedNode<R,T>::iterator_type itPair = hashNode.find(key);

    if(itPair != hashNode.end())
    {
        return itPair->second;
    }

    return insert(key, T());
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::const_lvaluereference cUnorderedMapImpl<T,R,N,hash,A>::operator[](const_lkeyreference key) const
{
    (m_records[m_hash(key) % m_maxNumBuckets].find(key))->second;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
size_t cUnorderedMapImpl<T,R,N,hash,A>::getSize() const
{
    return m_numItems;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
bool cUnorderedMapImpl<T,R,N,hash,A>::isEmpty() const
{
    return m_numItems == 0;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::iterator_type cUnorderedMapImpl<T,R,N,hash,A>::search(const_lkeyreference key) const
{
    size_t hashKey = m_hash(key) % m_maxNumBuckets;

    const cUnorderedNode<R,T>& currNode = m_records[hashKey];

    typename cUnorderedNode<R,T>::const_iterator_type itNode = currNode.find(key);

    return this->constructIteratorAtNode(detail::sendNodeToIterator<cPair<const R,T>*>(&*itNode));
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
const cPair<const R,T>& cUnorderedMapImpl<T,R,N,hash,A>::getValue(node_pointer_type currNode) const
{
    const iterator_pair* currIterator = detail::receiveNodeFromIterator<iterator_pair*>(currNode);

    return reinterpret_cast<const cPair<const R,T>&>(*currIterator->currPair);
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
cPair<const R,T>& cUnorderedMapImpl<T,R,N,hash,A>::getValue(node_pointer_type currNode)
{
    iterator_pair* currIterator = detail::receiveNodeFromIterator<iterator_pair*>(currNode);

    return reinterpret_cast<cPair<const R,T>&>(*currIterator->currPair);
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::node_pointer_type cUnorderedMapImpl<T,R,N,hash,A>::getFirstElem() const
{
    if(m_records.empty() == false)
    {
        size_t currBucket = 0;

        while(currBucket < m_records.getSize() && m_records[currBucket].empty())
        {
            currBucket++;
        }

        if(currBucket < m_records.getSize())
        {
            return detail::sendNodeToIterator<node_pointer_type>(create_iterator(const_cast<cUnorderedNode<R,T>*>(&m_records[currBucket]),m_records[currBucket].getFirstElem()));
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::node_pointer_type cUnorderedMapImpl<T,R,N,hash,A>::getLastElem() const
{
    if(m_records.empty() == false)
    {
        size_t currBucket = m_records.getSize()-1;

        while(currBucket >= 0 && m_records[currBucket].empty())
        {
            currBucket--;
        }

        if(currBucket >= 0)
        {
            return detail::sendNodeToIterator<node_pointer_type>(create_iterator(const_cast<cUnorderedNode<R,T>*>(&m_records[currBucket]),m_records[currBucket].getLastElem()));
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::node_pointer_type cUnorderedMapImpl<T,R,N,hash,A>::getNextElem(node_pointer_type currNode) const
{
    if(currNode != iterator_type::m_pHead)
    {
        iterator_pair* currIterator = detail::receiveNodeFromIterator<iterator_pair*>(currNode);

        cUnorderedNode<R,T>* currNode = currIterator->currNode;

        if(cPair<R,T>* nextPair = currNode->getNextElem(currIterator->currPair))
        {
            while(currNode < &m_records.back())
            {
                ++currNode;

                if(nextPair = currNode->getFirstElem())
                {
                    currIterator->currNode = currNode;
                    currIterator->currPair = nextPair;

                    return detail::sendNodeToIterator<node_pointer_type>(currIterator);
                }
            }

            delete_iterator(currIterator);
        }
        else if(currNode < &m_records.back())
        {
            while(currNode < &m_records.back())
            {
                ++currNode;

                if(nextPair = currNode->getFirstElem())
                {
                    currIterator->currNode = currNode;
                    currIterator->currPair = nextPair;

                    return detail::sendNodeToIterator<node_pointer_type>(currIterator);
                }
            }

            delete_iterator(currIterator);
        }
    }

    return iterator_type::m_pHead;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::node_pointer_type cUnorderedMapImpl<T,R,N,hash,A>::getPrevElem(node_pointer_type currNode) const
{
    if(currNode != iterator_type::m_pHead)
    {
        iterator_pair* currIterator = detail::receiveNodeFromIterator<iterator_pair*>(currNode);

        cUnorderedNode<R,T>* currNode = currIterator->currNode;

        if(cPair<R,T>* prevPair = currNode->getPrevElem(currIterator->currPair))
        {
            while(currNode > &m_records.front())
            {
                --currNode;

                if(prevPair = currNode->getLastElem())
                {
                    currIterator->currNode = currNode;
                    currIterator->currPair = prevPair;

                    return detail::sendNodeToIterator<node_pointer_type>(currIterator);
                }
            }

            delete_iterator(currIterator);
        }
        else if(currNode < &m_records.back())
        {
            while(currNode > &m_records.back())
            {
                --currNode;

                if(prevPair = currNode->getLastElem())
                {
                    currIterator->currNode = currNode;
                    currIterator->currPair = prevPair;

                    return detail::sendNodeToIterator<node_pointer_type>(currIterator);
                }
            }

            delete_iterator(currIterator);
        }
    }

    return iterator_type::m_pHead;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
void cUnorderedMapImpl<T,R,N,hash,A>::eraseNode(node_pointer_type i_pair)
{
    if(i_pair != iterator_type::m_pHead)
    {
        iterator_pair* currIterator = detail::receiveNodeFromIterator<iterator_pair*>(i_pair);

        currIterator->currNode->erase(currIterator->currPair);

        m_numItems--;
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::node_pointer_type cUnorderedMapImpl<T,R,N,hash,A>::addNode(typename mpl::PairType<const R,T>::type i_value)
{
    size_t hashKey = m_hash(i_value.first) % m_maxNumBuckets;

    cUnorderedNode<R,T>& hashNode = m_records[hashKey];

    hashNode.push(i_value.first, i_value.second);

    m_numItems++;

    CHECK_LOAD_FACTOR

    //return detail::sendNodeToIterator<node_pointer_type>(hashNode.back());
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
void cUnorderedMapImpl<T,R,N,hash,A>::setMaxLoadFactor(float i_maxLoadFactor)
{
    ASSERT(i_maxLoadFactor >= 1.f, "Load factor must equal or greater than 1.f");

    if(m_maxLoadFactor != i_maxLoadFactor)
    {
        m_maxLoadFactor = i_maxLoadFactor;

        CHECK_LOAD_FACTOR
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
float cUnorderedMapImpl<T,R,N,hash,A>::getMaxLoadFactor() const
{
    return m_maxLoadFactor;
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
float cUnorderedMapImpl<T,R,N,hash,A>::getLoadFactor() const
{
    return m_numItems / m_records.getSize();
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
void cUnorderedMapImpl<T,R,N,hash,A>::reHash()
{
    typename cArray<cUnorderedNode<R,T>,Allocator>::iterator_type itRecord = m_records.begin();

    m_records.resize(m_maxNumBuckets);

    for(size_t recordIndex=0;itRecord!=m_records.end();++itRecord,++recordIndex)
    {
        cUnorderedNode<R,T>& currRecord = *itRecord;
        typename cUnorderedNode<R,T>::iterator_type itBucket = currRecord.begin();

        for(;itBucket!=currRecord.end();++itBucket)
        {
            size_t hashKey = m_hash(itBucket->first) % m_maxNumBuckets;

            if(hashKey != recordIndex)
            {
                cUnorderedNode<R,T>& node = m_records[hashKey];

                node.push(itBucket->first, mpl::move(itBucket->second));

                itBucket = currRecord.erase(itBucket);
            }
        }
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::node_pointer_type cUnorderedMapImpl<T,R,N,hash,A>::acquireNode(node_pointer_type i_node) const
{
    if(i_node != iterator_type::m_pHead)
    {
        iterator_pair* currIterator = detail::receiveNodeFromIterator<iterator_pair*>(i_node);

        return create_iterator(currIterator->currNode,currIterator->currPair);
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
void cUnorderedMapImpl<T,R,N,hash,A>::releaseNode(node_pointer_type i_node) const
{
    if(i_node != iterator_type::m_pHead)
    {
        delete_iterator(detail::receiveNodeFromIterator<iterator_pair*>(i_node));
    }
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
typename cUnorderedMapImpl<T,R,N,hash,A>::iterator_pair* cUnorderedMapImpl<T,R,N,hash,A>::create_iterator(cUnorderedNode<R,T>* i_currNode, cPair<R,T>* i_currPair)
{
    return reinterpret_cast<iterator_pair*>((u8*)memcpy((u8*)memcpy(detail::k_pairAllocator.allocate(1, sizeof(iterator_pair)),&i_currNode, sizeof(i_currNode)) + sizeof(i_currNode),&i_currPair, sizeof(i_currPair)) - sizeof(i_currNode));
}
template<typename T, typename R, typename N, typename hash, template<typename> class A>
void cUnorderedMapImpl<T,R,N,hash,A>::delete_iterator(iterator_pair* i_pair)
{
    detail::k_pairAllocator.deallocate(i_pair);
}

}
}
}
