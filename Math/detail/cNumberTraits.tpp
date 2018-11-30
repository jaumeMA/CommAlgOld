
namespace yame
{
namespace math
{

template<typename Traits>
void ExtendedRationalSet<Traits>::init()
{
    Traits::assign(this->getValue(), 0);
}
template<typename Traits>
int ExtendedRationalSet<Traits>::numerator() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    mpz_t numerat;
    mpz_init(numerat);

    mpq_get_num(numerat, thisValue);
    int res = mpz_get_si(numerat);
    mpz_clear(numerat);

    return res;
}
template<typename Traits>
int ExtendedRationalSet<Traits>::denominator() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    mpz_t denom;
    mpz_init(denom);

    mpq_get_den(denom, thisValue);

    int res = mpz_get_si(denom);
    mpz_clear(denom);

    return res;
}
template<typename Traits>
rationalPair ExtendedRationalSet<Traits>::get_raw() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    mpz_t numerat;
    mpz_init(numerat);

    mpq_get_num(numerat, thisValue);

    mpz_t denom;
    mpz_init(denom);

    mpq_get_den(denom, thisValue);
    int num = mpz_get_si(numerat);
    int den = mpz_get_si(denom);

    mpz_clear(numerat);
    mpz_clear(denom);

    return rationalPair(num, den);
}
template<typename Traits>
detail::ISet<Traits>& ExtendedRationalSet<Traits>::operator=(const rationalPair& i_value)
{
    Traits::assign(this->getValue(), i_value);
}
template<typename Traits>
double ExtendedRealSet<Traits>::get_raw() const
{
    return mpf_get_d(this->getValue());
}
template<typename Traits>
detail::ISet<Traits>& ExtendedRealSet<Traits>::operator=(const double& i_value)
{
    Traits::assign(this->getValue(), i_value);
}

template<typename Traits>
complexPair ExtendedComplexSet<Traits>::get_raw() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return complexPair(mpf_get_d(thisValue.real), mpf_get_d(thisValue.imag));
}
template<typename Traits>
detail::ISet<Traits>& ExtendedComplexSet<Traits>::operator=(const complexPair& i_value)
{
    Traits::assign(this->getValue(), i_value);
}
template<typename Traits>
double ExtendedComplexSet<Traits>::real() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return mpf_get_d(thisValue.real);
}
template<typename Traits>
double ExtendedComplexSet<Traits>::imag() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return mpf_get_d(thisValue.imag);
}
template<typename Traits>
quaternionQuartet ExtendedQuaternionSet<Traits>::get_raw() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return quaternionQuartet(mpf_get_d(thisValue.n), mpf_get_d(thisValue.i), mpf_get_d(thisValue.j), mpf_get_d(thisValue.k));
}
template<typename Traits>
detail::ISet<Traits>& ExtendedQuaternionSet<Traits>::operator=(const quaternionQuartet& i_value)
{
    Traits::assign(this->getValue(), i_value);
}
template<typename Traits>
double ExtendedQuaternionSet<Traits>::n_part() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return mpf_get_d(thisValue.n);
}
template<typename Traits>
double ExtendedQuaternionSet<Traits>::i_part() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return mpf_get_d(thisValue.i);
}
template<typename Traits>
double ExtendedQuaternionSet<Traits>::j_part() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return mpf_get_d(thisValue.j);
}
template<typename Traits>
double ExtendedQuaternionSet<Traits>::k_part() const
{
    const typename Traits::underlying_type& thisValue = this->getValue();

    return mpf_get_d(thisValue.k);
}
template<typename Traits>
int ExtendedZpSet<Traits>::get_raw() const
{
    return mpz_get_ui(this->getValue());
}
template<typename Traits>
detail::ISet<Traits>& ExtendedZpSet<Traits>::operator=(const int& i_value)
{
    Traits::assign(this->getValue(), i_value);
}

template<unsigned char Precision>
typename RealGroupTraits<Precision>::underlying_type RealGroupTraits<Precision>::neutral_element = {};
template<unsigned char Precision>
typename RealGroupTraits<Precision>::element_initializer RealGroupTraits<Precision>::_initializer = typename RealGroupTraits<Precision>::element_initializer();
template<unsigned char Precision>
RealGroupTraits<Precision>::element_initializer::element_initializer()
{
    mpf_init2(RealGroupTraits<Precision>::neutral_element,Precision);
    mpf_set_d(RealGroupTraits<Precision>::neutral_element, 0.f);
}
template<unsigned char Precision>
typename RealRingTraits<Precision>::underlying_type RealRingTraits<Precision>::neutral_element = {};
template<unsigned char Precision>
typename RealRingTraits<Precision>::element_initializer RealRingTraits<Precision>::_initializer = typename RealRingTraits<Precision>::element_initializer();
template<unsigned char Precision>
RealRingTraits<Precision>::element_initializer::element_initializer()
{
    mpf_init2(RealRingTraits<Precision>::neutral_element,Precision);
    mpf_set_d(RealRingTraits<Precision>::neutral_element, 1.f);
}

