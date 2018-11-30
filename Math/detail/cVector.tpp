
#include "Math/cMath.h"

namespace yame
{
namespace math
{
template <typename T, u8 dim>
cVector<T,dim>::cVector(T value)
: baseType()
{
    for(u16 i=0;i<dim;i++)
    {
        (*this)[i] = value;
    }
}
template <typename T, u8 dim>
cVector<T,dim>::cVector(const baseType& tuple)
: baseType(tuple)
{
}
template <typename T, u8 dim>
cVector<T,dim>::cVector(const cVector<T,dim>& other)
: baseType(other)
{
}
template <typename T, u8 dim>
cVector<T,dim>::cVector(T *data, u8 length)
: baseType()
{
    for(u16 i=0;i<length;i++)
    {
        (*this)[i] = data[i];
    }
}
template <typename T, u8 dim>
cVector<T,dim>::~cVector<T,dim>()
{
}
template <typename T, u8 dim>
void cVector<T,dim>::repeat(T value)
{
    for(u16 i=0;i<dim;i++)
    {
        (*this)[i] = value;
    }
}
template <typename T, u8 dim>
cVector<T,dim>& cVector<T,dim>::operator = (const cVector<T,dim>& summand)
{
    container::cTupla<T,dim>::operator=(summand);
    //this->m_subTuple = summand.m_subTuple;

    return *this;
}
template <typename T, u8 dim>
cVector<T,dim>& cVector<T,dim>::operator=(const T& value)
{
  //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        (*this)[i] = value;
    }

    return *this;
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator==(const cVector<T,dim>& summand) const
{
    bool res = true;

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        if((*this)[i] != summand[i])
        {
            res = false;
            break;
        }
    }

