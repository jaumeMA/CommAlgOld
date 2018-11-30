
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

#include <math.h>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include "Utils/cBitUtilities.h"
#include "Utils/engineTypes.h"
#include <cmath>

namespace yame
{
namespace math
{
enum E_TYPE_COMBINATORICS
{
    ETC_REPETITION,
    ETC_NO_REPETITIION,
    ETC_COUNT
};
enum E_TYPE_DCFT
{
    ETD_TYPEII,
    ETD_TYPEIV,
    ETD_COUNT
};
enum E_COMPONENT_LETTER
{
    ECL_X,
    ECL_Y,
    ECL_Z,
    ECL_W,
    ECL_COUNT
};
enum E_ORDER
{
EPO_LEX,
EPO_GRLEX,
EPO_GREVLEX,
EPO_COUNT
};


#define DEG2RAD(deg) (deg)*M_PI/180
#define FLOAT_FTOI_MAGIC_NUM (float)(3<<21)
#define IT_FTOI_MAGIC_NUM (0x4ac00000)


inline u32 FastFloatToInt(f32 f)
{
    f += FLOAT_FTOI_MAGIC_NUM;
    return (*((int*)&f) - IT_FTOI_MAGIC_NUM)>>1;
}
inline bool even(u8 number)
{
    return (number&0x01)==0;
}
inline bool odd(u8 number)
{
    return !even(number);
}
inline bool even(u16 number)
{
    return (number&0x01)==0;
}
inline bool odd(u16 number)
{
    return !even(number);
}
inline bool even(u32 number)
{
    return (number&0x01)==0;
}
inline bool odd(u32 number)
{
    return !even(number);
}
inline s8 parity(u32 number)
{
    s8 res = ((number&0x01) == 1) ? -1 : 1;
    return res;
}
inline s8 sign(s8 number)
{
    s8 res = ((number&(1<<7)) == 0) ? 1 : -1;
    return res;
}
inline s16 sign(s16 number)
{
    s16 res = ((number&(1<<15)) == 0) ? 1 : -1;
    return res;
}
inline s8 sign(s32 number)
{
    s8 res = ((number&(1<<31)) == 0) ? 1 : -1;
    return res;
}
inline u8 abs(s8 number)
{
    u8 res = number & ~(1<<7);
    return res;
}
inline u16 abs(s16 number)
{
    u16 res = number & ~(1<<15);
    return res;
}
inline u32 abs(s32 number)
{
    u32 res = number & ~(1<<31);
    return res;
}
inline u32 nextPowerOf2(u32 number)
{
    if (number != 0)
    {
        u32 res = number - 1;

        res |= res >> 1;
        res |= res >> 2;
        res |= res >> 4;
        res |= res >> 8;
        res |= res >> 16;

        res++;

        return res;
    }
    else
    {
        return 1;
    }
}
inline u32 powerOf2BitPos(u32 number)
{
    if (number != 0)
    {
        const unsigned char firstNonZeroBit = (sizeof(u32) << 3) - __builtin_clz(number);

        return ((number & (number -1)) == 0) ? firstNonZeroBit : firstNonZeroBit + 1;
    }
    else
    {
        return 0;
    }
}
inline bool powerOf2(u32 number)
{
    return (number != 0)&&((number&(number-1))==0);
}
inline f32 invSqrt(f32 number)
{
    f32 numHalf = 0.5f*number;
    s32 numInt = *(s32*)&number;
    numInt = 0x5f3759d5 - (numInt >> 1);
    number = *(float*)&numInt;
    number = number*(1.5f - numHalf*number*number);
    number = number*(1.5f - numHalf*number*number);
    number = number*(1.5f - numHalf*number*number);

    return number;
}
inline f32 inv(f32 number)
{
    u32 *numInt = (u32 *)&number;
    *numInt = 0x7F000000 - *numInt;

    return number;
}
inline u32 factorial(const u32& number)
{
    u32 res = 0;

    if(number <= 1)
        res = 1;
    else
        res = number*factorial(number-1);

    return res;
}
inline u32 product(u8 rank, u8 val, ...)
{
    u32 res = val;
    va_list args;
    va_start(args,val);

    for(u16 i=1;i<rank;i++)
    {
        res *= va_arg(args,int);
    }
    va_end(args);

    return res;
}
inline u16 NoverM(u16 N, u16 M)
{
    u16 res = 1;

    if(N>M)
    {
        res = factorial(N)/(factorial(M)*factorial(N-M));
    }

    return res;
}
inline void sort(u16 *list, u16 length)
{
    u16 counter = 0;
    u16 temp = 0;

    for(u16 i=0;i<length;i++)
    {
        for(s16 j=i-1;j>=0;j--)
        {
            if(list[i]>list[(u16)j])
            {
                break;
            }
            counter++;
        }
        temp = list[i-counter];
        list[i-counter] = list[i];
        list[i] = temp;
        counter = 0;
    }
}
inline void permutation(u8 *perm, s8 *sign, u8 number, u8 level=3)
{
    u32 factLevel = factorial(level-1);
    u32 factNumber = factorial(number);
    u8 counter = level-1;
    s8 shift = -1;
    u8 numInv = 0;

    if(level == 3)
    {
        *perm = 1;
        *(perm+1) = 2;
        *(perm+number) = 2;
        *(perm+number+1) = 1;
        sign[0] = 1;
        sign[1] = -1;
    }

    if(level <= number)
    {
        u8 *currPerm = (u8 *)malloc(factNumber*number*sizeof(u8));
        memcpy(currPerm,perm,factNumber*number*sizeof(u8));
        s8 *currSign = (s8 *)malloc(factNumber*sizeof(s8));
        memcpy(currSign,sign,factNumber*sizeof(s8));

        for(u16 i=0;i<factLevel;i++)
        {
            for(u8 j=0;j<level;j++)
            {
                sign[j+i*level] = currSign[i];
                memcpy(perm+number*(j+i*level),currPerm+i*number,sizeof(u8)*number);
            }
        }

        memcpy(currPerm,perm,factNumber*number*sizeof(u8));

        for(u16 i=0;i<level*factLevel;i++)
        {
            sign[i] *= parity((u32)numInv);
            if(i%level == 0 && i > 0)
                shift *= -1;
            for(u8 j=counter;j<number-1;j++)
                *(perm + i*number + j + 1) = *(currPerm + i*number + j);

            *(perm + i*number + counter) = level;

            if((counter > 0 && shift == -1) || (counter < number-1 && shift == 1))
            {
                numInv -= shift;
                counter += shift;
            }
        }
        level += 1;
        free(currPerm);
        free(currSign);
        //recursivitat
        permutation(perm,sign,number,level);
    }

    return;
}
inline void combination(u16 number, u16 subNumber, u16 currNumber, u16 level, E_TYPE_COMBINATORICS type, u16 *comb)
{
    u16 factPrev = 0;
    u16 factAcc = 0;

    if(number>=subNumber && number>0)
    {
        for(u16 i=0;i<number-(currNumber-1)-(subNumber-level);i++)
        {
            factPrev = factorial(number-currNumber-i)/(factorial(subNumber-level)*factorial(number+level-(subNumber+currNumber+i)));
            for(u16 j=0;j<factPrev;j++)
            {
                comb[j*subNumber+subNumber*factAcc] = i+currNumber;
            }
            if(level < subNumber)
            {
                combination(number,subNumber,currNumber+1+i,level+1,type,comb+1+subNumber*factAcc);
            }
            factAcc += factPrev;
        }
    }

    return;
}
inline void ballsInBoxes(u8 balls, u8 boxes, u16 *comb)
{
    u16 numCombs = NoverM(balls+boxes-1,boxes-1);
    u16 totalSize = (numCombs+1)*boxes;
    u16 lastBox = 0;

    memset(comb,0,totalSize*sizeof(u16));

    if(balls > 0 && boxes > 0)
    {
        u16 *localComb = (u16 *)malloc(totalSize*sizeof(u16));

        combination(balls+boxes-1,boxes-1,1,1,ETC_NO_REPETITIION,localComb);

        for(u16 i=0;i<numCombs;i++)
        {
            sort(localComb+i*(boxes-1),boxes-1);
            lastBox = 0;
            for(u16 j=0;j<boxes-1;j++)
            {
                comb[i*boxes+j] = localComb[i*(boxes-1)+j] - lastBox - 1;
                lastBox = localComb[i*(boxes-1)+j];
            }
            //Valgrind says there is an invalid read here
            comb[(i+1)*boxes-1] = balls + boxes - 1 - localComb[i*(boxes-1)+boxes-1-1];
        }

        free(localComb);
    }

    return;
}
inline void expansion(u16 *exp, u8 rank, u8 val, ...)
{
    u32 dim = val;
    u8 *rankVector = (u8 *)malloc(rank*sizeof(u8));
    u8 shift = 0;
    u32 counter = 1;

    va_list args;
    va_start(args,val);
    rankVector[0] = val;

    for(u16 i=1;i<rank;i++)
    {
        rankVector[i] = va_arg(args,int);
        dim *= rankVector[i];
    }
    va_end(args);

    memset(exp,0,dim*rank*sizeof(u16));

    while(counter < dim)
    {
        memcpy(exp+counter*rank,exp+(counter-1)*rank,rank*sizeof(u16));

        if(exp[counter*rank+shift]+1 < rankVector[shift])
        {
            exp[counter*rank+shift] += 1;
        }
        else
        {
            do
            {
                shift++;
            }
            while(exp[counter*rank+shift]+1 >= rankVector[shift]);
            exp[counter*rank+shift] += 1;
            //we reset the lower part of teh expansion
            memset(exp+counter*rank,0,shift*sizeof(u16));
            shift = 0;
        }
        counter++;
    }
    free(rankVector);

    return;
}
inline u32 squarePow(u32 number)
{
    return (1<<number);
}
inline u32 logBase(u32 base, u32 n)
{
    u32 res = 0;

    if(base == 2)
    {
        const unsigned int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
        const unsigned int S[] = {1, 2, 4, 8, 16};
        int i;

        for (i = 4; i >= 0; i--) // unroll for speed...
        {
          if (n & b[i])
          {
            n >>= S[i];
            res |= S[i];
          }
        }
    }
    else if(base == 10)
    {


    }
    else
        res = (u32)(log(n)/log(base));

    return res;
}
inline u32 getByteRepresentation(u32 number, u8*& byteRep)
{
    u32 counter = 0;

    if(byteRep)
    {
        u32 remain = 0;
        u32 localDiv = 0;
        bool done = false;

        while(!done)
        {
            localDiv = number/256;
            remain = number - 256*(localDiv);
            byteRep[counter++] = remain;
            number = localDiv;
            done = number == 0;
        }
    }

    return counter;
}
inline f32 fastSqrt(f32 x)
{
    u32 t = *(u32*)&x;

    t -= 0x3f800000;
    t >>= 1;
    t += 0x3f800000;

    return *(float*)&t;
}
inline u32 intSqrt(u32 number)
{
    u32 res = 0;

    u32 arrelTemp = 0;
    u16 bit = 1;
    u8 counter = 0;

    while(1)
    {
        if(u32(bit<<counter)>=number)
        {
            break;
        }

        counter++;
    }

    res = squarePow(u32(counter/2));
    bit = squarePow(u32(counter/2));

    for(u16 j=u32(counter/2);j>=0;j--)
    {
        arrelTemp = res | bit;
        if(arrelTemp*arrelTemp > number)
            arrelTemp = arrelTemp & ~bit;
        bit>>=1;
        res = arrelTemp;
    }

    return res;
}
inline void solveSeconGradeEq(f32 A, f32 B, f32 C, f32 rootA, f32 rootB)
{//here we solve a second grade equation given by: Ax^2 + Bx + C = 0

    rootA = (-B + sqrt(B*B - 4*A*C))*inv(2*A);
    rootB = (-B - sqrt(B*B - 4*A*C))*inv(2*A);

    return;
}
inline u32 gcd(u32 numberA, u32 numberB)
{
    u32 res = 0;
    if(numberA != numberB)
    {
        u32 a = (numberA > numberB) ? numberA : numberB;
        u32 b = (numberA < numberB) ? numberA : numberB;
        u32 remainder = 1;

        while(remainder != 0)
        {
            res = remainder;
            remainder = a%b;
            a = b;
            b = remainder;
        }
    }
    else
        res = numberA;

    return res;
}
inline u32 lcm(u32 numberA, u32 numberB)
{
    u32 res = 0;
    if(numberA != numberB)
    {
        u32 a = (numberA > numberB) ? numberA : numberB;
        u32 b = (numberA < numberB) ? numberA : numberB;

        res = (a/gcd(a,b))*b;
    }
    else
        res = numberA;

    return res;
}
inline u32 phiEuler(u32 n)
{
    u32 res = 1;

    for(u16 count = 2;count<n;count++)
        if(gcd(count,n)==1)
            res *= count;

    return res;
}
inline u32 mod(u32 p, s32 n)
{
    u32 res = 0;

    while(n<0)
        n += p;

    n %= p;

    return res;
}
inline bool isPrime(u32 n)
{
    bool res = true;
    bool sortir = false;
    u32 s = 0;
    u32 r = 0;
    u32 a = 0;
    u32 y = 0;
    u16 j = 0;
    u32 temp = n-1;
    u32 counter;

    while(temp%2==0)
        temp /= 2;
    r = temp;
    s = (n-1)/temp;

    for(u8 k=0;k<20&&!sortir;k++)
    {
        a = 2 + rand()%(n-2);
        y = a;
        counter = 0;
        while(counter < r-1)
        {
            y *= a;
            y %= n;
            counter++;
        }
        if(y!=1 && y!=n-1)
        {
            j = 1;
            while(j <= s-1 && y != n-1 && !sortir)
            {
                y = (y*y)%n;
                if(y == 1)
                {
                    sortir = true;
                    res = false;
                }
                else
                    j++;
            }
            if(y != n-1)
            {
                res = false;
                sortir = true;
            }
        }
    }

    return res;
}
inline f64 fastExp(f64 a)
{
    const f64 expA = 1048576/M_LN2;
    const f64 expB = 6081;
    union expUnion
    {
            f64 d;
            struct
            {
#ifdef LITTLE_ENDIAN
                s32 j, i;
#else
                s32 i, j;
#endif
            } n;
    } res;

    //remember that in unions the space of memory reserved for it can be named by, in this case, d and i,j
    res.n.j = 0;
    res.n.i = (s32)(expA*a + (1072692348 - expB));

    printf("retornem %f\n",res.d);

    return res.d;
}
inline f64 fastPow(f64 a, f64 b)
{
    union
    {
            f64 d;
            struct
            {
#ifdef LITTLE_ENDIAN
                s32 j, i;
#else
                s32 i, j;
#endif
            } n;
    }res;

    //remember that in unions the space of memory reserved for it can be named by, in this case, d and i,j
    res = {a};
    res.n.i = (s32)(b * (res.n.i - 1072632447) + 1072632447);
    res.n.j = 0;

    return res.d;
}

}//namespace math

//general case
template<typename T>
inline T max(T a, T b, ...)
{
    return (a < b) ? b : a;
}
template<typename T>
inline T min(T a, T b, ...)
{
    return (a < b) ? a : b;
}

}//namespace yame
