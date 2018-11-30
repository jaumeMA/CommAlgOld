
namespace yame
{

template<typename T, int N>
container::string format(const math::vector<T,N>& i_value)
{
    container::string res = ")";

    for(size_t index=0;index<N;++index)
    {
        res += format(i_value[index]) + " ";
    }

    res += ")";

    return res;
}

template<typename T, int N, int M>
container::string format(const math::matrix<T,N,M>& i_value)
{
    container::string res = ")";

    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            res += format(i_value[indexI][indexJ]) + " ";
        }

        res += "; ";
    }

    res += ")";

    return res;
}

}
