#pragma once

namespace yame
{
namespace container
{
namespace detail
{

template<typename Map,typename Node>
class cAVLBalancer
{
private:
    inline static void setToLeft(Node* node, Node* newNode);
    inline static void setToRight(Node* node, Node* newNode);
    inline static void reParent(Node* node, Node* newNode);
    inline static Node* balanceSimpleToLeft(Node* fromNode);
    inline static Node* balanceSimpleToRight(Node* fromNode);
    inline static Node* balanceDoubleToLeft(Node* fromNode);
    inline static Node* balanceDoubleToRight(Node* fromNode);
    inline static Node* balance(Node* fromNode, bool heightIncrease);

public:
    typedef cAVLBalancer<Map,Node> Balancer;

    inline static Node* balance(Map* fromMap, Node* fromNode);
};

}
}
}

#include "YTL/container/detail/cTreeBalancer.tpp"
