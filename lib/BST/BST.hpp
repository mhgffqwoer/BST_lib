#pragma once
#include <iostream>
using std::initializer_list;
using std::pair;

// Pick:
struct Inorder {};
struct Preorder {};
struct Postorder {};

// Conditional
template <bool B, class U, class V>
struct conditional {
    using type = U;
};

template <class U, class V>
struct conditional<false, U, V> {
    using type = V;
};

template <bool B, class U, class V>
using conditional_t = typename conditional<B, U, V>::type;

// Reverse Iterator
template <typename Iterator>
class ReverseIterator {
   private:
    Iterator it_;

   public:
    ReverseIterator(Iterator it) : it_(it) {}

    auto operator*() const { return *it_; }

    ReverseIterator<Iterator>& operator++() {
        --it_;
        return *this;
    }

    ReverseIterator<Iterator>& operator--() {
        ++it_;
        return *this;
    }

    ReverseIterator<Iterator> operator++(int) {
        ReverseIterator<Iterator> tmp = *this;
        --it_;
        return tmp;
    }

    ReverseIterator<Iterator> operator--(int) {
        ReverseIterator<Iterator> tmp = *this;
        ++it_;
        return tmp;
    }

    bool operator==(const ReverseIterator<Iterator>& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const ReverseIterator<Iterator>& other) const {
        return it_ != other.it_;
    }

    Iterator& base() { return it_; }
};

// Node
template <class T>
struct Node {
    T data_;
    Node<T>* parent_;
    Node<T>* left_;
    Node<T>* right_;
};

// BST tree
template <class T, class Compare = std::less<T>,
          class Alloc = std::allocator<T>>
class BST {
    using allocator_type = Alloc;
    using node_allocator_traits = std::allocator_traits<allocator_type>;
    using node_allocator_type =
        typename node_allocator_traits::template rebind_alloc<Node<T>>;

   private:
    // in-order
    template <bool IsConst, class Pick = Inorder>
    class Iterator {
        using conditional_ptr =
            conditional_t<IsConst, const Node<T>*, Node<T>*>;
        using conditional_ref = conditional_t<IsConst, const T&, T&>;
        friend class BST<T, Compare, Alloc>;

       private:
        conditional_ptr it_;

        Iterator& Add(Inorder) {
            if (it_->right_ != nullptr) {
                it_ = it_->right_;
                while (it_->left_ != nullptr) {
                    it_ = it_->left_;
                }
            } else if (it_->parent_ != nullptr) {
                while (it_ == it_->parent_->right_) {
                    it_ = it_->parent_;
                    if (it_->parent_->parent_ == nullptr) {
                        break;
                    }
                }
                it_ = it_->parent_;
            }
            return *this;
        }

        Iterator& Add(Preorder) {
            if (it_->left_ != nullptr) {
                it_ = it_->left_;
            } else {
                while (it_->parent_ != nullptr &&
                       (it_ == it_->parent_->right_ ||
                        it_->parent_->right_ == nullptr)) {
                    it_ = it_->parent_;
                }
                if (it_->parent_ != nullptr) {
                    it_ = it_->parent_;
                }
                if (it_->right_ != nullptr) {
                    it_ = it_->right_;
                }
            }
            return *this;
        }

        Iterator& Add(Postorder) {
            if (it_ == it_->parent_->left_) {
                it_ = it_->parent_;
                if (it_->right_ != nullptr) {
                    it_ = it_->right_;
                    while (it_->left_ != nullptr) {
                        it_ = it_->left_;
                    }
                }
            } else {
                it_ = it_->parent_;
            }
            return *this;
        }

        Iterator& Subtract(Inorder) {
            if (it_->left_ != nullptr) {
                it_ = it_->left_;
                while (it_->right_ != nullptr) {
                    it_ = it_->right_;
                }
            } else if (it_->parent_ != nullptr) {
                while (it_ == it_->parent_->left_) {
                    it_ = it_->parent_;
                    if (it_->parent_->parent_ == nullptr) {
                        break;
                    }
                }
                it_ = it_->parent_;
            }
            return *this;
        }

        Iterator& Subtract(Preorder) {
            if (it_->parent_ == nullptr) {
                it_ = it_->left_;
                while (it_->right_ != nullptr) {
                    it_ = it_->right_;
                }
                return *this;
            }
            if (it_ == it_->parent_->left_) {
                it_ = it_->parent_;
            } else {
                it_ = it_->parent_;
                if (it_->left_ != nullptr) {
                    it_ = it_->left_;
                    while (it_->right_ != nullptr) {
                        it_ = it_->right_;
                    }
                }
            }
            return *this;
        }