template<unsigned char Precision>
typename ComplexGroupTraits<Precision>::underlying_type ComplexGroupTraits<Precision>::neutral_element = typename ComplexGroupTraits<Precision>::underlying_type();
template<unsigned char Precision>
typename ComplexGroupTraits<Precision>::element_initializer ComplexGroupTraits<Precision>::_initializer = typename ComplexGroupTraits<Precision>::element_initializer();
template<unsigned char Precision>
ComplexGroupTraits<Precision>::element_initializer::element_initializer()
{
    mpf_init2(ComplexGroupTraits<Precision>::neutral_element.real,Precision);
    mpf_set_d(ComplexGroupTraits<Precision>::neutral_element.real, 0.f);

    mpf_init2(ComplexGroupTraits<Precision>::neutral_element.imag,Precision);
    mpf_set_d(ComplexGroupTraits<Precision>::neutral_element.imag, 0.f);
}
template<unsigned char Precision>
typename ComplexRingTraits<Precision>::underlying_type ComplexRingTraits<Precision>::neutral_element = typename ComplexRingTraits<Precision>::underlying_type();
template<unsigned char Precision>
typename ComplexRingTraits<Precision>::element_initializer ComplexRingTraits<Precision>::_initializer = typename ComplexRingTraits<Precision>::element_initializer();
template<unsigned char Precision>
ComplexRingTraits<Precision>::element_initializer::element_initializer()
{
    mpf_init2(ComplexRingTraits<Precision>::neutral_element.real,Precision);
    mpf_set_d(ComplexRingTraits<Precision>::neutral_element.real, 1.f);

    mpf_init2(ComplexRingTraits<Precision>::neutral_element.imag,Precision);
    mpf_set_d(ComplexRingTraits<Precision>::neutral_element.imag, 0.f);
}

template<unsigned char Precision>
typename QuaternionGroupTraits<Precision>::underlying_type QuaternionGroupTraits<Precision>::neutral_element = typename QuaternionGroupTraits<Precision>::underlying_type();
template<unsigned char Precision>
typename QuaternionGroupTraits<Precision>::element_initializer QuaternionGroupTraits<Precision>::_initializer = typename QuaternionGroupTraits<Precision>::element_initializer();
template<unsigned char Precision>
QuaternionGroupTraits<Precision>::element_initializer::element_initializer()
{
    mpf_init2(QuaternionGroupTraits<Precision>::neutral_element.n,Precision);
    mpf_set_d(QuaternionGroupTraits<Precision>::neutral_element.n, 0.f);

    mpf_init2(QuaternionGroupTraits<Precision>::neutral_element.i,Precision);
    mpf_set_d(QuaternionGroupTraits<Precision>::neutral_element.i, 0.f);

    mpf_init2(QuaternionGroupTraits<Precision>::neutral_element.j,Precision);
    mpf_set_d(QuaternionGroupTraits<Precision>::neutral_element.j, 0.f);

    mpf_init2(QuaternionGroupTraits<Precision>::neutral_element.k,Precision);
    mpf_set_d(QuaternionGroupTraits<Precision>::neutral_element.k, 0.f);
}
template<unsigned char Precision>
typename QuaternionRingTraits<Precision>::underlying_type QuaternionRingTraits<Precision>::neutral_element = typename QuaternionRingTraits<Precision>::underlying_type();
template<unsigned char Precision>
typename QuaternionRingTraits<Precision>::element_initializer QuaternionRingTraits<Precision>::_initializer = typename QuaternionRingTraits<Precision>::element_initializer();
template<unsigned char Precision>
QuaternionRingTraits<Precision>::element_initializer::element_initializer()
{
    mpf_init2(QuaternionRingTraits<Precision>::neutral_element.n,Precision);
    mpf_set_d(QuaternionRingTraits<Precision>::neutral_element.n, 1.f);

    mpf_init2(QuaternionRingTraits<Precision>::neutral_element.i,Precision);
    mpf_set_d(QuaternionRingTraits<Precision>::neutral_element.i, 0.f);

    mpf_init2(QuaternionRingTraits<Precision>::neutral_element.j,Precision);
    mpf_set_d(QuaternionRingTraits<Precision>::neutral_element.j, 0.f);

    mpf_init2(QuaternionRingTraits<Precision>::neutral_element.k,Precision);
    mpf_set_d(QuaternionRingTraits<Precision>::neutral_element.k, 0.f);
}

