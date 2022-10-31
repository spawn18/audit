#include "Selection.hpp"

const double EPS = 1E-8;

Selection::Selection(double a, double b)
:
begin(a),
end(b)
{
}

double Selection::length() const
{
    return end - begin;
}

bool Selection::isEmpty() const
{
    return length() < EPS;
}
