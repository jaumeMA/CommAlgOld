
namespace yame
{
namespace container
{
namespace detail
{

template<typename T, typename R>
cAVLnode<T,R>::cAVLnode()
: cMapNode<T,R>()
, m_height(0)
{
}
template<typename T, typename R>
template<typename ... TT>
cAVLnode<T,R>::cAVLnode(const R& key, TT&& ... i_args)
: cMapNode<T,R>(key,mpl::forward<TT>(i_args)...)
, m_height(0)
{
}
template<typename T, typename R>
cAVLnode<T,R>::cAVLnode(const cAVLnode<T,R>& other)
: cMapNode<T,R>(other)
, m_height(other.m_height)
{
}
template<typename T, typename R>
cAVLnode<T,R>& cAVLnode<T,R>::operator=(const cAVLnode<T,R>& other)
{
    m_height = other.m_height;

    cMapNode<T,R>::operator=(other);

    return *this;
}
template<typename T, typename R>
void cAVLnode<T,R>::setParent(cAVLnode<T,R>* i_parent)
{
    this->m_pParent = i_parent;
}
template<typename T, typename R>
cAVLnode<T,R>* cAVLnode<T,R>::getParent()
{
    return static_cast<cAVLnode<T,R>*>(this->m_pParent);
}
template<typename T, typename R>
void cAVLnode<T,R>::setLeftChild(cAVLnode<T,R>* i_left)
{
    this->m_pLeftChild = i_left;
}
template<typename T, typename R>
cAVLnode<T,R>* cAVLnode<T,R>::getLeftChild()
{
    return static_cast<cAVLnode<T,R>*>(this->m_pLeftChild);
}
template<typename T, typename R>
void cAVLnode<T,R>::setRightChild(cAVLnode<T,R>* i_right)
{
    this->m_pRightChild = i_right;
}
template<typename T, typename R>
cAVLnode<T,R>* cAVLnode<T,R>::getRightChild()
{
    return static_cast<cAVLnode<T,R>*>(this->m_pRightChild);
}
template<typename T, typename R>
void cAVLnode<T,R>::setHeight(s8 i_height)
{
    m_height = i_height;
}
template<typename T, typename R>
s8 cAVLnode<T,R>::getHeight() const
{
    return m_height;
}

}
}
}
