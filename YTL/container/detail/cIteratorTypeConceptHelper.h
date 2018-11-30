#pragma once

namespace yame
{
namespace container
{

template<typename Type>
concept bool input_iterator_type = requires (Type& i_iterator){ { *i_iterator } } && (Type& i_iterator){ { i_iterator-> } } && (Type& i_iterator){ { ++i_iterator } } && (Type& i_iterator){ { *i_iterator++ } }

//TODO
//template<typename Type>
//concept bool output_iterator_type = { }
//
//template<typename Type>
//concept bool forward_iterator_type = { }
//
//template<typename Type>
//concept bool backward_iterator_type = { }
//
//template<typename Type>
//concept bool bidirectional_iterator_type = { }
//
//template<typename Type>
//concept bool random_access_iterator_type = { }

}
}
