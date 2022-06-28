#pragma once

#include "iterator.hpp"

namespace ft {

// If std::enable_if has a public member typedef type equal to T;
// Bool is true otherwise, there is no member typedef.

template <bool Bool, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> { typedef T type; };

struct true_type {
  static const bool valor = true;
  typedef true_type type;
  typedef bool value_type;
};

struct false_type {
  static const bool valor = false;
  typedef false_type type;
  typedef bool value_type;
};

// Gives a bool true if T is integral
template < class T >
struct is_integral : public false_type {};

template <>
struct is_integral<bool> : public true_type {};
template <>
struct is_integral<char> : public true_type {};
template <>
struct is_integral<char16_t> : public true_type {};
template <>
struct is_integral<char32_t> : public true_type {};
template <>
struct is_integral<wchar_t> : public true_type {};
template <>
struct is_integral<signed char> : public true_type {};
template <>
struct is_integral<short int> : public true_type {};
template <>
struct is_integral<int> : public true_type {};
template <>
struct is_integral<long int> : public true_type {};
template <>
struct is_integral<long long int> : public true_type {};
template <>
struct is_integral<unsigned char> : public true_type {};
template <>
struct is_integral<unsigned short int> : public true_type {};
template <>
struct is_integral<unsigned int> : public true_type {};
template <>
struct is_integral<unsigned long int> : public true_type {};
template <>
struct is_integral<unsigned long long int> : public true_type {};


// ptr utilities
struct nullptr_t {
private:
	void operator&() const;

 public:
	template <class T>
	inline operator T*() const { return 0; }

	template <class C, class T>
	inline operator T C::*() const { return 0; }
};

static nullptr_t null_pointer = {};

template <class InputIterator>
typename ft::iterator_traits<InputIterator>::difference_type difference(InputIterator first, InputIterator last) {
	typedef typename ft::iterator_traits<InputIterator>::difference_type size_type;
	size_type difference = 0;
	for (; first != last; ++first) { ++difference; }
	return difference;
}

template <typename T>
struct Identity {
  T& operator()(T& other) const { return other; }
  const T& operator()(const T& other) const { return other; }
};

// Specialization for const
template <typename T>
struct Identity<const T> : Identity<T> {};

};  // Namespace FT