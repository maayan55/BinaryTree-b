#include <iostream>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <new>
 
namespace ariel {

    //---------------------------------------------BINARY TREE CLASS DECLARATION------------------------------

    template <typename T>
    class BinaryTree {
    public:

        // Forward declarations.
        class Node;
        class InorderIterator;
        class PreorderIterator;
        class PostorderIterator;

        // Constructors.
        BinaryTree();
        BinaryTree(const BinaryTree& other); //copy constructor
        BinaryTree(BinaryTree&& other) noexcept; //move constructor
        ~BinaryTree();

        // Operators.
        BinaryTree& operator=(const BinaryTree& other);
        BinaryTree& operator=(BinaryTree&& other) noexcept;
        friend std::ostream& operator<< (std::ostream &os, const BinaryTree& bt) { return os; }; //operator << , print the tree
        
        // Modifiers.
        void add_root(const T& data); //get data, put in the tree root (replaces if another root already exists)
        void add_left(const T& existing_data, const T& new_data); //get existing data and new data, put the new in the left son (replaces if another left son already exists)
        void add_right(const T& existing_data, const T& new_data); //get existing data and new data, put the new in the right son (replaces if another right son already exists)

        // Access methods.
        InorderIterator begin() { return begin_inorder(); }
        InorderIterator end() { return end_inorder(); }
        PreorderIterator begin_preorder() { return PreorderIterator(root); }
        PreorderIterator end_preorder() { return PreorderIterator(nullptr); }
        InorderIterator begin_inorder() { return InorderIterator(root); }
        InorderIterator end_inorder() { return InorderIterator(nullptr); }
        PostorderIterator begin_postorder() { return PostorderIterator(root); }
        PostorderIterator end_postorder() { return PostorderIterator(nullptr); }

        // Iterators
        class InorderIterator {
        public:
            InorderIterator(Node* node);
            bool operator== (const InorderIterator &rhs) const { return curr_node == rhs.curr_node; }
            bool operator!= (const InorderIterator& rhs) const { return !operator==(rhs); }
            InorderIterator& operator++ ();
            InorderIterator operator++ (int);
            T& operator*() { return curr_node->data; }
            T* operator->() { return &curr_node->data; }
            friend std::ostream& operator<< (std::ostream& os, const InorderIterator& node) { return os; }

        private:
            Node* curr_node;
            std::stack<Node*> next_nodes;
        };

        class PreorderIterator {
        public:
            PreorderIterator(Node* node);
            bool operator== (const PreorderIterator &rhs) const { return curr_node == rhs.curr_node; }
            bool operator!= (const PreorderIterator& rhs) const { return !operator==(rhs); }
            PreorderIterator& operator++ ();
            PreorderIterator operator++ (int);
            T& operator*() { return curr_node->data; }
            T* operator->() { return &curr_node->data; }
            friend std::ostream& operator<< (std::ostream& os, const PreorderIterator& node) { return os; }

        private:
            Node* curr_node;
            std::stack<Node*> next_nodes;
        };

        class PostorderIterator {
        public:
            PostorderIterator(Node* node);
            bool operator== (const PostorderIterator &rhs) const { return curr_node == rhs.curr_node; }
            bool operator!= (const PostorderIterator& rhs) const { return !operator==(rhs); }
            PostorderIterator& operator++ ();
            PostorderIterator operator++ (int);
            T& operator* () { return curr_node->data; }
            T* operator-> () { return &curr_node->data; }
            friend std::ostream& operator<< (std::ostream& os, const PostorderIterator& node) { return os; }

        private:
            Node* curr_node;
            std::stack<Node*> next_nodes;
        };

        class Node {
        public:
            Node(T data) : data(data), right(nullptr), left(nullptr) { }

            T data;
            Node* right;
            Node* left;
        };

    private:
        Node* find(Node* node, const T& data);
        void deepCopy(Node* other, Node* node);
        void destroyTree(Node* node);

        Node* root;
    };

