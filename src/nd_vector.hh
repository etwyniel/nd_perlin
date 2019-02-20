#pragma once

#include <ostream>

template <size_t N, typename T = double>
struct Vector
{
    using Self = Vector<N, T>;

    Vector();
    Vector& operator+=(const Vector& rhs);
    Vector operator+(const Vector& rhs) const;
    Vector& operator-=(const Vector& rhs);
    Vector operator-(const Vector& rhs) const;
    const T& operator[](size_t ndx) const;
    T& operator[](size_t ndx);
    T dot(const Vector& rhs) const;
    
    double len_sq() const;
    double len() const;

    T coeffs[N];
};

template <size_t N, typename T>
std::ostream& operator<<(std::ostream& ostr, const Vector<N, T>& v);

#include "nd_vector.hxx"
