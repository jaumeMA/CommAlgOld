namespace yame
{
    namespace container
    {
        template<typename T, int rank, int ... ranks>
        cTupla<T,rank,ranks...>::cTupla()
        : detail::cTupla_interface<T,rank,ranks...>()
        {
        }
    }
}

