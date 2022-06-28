#pragma once

// Commons
#include "../headers/compares.hpp"
#include "../headers/iterator.hpp"
#include "../headers/pair.hpp"
#include "../headers/utilities.hpp"

// Specific
#include "../red_black_headers/tree_node.hpp"

namespace ft {

enum _colors {
    _node_red = false,
    _node_black = true
};

template <typename _value>
struct _tree_node {
    typedef _tree_node<_value>* _relationship;

    _colors _color;
    _relationship _parent;
    _relationship _left;
    _relationship _right;
    _value* _value_type;

    bool is_left_node() { return this == _parent->_left; }
    bool child_is_red() { return (_left && _left->_color == _node_red) || (_right && _right->_color == _node_red); }

    // Return a pointer to the uncle, sibling of the father
    _relationship uncle(){
        if (_parent == NULL || _parent->_parent == NULL) return NULL;
        if (_parent->is_left_node()) return _parent->_parent->_right;
        else { return _parent->_parent->_left; }
    }

    // Return a pointer to the sibling
    _relationship sibling() {
        if (_parent == NULL) return NULL;
        if (is_left_node()) return _parent->_right;
        return _parent->_left;
    }

    // Move down the node, replacing his old location with a new one
    void move_down(_relationship new_parent){
        if (_parent != NULL) {
            if (is_left_node()) _parent->_left = new_parent;
            else { _parent->_right = new_parent; }
            new_parent->_parent = _parent;
            _parent = new_parent;
        }
    }

    void swap_node(_relationship& other) {
        _relationship other_parent = other->_parent;
        _relationship other_right = other->_right;
        _relationship other_left = other->_left;
        _relationship this_parent = _parent;
        _relationship this_right = _right;
        _relationship this_left = _left;

        if (other_parent->_left == other) { other_parent->_left = this; }
        else if (other_parent->_right == other) {other_parent->_right = this; }
        if (other_left) other_left->_parent = this;
        if (other_right) other_right->_parent = this;

        if (is_left_node()) this_parent->_left = other;
        else this_parent->_right = other;
        if (this_left) this_left->_parent = other;
        if (this_right) this_right->_parent = other;

        std::swap(_color, other->_color);
        std::swap(_parent, other->_parent);
        std::swap(_right, other->_right);
        std::swap(_left, other->_left);
    }
}; // End of struct _tree_node

} // Namespace FT