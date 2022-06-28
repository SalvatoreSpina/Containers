#pragma once

namespace ft {

    // A pair of value
    template <class T1, class T2>
    struct pair {
        public:
            typedef T1 first_type;
            typedef T2 second_type;

            T1 first;
            T2 second;

            // Contruct pair
            pair() : first(), second(){}

            // Construct pair with pr pair objects
            template <class V1, class V2>
            pair(const pair<V1, V2>& pr) : first(pr.first), second(pr.second){}

            // Construct pair with a and b objects
            pair(const T1& a, const T2& b) : first(a), second(b){}

            // Pair, with =
            pair& operator=(const pair& other) {
                if (this == &other) return *this;
                first = other.first;
                second = other.second;
                return *this;
            }
    }; // Struct Pair

    template <class T1, class T2>
    ft::pair<T1, T2> make_pair(T1 lhs, T2 rhs) {
        return pair<T1, T2>(lhs, rhs);
    }

    template <class T1, class T2>
    bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class T1, class T2>
    bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs == rhs);
    }

    template <class T1, class T2>
    bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return lhs.first < rhs.first || 
            (!(rhs.first < lhs.first) && lhs.second < rhs.second);
    }

    template <class T1, class T2>
    bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(rhs < lhs);
    }

    template <class T1, class T2>
    bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return rhs < lhs;
    }

    template <class T1, class T2>
    bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs < rhs);
    }

    template <typename Pair>
    struct pick_first {
        typename Pair::first_type& operator()(Pair& x) const { return x.first; }
        const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
    };

}; // Namespace FT