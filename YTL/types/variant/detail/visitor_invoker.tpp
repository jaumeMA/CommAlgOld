
namespace yame
{
    namespace ytl
    {
        namespace detail
        {
            //invoke for inner visitors
            template<typename visitor, typename ... Types>
            template<typename Type>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::inner_invoke(visitor& _visitor, const variant_impl<Types...>& var)
            {
                typedef typename variant_type<Type>::ref_type ref_type;

        #if defined(_WIN32)
                return _visitor.operator()<Type>(std::forward<ref_type>(var.template get<Type>()));
        #else
                return _visitor.template operator()<Type>(std::forward<ref_type>(var.template get<Type>()));
        #endif
            }

            template<typename visitor, typename ... Types>
            template<typename Type>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::inner_invoke(visitor& _visitor, variant_impl<Types...>& var)
            {
                typedef typename variant_type<Type>::ref_type ref_type;

        #if defined(_WIN32)
                return _visitor.operator()<Type>(std::forward<ref_type>(var.template get<Type>()));
        #else
                return _visitor.template operator()<Type>(std::forward<ref_type>(var.template get<Type>()));
        #endif
            }

            //invoke for outer visitors
            template<typename visitor, typename ... Types>
            template<typename Type>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::outer_invoke(visitor& _visitor, const variant_impl<Types...>& var)
            {
                typedef typename variant_type<Type>::ref_type ref_type;

                return _visitor.operator()(std::forward<ref_type>(var.template get<Type>()));
            }

            template<typename visitor, typename ... Types>
            template<typename Type>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::outer_invoke(visitor& _visitor, variant_impl<Types...>& var)
            {
                typedef typename variant_type<Type>::ref_type ref_type;

                return _visitor.operator()(std::forward<ref_type>(var.template get<Type>()));
            }

            //const version
            template<typename visitor, typename ... Types>
            template<typename vvisitor>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::invoke(const uint8_t& currType, const visitor& _visitor, const variant_impl<Types...>& var, typename vvisitor::t_visitor* foo)
            {
                //JAUME: for inner visitors, attack directly call operator
                typedef result_type(*funcType)(visitor&, const variant_impl<Types...>&);

                static const funcType funcTable[_numTypes] = { &variant_visitor_invoker<visitor, Types...>::inner_invoke<Types> ... };

                return (*funcTable[currType])(const_cast<visitor&>(_visitor), var);
            }

            template<typename visitor, typename ... Types>
            template<typename vvisitor>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::invoke(const uint8_t& currType, const visitor& _visitor, const variant_impl<Types...>& var, ...)
            {
                //JAUME: for outer visitors, we know nothing about them, use one more indirection
                typedef result_type(*funcType)(visitor&, const variant_impl<Types...>&);

                static const funcType funcTable[_numTypes] = { &variant_visitor_invoker<visitor, Types...>::outer_invoke<Types> ... };

                return (*funcTable[currType])(const_cast<visitor&>(_visitor), var);
            }

            //non const version
            template<typename visitor, typename ... Types>
            template<typename vvisitor>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::invoke(const uint8_t& currType, const visitor& _visitor, variant_impl<Types...>& var, typename vvisitor::t_visitor* foo)
            {
                //JAUME: for inner visitors
                typedef result_type(*funcType)(visitor&, const variant_impl<Types...>&);

                static const funcType funcTable[_numTypes] = { &variant_visitor_invoker<visitor, Types...>::inner_invoke<Types> ... };

                return (*funcTable[currType])(const_cast<visitor&>(_visitor), var);
            }

            template<typename visitor, typename ... Types>
            template<typename vvisitor>
            typename visitor::result_type variant_visitor_invoker<visitor, Types...>::invoke(const uint8_t& currType, const visitor& _visitor, variant_impl<Types...>& var, ...)
            {
                //JAUME: for outer visitors
                typedef result_type(*funcType)(visitor&, variant_impl<Types...>&);

                static const funcType funcTable[_numTypes] = { &variant_visitor_invoker<visitor, Types...>::outer_invoke<Types> ... };

                return (*funcTable[currType])(const_cast<visitor&>(_visitor), var);
            }
        }
    }
}