    //---------------------------------------------POSTORDER ITERATOR DEFINITION------------------------------
    //POSTORDER- left, right, root

    template <typename T>
    BinaryTree<T>::PostorderIterator::PostorderIterator(BinaryTree<T>::Node* node) 
    {
        if (nullptr == node) {
            curr_node = nullptr;
        } else {
            std::stack<BinaryTree<T>::Node*> tmpstack;
            tmpstack.push(node);

            while (!tmpstack.empty()) {
                BinaryTree<T>::Node* tmpnode = tmpstack.top();
                tmpstack.pop();
                next_nodes.push(tmpnode);
                if (tmpnode->left) {
                    tmpstack.push(tmpnode->left);
                }
                if (tmpnode->right) {
                    tmpstack.push(tmpnode->right);
                }
            }
            curr_node = next_nodes.top();
        }
    }

    template <typename T>
    typename BinaryTree<T>::PostorderIterator& BinaryTree<T>::PostorderIterator::operator++()
    {
        if (next_nodes.empty()) {
            curr_node = nullptr;
            return *this;
        }
            
        next_nodes.pop();
        if (!next_nodes.empty()) {
            curr_node = next_nodes.top();
        } else {
            curr_node = nullptr;
        }
        return *this;
    }

    template <typename T>
    typename BinaryTree<T>::PostorderIterator BinaryTree<T>::PostorderIterator::operator++(int)
    {
        PostorderIterator new_iter(*this);
        new_iter.curr_node = new_iter.next_nodes.top();
        operator++();
        return new_iter;
    }

    //---------------------------------------------PREORDER ITERATOR DEFINITION------------------------------
    //PREORDER- root, left, right

    template <typename T>
    BinaryTree<T>::PreorderIterator::PreorderIterator(BinaryTree<T>::Node* node) : curr_node(node)
    {
        next_nodes.push(node);
    }

    template <typename T>
    typename BinaryTree<T>::PreorderIterator& BinaryTree<T>::PreorderIterator::operator++ ()
    {
        if (next_nodes.empty()) {
            curr_node = nullptr;
            return *this;
        }
        next_nodes.pop();
        if (curr_node->right) {
            next_nodes.push(curr_node->right);
        }
        if (curr_node->left) {
            next_nodes.push(curr_node->left);
        }
        if (next_nodes.empty()) {
            curr_node = nullptr;
        } else {
            curr_node = next_nodes.top();
        }
        return *this;
    }

    template <typename T>
    typename BinaryTree<T>::PreorderIterator BinaryTree<T>::PreorderIterator::operator++ (int)
    {
        PreorderIterator new_iter(*this);
        operator++();
        return new_iter;
    }

    //---------------------------------------------INORDER ITERATOR DEFINITION------------------------------
    //INORDER- left, root, right

    template <typename T>
    BinaryTree<T>::InorderIterator::InorderIterator(BinaryTree<T>::Node* node)
    {
        BinaryTree<T>::Node* curr = node;
        while (curr) {
            next_nodes.push(curr);
            curr = curr->left;
        }

        if (next_nodes.empty()) {
            curr_node = nullptr;
        } else {
            curr_node = next_nodes.top();
        }
    }


    template <typename T>
    typename BinaryTree<T>::InorderIterator& BinaryTree<T>::InorderIterator::operator++ ()
    {
        if (next_nodes.empty()) {
            curr_node = nullptr;
            return *this;
        }

        BinaryTree<T>::Node* tmp = next_nodes.top();
        next_nodes.pop();
        if (tmp->right) {
            next_nodes.push(tmp->right);
            tmp = tmp->right->left;

            while (tmp) {
                next_nodes.push(tmp);
                tmp = tmp->left;
            }
        }

        if (next_nodes.empty()) {
            curr_node = nullptr;
        } else {
            curr_node = next_nodes.top();
        }
        return *this;
    }

    template <typename T>
    typename BinaryTree<T>::InorderIterator BinaryTree<T>::InorderIterator::operator++ (int)
    {
        InorderIterator new_iter(*this);
        operator++();
        return new_iter;
    }

