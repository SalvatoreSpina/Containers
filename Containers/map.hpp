#pragma once

#include <limits>
#include "./red_black_headers/red_black_tree.hpp"

namespace ft {

// https://en.cppreference.com/w/cpp/container/map
template <typename _Key, typename _T, typename Compare = std::less<_Key>, typename _Alloc = std::allocator<ft::pair<const _Key, _T> > >
class map {

public:
    typedef _Key key_type;
    typedef _T mapped_type;
    typedef ft::pair<const _Key, _T> value_type;
    typedef Compare compare_key;
    typedef _Alloc allocator_type;
    typedef typename _Alloc::reference reference;
    typedef typename _Alloc::const_reference const_reference;
    typedef typename _Alloc::pointer pointer;
    typedef typename _Alloc::const_pointer const_pointer;

    // https://en.cppreference.com/w/cpp/utility/functional/binary_function
    class Value_Compare : public std::binary_function<value_type, value_type, bool> {
        
        public:
            Compare comp;
            Value_Compare(Compare _comp) : comp(_comp) {}

            bool operator()(const value_type& x, const value_type& y) const {
                return comp(x.first, y.first);
            }
    };

private: 
    typedef Red_Black_Tree<key_type, value_type, ft::pick_first<value_type>, compare_key, allocator_type> type_map;
    type_map _map_t;

public:
    typedef typename type_map::size_type size_type;
    typedef typename type_map::type_difference type_difference;
    typedef typename type_map::iterator iterator;
    typedef typename type_map::const_iterator const_iterator;
    typedef typename type_map::reverse_iterator reverse_iterator;
    typedef typename type_map::const_reverse_iterator const_reverse_iterator;

public:
        
    // Constructors (empties, ranges)
    map() : _map_t () {} 
    explicit map(const Compare& comp, const allocator_type& all = allocator_type()) : _map_t(comp, all) {}
    
    template <typename InputIt>
    map(InputIt first, InputIt last, const Compare& comp = Compare(), const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIt>::valor, InputIt>::type* = NULL)
        : _map_t(comp, alloc) { 
            insert(first, last); 
    }

    // Other canonical needed
    virtual ~map() {}
    map(const map& x) : _map_t(x._map_t) {}
    map& operator=(const map& x) {
        if (this == &x) return *this;
        _map_t = x._map_t; return *this;
    }

    // Iterators
    iterator begin() { return _map_t.begin(); }
    const_iterator begin() const { return _map_t.begin(); }
    iterator end() { return _map_t.end(); }
    const_iterator end() const { return _map_t.end(); }
    reverse_iterator rbegin() { return _map_t.rbegin(); }
    const_reverse_iterator rbegin() const { return _map_t.rbegin(); }
    reverse_iterator rend() { return _map_t.rend(); }
    const_reverse_iterator rend() const { return _map_t.rend(); }

    // Size related
    bool empty() const { return _map_t.empty(); }
    size_type size() const { return _map_t.size(); }
    size_type max_size() const { return _map_t.max_size(); }


    // Allocator getter
    allocator_type get_allocator() const { return _map_t.get_allocator(); }

    // Access elements
    mapped_type& at(const key_type& element) {
        iterator iter = lower_bound(element);
        if (iter == end() || key_comp()(element, (*iter).first))
            std::__throw_out_of_range("map::at");
        return (*iter).second;
    }

    const mapped_type& at(const key_type& element) const {
        iterator iter = lower_bound(element);
        if (iter == end() || key_comp()(element, (*iter).first))
            std::__throw_out_of_range("map::at");
        return (*iter).second;
    }

    // If it doesn't exist the element we are looking for, it will be
    // inserted with default valules, and return it
    mapped_type& operator[] (const key_type& element) {
        iterator iter = insert(value_type(element, mapped_type())).first;
        return iter->second;
    }

    // Modifiers
    void clear() { _map_t.clear(); }

    // Insert a single pair
    ft::pair<iterator, bool> insert(const value_type& x) {return _map_t.insert(x); }

    iterator insert(iterator position, const value_type& x) {
        (void)position;
        return insert(x).first;
    }

    // Insert a range (only integrals!)
    template<class InputIt>
    void insert(InputIt start, InputIt end, typename ft::enable_if<!ft::is_integral<InputIt>::valor, InputIt>::type* = NULL) {
        for (InputIt first = start; first != end; ++first) { insert(*first); }
    }

    void erase(iterator pos) { erase(pos->first); }
    size_type erase(const key_type& key) {return _map_t.erase(key); }
    void erase(iterator _start, iterator _end) {
        if (_start == begin() && _end == end())
            clear();
        else 
            while (_start != _end) 
                erase(_start++);
    }

    void swap(map& x) {_map_t.swap(x._map_t); }

    // Lookup
    size_type count(const key_type& x) const { return _map_t.find(x) == _map_t.end() ? 0 : 1; }

    //Iterator relative function
    iterator find(const key_type& x) { return _map_t.find(x); }
    const_iterator find(const key_type& x) const { return _map_t.find(x); }
    iterator lower_bound(const key_type& x) { return _map_t.lower_bound(x); }
    const_iterator lower_bound(const key_type& x) const { return _map_t.lower_bound(x); }
    iterator upper_bound(const key_type& x) { return _map_t.upper_bound(x); }
    const_iterator upper_bound(const key_type& x) const { return _map_t.upper_bound(x); }

    // Equal ranges
    ft::pair<iterator, iterator> equal_range(const key_type& x) { return _map_t.equal_range(x);}
    ft::pair<const_iterator, const_iterator> equal_range(const key_type& x) const { return _map_t.equal_range(x); }

    // Observer
    compare_key key_comp() const { return _map_t.key_comp(); };
    Value_Compare value_comp() const { return Value_Compare(_map_t.key_comp()); }

    // Non Members [friends declarations]
    template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
    friend bool operator==(const map<_KeyT, _TypeT, _CompT, _AllocT>&, 
                            const map<_KeyT, _TypeT, _CompT, _AllocT>&);
    template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
    friend bool operator<(const map<_KeyT, _TypeT, _CompT, _AllocT>&, 
                            const map<_KeyT, _TypeT, _CompT, _AllocT>&);

}; // Class map

template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
inline bool operator==(const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
                        const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    return lhs._map_t == rhs._map_t;
}

template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
inline bool operator!=(const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
                        const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    return !(lhs == rhs);
}

template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
inline bool operator<(const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
                        const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    return lhs._map_t < rhs._map_t;
}

template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
inline bool operator<=(const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
                        const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    return !(rhs < lhs);
}

template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
inline bool operator>(const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
                        const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    return rhs < lhs;
}

template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
inline bool operator>=(const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
                        const ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    return !(lhs < rhs);
}

} // FT Namespace

namespace std {
template <typename _KeyT, typename _TypeT, typename _CompT, typename _AllocT>
void swap(ft::map<_KeyT, _TypeT, _CompT, _AllocT>& lhs, 
            ft::map<_KeyT, _TypeT, _CompT, _AllocT>& rhs) {
    lhs.swap(rhs);
}

} // STD Namespace