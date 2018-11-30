#pragma once

#include "Utils/cMacroHelper.h"
#include <mutex>

#ifdef _DEBUG
    #define LENT_WITH_LOGIC
#endif

namespace yame
{
namespace ytl
{

enum class Policy
{
    Shared,
    Unique
};

template<Policy>
class reference_counter;

//reference counting for shared references
template<>
class reference_counter<Policy::Shared>
{
public:
	reference_counter()
	: m_numReferences(0)
	{
	}
	reference_counter(const reference_counter& other)
	: m_numReferences(other.m_numReferences)
	{
	}
	~reference_counter()
	{
	}
	size_t incrementReference()
	{
	    m_refMutex.lock();

		size_t res = ++m_numReferences;

        m_refMutex.unlock();

		return res;
	}
	size_t decrementReference()
	{
		size_t res = 0;

	    m_refMutex.lock();

		if(m_numReferences > 0)
		{
			res = --m_numReferences;
		}

	    m_refMutex.unlock();

		return res;
	}
	size_t getNumReferences() const
	{
	    m_refMutex.lock();

		size_t res = m_numReferences;

	    m_refMutex.unlock();

		return res;
	}
	bool hasReferences() const
	{
	    m_refMutex.lock();

		bool res =  m_numReferences > 0;

	    m_refMutex.unlock();

		return res;
	}

private:
	size_t m_numReferences;
	mutable std::mutex m_refMutex;
};

//reference counting for unique references

#ifdef LENT_WITH_LOGIC

template<>
class reference_counter<Policy::Unique>
{
public:
	reference_counter()
	: m_numStrongReferences(0)
	, m_numWeakReferences(0)
	{
	}
	reference_counter(const reference_counter& other)
	: m_numStrongReferences(other.m_numStrongReferences)
	, m_numWeakReferences(other.m_numWeakReferences)
	{
	}
	~reference_counter()
	{
	}
	size_t incrementStrongReference()
	{
	    m_refMutex.lock();

		size_t res = ++m_numStrongReferences;

	    m_refMutex.unlock();

		if (res > 1)
		{
			MAKE_IT_CRASH
		}

		return res;
	}
	size_t incrementWeakReference()
	{
	    m_refMutex.lock();

		size_t res = ++m_numWeakReferences;

	    m_refMutex.unlock();

		return res;
	}
	size_t decrementStrongReference()
	{
		size_t res = 0;

	    m_refMutex.lock();

		if(m_numStrongReferences > 0)
		{
			res = --m_numStrongReferences;
		}

	    m_refMutex.unlock();

		return res;
	}
	size_t decrementWeakReference()
	{
		size_t res = 0;

	    m_refMutex.lock();

		if(m_numWeakReferences > 0)
		{
			res = --m_numWeakReferences;
		}

	    m_refMutex.unlock();

		return res;
	}
	size_t getNumStrongReferences() const
	{
	    m_refMutex.lock();

		size_t res = m_numStrongReferences;

	    m_refMutex.unlock();

		return res;
	}
	bool hasStrongReferences() const
	{
	    m_refMutex.lock();

		bool res =  m_numStrongReferences > 0;

	    m_refMutex.unlock();

		return res;
	}
	size_t getNumWeakReferences() const
	{
	    m_refMutex.lock();

		size_t res = m_numWeakReferences;

	    m_refMutex.unlock();

		return res;
	}
	bool hasWeakReferences() const
	{
	    m_refMutex.lock();

		bool res =  m_numWeakReferences > 0;

	    m_refMutex.unlock();

		return res;
	}

private:
	size_t m_numStrongReferences;
	size_t m_numWeakReferences;
	mutable std::mutex m_refMutex;
};

#else

template<>
class reference_counter<Policy::Unique>
{
public:
	reference_counter()
	: m_numStrongReferences(0)
	{
	}
	reference_counter(const reference_counter& other)
	: m_numStrongReferences(other.m_numStrongReferences)
	{
	}
	~reference_counter()
	{
	}
	size_t incrementStrongReference()
	{
	    m_refMutex.lock();

		size_t res = ++m_numStrongReferences;

	    m_refMutex.unlock();

		if (res > 1)
		{
			MAKE_IT_CRASH
		}

		return res;
	}
	size_t decrementStrongReference()
	{
		size_t res = 0;

	    m_refMutex.lock();

		if(m_numStrongReferences > 0)
		{
			res = --m_numStrongReferences;
		}

	    m_refMutex.unlock();

		return res;
	}
	size_t getNumStrongReferences() const
	{
	    m_refMutex.lock();

		size_t res = m_numStrongReferences;

	    m_refMutex.unlock();

		return res;
	}
	bool hasStrongReferences() const
	{
	    m_refMutex.lock();

		bool res =  m_numStrongReferences > 0;

	    m_refMutex.unlock();

		return res;
	}

private:
	size_t m_numStrongReferences;
	mutable std::mutex m_refMutex;
};

#endif

typedef reference_counter<Policy::Shared> shared_reference_counter;
typedef reference_counter<Policy::Unique> unique_reference_counter;

}
}
