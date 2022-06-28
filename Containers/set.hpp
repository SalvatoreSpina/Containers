#pragma once

#include <limits>
#include "./red_black_headers/red_black_tree.hpp"
#include "./headers/utilities.hpp"

namespace ft {

// https://en.cppreference.com/w/cpp/container/set
template <typename _Key, typename Compare = std::less<_Key>, typename _Alloc = std::allocator<_Key> >
class set {

public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef Compare compare_key;
    typedef Compare value_compare;
    typedef _Alloc allocator_type;
    typedef typename _Alloc::reference reference;
    typedef typename _Alloc::const_reference const_reference;
    typedef typename _Alloc::pointer pointer;
    typedef typename _Alloc::const_pointer const_pointer;

private: 
    typedef Red_Black_Tree<key_type, value_type, ft::Identity<value_type>, compare_key, allocator_type> type_set;
    type_set _set_t;

public:
    typedef typename type_set::size_type size_type;
    typedef typename type_set::type_difference type_difference;
    typedef typename type_set::const_iterator iterator;
    typedef typename type_set::const_iterator const_iterator;
    typedef typename type_set::const_reverse_iterator reverse_iterator;
    typedef typename type_set::const_reverse_iterator const_reverse_iterator;

public:
        
    // Constructors (empties, ranges)
    set() : _set_t () {} 
    explicit set(const Compare& comp, const allocator_type& all = allocator_type()) : _set_t(comp, all) {}
    
    template <typename InputIt>
    set(InputIt first, InputIt last, const Compare& comp = Compare(), const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIt>::valor, InputIt>::type* = NULL)
        : _set_t(comp, alloc) { 
            insert(first, last); 
    }

    // Other canonical needed
    virtual ~set() {}
    set(const set& x) : _set_t(x._set_t) {}
    set& operator=(const set& x) {
        if (this == &x) return *this;
        _set_t = x._set_t;
        return *this;
    }

    // Iterators
    iterator begin() { return _set_t.begin(); }
    const_iterator begin() const { return _set_t.begin(); }
    iterator end() { return _set_t.end(); }
    const_iterator end() const { return _set_t.end(); }
    reverse_iterator rbegin() { return _set_t.rbegin(); }
    const_reverse_iterator rbegin() const { return _set_t.rbegin(); }
    reverse_iterator rend() { return _set_t.rend(); }
    const_reverse_iterator rend() const { return _set_t.rend(); }

    // Size related
    bool empty() const { return _set_t.empty(); }
    size_type size() const { return _set_t.size(); }
    size_type max_size() const { return _set_t.max_size(); }


    // Allocator getter
    allocator_type get_allocator() const { return _set_t.get_allocator(); }

    // Modifiers
    void clear() { _set_t.clear(); }

    // Insert a single pair
    ft::pair<iterator, bool> insert(const value_type& x) {return _set_t.insert(x); }

    iterator insert(iterator position, const value_type& x) {
        (void)position;
        return insert(x).first;
    }

    // Insert a range (only integrals!)
    template<class InputIt>
    void insert(InputIt start, InputIt end, typename ft::enable_if<!ft::is_integral<InputIt>::valor, InputIt>::type* = NULL) {
        for (InputIt first = start; first != end; ++first) { 
            insert(*first);
        }
    }

    void erase(iterator pos) { erase(*pos); }
    size_type erase(const key_type& key) {return _set_t.erase(key); }
    void erase(iterator _start, iterator _end) {
        if (_start == begin() && _end == end())
            clear();
        else 
            while (_start != _end) 
                erase(_start++);
    }

    void swap(set& x) {_set_t.swap(x._set_t); }

    // Lookup
    size_type count(const key_type& x) const { return _set_t.find(x) == _set_t.end() ? 0 : 1; }

    //Iterator relative function
    iterator find(const key_type& x) { return _set_t.find(x); }
    const_iterator find(const key_type& x) const { return _set_t.find(x); }
    iterator lower_bound(const key_type& x) { return _set_t.lower_bound(x); }
    const_iterator lower_bound(const key_type& x) const { return _set_t.lower_bound(x); }
    iterator upper_bound(const key_type& x) { return _set_t.upper_bound(x); }
    const_iterator upper_bound(const key_type& x) const { return _set_t.upper_bound(x); }

    // Equal ranges
    ft::pair<iterator, iterator> equal_range(const key_type& x) { return _set_t.equal_range(x);}
    ft::pair<const_iterator, const_iterator> equal_range(const key_type& x) const { return _set_t.equal_range(x); }

    // Observer
    compare_key key_comp() const { return _set_t.key_comp(); };
    value_compare value_comp() const { return value_compare(_set_t.key_comp()); }

    // Non Members [friends declarations]
    template <typename _KeyT, typename _CompT, typename _AllocT>
    friend bool operator==(const set<_KeyT, _CompT, _AllocT>&, 
                            const set<_KeyT, _CompT, _AllocT>&);
    template <typename _KeyT, typename _CompT, typename _AllocT>
    friend bool operator<(const set<_KeyT, _CompT, _AllocT>&, 
                            const set<_KeyT, _CompT, _AllocT>&);

}; // Class set

template <typename _KeyT, typename _CompT, typename _AllocT>
inline bool operator==(const ft::set<_KeyT, _CompT, _AllocT>& lhs, 
                        const ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    return lhs._set_t == rhs._set_t;
}

template <typename _KeyT, typename _CompT, typename _AllocT>
inline bool operator!=(const ft::set<_KeyT, _CompT, _AllocT>& lhs, 
                        const ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    return !(lhs == rhs);
}

template <typename _KeyT, typename _CompT, typename _AllocT>
inline bool operator<(const ft::set<_KeyT, _CompT, _AllocT>& lhs, 
                        const ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    return lhs._set_t < rhs._set_t;
}

template <typename _KeyT, typename _CompT, typename _AllocT>
inline bool operator<=(const ft::set<_KeyT, _CompT, _AllocT>& lhs, 
                        const ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    return !(rhs < lhs);
}

template <typename _KeyT, typename _CompT, typename _AllocT>
inline bool operator>(const ft::set<_KeyT, _CompT, _AllocT>& lhs, 
                        const ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    return rhs < lhs;
}

template <typename _KeyT, typename _CompT, typename _AllocT>
inline bool operator>=(const ft::set<_KeyT, _CompT, _AllocT>& lhs, 
                        const ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    return !(lhs < rhs);
}

} // FT Namespace

namespace std {
template <typename _KeyT, typename _CompT, typename _AllocT>
void swap(ft::set<_KeyT, _CompT, _AllocT>& lhs, 
            ft::set<_KeyT, _CompT, _AllocT>& rhs) {
    lhs.swap(rhs);
}

} // STD Namespace