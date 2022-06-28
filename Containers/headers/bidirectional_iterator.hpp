#pragma once

#include "iterator.hpp"
#include "utilities.hpp"

namespace ft {

template <class T>
class bidirectional_iterator : public ft::iterator<ft::bidirectional_iterator_tag, T> {

public:
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::iterator_category iterator_category;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::value_type value_type;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::difference_type difference_type;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::pointer pointer;
	typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::reference reference;

protected:
	pointer _current;

public:

	// Default constructor
	bidirectional_iterator(pointer _current = ft::u_nullptr) : _current(_current){};

	// Copy and cast constructor
	bidirectional_iterator(const bidirectional_iterator& other) : _current(other._current){};

	template <typename T>
	bidirectional_iterator(const bidirectional_iterator<T>& other) : _current(other.base()) {}

	reference operator*() const { return *_current; };

	pointer operator->() const { return &(operator*()); };

	pointer const& base() const { return _current; }

	bool operator==(const bidirectional_iterator<T>& other) {
		return (this->_current == other._current);
	};

	bool operator!=(const bidirectional_iterator<T>& other) {
		return (this->_current != other._current);
	};

	bidirectional_iterator& operator++() {
		++_current;
		return *this;
	};

	bidirectional_iterator operator++(int) {
		bidirectional_iterator tmp = *this;
		++(*this);
		return tmp;
	};

	bidirectional_iterator& operator--() {
		--_current;
		return *this;
	};

	bidirectional_iterator operator--(int) {
		bidirectional_iterator tmp = *this;
		--(*this);
		return tmp;
	};

};

} // Namespace FT