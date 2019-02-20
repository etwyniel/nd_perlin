#pragma once

#include <vector>

const size_t PERMUTATIONS = 512;

template <size_t N>
struct Perlin
{
    Perlin();
    size_t hash(const Vector<N, long>& v) const;
    double value_at(const Vector<N>& v) const;

    Vector<N> values[PERMUTATIONS];
    size_t perm_table[PERMUTATIONS * 2];
};

double lerp(double x, double y, double t);
double slerp(double x, double y, double t);

void render();

#include "nd_perlin.hxx"
