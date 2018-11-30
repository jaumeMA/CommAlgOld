
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "System/memory/cSystemAllocator.h"

namespace yame
{
namespace container
{

template<int blockSize, typename A = memory::cSystemAllocator>
class cBlockSet
{
    static const unsigned char ones = (1 << (1 << blockSize)) - 1;
    static const unsigned char particleSize = 3 - blockSize;
public:
	static const size_t invalid_value = 1 << (1 << blockSize);
    static const size_t npos = -1;
	typedef unsigned char particle;

    template<typename T>
	struct block_reference
	{
        template<int,typename>
		friend class cBlockSet;
	public:
		inline block_reference<T>& operator=(const particle&);
		inline block_reference<T>& operator=(const block_reference<T>&);
        inline bool operator==(const particle& i_value) const;
        inline bool operator!=(const particle& i_value) const;
        inline block_reference<T> operator&() const;
        inline size_t operator*() const;
        inline block_reference<T>& operator++();
        inline block_reference<T> operator++(int);

	private:
		block_reference<T>(T* i_reference, size_t i_position, size_t i_max_position);

		T* inner_reference;
		size_t inner_position;
        size_t inner_max_position;
	};

    typedef block_reference<unsigned char> reference;
    typedef block_reference<const unsigned char> const_reference;

	cBlockSet();
	template<typename T>
	cBlockSet(const T& i_numericValue);
	cBlockSet(const cBlockSet<blockSize,A>& other);
	cBlockSet(cBlockSet<blockSize,A>&& other);
	~cBlockSet();

	//modifiers
	cBlockSet& operator=(const cBlockSet<blockSize,A>& other);
	cBlockSet& operator=(cBlockSet<blockSize,A>&& other);
	cBlockSet operator+(const cBlockSet<blockSize,A>& other) const;
	cBlockSet& operator+=(const cBlockSet<blockSize,A>& other);
	reference operator[](size_t i_index);
	cBlockSet& operator<<(size_t i_numShifts);
	cBlockSet& operator>>(size_t i_numShifts);

	//accessors
	const_reference operator[](size_t i_index) const;
	size_t getMsb() const;
	bool test(size_t i_index) const;
	bool any() const;
	bool none() const;
	bool all() const;
	int toInt() const;

private:
	inline void append(const unsigned char* otherPtr, size_t otherMSB);
	inline void append(const cBlockSet<blockSize,A>& i_other);
	inline unsigned char* allocate(size_t i_newSize, size_t i_oldSize);
	inline void deallocate();

	unsigned char* m_pArena;
	const A& m_allocator;
	size_t m_msb;
	size_t m_capacity;
};

template<typename A = memory::cSystemAllocator>
using cBitSet = cBlockSet<0, A>;
template<typename A = memory::cSystemAllocator>
using cByteSet = cBlockSet<3, A>;

}
}

template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator<<(yame::container::cBlockSet<blockSize,A>& o_bitset, const yame::container::cBlockSet<blockSize,A>& i_bitset);
template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator>>(const yame::container::cBlockSet<blockSize,A>& i_bitset, yame::container::cBlockSet<blockSize,A>& o_bitset);
template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator<<(yame::container::cBlockSet<blockSize,A>& o_bitset, const typename yame::container::cBlockSet<blockSize,A>::particle& i_value);
template<int blockSize, typename A>
yame::container::cBlockSet<blockSize,A>& operator>>(const typename yame::container::cBlockSet<blockSize,A>::particle& i_value, yame::container::cBlockSet<blockSize,A>& o_bitset);

#include "YTL/container/detail/cBlockSet.tpp"