template<int p>
typename ZpGroupTraits<p>::underlying_type ZpGroupTraits<p>::neutral_element = {};
template<int p>
ZpGroupTraits<p>::element_initializer::element_initializer()
{
    mpz_init(ZpGroupTraits<p>::neutral_element);
    mpz_set_ui(ZpGroupTraits<p>::neutral_element, 0);
}
template<int p>
typename ZpRingTraits<p>::underlying_type ZpRingTraits<p>::neutral_element = {};
template<int p>
ZpRingTraits<p>::element_initializer::element_initializer()
{
    mpz_init(ZpRingTraits<p>::neutral_element);
    mpz_set_ui(ZpRingTraits<p>::neutral_element, 1);
}

template<unsigned char Precision>
void RealSetTraits<Precision>::init(underlying_type& o_value)
{
    mpf_init2(o_value,Precision);
}
template<unsigned char Precision>
void RealSetTraits<Precision>::init(underlying_type& o_value, const underlying_type& i_value)
{
    mpf_init2(o_value,Precision);
    mpf_set(o_value, i_value);
}
template<unsigned char Precision>
void RealSetTraits<Precision>::init(underlying_type& o_value, double i_value)
{
    mpf_init2(o_value,Precision);
    mpf_set_d(o_value, i_value);
}
template<unsigned char Precision>
void RealSetTraits<Precision>::deinit(underlying_type& o_value)
{
    mpf_clear(o_value);
}
template<unsigned char Precision>
void RealSetTraits<Precision>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    mpf_set(o_value, i_value);
}
template<unsigned char Precision>
void RealSetTraits<Precision>::assign(underlying_type& o_value, const double& i_value)
{
    mpf_set_d(o_value, i_value);
}
template<unsigned char Precision>
bool RealSetTraits<Precision>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    return mpf_cmp(i_lhs, i_rhs) == 0;
}

template<unsigned char Precision>
void RealGroupTraits<Precision>::sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    mpf_add(res,i_lhs,i_rhs);
}
template<unsigned char Precision>
void RealGroupTraits<Precision>::inv(underlying_type& res, const underlying_type& value)
{
    mpf_neg(res,value);
}

template<unsigned char Precision>
void RealRingTraits<Precision>::prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    mpf_mul(res,i_lhs,i_rhs);
}

template<unsigned char Precision>
void RealDivisionRingTraits<Precision>::inv(underlying_type& res, const underlying_type& value)
{
    mpf_ui_div(res,1,value);
}

template<unsigned char Precision>
double RealMetricSpaceTraits<Precision>::distance(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    underlying_type localSub;
    mpf_init2(localSub,Precision);

    mpf_sub(localSub,i_lhs,i_rhs);

    const double res = mpf_get_d(localSub);
    mpf_clear(localSub);

    return res;
}
template<unsigned char Precision>
void RealMetricSpaceTraits<Precision>::get_point_at_distance(underlying_type& res, const underlying_type& i_targetPoint, double i_distance)
{
    //we dont care about a given direction in the complex plane
    underlying_type localDistance;
    mpf_init2(localDistance,Precision);
    mpf_set_d(localDistance, i_distance);

    mpf_add(res,i_targetPoint,localDistance);

    mpf_clear(localDistance);
}
template<unsigned char Precision>
void RealMetricSpaceTraits<Precision>::get_next_elem(underlying_type& o_currPoint, const underlying_type& i_targetPoint)
{
    underlying_type localDiff;
    mpf_init2(localDiff,Precision);

    mpf_sub(localDiff, i_targetPoint, o_currPoint);

    underlying_type localDiv;
    mpf_init2(localDiv,Precision);

    mpf_div_ui(localDiv, localDiff, 10);

    mpf_add(o_currPoint, o_currPoint, localDiv);

    mpf_clear(localDiff);
    mpf_clear(localDiv);
}

