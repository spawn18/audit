#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <QtGlobal>

struct Selection
{
    Selection(double, double);

    Selection() = default;
    ~Selection() = default;

    double length() const;
    bool isEmpty() const;


    double begin{0};
    double end{0};
};


#endif // SELECTION_HPP
