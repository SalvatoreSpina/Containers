#pragma once

#include <iostream>
#include <limits>

#include "./headers/compares.hpp"
#include "./headers/iterator.hpp"
#include "./headers/random_access_iterator.hpp"
#include "./headers/utilities.hpp"

namespace ft {

template <typename T, typename Allocator = std::allocator<T> >
class vector {

    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename ft::random_access_iterator<value_type> iterator;
        typedef typename ft::random_access_iterator<const value_type> const_iterator;
        typedef typename ft::reverse_iterator<iterator> reverse_iterator;
        typedef typename ft::reverse_iterator<const_iterator> const_reverse_iterator;

        // Default constructor
        explicit vector(const allocator_type& alloc = allocator_type())
            : _alloc(alloc), _start(null_pointer), _end(null_pointer), _end_capacity(null_pointer){};

        // Fill constructor. All element will be equal to val
        explicit vector(size_type n, const value_type& val = value_type(), 
                const allocator_type& alloc = allocator_type())
                : _alloc(alloc) {
                this->_start = this->_alloc.allocate(n);
                this->_end = this->_start;
                this->_end_capacity = this->_start + n;
                while (n--) {this->_alloc.construct(this->_end++, val);}
        };

        // Range constructor. Create a container to store a range, ordered
        template<class InputIt>
        vector(InputIt first, InputIt last, 
                const allocator_type& alloc = allocator_type(),
                typename ft::enable_if<!ft::is_integral<InputIt>::valor>::type* = null_pointer)
                : _alloc(alloc) {
                    difference_type n = ft::difference(first, last);
                    this->_start = this->_alloc.allocate(n);
                    this->_end_capacity = this->_start + n;
                    this->_end = this->_start;
                    while (n--) {this->_alloc.construct(this->_end++, *first++);}
                };

        // Copy constructor.
        vector(const vector& x) : _alloc(x._alloc) {
            size_type n = x.size();
            this->_start = this->_alloc.allocate(n);
            this->_end_capacity = this->_start + n;
            this->_end = this->_start;
            pointer other = x._start;
            while (n--) {this->_alloc.construct(this->_end++, *other++);}
        };

        vector& operator=(const vector& x) {
            if (this != &x)
            {
                this->clear();
                this->insert(this->_start, x.begin(), x.end());
            }
            return *this;
        }

        // Destructor
        ~vector() {
            this->clear();
            this->_alloc.deallocate(this->_start, this->capacity());
        };

        // Iterator(s) to beginning
        iterator begin() { return (this->_start); };
        const_iterator begin() const { return (this->_start); };
        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return reverse_iterator(end()); };