        Iterator& Subtract(Postorder) {
            if (it_->parent_ == nullptr) {
                it_ = it_->left_;
                return *this;
            } else if (it_->right_ != nullptr) {
                it_ = it_->right_;
            } else if (it_->left_ != nullptr) {
                it_ = it_->left_;
            } else {
                while (it_->parent_->right_ == nullptr ||
                       it_ == it_->parent_->left_) {
                    it_ = it_->parent_;
                    if (it_->parent_ == nullptr) {
                        return *this;
                    }
                }
                it_ = it_->parent_;
                if (it_->left_ != nullptr) {
                    it_ = it_->left_;
                }
            }
            return *this;
        }

       public:
        Iterator(conditional_ptr current) : it_(current) {}

        conditional_ref operator*() const { return it_->data_; }

        Iterator& operator++() { return Add(Pick()); }

        Iterator& operator--() { return Subtract(Pick()); }

        Iterator operator++(int) {
            Iterator tmp = *this;
            Add(Pick());
            return tmp;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            Subtract(Pick());
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator& other) const {
            return !(it_ == other.it_);
        }
    };

    Node<T>* root_;
    size_t size_;
    Compare comp_;
    node_allocator_type alloc_;

    Node<T>* alloc_node() {
        Node<T>* node = alloc_.allocate(1);
        new (node) Node<T>();
        return node;
    }

    void delete_node(Node<T>* node_) {
        if (!node_) {
            return;
        }
        node_->~Node<T>();
        alloc_.deallocate(node_, 1);
    }

    void ClearRecursive(Node<T>* node) {
        if (node != nullptr) {
            ClearRecursive(node->left_);
            ClearRecursive(node->right_);
            delete_node(node);
        }
    }

    Node<T>* CopyRecursive(const Node<T>* other, Node<T>* parent = nullptr) {
        if (other == nullptr) {
            return nullptr;
        }
        Node<T>* node = alloc_node();
        node->data_ = other->data_;
        node->parent_ = parent;
        node->left_ = CopyRecursive(other->left_, node);
        node->right_ = CopyRecursive(other->right_, node);
        return node;
    }

    Node<T>* Leftmost() const {
        if (root_->left_ == nullptr) {
            return root_;
        }
        Node<T>* current = root_->left_;
        while (current->left_ != nullptr) {
            current = current->left_;
        }
        return current;
    }

    Node<T>* Leftmost(Node<T>* node) const {
        while (node->left_ != nullptr) {
            node = node->left_;
        }
        return node;
    }

    Node<T>* Rightmost() const {
        Node<T>* current = root_->left_;
        while (current->right_ != nullptr) {
            current = current->right_;
        }
        return current;
    }

    void DeleteNode(Node<T>* node) {
        if (node->left_ == nullptr && node->right_ == nullptr) {
            if (node == node->parent_->left_) {
                node->parent_->left_ = nullptr;
            } else {
                node->parent_->right_ = nullptr;
            }
        } else if (node->left_ == nullptr) {
            if (node == node->parent_->left_) {
                node->parent_->left_ = node->right_;
                node->right_->parent_ = node->parent_;
            } else {
                node->parent_->right_ = node->right_;
                node->right_->parent_ = node->parent_;
            }
        } else if (node->right_ == nullptr) {
            if (node == node->parent_->left_) {
                node->parent_->left_ = node->left_;
                node->left_->parent_ = node->parent_;
            } else {
                node->parent_->right_ = node->left_;
                node->left_->parent_ = node->parent_;
            }
        } else {
            Node<T>* temp = Leftmost(node->right_);
            if (node == node->parent_->left_) {
                node->parent_->left_ = node->right_;
                node->right_->parent_ = node->parent_;
            } else {
                node->parent_->right_ = node->right_;
                node->right_->parent_ = node->parent_;
            }
            temp->left_ = node->left_;
            node->left_->parent_ = temp;
        }
        delete_node(node);
    }

   public:
    BST() : root_(alloc_node()), size_(0), comp_(Compare()), alloc_(Alloc()) {}

    BST(const BST& other)
        : root_(CopyRecursive(other.root_)),
          size_(other.size_),
          comp_(Compare()),
          alloc_(Alloc()) {}

    BST& operator=(const BST& other) {
        if (this != &other) {
            ClearRecursive(root_);
            root_ = CopyRecursive(other.root_);
            size_ = other.size_;
        }
        return *this;
    }

    BST& operator=(BST&& other) {
        if (this != &other) {
            ClearRecursive(root_);
            root_ = other.root_;
            size_ = other.size_;
        }
        return *this;
    }

    BST(initializer_list<T> ilist) : BST() {
        insert(ilist.begin(), ilist.end());
    }

    BST& operator=(initializer_list<T> ilist) {
        clear();
        insert(ilist.begin(), ilist.end());
        return *this;
    }

    ~BST() { ClearRecursive(root_); }

    // Iterators (Inorder):
    Iterator<false, Inorder> begin(Inorder = Inorder()) {
        return Iterator<false, Inorder>(Leftmost());
    }