template<unsigned char Precision>
bool ComplexSetTraits<Precision>::complex_pair::operator==(const complex_pair& other) const
{
    return mpf_cmp(real,other.real) == 0 && mpf_cmp(imag,other.imag) == 0;
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::init(underlying_type& o_value)
{
    mpf_init2(o_value.real,Precision);
    mpf_init2(o_value.imag,Precision);
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::init(underlying_type& o_value, const underlying_type& i_value)
{
    mpf_init2(o_value.real,Precision);
    mpf_set(o_value.real, i_value.real);

    mpf_init2(o_value.imag,Precision);
    mpf_set(o_value.imag, i_value.imag);
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::init(underlying_type& o_value, double real, double imag)
{
    mpf_init2(o_value.real,Precision);
    mpf_set_d(o_value.real, real);

    mpf_init2(o_value.imag,Precision);
    mpf_set_d(o_value.imag, imag);
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::init(underlying_type& o_value, double real)
{
    mpf_init2(o_value.real,Precision);
    mpf_set_d(o_value.real, real);

    mpf_init2(o_value.imag,Precision);
    mpf_set_d(o_value.imag, 0.f);
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::deinit(underlying_type& o_value)
{
    mpf_clear(o_value.real);
    mpf_clear(o_value.imag);
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    mpf_set(o_value.real, i_value.real);
    mpf_set(o_value.imag, i_value.imag);
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::assign(underlying_type& o_value, const complexPair& i_value)
{
    mpf_set_d(o_value.real, i_value[0]);
    mpf_set_d(o_value.imag, i_value[1]);
}
template<unsigned char Precision>
bool ComplexSetTraits<Precision>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    return mpf_cmp(i_lhs.real, i_rhs.real) == 0 && mpf_cmp(i_lhs.imag, i_rhs.imag) == 0;
}
template<unsigned char Precision>
void ComplexSetTraits<Precision>::conjugate(underlying_type& res, const underlying_type& i_value)
{
    mpf_set(res.real, i_value.real);
    mpf_neg(res.imag, i_value.imag);
}

template<unsigned char Precision>
void ComplexGroupTraits<Precision>::sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    mpf_add(res.real, i_lhs.real, i_rhs.real);
    mpf_add(res.imag, i_lhs.imag, i_rhs.imag);
}
template<unsigned char Precision>
void ComplexGroupTraits<Precision>::inv(underlying_type& res, const underlying_type& i_complex)
{
    mpf_neg(res.real, i_complex.real);
    mpf_neg(res.imag, i_complex.imag);
}

template<unsigned char Precision>
void ComplexRingTraits<Precision>::prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    underlying_type localProd[2];
    mpf_init2(localProd[0].real,Precision);
    mpf_init2(localProd[0].imag,Precision);
    mpf_init2(localProd[1].real,Precision);
    mpf_init2(localProd[1].imag,Precision);

    mpf_mul(localProd[0].real, i_lhs.real, i_rhs.real);
    mpf_mul(localProd[0].imag, i_lhs.real, i_rhs.imag);

    mpf_mul(localProd[1].real, i_lhs.imag, i_rhs.imag);
    mpf_neg(localProd[1].real,localProd[1].real);
    mpf_mul(localProd[1].imag, i_lhs.imag, i_rhs.real);

    mpf_add(res.real, localProd[0].real, localProd[1].real);
    mpf_add(res.imag, localProd[0].imag, localProd[1].imag);

    double reall =  mpf_get_d(res.real);
    double imagg =  mpf_get_d(res.imag);

    mpf_clear(localProd[0].real);
    mpf_clear(localProd[1].real);
    mpf_clear(localProd[0].imag);
    mpf_clear(localProd[1].imag);
}
template<unsigned char Precision>
void ComplexRingTraits<Precision>::prod(underlying_type& res, const underlying_type& i_lhs, const typename underlying_type::base_type& i_rhs)
{
    mpf_mul(res.real, i_lhs.real, i_rhs);
    mpf_mul(res.imag, i_lhs.imag, i_rhs);
}
template<unsigned char Precision>
void ComplexRingTraits<Precision>::prod(underlying_type& res, const typename underlying_type::base_type& i_lhs, const underlying_type& i_rhs)
{
    mpf_mul(res.real, i_lhs, i_rhs.real);
    mpf_mul(res.imag, i_lhs, i_rhs.imag);
}
template<unsigned char Precision>
double ComplexRingTraits<Precision>::norm(const underlying_type& i_complex)
{
    mpf_t localProd[2];
    mpf_init2(localProd[0],Precision);
    mpf_init2(localProd[1],Precision);

    mpf_t localSum;
    mpf_init2(localSum,Precision);

    mpf_mul(localProd[0], i_complex.real, i_complex.real);
    mpf_mul(localProd[1], i_complex.imag, i_complex.imag);

    mpf_add(localSum, localProd[0], localProd[1]);

    const double res = mpf_get_d(localSum);

    mpf_clear(localProd[0]);
    mpf_clear(localProd[1]);
    mpf_clear(localSum);

    return res;
}

template<unsigned char Precision>
void ComplexDivisionRingTraits<Precision>::inv(underlying_type& res, const underlying_type& i_complex)
{
    underlying_type localConj;
    mpf_init2(localConj.real,Precision);
    mpf_init2(localConj.imag,Precision);

    ComplexSetTraits<Precision>::conjugate(localConj, i_complex);

    const double realVal = mpf_get_d(i_complex.real);
    const double imagVal = mpf_get_d(i_complex.imag);

    const double norm = ComplexRingTraits<Precision>::norm(i_complex);

    mpf_t localNorm;
    mpf_init2(localNorm,Precision);
    mpf_set_d(localNorm,norm);
    mpf_div(res.real, localConj.real,localNorm);
    mpf_div(res.imag, localConj.imag,localNorm);

    mpf_clear(localConj.real);
    mpf_clear(localConj.imag);
    mpf_clear(localNorm);
}

template<unsigned char Precision>
double ComplexMetricSpaceTraits<Precision>::distance(const typename ComplexSetTraits<Precision>::underlying_type& i_lhs, const typename ComplexSetTraits<Precision>::underlying_type& i_rhs)
{
    underlying_type localSub;
    mpf_init2(localSub.real,Precision);
    mpf_init2(localSub.imag,Precision);

    mpf_sub(localSub.real, i_lhs.real, i_rhs.real);
    mpf_sub(localSub.imag, i_lhs.imag, i_rhs.imag);

    if(mpf_cmp_ui(localSub.real,0) != 0.f && mpf_cmp_ui(localSub.imag,0) != 0.f)
    {
        mpf_t localSqrt;
        mpf_init2(localSqrt,Precision);

        mpf_t localSum;
        mpf_init2(localSum,Precision);

        underlying_type localProd;
        mpf_init2(localProd.real,Precision);
        mpf_init2(localProd.imag,Precision);

        mpf_mul(localProd.real, localSub.real, localSub.real);
        mpf_mul(localProd.imag, localSub.imag, localSub.imag);

        mpf_add(localSum, localProd.real, localProd.imag);

        mpf_sqrt(localSqrt, localSum);

        const double res = mpf_get_d(localSqrt);

        mpf_clear(localSub.real);
        mpf_clear(localSub.imag);
        mpf_clear(localSum);
        mpf_clear(localSqrt);
        mpf_clear(localProd.real);
        mpf_clear(localProd.imag);

        return res;
    }
    else if(mpf_cmp_ui(localSub.real,0) != 0.f)
    {
        const double res = mpf_get_d(localSub.real);

        mpf_clear(localSub.real);
        mpf_clear(localSub.imag);

        return res;
    }
    else
    {
        const double res = mpf_get_d(localSub.imag);

        mpf_clear(localSub.real);
        mpf_clear(localSub.imag);

        return res;
    }
}
template<unsigned char Precision>
void ComplexMetricSpaceTraits<Precision>::get_point_at_distance(underlying_type& res, const underlying_type& i_targetPoint, double i_distance)
{
    //we dont care about a given direction in the complex plane
    mpf_t localDistance;
    mpf_init2(localDistance,Precision);
    mpf_set_d(localDistance,i_distance);

    mpf_add(res.real,i_targetPoint.real,localDistance);
    mpf_set(res.imag,i_targetPoint.imag);

    mpf_clear(localDistance);
}
template<unsigned char Precision>
void ComplexMetricSpaceTraits<Precision>::get_next_elem(underlying_type& o_currPoint, const underlying_type& i_targetPoint)
{
    mpf_t localDiff;
    mpf_init2(localDiff,Precision);

    mpf_sub(localDiff, i_targetPoint.real, o_currPoint.real);

    mpf_t localDiv;
    mpf_init2(localDiv,Precision);

    mpf_div_ui(localDiv, localDiff, 10);

    mpf_add(o_currPoint.real, o_currPoint.real, localDiv);

    mpf_clear(localDiff);
    mpf_clear(localDiv);
}

template<unsigned char Precision>
void QuaternionSetTraits<Precision>::init(underlying_type& o_value)
{
    mpf_init2(o_value.n,Precision);
    mpf_init2(o_value.i,Precision);
    mpf_init2(o_value.j,Precision);
    mpf_init2(o_value.k,Precision);
}
template<unsigned char Precision>
void QuaternionSetTraits<Precision>::init(underlying_type& o_value, const underlying_type& i_value)
{
    mpf_init2(o_value.n,Precision);
    mpf_set(o_value.n, i_value.n);

    mpf_init2(o_value.i,Precision);
    mpf_set(o_value.i, i_value.i);

    mpf_init2(o_value.j,Precision);
    mpf_set(o_value.j, i_value.j);

    mpf_init2(o_value.k,Precision);
    mpf_set(o_value.k, i_value.k);
}
template<unsigned char Precision>
void QuaternionSetTraits<Precision>::init(underlying_type& o_value, double i_n, double i_i, double i_j, double i_k)
{
    mpf_init2(o_value.n,Precision);
    mpf_set_d(o_value.n, i_n);

    mpf_init2(o_value.i,Precision);
    mpf_set_d(o_value.i, i_i);

    mpf_init2(o_value.j,Precision);
    mpf_set_d(o_value.j, i_j);

    mpf_init2(o_value.k,Precision);
    mpf_set_d(o_value.k, i_k);
}
template<unsigned char Precision>
void QuaternionSetTraits<Precision>::deinit(underlying_type& o_value)
{
    mpf_clear(o_value.n);
    mpf_clear(o_value.i);
    mpf_clear(o_value.j);
    mpf_clear(o_value.k);
}
template<unsigned char Precision>
void QuaternionSetTraits<Precision>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    mpf_set(o_value.n, i_value.n);
    mpf_set(o_value.i, i_value.i);
    mpf_set(o_value.j, i_value.j);
    mpf_set(o_value.k, i_value.k);
}
template<unsigned char Precision>
void QuaternionSetTraits<Precision>::assign(underlying_type& o_value, const quaternionQuartet& i_value)
{
    mpf_set_d(o_value.n, i_value[0]);
    mpf_set_d(o_value.i, i_value[1]);
    mpf_set_d(o_value.j, i_value[2]);
    mpf_set_d(o_value.k, i_value[3]);
}
template<unsigned char Precision>
bool QuaternionSetTraits<Precision>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    return mpf_cmp(i_lhs.n, i_rhs.n) == 0 && mpf_cmp(i_lhs.i, i_rhs.i) == 0 && mpf_cmp(i_lhs.j, i_rhs.j) == 0 && mpf_cmp(i_lhs.k, i_rhs.k) == 0;
}
template<unsigned char Precision>
void QuaternionSetTraits<Precision>::conjugate(underlying_type& res, const underlying_type& i_value)
{
    mpf_set(res.n, i_value.n);
    mpf_set(res.i, i_value.i);
    mpf_neg(res.i,res.i);
    mpf_set(res.j, i_value.j);
    mpf_neg(res.j,res.j);
    mpf_set(res.k, i_value.k);
    mpf_neg(res.k,res.k);
}

template<unsigned char Precision>
void QuaternionGroupTraits<Precision>::sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    mpf_add(res.n, i_lhs.n, i_rhs.n);
    mpf_add(res.i, i_lhs.i, i_rhs.i);
    mpf_add(res.j, i_lhs.j, i_rhs.j);
    mpf_add(res.k, i_lhs.k, i_rhs.k);
}
template<unsigned char Precision>
void QuaternionGroupTraits<Precision>::inv(underlying_type& res, const underlying_type& i_value)
{
    mpf_neg(res.n, i_value.n);
    mpf_neg(res.i, i_value.i);
    mpf_neg(res.j, i_value.j);
    mpf_neg(res.k, i_value.k);
}