        // Iterator(s) to the end
        iterator end() { return (this->_end); };
        const_iterator end() const { return (this->_end); };
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return reverse_iterator(begin()); }

        // Size, MaxSize and reSize
        size_type size() const { return (this->_end - this->_start); };
        size_type max_size() const { return (this->_alloc.max_size()); };
        void resize(size_type n) {
            if (n < size()) {
                size_type diff = size() - n;
                destroy_until(rbegin() + diff);
                _end = _start + n;
            } else if (n > size()) {
            reserve(n);
                for (; _end != _end_capacity; ++_end) {
                    construct(_end);
                }
            }
        }

        void resize(size_type n, const_reference value) {
            if (n < size()) {
                size_type diff = size() - n;
                destroy_until(rbegin() + diff);
                _end = _start + n;
            } else if (n > size()) {
                reserve(n);
                for (; _end != _end_capacity; ++_end) {
                    construct(_end, value);
                }
            }
        }

        // Size utilities
        size_type capacity() const { return (this->_end_capacity - this->_start); };
        bool empty() const { return (this->size() == 0); };

        // Edit capacity
        void reserve(size_type n) {
            if (n > this->max_size())
                {throw std::out_of_range("ft::vector");}
            if (this->capacity() >= n)
                return;
            
            pointer prev_start = this->_start;
            pointer prev_end = this->_end;
            size_type prev_capacity = this->capacity();

            this->_start = this->_alloc.allocate(n);
            this->_end_capacity = this->_start + n;
            this->_end = this->_start;

            for (pointer target = prev_start; target != prev_end; ++target)
                this->_alloc.construct(this->_end++, *target);
            for (size_type len = prev_end - prev_start; len > 0; --len)
                this->_alloc.destroy(--prev_end);
            this->_alloc.deallocate(prev_start, prev_capacity);
        };

        // Access(s) element
        reference operator[](size_type n) {return (this->_start[n]); };
        const_reference operator[](size_type n ) const {return (this->_start[n]); };

        // Reference at N, first, last
        reference at(size_type n) {
            if (this->size() < n) {throw std::out_of_range("index is out of range."); };
            return (this->_start[n]);
        } 

        reference at(size_type n) const {
            if (this->size() < n) {throw std::out_of_range("index is out of range."); };
            return (this->_start[n]);
        } 

        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *(end() - 1); }
        const_reference back() const { return *(end() - 1); }
        pointer data() { return _start; }
        const_pointer data() const { return _start; }


        // Assign contents to vector, modify if needed
        template <class InpIt>
        void assign(InpIt first, InpIt last, typename ft::enable_if<!ft::is_integral<InpIt>::valor>::type* = null_pointer) {
            size_type n = ft::difference(first, last);
            if (this->capacity() < n) {
                this->_start = this->_alloc.allocate(n);
                this->_end_capacity = this->_start + n;
                this->_end = this->_start;
            } else { this->clear(); }
            while (n--) this->_alloc.construct(this->_end++, *first++);
        };

        void assign(size_type n, const value_type& val) {
            this->clear();
            if (this->capacity() >= n) {
                while (n--) this->_alloc.construct(this->_end++, val);
            } else {
                this->_start = this->_alloc.allocate(n);
                this->_end_capacity = this->_start + n;
                this->_end = this->_start;
                while (n--) this->_alloc.construct(this->_end++, val);
            }
        };

        // Push back
        void push_back(const value_type& val) {
            if (this->_end_capacity == this->_end) {
                size_type capacity = (this->size() == 0) ? 1 : (this->_end_capacity - this->_start) * 2;
                this->reserve(capacity);
            }
            this->_alloc.construct(this->_end++, val);
        };

        // Pop back
        void pop_back() { this->_alloc.destroy(--this->_end); };

        // Insert at a certain position, return that position
        iterator insert(iterator position, const value_type& val) {
            size_type pos_insert = &(*position) - this->_start;
            this->insert(position, 1, val);
            return (this->_start + pos_insert);
        };

        // Insert a element, filling the given range
        void insert(iterator position, size_type n, const value_type& val) {
            size_type pos_at = &(*position) - this->_start;

            if (this->capacity() >= this->size() + n) {
                for (size_type i = 0; i < this->size() - pos_at; ++i) {
                    this->_alloc.construct(this->_end + n - i, *(this->_end - i));
                    this->_alloc.destroy(this->_end - i);
            }
            this->_end = this->_start + this->size() + n;
            for (size_type i = 0; i < n; ++i) {this->_alloc.construct(this->_start + pos_at + i, val);}
            return;
            }

            size_type next_capacity = this->size() + n;
            pointer prev_start = this->_start;
            pointer prev_end = this->_end;
            size_type prev_size = this->size();
            size_type prev_capacity = this->capacity();
            this->_start = this->_alloc.allocate(next_capacity);
            this->_end = this->_start + prev_size + n;
            this->_end_capacity = this->_end;
            for (size_type i = 0; i < pos_at; ++i) {
                this->_alloc.construct(this->_start + i, *(prev_start + i));
                this->_alloc.destroy(prev_start + i);
            }
            for (size_type i = 0; i < prev_size - pos_at; ++i) {
                this->_alloc.construct(this->_end - i - 1, *(prev_end - i - 1));
                this->_alloc.destroy(prev_end - i - 1);
            }
            for (size_type i = 0; i < n; ++i) {this->_alloc.construct(this->_start + pos_at + i, val);}
            this->_alloc.deallocate(prev_start, prev_capacity);
            return;
        };

        // Insert a range of elements
        template <class InpIt>
        void insert(iterator position, InpIt first, InpIt last, typename ft::enable_if<!ft::is_integral<InpIt>::valor>::type* = null_pointer) {
            size_type pos_at = &(*position) - this->_start;
            size_type n = ft::difference(first, last);

            if (this->capacity() >= this->size() + n) {
                for (size_type i = 0; i < this->size() - pos_at; ++i) {
                    this->_alloc.construct(this->_end + n - i, *(this->_end - i));
                    this->_alloc.destroy(this->_end - i);
                }
                this->_end = this->_start + this->size() + n;
                for (size_type i = 0; i < n; ++i) {
                    this->_alloc.construct(this->_start + pos_at + i, *first++);
                }
                return;
            }

            size_type next_capacity = this->size() + n;
            pointer prev_start = this->_start;
            pointer prev_end = this->_end;
            size_type prev_size = this->size();
            size_type prev_capacity = this->capacity();

            this->_start = this->_alloc.allocate(next_capacity);
            this->_end = this->_start + prev_size + n;
            this->_end_capacity = this->_end;

            for (size_type i = 0; i < pos_at; ++i) {
                this->_alloc.construct(this->_start + i, *(prev_start + i));
                this->_alloc.destroy(prev_start + i);
            }

            for (size_type i = 0; i < prev_size - pos_at; ++i) {
                this->_alloc.construct(this->_end - i - 1, *(prev_end - i - 1));
                this->_alloc.destroy(prev_end - i - 1);
            }

            for (size_type i = 0; i < n; ++i) {this->_alloc.construct(this->_start + pos_at + i, *first++);}
            this->_alloc.deallocate(prev_start, prev_capacity);
            return;
        }

        // Erase
        iterator erase(iterator pos) {
            if (_start == _end) return NULL;
            difference_type offset = pos - begin();
            for (iterator src = pos + 1; src < end(); ++src) { *(src - 1) = *src; }
            destroy(--_end);
            return (begin() + offset);
        }

        iterator erase(iterator first, iterator last) {
            size_type pos_at = &(*first) - this->_start;
            size_type n = last - first;
            for (size_type i = 0; i < n; ++i)
                this->_alloc.destroy(&(*(first + i)));
            for (size_type i = 0; i < this->size() - pos_at; ++i) {
                this->_alloc.construct(this->_start + pos_at + i, *(this->_start + pos_at + i + n));
                this->_alloc.destroy(this->_start + pos_at + i + n);
            }
            this->_end = this->_start + this->size() - n;
            return this->_start + pos_at;
        };

        // Swap
        void swap(vector& x) {
            if (&x == this)
                return;
            pointer start = x._start;
            pointer end = x._end;
            pointer end_capacity = x._end_capacity;
            allocator_type alloc = x._alloc;
            x._start = this->_start;
            x._end = this->_end;
            x._end_capacity = this->_end_capacity;
            x._alloc = this->_alloc;
            this->_start = start;
            this->_end = end;
            this->_end_capacity = end_capacity;
            this->_alloc = alloc;
        }

        void clear() {
            while (this->_start != this->_end)
                this->_alloc.destroy(--this->_end);
        }

        allocator_type get_allocator() const { return allocator_typr(this->_alloc); };

    protected:
        allocator_type _alloc;
        pointer _start;
        pointer _end;
        pointer _end_capacity;

        pointer allocate(size_type n) { return _alloc.allocate(n); }
        void deallocate() { _alloc.deallocate(_start, capacity()); }
        void construct(pointer ptr) { _alloc.construct(ptr, 0); }
        void construct(pointer ptr, const_reference value) {_alloc.construct(ptr, value);}
        void destroy(pointer ptr) { _alloc.destroy(ptr); }
        void destroy_until(reverse_iterator rend) {
            for (reverse_iterator riter = rbegin(); riter != rend; ++riter, --_end) {
                destroy(&*riter);
            }
        }

    }; // END CLASS VECTOR

    // Swap Vector
    template <class T, class Alloc>
    void swap (vector<T, Alloc>& x, vector<T, Alloc>& y){
        x.swap(y);
    }

    // Vector Operators
    template < class T, class Alloc >
    bool operator==(const vector< T, Alloc >& lhs, const vector< T, Alloc >& rhs) {
        if (lhs.size() != rhs.size()) { return false; }
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    };

    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return !(lhs == rhs);
    }

    template <class T, class Alloc>
    bool operator<(const vector<T, Alloc>& lhs, const vector <T, Alloc>& rhs) {
        return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc>& lhs, const vector <T, Alloc>& rhs) {
        return !(rhs < lhs);
    }

    template <class T, class Alloc>
    bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return (rhs < lhs);
    }

    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return !(lhs < rhs);
    }

} // FT Namespace