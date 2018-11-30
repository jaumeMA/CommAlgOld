
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

#include <string>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "Utils/engineTypes.h"
#include <math.h>
#include <stdlib.h>
#include "cArray.h"

#define NUM_STACK_CHARS 12

namespace yame
{
namespace container
{


typedef array_view<char> string_view;
typedef const_array_view<char> const_string_view;

//this wrapper only sets a null value at the end of each array of char's
template<typename A = memory::cTypedSystemAllocator<char> >
class cString : public cArray<char,A>
{
private:
    char  m_string[NUM_STACK_CHARS];//SBO

    protected:
    void endString();
    size_t getLengthStringArray(const char *data) const;
    bool isStaticAddress(const char* address) const;
    bool isStaticAddress() const;
    bool isInBounds(const char* itPos) const;
    char* acquireMem(size_t size);
    void releaseMem(char* node);

public:
    static const char invalid_value = '\0';
    typedef typename cArray<char,A>::iterator_type iterator_type;
    typedef typename cArray<char,A>::const_iterator_type const_iterator_type;
    typedef typename cArray<char,A>::reverse_iterator_type reverse_iterator_type;
    typedef typename cArray<char,A>::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename cArray<char,A>::iterable_public_interface iterable_public_interface;
    typedef typename cArray<char,A>::value_type value_type;
    typedef typename cArray<char,A>::pointer_type pointer_type;
    typedef typename cArray<char,A>::node_pointer_type node_pointer_type;


    cString<A>();
    cString<A>(const cArray<char,A>& other);
    cString<A>(cArray<char,A>&& other);
    cString<A>(const cString<A>& other);
    cString<A>(cString<A>&& other);
    cString<A>(const char* data);
    cString<A>(const char* data, u32 length);
    cString<A>(char value, u16 length=1);
    cString<A>(const std::string& str);
    cString<A>(const const_string_view& str);
    cString<A>(const string_view& str);
    ~cString<A>();
    void resize(u16 size, u16 initPos=0);
    const char *getStr() const;
    char *getStr();
    cString<A> subStr(const u32 initPos, const u32 length) const;
    cString<A> subStr(const u32 initPos) const;
    std::string getStdString() const;
    template<typename ... T>
    void Scan(const char* format, T&& ... args) const;
    static cString<A> Format(const char *format, ... );
    cArray<cString<A>> tokenize(const cString<A>& separator) const;
    cString<A> subArr(const u32 initPos, const u32 length) const;
    static bool equal(const cString<A>& stringA, const cString<A>& stringB);
    bool equal(const cString<A>& other) const;
    cString<A>& operator=(const cString<A>& other);
    cString<A>& operator=(cString<A>&& other);
    cString<A>& operator+=(const char& append);
    cString<A>& operator+=(const cString<A>& append);
    cString<A>& operator<<(const cString<A>& append);
    cArray<char,A>& operator()();
};

typedef cString<memory::cTypedSystemAllocator<char> > string;

}
}

template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::cString<A>& i_lhs, const char* i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const char* i_lhs, const yame::container::cString<A>& i_rhs);
inline bool operator==(const yame::container::const_string_view& i_lhs, const yame::container::const_string_view& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::const_string_view& i_lhs, const yame::container::cString<A>& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator==(const yame::container::cString<A>& i_lhs, const yame::container::const_string_view& i_rhs);
inline bool operator==(const yame::container::const_string_view& i_lhs, const char* i_rhs);
inline bool operator==(const char* i_lhs, const yame::container::const_string_view& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::cString<A>& i_lhs, const char* i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const char* i_lhs, const yame::container::cString<A>& i_rhs);
inline bool operator!=(const yame::container::const_string_view& i_lhs, const yame::container::const_string_view& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::const_string_view& i_lhs, const yame::container::cString<A>& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator!=(const yame::container::cString<A>& i_lhs, const yame::container::const_string_view& i_rhs);
inline bool operator!=(const yame::container::const_string_view& i_lhs, const char* i_rhs);
inline bool operator!=(const char* i_lhs, const yame::container::const_string_view& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline yame::container::cString<A> operator+(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline yame::container::cString<A> operator+(const yame::container::cString<A>& i_lhs, const char* i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline yame::container::cString<A> operator+(const char* i_lhs, const yame::container::cString<A>& i_rhs);
//template<typename A = yame::memory::cTypedSystemAllocator<char> >
//inline yame::container::cString<A> operator+(const yame::container::cString<A>& i_lhs, const yame::container::const_string_view& i_rhs);
//inline yame::container::string operator+(const yame::container::const_string_view& i_lhs, const yame::container::string& i_rhs);
inline yame::container::string operator+(const yame::container::const_string_view& i_lhs, const yame::container::const_string_view& i_rhs);
inline yame::container::string operator+(const yame::container::const_string_view& i_lhs, const char* i_rhs);
inline yame::container::string operator+(const char* i_lhs, const yame::container::const_string_view& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator<(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs);
template<typename A = yame::memory::cTypedSystemAllocator<char> >
inline bool operator>(const yame::container::cString<A>& i_lhs, const yame::container::cString<A>& i_rhs);

#include "detail/cString.tpp"