template<unsigned char Precision>
void QuaternionRingTraits<Precision>::prod(underlying_type& res, const underlying_type& i_lhs, const typename underlying_type::base_type& i_rhs)
{
    mpf_mul(res.n, i_lhs.n, i_rhs);
    mpf_mul(res.i, i_lhs.i, i_rhs);
    mpf_mul(res.j, i_lhs.j, i_rhs);
    mpf_mul(res.k, i_lhs.k, i_rhs);
}
template<unsigned char Precision>
void QuaternionRingTraits<Precision>::prod(underlying_type& res, const typename underlying_type::base_type& i_lhs, const underlying_type& i_rhs)
{
    mpf_mul(res.n, i_lhs, i_rhs.n);
    mpf_mul(res.i, i_lhs, i_rhs.i);
    mpf_mul(res.j, i_lhs, i_rhs.j);
    mpf_mul(res.k, i_lhs, i_rhs.k);
}
template<unsigned char Precision>
void QuaternionRingTraits<Precision>::prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    mpf_t localSum[2];
    mpf_init2(localSum[0],Precision);
    mpf_init2(localSum[1],Precision);

    mpf_t localProd[2];
    mpf_init2(localProd[0],Precision);
    mpf_init2(localProd[1],Precision);

    mpf_t localNeg[2];
    mpf_init2(localNeg[0],Precision);
    mpf_init2(localNeg[1],Precision);

    //! n component

    mpf_mul(localProd[0], i_lhs.n, i_rhs.n);
    mpf_mul(localProd[1], i_lhs.i, i_rhs.i);
    mpf_neg(localNeg[1], localProd[1]);

    mpf_add(localSum[0], localProd[0], localNeg[1]);

    mpf_mul(localProd[0], i_lhs.j, i_rhs.j);
    mpf_neg(localNeg[0], localProd[0]);
    mpf_mul(localProd[1], i_lhs.k, i_rhs.k);
    mpf_neg(localNeg[1], localProd[1]);

    mpf_add(localSum[1], localNeg[0], localNeg[1]);
    mpf_add(res.n, localSum[0], localSum[1]);

    //! i component

    mpf_mul(localProd[0], i_lhs.n, i_rhs.i);
    mpf_mul(localProd[1], i_lhs.i, i_rhs.n);

    mpf_add(localSum[0], localProd[0], localProd[1]);

    mpf_mul(localProd[0], i_lhs.j, i_rhs.k);
    mpf_mul(localProd[1], i_lhs.k, i_rhs.j);
    mpf_neg(localNeg[1], localProd[1]);

    mpf_add(localSum[1], localProd[0], localNeg[1]);
    mpf_add(res.i, localSum[0], localSum[1]);

    //! j component

    mpf_mul(localProd[0], i_lhs.n, i_rhs.j);
    mpf_mul(localProd[1], i_lhs.i, i_rhs.k);
    mpf_neg(localNeg[1], localProd[1]);

    mpf_add(localSum[0], localProd[0], localNeg[1]);

    mpf_mul(localProd[0], i_lhs.j, i_rhs.n);
    mpf_mul(localProd[1], i_lhs.k, i_rhs.i);

    mpf_add(localSum[1], localProd[0], localProd[1]);
    mpf_add(res.j, localSum[0], localSum[1]);

    //! k component

    mpf_mul(localProd[0], i_lhs.n, i_rhs.k);
    mpf_mul(localProd[1], i_lhs.i, i_rhs.j);

    mpf_add(localSum[0], localProd[0], localProd[1]);

    mpf_mul(localProd[0], i_lhs.j, i_rhs.i);
    mpf_neg(localNeg[0], localProd[0]);
    mpf_mul(localProd[1], i_lhs.k, i_rhs.n);
    mpf_neg(localNeg[1], localProd[1]);

    mpf_add(localSum[1], localNeg[0], localNeg[1]);
    mpf_add(res.k, localSum[0], localSum[1]);

    mpf_clear(localSum[0]);
    mpf_clear(localSum[1]);
    mpf_clear(localProd[0]);
    mpf_clear(localProd[1]);
    mpf_clear(localNeg[0]);
    mpf_clear(localNeg[1]);
}
template<unsigned char Precision>
double QuaternionRingTraits<Precision>::norm(const underlying_type& i_quat)
{
    mpf_t localProd[4];
    mpf_init2(localProd[0],Precision);
    mpf_init2(localProd[1],Precision);
    mpf_init2(localProd[2],Precision);
    mpf_init2(localProd[3],Precision);

    mpf_t localSum[3];
    mpf_init2(localSum[0],Precision);
    mpf_init2(localSum[1],Precision);
    mpf_init2(localSum[2],Precision);

    mpf_mul(localProd[0], i_quat.n, i_quat.n);
    mpf_mul(localProd[1], i_quat.i, i_quat.i);
    mpf_mul(localProd[2], i_quat.j, i_quat.j);
    mpf_mul(localProd[3], i_quat.k, i_quat.k);

    mpf_add(localSum[0], localProd[0], localProd[1]);
    mpf_add(localSum[1], localProd[2], localProd[3]);
    mpf_add(localSum[2], localSum[0], localSum[1]);

    const double res = mpf_get_d(localSum[2]);

    mpf_clear(localProd[0]);
    mpf_clear(localProd[1]);
    mpf_clear(localProd[2]);
    mpf_clear(localProd[3]);
    mpf_clear(localSum[0]);
    mpf_clear(localSum[1]);
    mpf_clear(localSum[2]);

    return res;
}

