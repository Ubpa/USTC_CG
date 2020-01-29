#ifndef POLYNOMIALMAP_H
#define POLYNOMIALMAP_H
#include <map>
#include <vector>
#include <string>
class PolynomialMap
{
public:
    PolynomialMap();
    PolynomialMap(const PolynomialMap& other);
    PolynomialMap(const std::string& file); // initialization using file
    PolynomialMap(const double *cof,const int *deg, int n);
    PolynomialMap( const std::vector<int>& deg,const std::vector<double>& cof);
    virtual ~PolynomialMap();

    double coff(int i) const;
    double& coff(int i);

    void compress();
    // overload
    PolynomialMap operator+( const PolynomialMap &right ) const; //Overload operator +
    PolynomialMap operator-( const PolynomialMap &right ) const; //Overload operator -
    PolynomialMap operator*( const PolynomialMap &right ) const; //Overload operator *
    PolynomialMap& operator=(const PolynomialMap &right ); //Overload operator =

    void Print() const;
private:
    void ReadFromFile(const std::string& file);
    std::map<int,double> m_Polynomial;
};

#endif // POLYNOMIALMAP_H
