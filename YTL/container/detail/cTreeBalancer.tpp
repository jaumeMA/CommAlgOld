
namespace yame
{
namespace container
{
namespace detail
{

template<typename Map,typename Node>
void cAVLBalancer<Map,Node>::reParent(Node* node, Node* newNode)
{
    Node* nodeParent = node->getParent();

    if(nodeParent)
    {
        if(nodeParent->getLeftChild() == node)
        {
           nodeParent->setLeftChild(newNode);
        }
        else
        {
           nodeParent->setRightChild(newNode);
        }
    }

    newNode->setParent(nodeParent);
}
template<typename Map,typename Node>
void cAVLBalancer<Map,Node>::setToLeft(Node* node, Node* newNode)
{
    node->setLeftChild(newNode);

    if(newNode)
    {
        newNode->setParent(node);
    }
}
template<typename Map,typename Node>
void cAVLBalancer<Map,Node>::setToRight(Node* node, Node* newNode)
{
    node->setRightChild(newNode);

    if(newNode)
    {
        newNode->setParent(node);
    }
}
template<typename Map,typename Node>
Node* cAVLBalancer<Map,Node>::balanceSimpleToLeft(Node* fromNode)
{
    Node* rightChild = fromNode->getRightChild();
    Node* rightLeftChild = (rightChild) ? rightChild->getLeftChild() : NULL;

    if(rightChild)
    {
        rightChild->setHeight(0);
        reParent(fromNode,rightChild);
        setToLeft(rightChild,fromNode);

        fromNode->setHeight(0);
        setToRight(fromNode,rightLeftChild);
    }

    return (rightChild) ? rightChild : fromNode;
}
template<typename Map,typename Node>
Node* cAVLBalancer<Map,Node>::balanceSimpleToRight(Node* fromNode)
{
    Node* leftChild = fromNode->getLeftChild();
    Node* leftRightChild = (leftChild) ? leftChild->getRightChild() : NULL;

    //the left child is going to be the parent of the current node
    if(leftChild)
    {
        leftChild->setHeight(0);
        reParent(fromNode,leftChild);
        setToRight(leftChild,fromNode);

        fromNode->setHeight(0);
        setToLeft(fromNode,leftRightChild);
    }

    return (leftChild) ? leftChild : fromNode;
}
template<typename Map,typename Node>
Node* cAVLBalancer<Map,Node>::balanceDoubleToLeft(Node* fromNode)
{
    Node* rightChild = fromNode->getRightChild();
    Node* rightLeftChild = (rightChild) ? rightChild->getLeftChild() : NULL;
    Node* rightLeftRightChild = (rightLeftChild) ? rightLeftChild->getRightChild() : NULL;
    Node* rightLeftLeftChild = (rightLeftChild) ? rightLeftChild->getLeftChild() : NULL;

    //the left child is going to be the parent of the current node

    if(rightChild)
    {
        rightChild->setHeight(0);
        setToLeft(rightChild,rightLeftRightChild);

        if(rightLeftChild)
        {
            rightLeftChild->setHeight(0);
            reParent(fromNode,rightLeftChild);
            setToRight(rightLeftChild,rightChild);
            setToLeft(rightLeftChild,fromNode);
        }

        fromNode->setHeight(0);
        setToRight(fromNode,rightLeftLeftChild);
    }

    return (rightLeftChild) ? rightLeftChild : fromNode;
}
template<typename Map,typename Node>
Node* cAVLBalancer<Map,Node>::balanceDoubleToRight(Node* fromNode)
{
    Node* leftChild = fromNode->getLeftChild();
    Node* leftRightChild = (leftChild) ? leftChild->getRightChild() : NULL;
    Node* leftRightLeftChild = (leftRightChild) ? leftRightChild->getLeftChild() : NULL;
    Node* leftRightRightChild = (leftRightChild) ? leftRightChild->getRightChild() : NULL;

    //the left child is going to be the parent of the current node

    if(leftChild)
    {
        leftChild->setHeight(0);
        setToRight(leftChild,leftRightLeftChild);

        if(leftRightChild)
        {
            leftRightChild->setHeight(0);
            reParent(fromNode,leftRightChild);
            setToLeft(leftRightChild,leftChild);
            setToRight(leftRightChild,fromNode);
        }

        fromNode->setHeight(0);
        setToLeft(fromNode,leftRightRightChild);
    }

    return (leftRightChild) ? leftRightChild : fromNode;
}
template<typename Map,typename Node>
Node* cAVLBalancer<Map,Node>::balance(Node* fromNode, bool heightIncrease)
{
    if(Node* thisParent = fromNode->getParent())
    {
        Node* localRoot = thisParent;
        Node* leftChild = thisParent->getLeftChild();
        Node* rightChild = thisParent->getRightChild();

        if(leftChild == fromNode)
        {
            if(heightIncrease)
            {
                if(thisParent->getHeight() == -1)
                {
                    thisParent->setHeight(0);
                    heightIncrease = false;
                }
                else if(thisParent->getHeight() == 0)
                {
                    thisParent->setHeight(1);
                    heightIncrease = true;
                }
                else if(thisParent->getHeight() == 1)
                {
                    if(leftChild->getHeight() == 1)
                    {
                        localRoot = balanceSimpleToRight(thisParent);
                    }
                    else
                    {
                        localRoot = balanceDoubleToRight(thisParent);
                    }
                    heightIncrease = false;
                }
            }
        }
        else if (rightChild == fromNode)
        {
            if(heightIncrease)
            {
                if(thisParent->getHeight() == -1)
                {
                    if(rightChild->getHeight() == 1)
                    {
                        localRoot = balanceDoubleToLeft(thisParent);
                    }
                    else
                    {
                        localRoot = balanceSimpleToLeft(thisParent);
                    }
                    heightIncrease = false;
                }
                else if(thisParent->getHeight() == 0)
                {
                    thisParent->setHeight(-1);
                    heightIncrease = true;
                }
                else if(thisParent->getHeight() == 1)
                {
                    thisParent->setHeight(0);
                    heightIncrease = false;
                }
            }
        }

        //forward the action to its parent
        return balance(localRoot,heightIncrease);
    }
    else
    {
        return fromNode;
    }
}
template<typename Map,typename Node>
Node* cAVLBalancer<Map,Node>::balance(Map* fromMap, Node* fromNode)
{
    return balance(fromNode,true);
}

}
}
}