template<unsigned char Precision>
void QuaternionDivisionRingTraits<Precision>::inv(underlying_type& res, const underlying_type& i_quat)
{
    underlying_type localConj;
    mpf_init2(localConj.n,Precision);
    mpf_init2(localConj.i,Precision);
    mpf_init2(localConj.j,Precision);
    mpf_init2(localConj.k,Precision);

    QuaternionSetTraits<Precision>::conjugate(localConj, i_quat);

    const double norm = QuaternionRingTraits<Precision>::norm(i_quat);
    mpf_div_ui(res.n, localConj.n, norm);
    mpf_div_ui(res.i, localConj.i, norm);
    mpf_div_ui(res.j, localConj.j, norm);
    mpf_div_ui(res.k, localConj.k, norm);

    mpf_clear(localConj.n);
    mpf_clear(localConj.i);
    mpf_clear(localConj.j);
    mpf_clear(localConj.k);
}

template<unsigned char Precision>
double QuaternionMetricSpaceTraits<Precision>::distance(const typename QuaternionSetTraits<Precision>::underlying_type& i_lhs, const typename QuaternionSetTraits<Precision>::underlying_type& i_rhs)
{
    mpf_t localProd[4];
    mpf_init2(localProd[0],Precision);
    mpf_init2(localProd[1],Precision);
    mpf_init2(localProd[2],Precision);
    mpf_init2(localProd[3],Precision);

    mpf_t localSum[3];
    mpf_init2(localSum[0],Precision);
    mpf_init2(localSum[1],Precision);
    mpf_init2(localSum[2],Precision);

    mpf_mul(localProd[0], i_lhs.n, i_rhs.n);
    mpf_mul(localProd[1], i_lhs.i, i_rhs.i);
    mpf_mul(localProd[2], i_lhs.j, i_rhs.j);
    mpf_mul(localProd[3], i_lhs.k, i_rhs.k);

    mpf_add(localSum[0], localProd[0], localProd[1]);
    mpf_add(localSum[1], localProd[2], localProd[3]);
    mpf_add(localSum[2], localSum[0], localSum[1]);

    const double res = mpf_get_d(localSum[2]);

    mpf_clear(localProd[0]);
    mpf_clear(localProd[1]);
    mpf_clear(localProd[2]);
    mpf_clear(localProd[3]);
    mpf_clear(localSum[0]);
    mpf_clear(localSum[1]);
    mpf_clear(localSum[2]);

    return res;
}

