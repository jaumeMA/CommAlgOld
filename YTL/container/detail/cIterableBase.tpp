

namespace yame
{
namespace container
{
namespace detail
{

template<typename To,typename From>
To sendNodeToIterator(From i_from)
{
    return reinterpret_cast<To>(i_from);
}
template<typename To,typename From>
To receiveNodeFromIterator(From i_from)
{
    return reinterpret_cast<To>(i_from);
}

template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type const cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::m_pHead = reinterpret_cast<node_pointer_type const>(0xc1c1);

template<typename PrivateInterface, typename PublicInterface, typename Iterator>
void cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::setRootNode(node_pointer_type i_newRoot)
{
    m_pRoot = i_newRoot;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getRootNode() const
{
    return m_pRoot;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
template<typename T>
typename mpl::add_pointer<T>::type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getTypedRootNode() const
{
    return (m_pRoot) ? static_cast<typename mpl::add_pointer<T>::type>(m_pRoot) : static_cast<T*>(iterator_type::m_pHead);
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
size_t cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getSize() const
{
    size_t size = 0;
    node_pointer_type currNode = this->getFirstElem();

    while(currNode != iterator_type::m_pHead)
    {
        size++;
        currNode = this->getNextElem(currNode);
    }

    return size;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
bool cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::empty() const
{
    return m_pRoot == reinterpret_cast<node_pointer_type>(iterator_type::m_pHead);
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterator_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::begin(const ytl::function<bool(const_reference)>& filter)
{
    iterator_type res(*this, filter);

    node_pointer_type currNode = this->getFirstElem();

    if(filter != null_ptr)
    {
        while(currNode != iterator_type::m_pHead && filter.eval(this->getValue(currNode)) == false)
        {
            currNode = this->getNextElem(currNode);
        }
    }

    res.setCurrentNode(currNode);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterator_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::end()
{
    iterator_type res(*this);

    res.setCurrentNode(iterator_type::m_pHead);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::const_iterator_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::cbegin(const ytl::function<bool(const_reference)>& filter) const
{
    const_iterator_type res(*this,filter);

    node_pointer_type currNode = this->getFirstElem();

    if(filter != null_ptr)
    {
        while(currNode != iterator_type::m_pHead && filter.eval(this->getValue(currNode)) == false)
        {
            currNode = this->getNextElem(currNode);
        }
    }

    res.setCurrentNode(currNode);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::const_iterator_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::cend() const
{
    const_iterator_type res(*this);

    res.setCurrentNode(iterator_type::m_pHead);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getHead()
{
    return iterator_type::m_pHead;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterator_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::constructIteratorAtNode(node_pointer_type i_node, const ytl::function<bool(const_reference)>& filter)
{
    iterator_type res(*this, filter);

    res.setCurrentNode(i_node);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::const_iterator_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::constructConstIteratorAtNode(node_pointer_type i_node, const ytl::function<bool(const_reference)>& filter) const
{
    const_iterator_type res(*this,filter);

    res.setCurrentNode(i_node);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::acquireNode(node_pointer_type i_node) const
{
    return i_node;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
void cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::releaseNode(node_pointer_type i_node) const
{
    //do nothing
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
ytl::enable_ref_from_this& cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::ref_from_this()
{
    return *this;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
const ytl::enable_ref_from_this& cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::ref_from_this() const
{
    return *this;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getCurrentNode(const iterator_type& i_it)
{
    return i_it.getCurrentNode();
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getCurrentNode(const const_iterator_type& i_it) const
{
    return i_it.getCurrentNode();
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
template<typename T>
typename mpl::add_pointer<T>::type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getTypedCurrentNode(const iterator_type& i_it)
{
    return reinterpret_cast<typename mpl::add_pointer<T>::type>(i_it.getCurrentNode());
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
template<typename T>
typename mpl::add_pointer<T>::type cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::getTypedCurrentNode(const const_iterator_type& i_it) const
{
    return reinterpret_cast<typename mpl::add_pointer<T>::type>(i_it.getCurrentNode());
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::operator PrivateInterface&() const
{
    return const_cast<PrivateInterface>(*this);
}

template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterator_type cIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::erase(iterator_type i_it)
{
    iterator_type it(*this);

    node_pointer_type currNode = i_it.getCurrentNode();

    if(currNode != iterator_type::m_pHead)
    {
        node_pointer_type nextNode = this->getNextElem(currNode);
        this->eraseNode(currNode);

        it.setCurrentNode(nextNode);
    }

    return it;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
typename cIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterator_type cIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::add(add_type i_value)
{
    node_pointer_type newNode = this->addNode(mpl::forward<add_type>(i_value));

    iterator_type it(*this);

    it.setCurrentNode(newNode);

    return it;
}

template<typename PrivateInterface, typename PublicInterface, typename Iterator, template<typename,typename,typename> class Base>
typename cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::reverse_iterator_type cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::rbegin(const ytl::function<bool(const_reference)>& filter)
{
    reverse_iterator_type res(*this, filter);

    node_pointer_type currNode = this->getLastElem();

    if(filter != null_ptr)
    {
        while(currNode != iterator_type::m_pHead && filter.eval(this->getValue(currNode)) == false)
        {
            currNode = this->getPrevElem(currNode);
        }
    }

    res.setCurrentNode(currNode);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator, template<typename,typename,typename> class Base>
typename cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::reverse_iterator_type cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::rend()
{
    reverse_iterator_type res(*this);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator, template<typename,typename,typename> class Base>
typename cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::const_reverse_iterator_type cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::crbegin(const ytl::function<bool(const_reference)>& filter) const
{
    const_reverse_iterator_type res(*this,filter);

    node_pointer_type currNode = this->getLastElem();

    if(filter != null_ptr)
    {
        while(currNode != iterator_type::m_pHead && filter.eval(this->getValue(currNode)) == false)
        {
            currNode = this->getPrevElem(currNode);
        }
    }

    res.setCurrentNode(currNode);

    return res;
}
template<typename PrivateInterface, typename PublicInterface, typename Iterator, template<typename,typename,typename> class Base>
typename cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::const_reverse_iterator_type cReversableBaseIterableImpl<PrivateInterface,PublicInterface,Iterator,Base>::crend() const
{
    const_reverse_iterator_type res(*this);

    return res;
}

}
}
}
