
namespace yame
{
namespace memory
{

template<typename T>
cTypedMemoryPool<T>::cTypedMemoryPool(size_t maxNumChunks, IMemoryProvider& i_memProvider)
: cMemoryPool(maxNumChunks, sizeof(T), i_memProvider)
{}

}
}