    return res;
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator!=(const cVector<T,dim>& summand) const
{
    return !(*this==summand);
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator<(const cVector<T,dim>& summand) const
{//ordre lexicografic
    bool res = false;

    for(u16 i=0;i<dim;i++)
    {
        if((*this)[i] < summand[i])
        {
            res = true;
            break;
        }
        else if((*this)[i] > summand[i])
        {
            break;
        }
    }

    return res;
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator>(const cVector<T,dim>& summand) const
{
    return !(*this<summand);
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator<(const T& value) const
{
    bool res = true;

    for(u32 i=0;i<dim;i++)
    {
        res = res && (*this)[i] < value;
    }

    return res;
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator>(const T& value) const
{
    return !(*this < value);
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator<=(const cVector<T,dim>& summand) const
{
    return *this<(summand) || *this==(summand);
}
template <typename T, u8 dim>
bool cVector<T,dim>::operator>=(const cVector<T,dim>& summand) const
{
    return *this>(summand) || *this==(summand);
}
template <typename T, u8 dim>
cVector<T,dim> cVector<T,dim>::operator + (const cVector<T,dim>& summand) const
{
    cVector<T,dim> res = cVector<T,dim>();

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        res[i] = (*this)[i] + summand[i];
    }

    return res;
}
template <typename T, u8 dim>
cVector<T,dim>& cVector<T,dim>::operator += (const cVector<T,dim> &summand)
{
    *this = *this + summand;

    return *this;
}
template <typename T, u8 dim>
cVector<T,dim> cVector<T,dim>::operator- (const cVector<T,dim>& summand) const
{
    cVector<T,dim> res = cVector<T,dim>();

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        res[i] = (*this)[i] - summand[i];
    }

    return res;
}
template <typename T, u8 dim>
cVector<T,dim> cVector<T,dim>::operator-() const
{
    cVector<T,dim> res = cVector<T,dim>();

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        res[i] = -(*this)[i];
    }

    return res;
}
template <typename T, u8 dim>
cVector<T,dim>& cVector<T,dim>::operator-=(const cVector<T,dim> &summand)
{
    *this = *this - summand;

    return *this;
}
template <typename T, u8 dim>
T cVector<T,dim>::operator*(const cVector<T,dim> &summand) const
{
    T res = (T)0;

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        res += (*this)[i]*summand[i];
    }

    return res;
}
template <typename T, u8 dim>
T cVector<T,dim>::operator*=(const cVector<T,dim> &summand)
{
    *this = *this * summand;

    return *this;
}
template <typename T, u8 dim>
cVector<T,dim> cVector<T,dim>::operator* (const T &value) const
{
    cVector<T,dim> res = cVector<T,dim>();

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        res[i] = (*this)[i]*value;
    }

    return res;
}
template <typename T, u8 dim>
cVector<T,dim>& cVector<T,dim>::operator*=(const T& value)
{
    *this = *this*value;

    return *this;
}
template <typename T, u8 dim>
cVector<T,dim> cVector<T,dim>::operator/(const T& value) const
{
    cVector<T,dim> res = cVector<T,dim>();
    T invValue = inv(value);

    res = *this*invValue;

    return res;
}
template <typename T, u8 dim>
cVector<T,dim>& cVector<T,dim>::operator/=(const T& value)
{
    *this = *this/value;

    return *this;
}
template <typename T, u8 dim>
cVector<T,dim>::operator container::cTupla<T,dim>()
{
    return *this;
}
template <typename T, u8 dim>
const T* cVector<T,dim>::getComp() const
{
    return *this.getRef();
}
template <typename T, u8 dim>
T cVector<T,dim>::sum() const
{
    T res = T();

    for(u16 i=0;i<dim;i++)
    {
        res += (*this)[i];
    }

    return res;
}
template <typename T, u8 dim>
T cVector<T,dim>::norm()
{
    T sum = (T)0;

    //PARALLELIZE!!
    for(u16 i=0;i<dim;i++)
    {
        sum += (*this)[i]*(*this)[i];
    }

    T norma = sqrt(sum);

    return norma;
}
template <typename T, u8 dim>
void cVector<T,dim>::normalize()
{
    T invNorma, sum;
    sum = (T)0;

    if(*this != cVector<T,dim>())
    {
        //PARALLELIZE!!
        for(u16 i=0;i<dim;i++)
        {
            sum += (*this)[i]*(*this)[i];
        }
        invNorma = invSqrt(sum);
        *this *= invNorma;
    }

    return;
}
template <typename T, u8 dim>
bool cVector<T,dim>::isInNeighborhood(const cVector<T,dim>& ref, const cVector<T,dim>& point, f32 radius)
{
    bool res;
    cVector<T,dim> dif = point - ref;

    res = dif.norm() < radius;

    return res;
}
template <typename T, u8 dim>
bool cVector<T,dim>::isInNeighborhood(const cVector<T,dim>& point, f32 radius)
{
    return isInNeighborhood(*this,point,radius);
}
template <typename T, u8 dim>
void cVector<T,dim>::dotProd(const cVector<T,dim>& other)
{
    cVector<T,dim> res = cVector<T,dim>();

    res = res.embedding(dim);

    for(u16 i=0;i<dim;i++)
    {
        res[i] = (*this)[i]*other[i];
    }

    return res;
}
template <typename T, u8 dim>
cVector<T,mpl::NoverM<dim,2>::value> cVector<T,dim>::operator^(const cVector<T,dim>& other) const
{
    u16 index = 0;
    const u16 numCombs = factorial(dim)/2;
    u16 comb[numCombs*dim];
    combination(dim,2,1,1,ETC_NO_REPETITIION,&(comb[0]));
    cVector<T,mpl::NoverM<dim,2>::value> res = cVector<T,mpl::NoverM<dim,2>::value>();

    for(u16 i=0;i<numCombs;i++)
    {
        u16 subIndex = (comb[i*2] == 1) ? (comb[i*2 + 1] == 2) ? 3 : 2 : 1;
        res[subIndex - 1] = math::parity(i)*((*this)[comb[i*2]-1]*other[comb[i*2+1]-1] - (*this)[comb[i*2+1]-1]*other[comb[i*2]-1]);
    }

    return res;
}
template <typename T, u8 dim>
T cVector<T,dim>::innerProd()
{
    T res = (*this)[0];

    for(u8 i=1;i<dim;i++)
    {
        res *= (*this)[i];
    }

    return res;
}
template <typename T, u8 dim>
f32 cVector<T,dim>::angleBetween(const cVector<T,dim>& vect1, const cVector<T,dim>& vect2)
{
    f32 res = 0;

    //per evitar rollos de moduls normalitzem els vectors
    cVector<T,dim> tmp1 = vect1;
    cVector<T,dim> tmp2 = vect2;
    //apliquem la formuleta del cosinus, que es molt curteta en tenir vectors unitaris
    tmp1.normalize();
    tmp2.normalize();

    res = acos(tmp1*tmp2);

    return res;
}
template <typename T, u8 dim>
template<int dimensio>
cVector<T,dim> cVector<T,dim>::embedding()
{
    cVector<T,dimensio> res = cVector<T,dimensio>();

    for(u8 i=0;i<dimensio;i++)
    {
        if(i < dim)
        {
            res[i] = (*this)[i];
        }
        else
        {
            res[i] = 0;
        }
    }

    return res;
}
template <typename T, u8 dim>
bool cVector<T,dim>::colineal(const cVector<T,dim>& other)
{
    bool res = true;

    f32 ratio = other[0]/(*this)[0];
    for(u16 i=1;i<dim;i++)
    {
        if(ratio != other[i]/(*this)[i])
        {
            res = false;
            break;
        }
    }

    return res;
}
template <typename T, u8 dim>
void cVector<T,dim>::ortoNormalize(container::cArray<cVector<T,dim> >& base)
{//apliquem el metode de Ghram-Schmidt

    for(u8 i=0;i<base.getSize();i++)
    {
        for(u8 j=0;j<i;j++)
        {
            base[i] -= base[j]*(base[i]*base[j]);
        }
        base[i].normalize();
    }

    return;
}
template <typename T, u8 dim>
const T* cVector<T,dim>::getRawVector() const
{
    return this->getTupla();
}
}
}
