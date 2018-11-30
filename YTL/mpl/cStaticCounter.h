#pragma once

#pragma GCC diagnostic ignored "-Wnon-template-friend"

namespace yame
{
namespace mpl
{
namespace detail
{

template<typename T, int N>
struct counter_tag
{
    friend constexpr int counter_index(const counter_tag<T,N>&);
};

template<typename T, int N>
struct producer
{
    friend constexpr int counter_index(const counter_tag<T,N>&)
    {
        return N;
    }

    static constexpr int value = counter_index(counter_tag<T,N>{});
};

template<typename T, int N, int = producer<T,N>::value>
int constexpr consumer(float, const counter_tag<T,N>&, ...)
{
    return N;
}

template<typename T, int N, int V = counter_index(counter_tag<T,N>{})>
int constexpr consumer(int,const counter_tag<T,N>&, int R = consumer<T>(0,counter_tag<T,N+1> {}))
{
    return R;
}

template<typename T, int N>
int constexpr curr_tag(float, const counter_tag<T,N>&)
{
    return N - 1;
}

template<typename T, int N, int V = counter_index(counter_tag<T,N>{})>
int constexpr curr_tag(int,const counter_tag<T,N>&, int R = curr_tag<T>(0,counter_tag<T,N+1> {}))
{
    return R;
}

template<typename T>
int constexpr next_value(int R = consumer<T>(0,counter_tag<T,0> {}))
{
    return R;
}

template<typename T>
int constexpr curr_value(int R = curr_tag<T>(0,counter_tag<T,0> {}))
{
    return R;
}

}

template<typename T>
struct static_counter
{
  static constexpr int get_next_count(int res = detail::next_value<T>())
  {
      return res;
  }
  static constexpr int get_curr_count(int res = detail::curr_value<T>())
  {
      return res;
  }
};

}
}