template<int p>
void ZpSetTraits<p>::init(underlying_type& o_value)
{
    mpz_init(o_value);
}
template<int p>
void ZpSetTraits<p>::init(underlying_type& o_value, const underlying_type& i_value)
{
    mpz_init(o_value);
    mpz_set(o_value, i_value);
}
template<int p>
void ZpSetTraits<p>::init(underlying_type& o_value, int i_value)
{
    mpz_init(o_value);
    mpz_set_si(o_value, i_value);
}
template<int p>
void ZpSetTraits<p>::deinit(underlying_type& o_value)
{
    mpz_clear(o_value);
}
template<int p>
void ZpSetTraits<p>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    mpz_set(o_value, i_value);
}
template<int p>
void ZpSetTraits<p>::assign(underlying_type& o_value, const int& i_value)
{
    mpz_set_si(o_value, i_value);
}
template<int p>
bool ZpSetTraits<p>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    return mpz_cmp(i_lhs, i_rhs) == 0;
}

template<int p>
void ZpGroupTraits<p>::sum(underlying_type& o_value, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    mpz_t localSum;
    mpz_init(localSum);

    mpz_add(localSum, i_lhs, i_rhs);

    mpz_mod_ui(o_value, localSum, p);

    mpz_clear(localSum);
}
template<int p>
void ZpGroupTraits<p>::inv(underlying_type& o_value, const underlying_type& i_zp)
{
    mpz_ui_sub(o_value, p, i_zp);
}

template<int p>
void ZpRingTraits<p>::prod(underlying_type& o_value, const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    mpz_t localProd;
    mpz_init(localProd);

    mpz_mul(localProd, i_lhs, i_rhs);

    mpz_mod_ui(o_value, localProd, p);

    mpz_clear(localProd);
}

template<int p>
void ZpDivisionRingTraits<p>::inv(underlying_type& o_value, const underlying_type& i_zp)
{
    mpz_t localPow;
    mpz_init(localPow);

    mpz_pow_ui(localPow, i_zp, p-1);

    mpz_mod_ui(o_value, localPow, p);

    mpz_clear(localPow);
}

}
}
