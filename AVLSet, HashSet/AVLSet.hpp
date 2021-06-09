// AVLSet.hpp
//
// ICS 46 Spring 2021
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include <string>
#include <algorithm>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;

    
private:
    struct Node
    {
        ElementType value;
        Node* right;
        Node* left;
    };

    Node* head;
    int treeHeight;
    unsigned int treeSize;

    void deleteNode(Node* node);
    int calculateHeight(Node* node);
    Node* clone(Node* node);

    /*
    Depth First Traversals: 
    (a) Inorder (Left, Root, Right): node->left, then node->value, last node->right
    (b) Preorder (Root, Left, Right): node->value, node->left, node->right
    (c) Postorder (Left, Right, Root) node->left, node->right, node->value
    */


    void post(VisitFunction visit, Node* node) const;
    void in(VisitFunction visit, Node* node) const;
    void pre(VisitFunction visit, Node* node) const;
    int maxHeight(Node* node) const;
    int difference(Node* node) const;
    Node* rrRotation(Node* parent);
    Node* llRotation(Node* parent);
    Node* lrRotation(Node* parent);
    Node* rlRotation(Node* parent);
    Node* balance(Node* T);
};

template <typename ElementType>
int AVLSet<ElementType>::maxHeight(Node* node) const
{
    int h = 0;
    if (node != nullptr)
    {
        int l_height = maxHeight(node->left);
        int r_height = maxHeight(node->right);
        int max_height = std::max(l_height, r_height);
        h = max_height + 1;
    }
    return h;
}

template <typename ElementType>
int AVLSet<ElementType>::difference(Node* node) const
{
    int l_height = maxHeight(node->left);
    int r_height = maxHeight(node->right);
    int factor = l_height - r_height;
    return factor;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::rrRotation(Node* parent)
{
    Node* temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::llRotation(Node* parent)
{
    Node* temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::lrRotation(Node* parent)
{
    Node* temp;
    temp = parent->left;
    parent->left = rrRotation(temp);
    return llRotation(parent);
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::rlRotation(Node* parent)
{
    Node* temp;
    temp = parent->right;
    parent->right = llRotation(temp);
    return rrRotation(parent);

}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::balance(Node* T)
{
    int factor = difference(T);
    if (factor > 1)
    {
        if (difference(T->left)>0)
        {
            T = llRotation(T);
        }
        else
        {
            T = lrRotation(T);
        }
    }
    else if (factor < -1)
    {
        if (difference(T->right)>0)
        {
            T = rlRotation(T);
        }
        else
        {
            T = rrRotation(T);
        }
    }
    return T;
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    head = nullptr;
    treeHeight = -1;
    treeSize = 0;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    deleteNode(head);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    head = nullptr;
    treeHeight = -1;
    treeSize = 0;

    head = clone(s.head);
    treeSize = s.treeSize;
    treeHeight = s.treeHeight;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    head = nullptr;
    treeHeight = -1;
    treeSize = 0;
    std::swap(head, s.head);
    std::swap(treeSize, s.treeSize);
    std::swap(treeHeight, s.treeHeight);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if (this != &s)
    {
        Node* newHead = clone(s.head);
        head = newHead;
    }
    treeHeight = s.treeHeight;
    treeSize = s.treeSize;
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    std::swap(head, s.head);
    std::swap(treeSize, s.treeSize);
    std::swap(treeHeight, s.treeHeight);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    Node* previous;
    std::string direction;
    if (treeSize == 0)
    {
        Node* newNode = new Node();
        newNode->value = element;
        newNode->left = nullptr;
        newNode->right = nullptr;
        head = newNode;
        treeHeight = 0;
    }
    else
    {
        Node* current = head;
        while (current != nullptr)
        {
            previous = current;
            if (current->value > element)
            {
                current = current->left;
                direction = "left";
            }
            else if (current->value < element)
            {
                current = current->right;
                direction = "right";
            }
        }
        
        if (direction == "right")
        {
            Node* add = new Node();
            add->value = element;
            add->left = nullptr;
            add->right = nullptr;
            previous->right = add;
        }
        else if(direction == "left")
        {   
            Node* add = new Node();
            add->value = element;
            add->left = nullptr;
            add->right = nullptr;
            previous->left = add;
        }
    }
    treeHeight = calculateHeight(head);
    treeSize++;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    Node* contain = head;
    Node* newHead = head;
    int factor = difference(newHead);
    std::cout << newHead->left->right->value << "is the last node" << std::endl;
    std::cout << "The different between is : " << factor << std::endl;

    while(contain != nullptr)
    {
        if (contain->value > element)
        {
            contain = contain->left;
        }
        else if (contain->value < element)
        {
            contain = contain->right;
        }
        else
        {
            return true;
        }
    }
    return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return treeHeight;
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    pre(visit, head);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    in(visit, head);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    post(visit, head);
}


template <typename ElementType>
void AVLSet<ElementType>::deleteNode(Node* node)
{
    if (node != nullptr)
    {
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }
}

template <typename ElementType>
int AVLSet<ElementType>::calculateHeight(Node* node)
{
    if (node == nullptr)
    {
        return -1;
    }
    else
    {
        int leftDepth = calculateHeight(node->left);
        int rightDepth = calculateHeight(node->right);

        return std::max(leftDepth, rightDepth)+1;
    }
    
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::clone(Node* node)
{
    if (node==nullptr)
    {
        return nullptr;
    }
    else
    {
        Node* temp = new Node();
        temp->value = node->value;
        temp->right = clone(node->right);
        temp->left = clone(node->left);
        return temp;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::pre(VisitFunction visit, Node* node) const
{
    if (node != nullptr)
    {
        visit(node->value);
        pre(visit, node->left);
        pre(visit, node->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::post(VisitFunction visit, Node* node) const
{
    if (node != nullptr)
    {
        post(visit, node->left);
        post(visit, node->right);
        visit(node->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::in(VisitFunction visit, Node* node) const
{
    if (node != nullptr)
    {
        in(visit, node->left);
        visit(node->value);
        in(visit, node->right);
    }
}
#endif