    Iterator<false, Inorder> end(Inorder = Inorder()) {
        return Iterator<false, Inorder>(root_);
    }

    Iterator<true, Inorder> cbegin(Inorder = Inorder()) const {
        return Iterator<true, Inorder>(Leftmost());
    }

    Iterator<true, Inorder> cend(Inorder = Inorder()) const {
        return Iterator<true, Inorder>(root_);
    }

    // Iterators (Preorder):
    Iterator<false, Preorder> begin(Preorder) {
        return Iterator<false, Preorder>(root_->left_);
    }

    Iterator<false, Preorder> end(Preorder) {
        return Iterator<false, Preorder>(root_);
    }

    Iterator<true, Preorder> cbegin(Preorder) const {
        return Iterator<true, Preorder>(root_->left_);
    }

    Iterator<true, Preorder> cend(Preorder) const {
        return Iterator<true, Preorder>(root_);
    }

    // Iterators (Postorder):
    Iterator<false, Postorder> begin(Postorder) {
        return Iterator<false, Postorder>(Leftmost(root_));
    }

    Iterator<false, Postorder> end(Postorder) {
        return Iterator<false, Postorder>(root_);
    }

    Iterator<true, Postorder> cbegin(Postorder) const {
        return Iterator<true, Postorder>(Leftmost(root_));
    }

    Iterator<true, Postorder> cend(Postorder) const {
        return Iterator<true, Postorder>(root_);
    }

    // Reverse Iterator:
    template <class Pick = Inorder>
    ReverseIterator<Iterator<false, Pick>> rbegin(Pick = Inorder()) {
        return ++(ReverseIterator<Iterator<false, Pick>>(end(Pick())));
    }

    template <class Pick = Inorder>
    ReverseIterator<Iterator<false, Pick>> rend(Pick = Inorder()) {
        return ++(ReverseIterator<Iterator<false, Pick>>(begin(Pick())));
    }

    template <class Pick = Inorder>
    ReverseIterator<Iterator<true, Pick>> crbegin(Pick = Inorder()) const {
        return ++(ReverseIterator<Iterator<true, Pick>>(cend(Pick())));
    }

    template <class Pick = Inorder>
    ReverseIterator<Iterator<true, Pick>> crend(Pick = Inorder()) const {
        return ++(ReverseIterator<Iterator<true, Pick>>(cbegin(Pick())));
    }

    // Capacity:
    bool empty() const { return root_->left_ == nullptr; }

    size_t size() const { return size_; }

    size_t max_size() const {
        return (SIZE_T_MAX - sizeof(BST)) / sizeof(Node<T>);
    }

    // Modifiers:
    void clear() {
        ClearRecursive(root_->left_);
        root_->left_ = nullptr;
        size_ = 0;
    }

    void swap(BST& other) {
        Node<T>* temp = root_;
        root_ = other.root_;
        other.root_ = temp;
    }

    template <class Pick = Inorder>
    pair<Iterator<false, Pick>, bool> insert(const T& value, Pick = Inorder()) {
        Node<T>* current = root_->left_;
        Node<T>* parent = root_;
        while (current != nullptr) {
            parent = current;
            if (comp_(value, current->data_)) {
                current = current->left_;
            } else {
                current = current->right_;
            }
        }
        Node<T>* newNode = alloc_node();
        newNode->data_ = value;
        newNode->parent_ = parent;
        newNode->left_ = nullptr;
        newNode->right_ = nullptr;
        if (parent->parent_ == nullptr) {
            root_->left_ = newNode;
        } else if (comp_(value, parent->data_)) {
            parent->left_ = newNode;
        } else {
            parent->right_ = newNode;
        }
        size_++;
        return {Iterator<false, Pick>(newNode), true};
    }

