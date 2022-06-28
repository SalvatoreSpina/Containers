#pragma once

// Commons
#include "../headers/compares.hpp"
#include "../headers/iterator.hpp"
#include "../headers/pair.hpp"
#include "../headers/utilities.hpp"

// Specific
#include "./tree_node.hpp"
#include "./red_black_tree.hpp"

namespace ft {

template <typename _T>
struct _red_black_iter {

    typedef _T value_type;
    typedef _T& reference;
    typedef _T* pointer;
    typedef ft::bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    typedef _red_black_iter<_T> RBIter;
    typedef typename _tree_node<_T>::_relationship _relationship;

    // Member value
    _relationship _current_node;

    // Usefull
    _colors get_color() { return _current_node->_color; }
    _relationship get_link() { return _current_node; }

    // Constructors
    _red_black_iter() : _current_node() {}
    explicit _red_black_iter(_relationship x) : _current_node(x) {}

    // Operators
    reference operator*() const { return *_current_node->_value_type; }
    const pointer operator->() const { return &(operator*()); }
    RBIter& operator++() {
        _current_node = tree_increment(_current_node);
        return *this;
    }
    RBIter operator++(int) {
        RBIter tmp = *this;
        _current_node = tree_increment(_current_node);
        return tmp;
    }
    RBIter& operator--() {
        _current_node = tree_decrement(_current_node);
        return *this;
    }
    RBIter operator--(int) {
        RBIter tmp = *this;
        _current_node = tree_decrement(_current_node);
        return tmp;
    }

    bool operator==(const RBIter& x) const { return _current_node == x._current_node; }
    bool operator!=(const RBIter& x) const { return _current_node != x._current_node; }

}; // Red Black Iter Struct

template <typename _T>
struct _red_black_const_iter {

    typedef _T value_type;
    typedef const _T& reference;
    typedef const _T* pointer;
    typedef _red_black_iter<_T> RBIter;
    typedef ft::bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    typedef _red_black_const_iter<_T> CRBIter;
    typedef const _tree_node<_T>* _relationship;

    // Member value
    _relationship _current_node;

    // Constructors
    _red_black_const_iter() : _current_node() {}
    explicit _red_black_const_iter(_relationship x) : _current_node(x) {}
    _red_black_const_iter(const RBIter& x) : _current_node(x._current_node) {}
    RBIter _current_node_const_cast() const { return RBIter(const_cast<typename RBIter::_relationship>(_current_node)); }

    // Operators
    reference operator*() const { return *_current_node->_value_type; }
    const pointer operator->() const { return &(operator*()); }
    bool operator==(const CRBIter& x) const { return _current_node == x._current_node; }
    bool operator!=(const CRBIter& x) const { return _current_node != x._current_node; }

    CRBIter& operator++() {
        _current_node = tree_increment(_current_node);
        return *this;
    }
    CRBIter operator++(int) {
        CRBIter tmp = *this;
        _current_node = tree_increment(_current_node);
        return tmp;
    }
    CRBIter& operator--() {
        _current_node = tree_decrement(_current_node);
        return *this;
    }
    CRBIter operator--(int) {
        CRBIter tmp = *this;
        _current_node = tree_decrement(_current_node);
        return tmp;
    }

}; // Red Black Const Iter Struct

template <typename _valor>
inline bool operator==(const _red_black_iter<_valor>& x, const _red_black_const_iter<_valor>& y) {
    return x._current_node == y._current_node;
}

template <typename _valor>
inline bool operator!=(const _red_black_iter<_valor>& x, const _red_black_const_iter<_valor>& y) {
    return x._current_node != y._current_node;
}

// Increments function
template <typename _valor>
static _tree_node<_valor>* static_tree_increment(_tree_node<_valor>* x) throw() {
	if (x->_right == x) return x;
	if (x->_right != 0) {x = x->_right; while (x->_left != 0) x = x->_left; }
	else {
		_tree_node<_valor>* y = x->_parent;
		while (x == y->_right) {x = y; y = y->_parent;}
		if (x->_right != y) x = y;
	}
	return x;
} 

template <typename _valor>
_tree_node<_valor>* tree_increment(_tree_node<_valor>* x) throw () {
	return static_tree_increment(x);
}

template <typename _valor>
const _tree_node<_valor>* tree_increment(const _tree_node<_valor>* x) throw() {
	return static_tree_increment(const_cast<_tree_node<_valor>*>(x));
}

// Decrements functions
template <typename _valor>
static _tree_node<_valor>* static_tree_decrement(_tree_node<_valor>* x) throw() {
	if (x->_right == x) return x;
	if (x->_color == _node_red && x->_parent->_parent == x) x = x->_right;
	else if (x->_left != 0) {
		_tree_node<_valor>* y = x->_left;
		while (y->_right != 0) y = y->_right;
		x = y;
	} else {
		_tree_node<_valor>* y = x->_parent;
		while (x == y->_left) {x = y; y = y->_parent;}
		x = y;
	}
	return x;
}

template <typename _valor>
_tree_node<_valor>* tree_decrement(_tree_node<_valor>* x) throw() {
  return static_tree_decrement(x);
}

template <typename _valor>
const _tree_node<_valor>* tree_decrement(const _tree_node<_valor>* x) throw() {
  return static_tree_decrement(const_cast<_tree_node<_valor>*>(x));
}

} // Namespace FT