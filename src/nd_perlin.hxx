#pragma once

#include <cmath>
#include <random>

template <size_t N>
inline double interpolate(const Vector<1 << N>& weights, const Vector<N>& t)
{
    if constexpr (N == 1) {
        return slerp(weights[0], weights[1], t[0]);
    } else {
        Vector<1 << (N - 1)> w0, w1;
#pragma unroll
        for (size_t i = 0; i < 1 << (N - 1); i++)
        {
            w0[i] = weights[2 * i];
            w1[i] = weights[2 * i + 1];
        }
        Vector<N - 1> t0;
#pragma unroll
        for (size_t i = 0; i < N - 1; i++)
            t0[i] = t[i + 1];
        return slerp(interpolate(w0, t0), interpolate(w1, t0), t[0]);
    }
}

template <size_t N>
Perlin<N>::Perlin()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distr(-1, 1);
    std::uniform_int_distribution<size_t> int_distr(0, PERMUTATIONS - 1);

    for (size_t p = 0; p < PERMUTATIONS; p++)
    {
        Vector<N>& v = values[p];
        double len = 0;
        do {
            for (size_t i = 0; i < N; i++)
                v[i] = distr(generator);
            len = v.len_sq();
        } while (len > 1);

        if constexpr (N > 1) {
            len = std::sqrt(len);
            if (len > 0)
                for (size_t i = 0; i < N; i++)
                    v[i] /= len;
        }
        perm_table[p] = p;
    }

    for (size_t p = 0; p < PERMUTATIONS; p++)
    {
        size_t i = int_distr(generator);
        std::swap(perm_table[p], perm_table[i]);
        perm_table[p + PERMUTATIONS] = perm_table[p];
    }
}

template <size_t N>
inline size_t Perlin<N>::hash(const Vector<N, long>& v) const
{
    size_t ndx = 0;
#pragma unroll
    for (size_t i = 0; i < N; i++)
        ndx = perm_table[v[i] + ndx];

    return ndx;
}

template <size_t N>
inline double Perlin<N>::value_at(const Vector<N>& v) const
{
    const long perm_mask = PERMUTATIONS - 1;

    Vector<N, long> i;
#pragma unroll
    for (size_t j = 0; j < N; j++)
        i[j] = ((long)v[j]) & perm_mask;

    Vector<N> t;
#pragma unroll
    for (size_t j = 0; j < N; j++)
        t[j] = v[j] - i[j];

    Vector<N, long> r0, r1;
#pragma unroll
    for (size_t j = 0; j < N; j++)
    {
        r0[j] = i[j];
        r1[j] = (i[j] + 1) & perm_mask;
    }

    Vector<1 << N, Vector<N>> corner_values;
    Vector<N, long> corner_pos;
#pragma unroll
    for (size_t corner = 0; corner < 1 << N; corner++)
    {
#pragma unroll
        for (size_t n = 0; n < N; n++)
        {
            if (corner & (1 << n))
                corner_pos[n] = r1[n];
            else
                corner_pos[n] = r0[n];
        }
        corner_values[corner] = values[hash(corner_pos)];
    }

    Vector<1 << N, Vector<N>> distances;
#pragma unroll
    for (size_t corner = 0; corner < 1 << N; corner++)
    {
#pragma unroll
        for (size_t n = 0; n < N; n++)
        {
            if (corner & (1 << n))
                distances[corner][n] = t[n] - 1;
            else
                distances[corner][n] = t[n];
        }
    }

    Vector<1 << N> weights;
#pragma unroll
    for (size_t corner = 0; corner < 1 << N; corner++)
         weights[corner] = corner_values[corner].dot(distances[corner]);

    return (interpolate(weights, t) + 1) * 0.5;
}

double lerp(double x, double y, double t)
{
    return x + (y - x) * t;
}

double slerp(double x, double y, double t)
{
    double t2 = t * t;
    return lerp(x, y, t2 * t * (6 * t2 - 15 * t + 10));
}
