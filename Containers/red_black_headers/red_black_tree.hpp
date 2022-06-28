#pragma once

// Specific
#include "./tree_node.hpp"
#include "./red_black_iterator.hpp"

// https://www.programiz.com/dsa/red-black-tree
// https://www.geeksforgeeks.org/red-black-tree-set-1-introduction-2/
// https://www.rpi.edu/dept/acm/packages/stl/stl_tree.h
namespace ft {

template <typename _Key, typename _Valor, typename _KeyValue, typename _Compare, typename _Alloc = std::allocator<_Valor> >
class Red_Black_Tree {
	typedef typename _Alloc::template rebind<_tree_node<_Valor> >::other _node_allocator;

protected:
	typedef _tree_node<_Valor>* _relationship;
	typedef const _tree_node<_Valor>* _const_relationship;

public:
	typedef _Key key_type;
	typedef _Valor type_val;
	typedef type_val* pointer;
	typedef const type_val* const_pointer;
	typedef type_val& reference;
	typedef const type_val& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t type_difference;
	typedef _Alloc allocator_type;
	typedef _Compare compare_key;

private:
	// Members
	_relationship _head;
	size_type _total_node;
	compare_key _compare_key;
	_node_allocator _node_alloc;

public:
	typedef _red_black_iter<type_val> iterator;
	typedef _red_black_const_iter<type_val> const_iterator;
	typedef ft::reverse_iterator<iterator> reverse_iterator;
	typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	// Iterators function
	iterator begin() { return iterator(_head->_left); }
	const_iterator begin() const { return const_iterator(_head->_left); }
	iterator end() { return iterator(_head); }
	const_iterator end() const { return const_iterator(_head); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	compare_key key_comp() const { return _compare_key; }
	allocator_type get_allocator() const { return allocator_type(_node_alloc); }
	bool empty() const { return _total_node == 0; }
	size_type size() const { return _total_node; }
	size_type max_size() const {
		size_t div = sizeof(_relationship) * 4 + sizeof(type_val);
		// Without the next line it will give a wrong max_size()
		div = (div / 8) * 8;
		return std::numeric_limits<size_type>::max() / div;
	}

protected:
	key_type _true_key(_relationship x) const { return _KeyValue()(*(x->_value_type)); }
	key_type _true_key(_const_relationship x) const { return _KeyValue()(*(x->_value_type)); }
	key_type _true_key(type_val x) const { return _KeyValue()(x); }
	key_type _true_key(iterator x) const { return _KeyValue()(*x); }
	key_type _true_key(const_iterator x) const { return _KeyValue()(*x); }
	
	_relationship _true_end() { return _head; }
	_const_relationship _true_end() const { return _head; }
	_relationship _true_root() { return _head->_parent; }
	_const_relationship _true_root() const { return _head->_parent; }
	_relationship& _ref_true_root() { return _head->_parent; }
	_const_relationship _ref_true_root() const {return _head->_parent; }


	// Most right node
	_relationship _most_right() {
		_relationship current = _true_root();
		if (current == NULL) return _head;
		while (current->_right) current = current->_right;
		return current;
	}

	// Most left node
	_relationship _most_left() {
		_relationship current = _true_root();
		if (current == NULL) return _head;
		while (current->_left) current = current->_left;
		return current;
	}
  
	// Two constant overloads
	_const_relationship _most_right() const {return (_const_relationship)_most_right(); }
	_const_relationship _most_left() const {return (_const_relationship)_most_left(); }
	
	// Reinitialise
	void _reinitialize() {
		_head->_color = _node_red;
		_head->_parent = 0;
		_head->_left = _head;
		_head->_right = _head;
		_total_node = 0;
	}

	// Move node
	void node_move(Red_Black_Tree& _starting_tree) {
		_head->_color = _starting_tree._head->_color;
		_ref_true_root() = _starting_tree._head->_parent;
		_head->_left = _starting_tree._head->_left;
		_head->_right = _starting_tree._head->_right;
		_true_root()->_parent = _head;
		_total_node = _starting_tree._total_node;
		_starting_tree._reinitialize();
	}

public:
	void clear() {
		_erase(_true_root());
		_reinitialize();
	}

public:
	// Constructors
	Red_Black_Tree() : _compare_key(compare_key()), _node_alloc(_node_allocator()) {
		_head = _node_creator(type_val());
		_reinitialize();
	}

	Red_Black_Tree(const compare_key& key_comp, const _node_allocator& alloc) 
		: _compare_key(key_comp), _node_alloc(alloc) {
		_head = _node_creator(type_val());
		_reinitialize();
	}

	Red_Black_Tree(Red_Black_Tree const& other) {
		_head = _node_creator(type_val());
		_reinitialize();
		_compare_key = other._compare_key;
		_node_alloc = other._node_alloc;
		if (other._ref_true_root() != 0)
		{
			_ref_true_root() = _node_copier(other._head->_parent, _head);
			_head->_right = _most_right();
			_head->_left = _most_left();
		}
	}

	~Red_Black_Tree() {
		_erase(_true_root());
		++_total_node;
		_node_destroyer(_head);
	}

	Red_Black_Tree& operator=(const Red_Black_Tree& other) {
		if (this == &other) return *this;
		_erase(_true_root());
		_reinitialize();
		_compare_key = other._compare_key;
		_node_alloc = other._node_alloc;
		if (other._total_node) {
			_ref_true_root() = _node_copier(other._head->_parent, _head);
			_head->_left = _most_left();
			_head->_right = _most_right();
		}
		return *this;
	}

protected:

	// Insertion of a new_node in a tree root
	_relationship insert_tree_node(_relationship root, _relationship new_node){
		// In an empty tree
		if (root == NULL) return new_node;
		// Or go down 'till the right position
		if (_compare_key(_true_key(new_node), _true_key(root))) {
			root->_left = insert_tree_node(root->_left, new_node);
			root->_left->_parent = root;
		} else if (_compare_key(_true_key(root), _true_key(new_node))) {
			root->_right = insert_tree_node(root->_right, new_node);
			root->_right->_parent = root;
		}
		// And return is the new_node pointer
		return root;
	}

	// Rotations
	// https://www.youtube.com/watch?v=95s3ndZRGbk&ab_channel=MichaelSambol
	void left_rotate(_relationship x) {
		// New node, that will be the node child right
		_relationship a_parent = x->_right;
		// Update root if x is root
		if (x == _true_root()) _ref_true_root() = a_parent;
		x->move_down(a_parent);
		// Connect x with a_parent left
		x->_right = a_parent->_left;
		// Connect a_aprent left with x if it's not null
		if (a_parent->_left != NULL) a_parent->_left->_parent = x;
		// Connect a_parent and x
		a_parent->_left = x;
	}

	void right_rotate(_relationship x) {
		// New node, that will be the node child left
		_relationship a_parent = x->_left;
		// Update root if x is root
		if (x == _true_root()) _ref_true_root() = a_parent;
		x->move_down(a_parent);
		// Connect x with a_parent right
		x->_left = a_parent->_right;
		// Connect a_aprent right with x if it's not null
		if (a_parent->_right != NULL) a_parent->_right->_parent = x;
		// Connect a_parent and x
		a_parent->_right = x;
	}

	// https://www.youtube.com/watch?v=bqOSo1f1jbo&t=225s&ab_channel=Geekific
	void no_red_red_nodes(_relationship& ptr) {
		_relationship ptr_parent = NULL;
		_relationship ptr_grand_parent = NULL;
		while ((ptr != _true_root()) 
				&& (ptr->_color != _node_black) // So NULL too
				&& (ptr->_parent->_color == _node_red)) 
		{
			ptr_parent = ptr->_parent;
			ptr_grand_parent = ptr->_parent->_parent;

			// Case A: Parent is left child of ptr_grand_parent
			if (ptr_parent == ptr_grand_parent->_left) {
				_relationship ptr_uncle = ptr_grand_parent->_right;

				// Case A1: uncle red, just recolor
				if (ptr_uncle != NULL && ptr_uncle->_color == _node_red) {
					ptr_grand_parent->_color = _node_red;
					ptr_parent->_color = _node_black;
					ptr_uncle->_color = _node_black;
					ptr = ptr_grand_parent;
				} else {
					/* Case A2: left rotation is needed
					// ptr is right child of is parent
					//         B ptr_grand_parent
					//            / \
					//     ptr_p R   B ptr_uncle
					//            \
					//             R                   */
					if (ptr == ptr_parent->_right) {
						left_rotate(ptr_parent);
						ptr = ptr_parent;
						ptr_parent = ptr->_parent;
					}
					/* Case A3: right rotation needed
							B ptr_grand_parent
							/ \
			   ptr_parent  R   B ptr_uncle
						  /
						 R

						ptr_parent will be new root    */
					right_rotate(ptr_grand_parent);
					std::swap(ptr_parent->_color, ptr_grand_parent->_color);
					ptr = ptr_parent;
				}
			} else {
				_relationship ptr_uncle = ptr_grand_parent->_left;
				// Case B: Parent is right child of ptr_ptr 
				if (ptr_uncle != NULL && ptr_uncle->_color == _node_red) {
						ptr_grand_parent->_color = _node_red;
						ptr_parent->_color = _node_black;
						ptr_uncle->_color = _node_black;
						ptr = ptr_grand_parent;
					} else {
						if (ptr == ptr_parent->_left) {
							right_rotate(ptr_parent);
							ptr = ptr_parent;
							ptr_parent = ptr->_parent;
						}
						left_rotate(ptr_grand_parent);
						std::swap(ptr_parent->_color, ptr_grand_parent->_color);
						ptr = ptr_parent;
					}
			}
		}
		_ref_true_root()->_color = _node_black;
	}

	void no_black_black(_relationship node) {
		if (node == _true_root()) return;
		_relationship node_sibling = node->sibling();
		_relationship node_parent = node->_parent;
		// No sibling, we fix it in the previous node
		if (node_sibling == NULL) no_black_black(node_parent);
		else {
			// Sibling red, recolor
			if (node_sibling->_color == _node_red) {
				node_parent->_color = _node_red;
				node_sibling->_color = _node_black;
				if (node_sibling->is_left_node())
					right_rotate(node_parent);
				else
					left_rotate(node_parent);
				no_black_black(node);
			} else {
				// Sibling black
			if (node_sibling->child_is_red()) {
				if (node_sibling->_left != NULL && node_sibling->_left->_color == _node_red) {
					if (node_sibling->is_left_node()) {
						// left left case
						node_sibling->_left->_color = node_sibling->_color;
						node_sibling->_color = node_parent->_color;
						right_rotate(node_parent);
					} else {
						// right left case
						node_sibling->_left->_color = node_parent->_color;
						right_rotate(node_sibling);
						left_rotate(node_parent);
					}
				} else {
					// right left
					if (node_sibling->is_left_node()) {
						node_sibling->_right->_color = node_parent->_color;
						left_rotate(node_sibling);
						right_rotate(node_parent);
					} else {
					// right right
						node_sibling->_right->_color = node_sibling->_color;
						node_sibling->_color = node_parent->_color;
						left_rotate(node_parent);
					}
				}
				node_parent->_color = _node_black;
			} else {
				node_sibling->_color = _node_red;
				if (node_parent->_color == _node_black)
					no_black_black(node_parent);
				else node_parent->_color = _node_black;
				}
			}
		}
	}

	// Return a node with no left child
	_relationship no_left_child(_relationship other) {
		_relationship temp = other;
		while (temp->_left != NULL) temp = temp->_left;
		return temp;
	}

	// Find a node that replace a deleted one
	_relationship replace_node(_relationship other) {
		// Node with two children
		if (other->_left != NULL && other->_right != NULL) return no_left_child(other->_right);
		// Node with no children (leaf)
		if (other->_left == NULL && other->_right == NULL) return NULL;
		if (other->_right != NULL) return other->_right;
		else { return other->_left; }
	}

	// Delete a node
	void node_deleter(_relationship deleted) {
		_relationship delete_helper = replace_node(deleted);

		bool both_black = ((delete_helper == NULL || delete_helper->_color == _node_black) && (deleted->_color == _node_black));
		_relationship parent = deleted->_parent;
		if (delete_helper == NULL) {
			// if the helper is NULL, deleted is a leaf
			if (deleted == _true_root()) {
				_ref_true_root() = _head;
			} else {
				if (both_black)
					// Both black, deleted is a leaf, so just fix black_black
					no_black_black(deleted);
				else { // There's a sibling, so it must be red
					if (deleted->sibling() != NULL)
						deleted->sibling()->_color = _node_red;
				}
				if (deleted->is_left_node())
					parent->_left = NULL;
				else
					parent->_right = NULL;
			}
			_node_destroyer(deleted);
			return;
		}
		if (deleted->_left == NULL || deleted->_right == NULL) {
			// Deleted has 1 child
			if (deleted == _true_root()) {
				_ref_true_root() = delete_helper;
				deleted->swap_node(delete_helper);
				delete_helper->_left = delete_helper->_right = NULL;
				_node_destroyer(deleted);
			} else {
				if (deleted->is_left_node())
					parent->_left = delete_helper;
				else
					parent->_right = delete_helper;
				_node_destroyer(deleted);
				delete_helper->_parent = parent;
				if (both_black)
					no_black_black(deleted);
				else
					delete_helper->_color = _node_black;
			}
			return;
		}
		// Deleted has 2 child
		if (deleted == _true_root()) _ref_true_root() = delete_helper;
		deleted->swap_node(delete_helper);
		node_deleter(deleted);
	}

	void _node_destroyer(_relationship node) {
		--_total_node;
		_Alloc value_alloc;
		value_alloc.destroy(node->_value_type);
		value_alloc.deallocate(node->_value_type, 1);
		_node_alloc.destroy(node);
		_node_alloc.deallocate(node, 1);
	}

	_relationship _node_getter() {
		++_total_node;
		return _node_alloc.allocate(1);
	}

	void _node_constructor(_relationship node, const type_val& x) {
		try {
			node->_color = _node_red;
			node->_left = NULL;
			node->_right = NULL;
			node->_parent = NULL;
			_Alloc value_alloc;
			typename _Alloc::pointer ptr = value_alloc.allocate(1);
			value_alloc.construct(ptr, x);
			node->_value_type = ptr;
		} catch (...) {
			_node_destroyer(node);
			throw;
		}
	}

	_relationship _node_creator(const type_val& node) {
		_relationship temp = _node_getter();
		_node_constructor(temp, node);
		return temp;
	}

	_relationship _node_cloner(_const_relationship x) {
		_relationship temp = _node_creator(*x->_value_type);
		temp->_color = x->_color;
		temp->_right = 0;
		temp->_left = 0;
		return temp;
	}

	_relationship _node_copier (_const_relationship x, _relationship y) {
		_relationship up = _node_cloner(x);
		up->_parent = y;
		try {
			if (x->_right) up->_right = _node_copier(x->_right, up);
			y = up;
			x = x->_left;
			while (x != 0) {
				_relationship z = _node_cloner(x);
				y->_left = z;
				z->_parent = y;
				if (x->_right) z->_right = _node_copier(x->_right, z);
				y = z;
				x = x->_left;
			}
		} catch (...) {
			_erase(up);
			throw;
		}
		return up;
	}
		

	void _erase(_relationship x) {
		while (x != 0) {
			_erase(x->_right);
			_relationship y = x->_left;
			_node_destroyer(x);
			x = y;
		}
	}

	iterator _lower_bound(_relationship x, _relationship y, const _Key& k) {
		while (x != 0) {
			if (!_compare_key(_true_key(x), k))
				y = x, x = x->_left;
			else
				x = x->_right;
		}
		return iterator(y);
	}

	const_iterator _lower_bound(_const_relationship x, _const_relationship y, const _Key& k) const {
		while (x != 0) {
			if (!_compare_key(_true_key(x), k))
				y = x, x = x->_left;
			else
				x = x->_right;
		}
		return const_iterator(y);
	}

	iterator _upper_bound(_relationship x, _relationship y, const _Key& k) {
		while (x != 0) {
			if (_compare_key(k, _true_key(x)))
				y = x, x = x->_left;
			else
				x = x->_right;
		}
		return iterator(y);
	}

	const_iterator _upper_bound(_const_relationship x, _const_relationship y, const _Key& k) const {
		while (x != 0) {
			if (_compare_key(k, _true_key(x)))
				y = x, x = x->_left;
			else
				x = x->_right;
		}
		return const_iterator(y);
	}

public:
	//https://en.cppreference.com/w/cpp/container/map/insert
	ft::pair<iterator, bool> insert(const type_val& other) {
		ft::pair<iterator, bool> current;
		iterator iter = lower_bound(_true_key(other));
		if (iter != end() && !_compare_key(_true_key(other), _true_key(iter))) {
			current.first = iter;
			current.second = false;
			return current;
		}
		_relationship ptr = _node_creator(other);
		_relationship previous = ptr;
		_ref_true_root() = insert_tree_node(_true_root(), ptr);
		// Balance if needed
		no_red_red_nodes(ptr);
		_ref_true_root()->_parent = _head;
		_head->_left = _most_left();
		_head->_right = _most_right();
		current.first = (iterator)previous;
		current.second = true;
		return current;
	}

	// Delete the node with the given value
	size_type erase(const key_type& key) {
		// Empry tree
		if (_true_root() == NULL)
			return 0;
		iterator to_delete = lower_bound(key);
		if (to_delete == end() || _compare_key(key, _true_key(to_delete))) return 0;
		node_deleter(to_delete.get_link());
		if (size()) {
			_ref_true_root()->_parent = _head;
			_head->_left = _most_left();
			_head->_right = _most_right();
		} else {
			_ref_true_root() = NULL;
			_head->_left = _head;
			_head->_right = _head;
		}
		return 1;
	}

	void swap(Red_Black_Tree &tree) {
		if (_true_root() == 0) {
			if (tree._true_root() != 0) node_move(tree);
		} else if (tree._ref_true_root() == 0)
			tree.node_move(*this);
			else {
				std::swap(_ref_true_root(), tree._ref_true_root());
				std::swap(_head->_left, tree._head->_left);
				std::swap(_head->_right, tree._head->_right);
				_ref_true_root()->_parent = _true_end();
				tree._ref_true_root()->_parent = tree._true_end();
				std::swap(_total_node, tree._total_node);
			}
			std::swap(_compare_key, tree._compare_key);
			std::swap(_node_alloc, tree._node_alloc);
		}

	iterator find(const _Key& _key) {
		iterator helper_key = lower_bound(_key);
		if (helper_key == end() || _compare_key(_key, _true_key(helper_key)))
			return end();
		else
			return helper_key;
	}

	const_iterator find(const _Key& _key) const {
		const_iterator helper_key = lower_bound(_key);
		if (helper_key == end() || _compare_key(_key, _true_key(helper_key)))
			return end();
		else
			return helper_key;
	}

	iterator lower_bound(const key_type& key) {
		return _lower_bound(_true_root(), _true_end(), key);
	}

	const_iterator lower_bound(const key_type& key) const {
		return _lower_bound(_true_root(), _true_end(), key);
	}

	iterator upper_bound(const key_type& key) {
		return _upper_bound(_true_root(), _true_end(), key);
	}

	const_iterator upper_bound(const key_type& key) const {
		return _upper_bound(_true_root(), _true_end(), key);
	}

	ft::pair<iterator, iterator> equal_range(const _Key& _key) {
    _relationship x = _true_root();
    _relationship y = _true_end();
    while (x != 0) {
      if (_compare_key(_true_key(x), _key))
        x = x->_right;
      else if (_compare_key(_key, _true_key(x)))
        y = x, x = x->_left;
      else {
        _relationship x1(x);
        _relationship y1(y);
        y = x;
        x = x->_left;
        x1 = x1->_right; 
        return ft::pair<iterator, iterator>(_lower_bound(x, y, _key), _upper_bound(x1, y1, _key));
      }
    }
    return ft::pair<iterator, iterator>(iterator(y), iterator(y));
  }

	ft::pair<const_iterator, const_iterator> equal_range(const key_type& _key) const {
    _const_relationship x = _true_root();
    _const_relationship y = _true_end();
    while (x != 0) {
      if (_compare_key(_true_key(x), _key))
        x = x->_right;
      else if (_compare_key(_key, _true_key(x)))
        y = x, x = x->_left;
      else {
        _const_relationship x1(x);
        _const_relationship y1(y);
        y = x;
        x = x->_left;
        x1 = x1->_right; 
        return ft::pair<const_iterator, const_iterator>(_lower_bound(x, y, _key), _upper_bound(x1, y1, _key));
      }
    }
    return ft::pair<const_iterator, const_iterator>(const_iterator(y), const_iterator(y));
  }
}; // Class Tree

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator==(const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& lhs,
                       const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& rhs) {
	return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator!=(const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& lhs,
                       const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& rhs) {
	return !(lhs == rhs);
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator<(const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& lhs,
                       const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator<=(const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& lhs,
                       const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& rhs) {
	return !(rhs < lhs);
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator>(const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& lhs,
                       const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& rhs) {
	return rhs < lhs;
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator>=(const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& lhs,
                       const ft::Red_Black_Tree<_Key, _Val, Compare, _Alloc>& rhs) {
	return !(lhs < rhs);
}

} // Namespace FT