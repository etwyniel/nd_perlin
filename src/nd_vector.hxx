#pragma once

#include <cmath>
#include "nd_vector.hh"

template <size_t N, typename T>
Vector<N, T>::Vector()
{}

/* template <size_t N> */
/* Vector<N>::Vector() */
/* { */
/*     for (size_t i = 0; i < N; i++) */
/*         coeffs[i] = 0; */
/* } */

template <size_t N, typename T>
Vector<N, T>& Vector<N, T>::operator+=(const Self& rhs)
{
    for (size_t i = 0; i < N; i++)
        coeffs[i] += rhs.coeffs[i];
    return *this;
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::operator+(const Self& rhs) const
{
    Vector ret(*this);
    ret += rhs;
    return ret;
}

template <size_t N, typename T>
Vector<N, T>& Vector<N, T>::operator-=(const Self& rhs)
{
    for (size_t i = 0; i < N; i++)
        coeffs[i] -= rhs.coeffs[i];
    return *this;
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::operator-(const Self& rhs) const
{
    Vector ret(*this);
    ret -= rhs;
    return ret;
}

template <size_t N, typename T>
T Vector<N, T>::dot(const Self& rhs) const
{
    T ret = 0;
    for (size_t i = 0; i < N; i++)
        ret += coeffs[i] * rhs.coeffs[i];
    return ret;
}

template <size_t N, typename T>
std::ostream& operator<<(std::ostream& ostr, const Vector<N, T>& v)
{
    ostr << '{';
    if constexpr (N > 0) {
        for (size_t i = 0; i < N - 1; i++)
            ostr << v.coeffs[i] << ", ";
        ostr << v.coeffs[N - 1];
    }
    ostr << '}';
    return ostr;
}

template <size_t N, typename T>
const T& Vector<N, T>::operator[](size_t ndx) const
{
    return coeffs[ndx];
}

template <size_t N, typename T>
T& Vector<N, T>::operator[](size_t ndx)
{
    return coeffs[ndx];
}

template <size_t N, typename T>
double Vector<N, T>::len_sq() const
{
    double sum = 0;
    for (size_t i = 0; i < N; i++)
        sum += coeffs[i] * coeffs[i];
    return sum;
}

template <size_t N, typename T>
double Vector<N, T>::len() const
{
    return std::sqrt(len_sq());
}
