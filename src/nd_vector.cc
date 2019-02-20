#pragma once

#include "nd_vector.hh"

template <size_t N>
Vector<N>::Vector()
{
    for (size_t i = 0; i < N; i++)
        coeffs[i] = 0;
}
