#pragma once

#include <cstddef>
#include <iostream>

namespace ft {

    // 5 Class to identify the categories
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    
    // Semplify declaration of iterators.
    // Category will be a tag
    // T the dype of data if you dereference
    // Distance a value you can use to calc. distance between iterators   
    template <class Category, class T, class Distance = std::ptrdiff_t,
              class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    // Iterator traits [properties of a iterators]
    // STD algorithms uses certain properties of iterator traits
    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    // Specialization pointer
    template <class T>
    struct iterator_traits<T*> {
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ft::random_access_iterator_tag iterator_category;
    };

    // Specialization pointer const
    template <class T>
    struct iterator_traits<T* const> {
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef ft::random_access_iterator_tag iterator_category;
    };

    template <class Iterator>
    class reverse_iterator
    {
        public:
            typedef Iterator iterator_type;
            typedef iterator_traits<Iterator> traits;

        protected:
            Iterator current;

        public:
            typedef typename traits::iterator_category iterator_category;
            typedef typename traits::value_type value_type;
            typedef typename traits::difference_type difference_type;
            typedef typename traits::pointer pointer;
            typedef typename traits::reference reference;

        /// Member functions
        reverse_iterator() : current() {}
        
        // Construct a iterator from an existing iterator. 
        // It will have the same behaviour, but inversed iteration
        explicit reverse_iterator(iterator_type x) : current(x) {}

        // Copy an iterator, behaviour included
        template <class Other>
        reverse_iterator(const reverse_iterator<Other>& other) : current(other.base()) {}

        virtual ~reverse_iterator(){};

        iterator_type base() const { return current; }

        // Cast operator
        operator reverse_iterator<const Iterator>() const { return this->current; }

        reference operator*() const { iterator_type tmp = current; return *(--tmp); }
       
        pointer operator->() const { return &(operator*()); }

        reference operator[](difference_type n) const {return *(*this + n); }
        
        reverse_iterator& operator++() {
            --current;
            return *this;
        }

        reverse_iterator& operator--() {
            ++current;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp = *this;
            --current;
            return tmp;
        }

        reverse_iterator operator--(int) {
            reverse_iterator tmp = *this;
            ++current;
            return tmp;
        }

        reverse_iterator operator+(difference_type n) const {
            return reverse_iterator(current - n);
        }

        reverse_iterator operator-(difference_type n) const {
            return reverse_iterator(current + n);
        }

        reverse_iterator& operator+=(difference_type n) {
            current -= n;
            return *this;
        }

        reverse_iterator& operator-=(difference_type n) {
            current += n;
            return *this;
        }
    };

    // Nonmember function

    template <class Iterator1, class Iterator2>
    bool operator==(const ft::reverse_iterator<Iterator1>& lhs,
                    const ft::reverse_iterator<Iterator2>& rhs) {
        return (lhs.base() == rhs.base());
    }

    template <class Iterator1, class Iterator2>
    bool operator!=(const ft::reverse_iterator<Iterator1>& lhs,
                    const ft::reverse_iterator<Iterator2>& rhs) {
        return (lhs.base() != rhs.base());
    }

    template <class Iterator1, class Iterator2>
    bool operator<(const ft::reverse_iterator<Iterator1>& lhs,
                   const ft::reverse_iterator<Iterator2>& rhs) {
        return (lhs.base() > rhs.base());
    }

    template <class Iterator1, class Iterator2>
    bool operator>(const ft::reverse_iterator<Iterator1>& lhs,
                   const ft::reverse_iterator<Iterator2>& rhs) {
        return (lhs.base() < rhs.base());
    }

    template <class Iterator1, class Iterator2>
    bool operator<=(const ft::reverse_iterator<Iterator1>& lhs,
                    const ft::reverse_iterator<Iterator2>& rhs) {
        return (lhs.base() >= rhs.base());
    }

    template <class Iterator1, class Iterator2>
    bool operator>=(const ft::reverse_iterator<Iterator1>& lhs,
                    const ft::reverse_iterator<Iterator2>& rhs) {
        return (lhs.base() <= rhs.base());
    }

    template <class Iter>
    ft::reverse_iterator<Iter> operator+(
            typename ft::reverse_iterator<Iter>::difference_type n,
            const ft::reverse_iterator<Iter>& it) {
        return ft::reverse_iterator<Iter>(it.base() - n);
    }
    
    template <class Iterator, class Iterator2>
    typename ft::reverse_iterator<Iterator>::difference_type operator-(
            const ft::reverse_iterator<Iterator>& lhs,
            const ft::reverse_iterator<Iterator2>& rhs) {
        return rhs.base() - lhs.base();
    }

}; // Namespace FT