    template <class InputIt>
    void insert(InputIt first, InputIt last) {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    void insert(initializer_list<T> il) { insert(il.begin(), il.end()); }

    template <class Container>
    void insert_range(const Container& c) {
        insert(c.cbegin(), c.cend());
    }

    template <class Pick>
    Iterator<false, Pick> erase(Iterator<false, Pick> it) {
        if (it == end(Pick())) {
            return it;
        }
        Node<T>* node = it.it_;
        ++it;
        DeleteNode(node);
        --size_;
        return it;
    }

    template <class Pick>
    Iterator<true, Pick> erase(Iterator<true, Pick> it) {
        if (it == cend(Pick())) {
            return it;
        }
        Node<T>* node = const_cast<Node<T>*>(it.it_);
        ++it;
        DeleteNode(node);
        --size_;
        return it;
    }

    size_t erase(const T& value) {
        size_t count = 0;
        auto it = find(value);
        while (it != end()) {
            erase(it);
            ++count;
            it = find(value);
        }
        return count;
    }

    template <class Pick>
    void erase(Iterator<false, Pick> first, Iterator<false, Pick> last) {
        while (first != last) {
            erase(first++);
        }
    }

    template <class Pick>
    void erase(Iterator<true, Pick> first, Iterator<true, Pick> last) {
        while (first != last) {
            erase(first++);
        }
    }

    template <class Pick>
    T extract(Iterator<false, Pick> it) {
        T value = *it;
        erase(it);
        return value;
    }

    template <class Pick>
    T extract(Iterator<true, Pick> it) {
        T value = *it;
        erase(it);
        return value;
    }

    T extract(const T& value) {
        auto it = find(value);
        return extract(it);
    }

    void merge(const BST& other) { insert_range(other); }

    // Lookup: +++++++++++++++++++++
    size_t count(const T& value) const {
        Node<T>* current = root_->left_;
        size_t count = 0;
        while (current != nullptr) {
            if (current->data_ == value) {
                count++;
            }
            if (comp_(value, current->data_)) {
                current = current->left_;
            } else {
                current = current->right_;
            }
        }
        return count;
    }

    template <class Pick = Inorder>
    Iterator<false, Pick> find(const T& value, Pick = Inorder()) {
        auto it = begin(Pick());
        while (it != end(Pick()) && *it != value) {
            ++it;
        }
        return it;
    }

    template <class Pick = Inorder>
    Iterator<true, Pick> find(const T& value, Pick = Inorder()) const {
        auto it = cbegin(Pick());
        while (it != cend(Pick()) && *it != value) {
            ++it;
        }
        return it;
    }

    template <class Pick = Inorder>
    Iterator<false, Pick> rfind(const T& value, Pick = Inorder()) {
        auto it = find(value, Pick());
        while (it != end(Pick()) && *it == value) {
            ++it;
        }
        if (it != end(Pick())) {
            --it;
        }
        return it;
    }

    template <class Pick = Inorder>
    Iterator<true, Pick> rfind(const T& value, Pick = Inorder()) const {
        auto it = find(value, Pick());
        while (it != cend(Pick()) && *it == value) {
            ++it;
        }
        if (it != cend(Pick())) {
            --it;
        }
        return it;
    }

    bool contains(const T& value) const { return find(value) != cend(); }

    template <class Pick = Inorder>
    pair<Iterator<false, Pick>, Iterator<false, Pick>> equal_range(
        const T& value, Pick = Inorder()) {
        return {lower_bound(value, Pick()), upper_bound(value, Pick())};
    }

    template <class Pick = Inorder>
    pair<Iterator<true, Pick>, Iterator<true, Pick>> equal_range(
        const T& value, Pick = Inorder()) const {
        return {lower_bound(value, Pick()), upper_bound(value, Pick())};
    }

    template <class Pick = Inorder>
    Iterator<false, Pick> lower_bound(const T& value, Pick = Inorder()) {
        return find(value, Pick());
    }

    template <class Pick = Inorder>
    Iterator<true, Pick> lower_bound(const T& value, Pick = Inorder()) const {
        return find(value, Pick());
    }

    template <class Pick = Inorder>
    Iterator<false, Pick> upper_bound(const T& value, Pick = Inorder()) {
        auto it = rfind(value, Pick());
        if (it != end(Pick())) {
            ++it;
        }
        return it;
    }

    template <class Pick = Inorder>
    Iterator<true, Pick> upper_bound(const T& value, Pick = Inorder()) const {
        auto it = rfind(value, Pick());
        if (it != cend(Pick())) {
            ++it;
        }
        return it;
    }

    // Observers: ++++++++++++++++++
    Compare value_comp() const { return comp_; }

    Compare key_comp() const { return comp_; }

    Alloc get_allocator() const { return alloc_; }

    // Operators: +++++++++++++++++++++
    bool operator==(const BST& other) const {
        if (size_ != other.size_) {
            return false;
        }
        auto other_it = other.cbegin();
        for (auto it = cbegin(); it != cend(); ++it) {
            if (*it != *other_it) {
                return false;
            }
            ++other_it;
        }
        return true;
    }

    bool operator!=(const BST& other) const { return !(*this == other); }
};

template <class T, class Compare, class Alloc>
void swap(BST<T, Compare, Alloc>& lhs, BST<T, Compare, Alloc>& rhs) {
    lhs.swap(rhs);
}

template <class T, class Compare, class Alloc>
bool operator==(const BST<T, Compare, Alloc>& lhs,
                const BST<T, Compare, Alloc>& rhs) {
    return lhs.operator==(rhs);
}

template <class T, class Compare, class Alloc>
bool operator!=(const BST<T, Compare, Alloc>& lhs,
                const BST<T, Compare, Alloc>& rhs) {
    return lhs.operator!=(rhs);
}
