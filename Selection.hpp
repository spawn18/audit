#ifndef SELECTION_HPP
#define SELECTION_HPP

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


#endif
