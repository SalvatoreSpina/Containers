#pragma once

#include "iterator.hpp"
#include "random_access_iterator.hpp"
#include "utilities.hpp"

namespace ft {

template <typename T>
class random_access_iterator : public ft::iterator<ft::random_access_iterator_tag, T> {

    protected:
        T* _current;

    public:
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::iterator_category iterator_category;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::value_type value_type;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::difference_type difference_type;        
        typedef T* pointer;
        typedef T& reference;

        // Deafult constructor
        random_access_iterator(pointer _current = ft::null_pointer) : _current(_current){};

        // Copy Constructor
        random_access_iterator(const random_access_iterator<T>& other) : _current(other._current) {};
        
        // Destructor
        ~random_access_iterator() {}

        // Casting operator
        operator random_access_iterator<const T>() const { return this->_current; }

        pointer base() const {return _current;}
        reference operator*() const {return *_current;}
        pointer operator->() const {return &(operator *());}
        reference operator[](difference_type n) const {return *(*this+n);}
        random_access_iterator& operator++() {
            ++_current;
            return *this;
        }
        random_access_iterator& operator--() {
            --_current;
            return *this;
        }
        random_access_iterator operator++(int) {
            random_access_iterator tmp = *this;
            ++_current;
            return tmp;
        }
        random_access_iterator operator--(int){
            random_access_iterator tmp = *this;
            --_current;
            return tmp;
        }
        random_access_iterator operator+(difference_type n) const {
            return random_access_iterator(_current + n);
        }
        random_access_iterator operator-(difference_type n) const {
            return random_access_iterator(_current - n);
        }
        random_access_iterator& operator+=(difference_type n) {
            _current += n;
            return *this;
        }
        random_access_iterator& operator-=(difference_type n) {
            _current -= n;
            return *this;
        }
    };

    // Non member
    template <class Iterator1, class Iterator2>
    bool operator==(const ft::random_access_iterator<Iterator1>& lhs,
                    const ft::random_access_iterator<Iterator2>& rhs) {
        return (lhs.base() == rhs.base());
    }
    template <class Iterator1, class Iterator2>
    bool operator!=(const ft::random_access_iterator<Iterator1>& lhs,
                    const ft::random_access_iterator<Iterator2>& rhs) {
        return (lhs.base() != rhs.base());
    }
    template <class Iterator1, class Iterator2>
    bool operator<(const ft::random_access_iterator<Iterator1>& lhs,
                    const ft::random_access_iterator<Iterator2>& rhs) {
        return (lhs.base() < rhs.base());
    }
    template <class Iterator1, class Iterator2>
    bool operator<=(const ft::random_access_iterator<Iterator1>& lhs,
                    const ft::random_access_iterator<Iterator2>& rhs) {
        return (lhs.base() <= rhs.base());
    }
    template <class Iterator1, class Iterator2>
    bool operator>(const ft::random_access_iterator<Iterator1>& lhs,
                const ft::random_access_iterator<Iterator2>& rhs) {
        return (lhs.base() > rhs.base());
    }
    template <class Iterator1, class Iterator2>
    bool operator>=(const ft::random_access_iterator<Iterator1>& lhs,
                    const ft::random_access_iterator<Iterator2>& rhs) {
        return (lhs.base() >= rhs.base());
    }

    template <class Iter>
    ft::random_access_iterator<Iter> operator+(
            typename ft::random_access_iterator<Iter>::difference_type n,
            const ft::random_access_iterator<Iter>& it) {
        return ft::random_access_iterator<Iter>(it.base() + n);
    }

    template <class Iterator1, class Iterator2>
    typename ft::random_access_iterator<Iterator1>::difference_type operator-(
            const ft::random_access_iterator<Iterator1>& lhs,
            const ft::random_access_iterator<Iterator2>& rhs) {
        return lhs.base() - rhs.base();
    }
} // FT Namespace