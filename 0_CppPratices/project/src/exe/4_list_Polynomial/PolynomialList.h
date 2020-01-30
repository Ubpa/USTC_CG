#ifndef POLYNOMIALLIST_H
#define POLYNOMIALLIST_H
#include <list>
#include <string>
#include <vector>
class PolynomialList
{
public:
    PolynomialList();
    PolynomialList(const PolynomialList& other);
    PolynomialList(const std::string& file); // initialization using file
    PolynomialList(const double *cof,const int *deg, int n);
    PolynomialList( const std::vector<int>& deg,const std::vector<double>& cof);
    virtual ~PolynomialList();

    double coff(int i) const;
    double& coff(int i);

    void compress();
    // overload
    PolynomialList operator+( const PolynomialList &right ) const; //Overload operator +
    PolynomialList operator-( const PolynomialList &right ) const; //Overload operator -
    PolynomialList operator*( const PolynomialList &right ) const; //Overload operator *
    PolynomialList& operator=(const PolynomialList &right ); //Overload operator =

    void Print() const;
};

#endif // POLYNOMIALLIST_H
