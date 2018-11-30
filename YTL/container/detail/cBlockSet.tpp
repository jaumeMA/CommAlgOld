
#include "System/cException.h"
#include <cstring>
#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace container
{

template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::block_reference<T>& cBlockSet<blockSize,A>::block_reference<T>::operator=(const particle& i_value)
{
    size_t absPos = inner_position >> cBlockSet<blockSize,A>::particleSize;
    unsigned char localPos = inner_position - (absPos << cBlockSet<blockSize,A>::particleSize);

	*(inner_reference + absPos) = *(inner_reference + absPos) ^ i_value << localPos;

	return *this;
}
template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::block_reference<T>& cBlockSet<blockSize,A>::block_reference<T>::operator=(const block_reference<T>& other)
{
    size_t absPos = inner_position >> cBlockSet<blockSize,A>::particleSize;
    unsigned char localPos = inner_position - (absPos << cBlockSet<blockSize,A>::particleSize);

	*(inner_reference + absPos) = *(inner_reference + absPos) ^ *other << localPos;

	return *this;
}
template<int blockSize, typename A>
template<typename T>
bool cBlockSet<blockSize,A>::block_reference<T>::operator==(const particle& i_value) const
{
    size_t absPos = inner_position >> cBlockSet<blockSize,A>::particleSize;
    unsigned char localPos = inner_position - (absPos << cBlockSet<blockSize,A>::particleSize);

    return *(inner_reference + absPos) & (cBlockSet<blockSize,A>::ones << localPos) == i_value;
}
template<int blockSize, typename A>
template<typename T>
bool cBlockSet<blockSize,A>::block_reference<T>::operator!=(const particle& i_value) const
{
    size_t absPos = inner_position >> cBlockSet<blockSize,A>::particleSize;
    unsigned char localPos = inner_position - (absPos << cBlockSet<blockSize,A>::particleSize);

    return *(inner_reference + absPos) & (cBlockSet<blockSize,A>::ones << localPos) != i_value;
}
template<int blockSize, typename A>
template<typename T>
size_t cBlockSet<blockSize,A>::block_reference<T>::operator*() const
{
    size_t absPos = inner_position >> cBlockSet<blockSize,A>::particleSize;
    unsigned char localPos = inner_position - (absPos << cBlockSet<blockSize,A>::particleSize);

    return (inner_position < inner_max_position) ? (*(inner_reference + absPos) & (cBlockSet<blockSize,A>::ones << localPos)) >> localPos : cBlockSet<blockSize,A>::invalid_value;
}
template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::block_reference<T> cBlockSet<blockSize,A>::block_reference<T>::operator&() const
{
    return block_reference<T>(*this);
}
template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::block_reference<T> cBlockSet<blockSize,A>::block_reference<T>::operator++(int)
{
    block_reference<T> res = *this;

    if(inner_position<inner_max_position)
    {
        ++inner_position;
    }

    return res;
}
template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::block_reference<T>& cBlockSet<blockSize,A>::block_reference<T>::operator++()
{
    if(inner_position<inner_max_position)
    {
        ++inner_position;
    }

    return *this;
}
template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::block_reference<T>::block_reference(T* i_reference, size_t i_position, size_t i_max_position)
: inner_reference(i_reference)
, inner_position(i_position)
, inner_max_position(i_max_position)
{
}


template<int blockSize, typename A>
cBlockSet<blockSize,A>::cBlockSet()
: m_pArena(NULL)
, m_msb(npos)
, m_capacity(0)
, m_allocator(memory::cMemoryProvider<A>::acquire())
{
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>::cBlockSet(const cBlockSet& other)
: m_pArena(NULL)
, m_msb(npos)
, m_capacity(0)
, m_allocator(memory::cMemoryProvider<A>::acquire())
{
	m_msb = other.m_msb;
	m_capacity = other.m_capacity;

	m_pArena = allocate(1 << m_capacity, 0);

	memcpy(m_pArena, other.m_pArena, m_capacity);
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>::cBlockSet(cBlockSet&& other)
: m_pArena(NULL)
, m_msb(npos)
, m_capacity(0)
, m_allocator(memory::cMemoryProvider<A>::acquire())
{
	m_msb = other.m_msb;
	m_capacity = other.m_capacity;
	m_pArena = other.m_pArena;

	other.m_msb = npos;
	other.m_capacity = 0;
	other.m_pArena = 0;
}
template<int blockSize, typename A>
template<typename T>
cBlockSet<blockSize,A>::cBlockSet(const T& i_numericValue)
: m_pArena(NULL)
, m_msb(npos)
, m_capacity(0)
, m_allocator(memory::cMemoryProvider<A>::acquire())
{
    static_assert(mpl::is_integral<T>::value, "You shall provide an integral value");

	//set each byte into arena
	const int numMSB = ((sizeof(unsigned int) << 3) - 1 - __builtin_clz(i_numericValue)) >> blockSize;
	m_msb = numMSB;
	m_capacity = m_msb + 1;
	m_pArena = allocate(((m_capacity << blockSize) >> 3) + 1, 0);

    if(m_msb != npos)
    {
        memcpy(m_pArena, &i_numericValue, ((m_msb << blockSize) >> 3) + 1);
    }
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>::~cBlockSet()
{
    m_msb = npos;
    m_capacity = 0;

    deallocate();
}
template<int blockSize, typename A>
typename cBlockSet<blockSize,A>::reference cBlockSet<blockSize,A>::operator[](size_t i_index)
{
	ASSERT(i_index <= m_msb, "Index out of bounds");

	return reference(m_pArena, i_index, m_msb+1);
}
template<int blockSize, typename A>
typename cBlockSet<blockSize,A>::const_reference cBlockSet<blockSize,A>::operator[](size_t i_index) const
{
	ASSERT(i_index <= m_msb, "Index out of bounds");

	return const_reference(m_pArena, i_index, m_msb+1);
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>& cBlockSet<blockSize,A>::operator=(const cBlockSet<blockSize,A>& other)
{
	m_msb = other.m_msb;
    size_t oldCapacity = m_capacity;
	m_capacity = other.m_capacity;

	m_pArena = allocate(((m_capacity << blockSize) >> 3) + 1, ((oldCapacity << blockSize) >> 3) + 1);

	memcpy(m_pArena, other.m_pArena, m_capacity);

    return *this;
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>& cBlockSet<blockSize,A>::operator=(cBlockSet<blockSize,A>&& other)
{
    m_pArena = other.m_pArena;
    m_msb = other.m_msb;
    m_capacity = other.m_capacity;

    other.m_pArena = NULL;
    other.m_msb = npos;
    other.m_capacity = 0;

    return *this;
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>& cBlockSet<blockSize,A>::operator>>(size_t i_numShifts)
{
    if(m_msb != npos)
    {
        for(size_t shiftIndex = 0;shiftIndex<i_numShifts;++shiftIndex)
        {
            const size_t numBytes = (m_msb << blockSize) >> 3;

            for(size_t byteIndex = 0; byteIndex <= numBytes; ++byteIndex)
            {
                if(byteIndex > 0)
                {
                    *(m_pArena + byteIndex - 1) |= (*(m_pArena + byteIndex) & ones) << (8 - (1 << blockSize));
                }

                *(m_pArena + byteIndex) = *(m_pArena + byteIndex) >> (1 << blockSize);
            }

            m_msb--;

            if(m_msb == npos)
            {
                break;
            }
        }
    }

    return *this;
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>& cBlockSet<blockSize,A>::operator<<(size_t i_numShifts)
{
    if(m_msb != npos)
    {
        for(size_t shiftIndex=0;shiftIndex<i_numShifts;++shiftIndex)
        {
            const size_t numBytes =  m_msb >> particleSize;

            if(m_msb >= m_capacity)
            {
                m_pArena = allocate(1 << (m_capacity << 1), 1 << m_capacity);
                m_capacity = m_capacity << 1;
            }

            for(size_t byteIndex = 0; byteIndex <= numBytes; ++byteIndex)
            {
                *(m_pArena + byteIndex + 1) |= (*(m_pArena + byteIndex) & (ones << (8 - (1 << blockSize)))) >> (8 - (1 << blockSize));
                *(m_pArena + byteIndex) = *(m_pArena + byteIndex) << (1 << blockSize);
            }

            m_msb++;

            if(m_msb == npos)
            {
                break;
            }

        }
    }

    return *this;
}
template<int blockSize, typename A>
cBlockSet<blockSize,A> cBlockSet<blockSize,A>::operator+(const cBlockSet<blockSize,A>& other) const
{
	cBlockSet<blockSize,A> res(*this);

	res.append(other);

	return res;
}
template<int blockSize, typename A>
cBlockSet<blockSize,A>& cBlockSet<blockSize,A>::operator+=(const cBlockSet<blockSize,A>& other)
{
	append(other);

	return *this;
}
template<int blockSize, typename A>
size_t cBlockSet<blockSize,A>::getMsb() const
{
	return m_msb;
}
template<int blockSize, typename A>
bool cBlockSet<blockSize,A>::test(size_t i_index) const
{
	ASSERT(i_index <= m_msb, "Index out of bounds");

	const size_t absPos = i_index >> particleSize;
	const unsigned char localPos = i_index - (absPos << particleSize);

	return *(m_pArena + absPos) & (ones << localPos);
}
template<int blockSize, typename A>
bool cBlockSet<blockSize,A>::any() const
{
	const size_t numBytes = m_msb >> particleSize;

	for(size_t byteIndex = 0; byteIndex <= numBytes; ++byteIndex)
	{
		const unsigned char currValue = *(m_pArena + byteIndex);

		if(((currValue | (~currValue + 1)) >> 7) & 1)
		{
			return true;
		}
	}

	return false;
}
template<int blockSize, typename A>
bool cBlockSet<blockSize,A>::none() const
{
	const size_t numBytes = m_msb >> particleSize;

	for(size_t byteIndex = 0; byteIndex <= numBytes; ++byteIndex)
	{
		const unsigned char currValue = *(m_pArena + byteIndex);

		if(((currValue | (~currValue + 1)) >> 7) & 1)
		{
			return false;
		}
	}

	return true;
}
template<int blockSize, typename A>
bool cBlockSet<blockSize,A>::all() const
{
	static const unsigned char bitmask = 0xFF;
	const size_t numBytes = m_msb >> particleSize;

	for(size_t byteIndex = 0; byteIndex <= numBytes; ++byteIndex)
	{
		const unsigned char currValue = *(m_pArena + byteIndex);

		if((currValue & bitmask) == 0)
		{
			return false;
		}
	}

	return true;
}
template<int blockSize, typename A>
void cBlockSet<blockSize,A>::append(const unsigned char* otherPtr, size_t otherMSB)
{
    const size_t originalMsb = m_msb;

	if(m_msb + otherMSB > m_capacity)
	{
        size_t oldCapacity = m_capacity;
        m_capacity = m_msb + otherMSB + 1;

        m_pArena = allocate(((m_capacity << blockSize) >> 3) + 1, ((oldCapacity << blockSize) >> 3) + 1);
	}

	const size_t absPos = m_msb >> particleSize;
	const unsigned char localPos = m_msb - (absPos << particleSize) + (1 << blockSize);
    const unsigned char complLocalPos = 8 - localPos;
    const unsigned char bitMask = (1 << complLocalPos) - 1;
    const unsigned char complBitMask = ~bitMask;
    unsigned char* thisPtr = m_pArena + absPos;

	const size_t numOtherBytes = otherMSB >> particleSize;
	for(size_t byteIndex=0;byteIndex<=numOtherBytes;++byteIndex)
	{
		if(localPos == 0)
		{
			*(thisPtr + originalMsb + byteIndex) = *otherPtr;
		}
		else
		{
			*(thisPtr + byteIndex) |= (*(otherPtr + byteIndex) & bitMask) << localPos;
			*(thisPtr + byteIndex + 1) |= (*(otherPtr + byteIndex) & complBitMask) >> complLocalPos;
		}
	}

    m_msb += otherMSB + 1;
}
template<int blockSize, typename A>
void cBlockSet<blockSize,A>::append(const cBlockSet<blockSize,A>& i_other)
{
    append(i_other.m_pArena, i_other.m_msb);
}
template<int blockSize, typename A>
unsigned char* cBlockSet<blockSize,A>::allocate(size_t i_newSize, size_t i_oldSize)
{
	if(i_newSize > i_oldSize)
	{
		unsigned char* newPtr = static_cast<unsigned char *>(m_allocator.allocate(i_newSize, 1));

        memset(newPtr, 0, i_newSize);

		if(m_pArena)
		{
			size_t numBytes = m_msb >> particleSize;

			for(size_t byteIndex=0;byteIndex<=numBytes;++byteIndex)
			{
				*(newPtr + byteIndex) = *(m_pArena + byteIndex);
			}

			m_allocator.deallocate(m_pArena);
		}

		return newPtr;
	}
	else
	{
		return m_pArena;
	}
}
template<int blockSize, typename A>
void cBlockSet<blockSize,A>::deallocate()
{
    if(m_pArena)
    {
        m_allocator.deallocate(m_pArena);
    }
}
template<int blockSize, typename A>
int cBlockSet<blockSize,A>::toInt() const
{
    const size_t numBytes = m_msb >> particleSize;
    int res = 0;

    for(size_t byteIndex=0;byteIndex<=numBytes;++byteIndex)
    {
        res |= *(m_pArena + byteIndex) << (byteIndex << 3);
    }

    return res;
}

}
}

template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator<<(yame::container::cBlockSet<blockSize,A>& o_bitset, const yame::container::cBlockSet<blockSize,A>& i_bitset)
{
    o_bitset += i_bitset;

    return o_bitset;
}
template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator>>(const yame::container::cBlockSet<blockSize,A>& i_bitset, yame::container::cBlockSet<blockSize,A>& o_bitset)
{
    o_bitset = i_bitset + o_bitset;

    return o_bitset;
}
template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator<<(yame::container::cBlockSet<blockSize,A>& o_bitset, const typename yame::container::cBlockSet<blockSize,A>::particle& i_value)
{
    o_bitset += yame::container::cBlockSet<blockSize,A>(i_value);

    return o_bitset;
}
template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator>>(const typename yame::container::cBlockSet<blockSize,A>::particle& i_value, yame::container::cBlockSet<blockSize,A>& o_bitset)
{
    o_bitset = yame::container::cBlockSet<blockSize,A>(i_value) + o_bitset;

    return o_bitset;
}