    //---------------------------------------------BINARY TREE DEFINITION------------------------------

    // Public interface definition

    template <typename T>
    BinaryTree<T>::BinaryTree() : root(nullptr) { }

    template <typename T>
    BinaryTree<T>::BinaryTree(const BinaryTree& other)
    {
        if (other.root) {
            root = new (std::nothrow) BinaryTree<T>::Node(other.root->data);
            deepCopy(other.root, root);
        }
    }

    template <typename T>
    BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept
    {
        root = other.root;
        other.root = nullptr;
    }

    template <typename T>
    BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree<T>& other)
    {
        if (this == &other) {
            return *this;
        }

        destroyTree(root);
        root = nullptr;
        if (other.root) {
            root = new (std::nothrow) BinaryTree<T>::Node(other.root->data);
            deepCopy(other.root, root);
        }
        return *this;
    }

    template <typename T>
    BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree<T>&& other) noexcept 
    {
        if (root) { 
            destroyTree(); 
        }
        root  = other.root;
        other.root = nullptr;
        return *this;
    }

    template <typename T>
    BinaryTree<T>::~BinaryTree() 
    {
        destroyTree(root);
        root = nullptr; 
    }

    template <typename T>
    void BinaryTree<T>::add_root(const T& data)
    {
        if (nullptr == root) {
            root = new (std::nothrow) Node(data);
            if (!root) {
                exit(1);
            }
            root->right = nullptr;
            root->left = nullptr;
        } else {
            root->data = data;
        }
    }

    template <typename T>
    void BinaryTree<T>::add_left(const T& existing_data, const T& new_data)
    {
        BinaryTree<T>::Node* node = find(root, existing_data);
        if (nullptr == node) {
            std::stringstream str;
            str << "Value doesn't exist in tree.";
            throw std::invalid_argument(str.str());
        }
        if (nullptr == node->left) {
            node->left = new (std::nothrow) Node(new_data);
            if (!node->left) {
                exit(1);
            }
            node->left->right = nullptr;
            node->left->left = nullptr;
        } else {
            node->left->data = new_data;
        }
    }

    template <typename T>
    void BinaryTree<T>::add_right(const T& existing_data, const T& new_data)
    {
        BinaryTree<T>::Node* node = find(root, existing_data);
        if (nullptr == node) {
            std::stringstream str;
            str << "Value doesn't exist in tree.";
            throw std::invalid_argument(str.str());
        }
        if (nullptr == node->right) {
            node->right = new (std::nothrow) Node(new_data);
            if (!node->right) {
                exit(1);
            }
            node->right->right = nullptr;
            node->right->left = nullptr;
        } else {
            node->right->data = new_data;
        }
    }

    // Private methods definition

    template <typename T>
    typename BinaryTree<T>::Node* BinaryTree<T>::find(BinaryTree<T>::Node* node, const T& data)
    {
        if (nullptr == node || node->data == data) {
            return node;
        }

        Node* left = nullptr;
        Node* right = nullptr;

        if (node->left) {
            left = find(node->left, data);
        }
        if (node->right) {
            right = find (node->right, data);
        }
        
        if (left) { 
            return left;
        }
        return right;
    }

    template <typename T>
    void BinaryTree<T>::deepCopy(BinaryTree<T>::Node* other, BinaryTree<T>::Node* node)
    {
        if (nullptr == other) { 
            return; 
        }

        if (other->right) {
            node->right = new (std::nothrow) BinaryTree<T>::Node(other->right->data);
            deepCopy(other->right, node->right);
        }

        if (other->left) {
            node->left = new (std::nothrow) BinaryTree<T>::Node(other->left->data);
            deepCopy(other->left, node->left);
        }
    }

    template <typename T>
    void BinaryTree<T>::destroyTree(BinaryTree<T>::Node* node)
    {
        if (nullptr == node) {
            return;
        }
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

}