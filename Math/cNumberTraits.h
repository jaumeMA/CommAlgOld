#pragma once

#include "YTL/container/cTupla.h"
#include "Math/detail/cSetInterface.h"
#include "gmp.h"

namespace yame
{
namespace math
{

typedef container::cTupla<int, 2> rationalPair;

template<typename Traits>
struct ExtendedRationalSet : virtual public detail::ISet<Traits>
{
    typedef rationalPair raw_type;

    void init();
    rationalPair get_raw() const;
    int numerator() const;
    int denominator() const;
    detail::ISet<Traits>& operator=(const rationalPair& i_value);
};

struct RationalSetTraits
{
	typedef mpq_t underlying_type;
    typedef ExtendedRationalSet<RationalSetTraits> extended_structure;

	static void init(underlying_type& o_value);
	static void init(underlying_type& o_value, const underlying_type& i_value);
	static void init(underlying_type& o_value, int i_numerator, unsigned int i_denominator);
	static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static void assign(underlying_type& o_value, const rationalPair& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
};


struct RationalGroupTraits
{
    typedef RationalSetTraits set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& res, const underlying_type& value);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

struct RationalRingTraits
{
    typedef RationalGroupTraits group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

struct RationalDivisionRingTraits
{
    typedef RationalRingTraits ring_traits;
	typedef typename ring_traits::underlying_type underlying_type;
    static const unsigned int characteristic = 0;

	static void inv(underlying_type& res, const underlying_type& value);
};

template<typename Traits>
struct ExtendedRealSet : virtual public detail::ISet<Traits>
{
    typedef double raw_type;
    double get_raw() const;
    detail::ISet<Traits>& operator=(const double&);
};

template<unsigned char Precision>
struct RealSetTraits
{
	typedef mpf_t underlying_type;
    typedef ExtendedRealSet<RealSetTraits> extended_structure;

	static void init(underlying_type& o_value);
	static void init(underlying_type& o_value, const underlying_type& i_value);
	static void init(underlying_type& o_value, double i_value);
	static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static void assign(underlying_type& o_value, const double& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
};

template<unsigned char Precision>
struct RealGroupTraits
{
    typedef RealSetTraits<Precision> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& res, const underlying_type& value);
private:
    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<unsigned char Precision>
struct RealRingTraits
{
    typedef RealGroupTraits<Precision> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<unsigned char Precision>
struct RealDivisionRingTraits
{
    typedef RealRingTraits<Precision> ring_traits;
	typedef typename ring_traits::underlying_type underlying_type;
    static const unsigned int characteristic = 0;

	static void inv(underlying_type& res, const underlying_type& value);
};

template<unsigned char Precision>
struct RealMetricSpaceTraits
{
    typedef typename RealSetTraits<Precision>::underlying_type underlying_type;
    typedef RealSetTraits<Precision> set_traits;

    static double distance(const underlying_type& i_lhs, const underlying_type& i_rhs);
    static void get_point_at_distance(underlying_type& res, const underlying_type& i_targetPoint, double i_distance);
    static void get_next_elem(underlying_type& o_currPoint, const underlying_type& i_targetPoint);
};

typedef container::cTupla<double,2> complexPair;

template<typename Traits>
struct ExtendedComplexSet : virtual public detail::ISet<Traits>
{
    typedef complexPair raw_type;
    complexPair get_raw() const;
    detail::ISet<Traits>& operator=(const complexPair& i_value);
    double real() const;
    double imag() const;
};

template<unsigned char Precision>
struct ComplexSetTraits
{
	struct complex_pair
	{
        typedef mpf_t base_type;

		base_type real;
		base_type imag;

        bool operator==(const complex_pair& other) const;
	};

	typedef complex_pair underlying_type;
    typedef ExtendedComplexSet<ComplexSetTraits> extended_structure;

	static void init(underlying_type& o_value);
	static void init(underlying_type& o_value, const underlying_type& i_value);
	static void init(underlying_type& o_value, double real, double imag);
	static void init(underlying_type& o_value, double real);
	static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static void assign(underlying_type& o_value, const complexPair& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
    static void conjugate(underlying_type& o_value, const underlying_type& i_value);
};

template<unsigned char Precision>
struct ComplexGroupTraits
{
    typedef ComplexSetTraits<Precision> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& res, const underlying_type& i_complex);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<unsigned char Precision>
struct ComplexRingTraits
{
    typedef ComplexGroupTraits<Precision> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void prod(underlying_type& res, const underlying_type& i_lhs, const typename underlying_type::base_type& i_rhs);
	static void prod(underlying_type& res, const typename underlying_type::base_type& i_lhs, const underlying_type& i_rhs);
	static void prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs);
	static double norm(const underlying_type& i_complex);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<unsigned char Precision>
struct ComplexDivisionRingTraits
{
    typedef ComplexRingTraits<Precision> ring_traits;
	typedef typename ring_traits::underlying_type underlying_type;
    static const unsigned int characteristic = 0;

