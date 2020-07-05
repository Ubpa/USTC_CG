#pragma once

#include <list>
#include <string>
#include <vector>

class PolynomialList
{
public:
    PolynomialList() { };
    PolynomialList(const PolynomialList& other);
    PolynomialList(const std::string& file); // initialization using file
    PolynomialList(const double* cof, const int* deg, int n);
    PolynomialList(const std::vector<int>& deg, const std::vector<double>& cof);

    double& coff(int i);
    double coff(int i) const;

    void compress();

    // overload
    PolynomialList operator+(const PolynomialList& right) const; //Overload operator +
    PolynomialList operator-(const PolynomialList& right) const; //Overload operator -
    PolynomialList operator*(const PolynomialList& right) const; //Overload operator *
    PolynomialList& operator=(const PolynomialList& right); //Overload operator =

    void Print() const;

private:
    struct Term {
        int deg;
        double cof;

        Term(int deg, double cof) : deg(deg), cof(cof) { }
        Term() : Term(0, 0) { }
    };
    bool ReadFromFile(const std::string& file);
    Term& AddOneTerm(const Term& term); // add one term into m_Polynomial

private:
    std::list<Term> m_Polynomial; // high degree -> low degree
};