	static void inv(underlying_type& res, const underlying_type& i_complex);
};

template<unsigned char Precision>
struct ComplexMetricSpaceTraits
{
    typedef typename ComplexSetTraits<Precision>::underlying_type underlying_type;
    typedef ComplexSetTraits<Precision> set_traits;

    static double distance(const typename ComplexSetTraits<Precision>::underlying_type& i_lhs, const typename ComplexSetTraits<Precision>::underlying_type& i_rhs);
    static void get_point_at_distance(underlying_type& res, const underlying_type& i_targetPoint, double i_distance);
    static void get_next_elem(underlying_type& o_currPoint, const underlying_type& i_targetPoint);
};

typedef container::cTupla<double, 4> quaternionQuartet;

template<typename Traits>
struct ExtendedQuaternionSet : virtual public detail::ISet<Traits>
{
    typedef quaternionQuartet raw_type;
    quaternionQuartet get_raw() const;
    detail::ISet<Traits>& operator=(const quaternionQuartet& i_value);
    double n_part() const;
    double i_part() const;
    double j_part() const;
    double k_part() const;
};

template<unsigned char Precision>
struct QuaternionSetTraits
{
	struct quat_type
	{
        typedef mpf_t base_type;

		base_type n;
		base_type i;
		base_type j;
		base_type k;
	};

	typedef quat_type underlying_type;
    typedef ExtendedQuaternionSet<QuaternionSetTraits> extended_structure;

	static void init(underlying_type& o_value);
	static void init(underlying_type& o_value, const underlying_type& i_value);
	static void init(underlying_type& o_value, double i_n, double i_i, double i_j, double i_k);
	static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static void assign(underlying_type& o_value, const quaternionQuartet& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
    void conjugate(underlying_type& res, const underlying_type& i_quat);
};

template<unsigned char Precision>
struct QuaternionGroupTraits
{
    typedef QuaternionSetTraits<Precision> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& res, const underlying_type& value);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<unsigned char Precision>
struct QuaternionRingTraits
{
    typedef QuaternionGroupTraits<Precision> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = false;

	static void prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs);
	static void prod(underlying_type& res, const underlying_type& i_lhs, const typename underlying_type::base_type& i_rhs);
	static void prod(underlying_type& res, const typename underlying_type::base_type& i_lhs, const underlying_type& i_rhs);
	static double norm(const underlying_type& i_quat);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<unsigned char Precision>
struct QuaternionDivisionRingTraits
{
    typedef QuaternionRingTraits<Precision> ring_traits;
	typedef typename ring_traits::underlying_type underlying_type;

	static void inv(underlying_type& res, const underlying_type& value);
};

template<unsigned char Precision>
struct QuaternionMetricSpaceTraits
{
    typedef typename QuaternionSetTraits<Precision>::underlying_type underlying_type;
    typedef QuaternionSetTraits<Precision> set_traits;

    static double distance(const typename QuaternionSetTraits<Precision>::underlying_type& i_lhs, const typename QuaternionSetTraits<Precision>::underlying_type& i_rhs);
};

template<typename Traits>
struct ExtendedZpSet : virtual public detail::ISet<Traits>
{
    typedef int raw_type;
    int get_raw() const;
    detail::ISet<Traits>& operator=(const int& i_value);
};

template<int p>
struct ZpSetTraits
{
    //pnding to add compile time check for weather p is prime or not
    typedef mpz_t underlying_type;
    typedef ExtendedZpSet<ZpSetTraits> extended_structure;

	static void init(underlying_type& o_value);
	static void init(underlying_type& o_value, const underlying_type& i_value);
	static void init(underlying_type& o_value, int i_value);
	static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static void assign(underlying_type& o_value, const int& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
};

template<int p>
struct ZpGroupTraits
{
    typedef ZpSetTraits<p> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& res, const underlying_type& i_zp);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<int p>
struct ZpRingTraits
{
    typedef ZpGroupTraits<p> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static const bool is_commutative = true;

	static void prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs);

    struct element_initializer
    {
        element_initializer();
    };
    static element_initializer _initializer;
    //force static initialization
    template<typename T, T> struct value { };
    typedef value<element_initializer&,_initializer> forceinitialize;
};

template<int p>
struct ZpDivisionRingTraits
{
    typedef ZpRingTraits<p> ring_traits;
	typedef typename ring_traits::underlying_type underlying_type;
    static const unsigned int characteristic = p;

	static void inv(underlying_type& res, const underlying_type& i_zp);
};

}
}

#include "Math/detail/cNumberTraits.tpp"
