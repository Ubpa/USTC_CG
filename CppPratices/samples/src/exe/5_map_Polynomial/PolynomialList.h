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
private:
    struct Node
    {
        Node():deg(0),cof(0)
        {

        }

        Node(int _deg, double _cof):
            deg(_deg),cof(_cof)
        {

        }
        int deg;
        double cof;
        bool operator < (const Node& n) const
        {
            if(deg <n.deg)
                return true;
            return false;
        }
    };
    bool ReadFromFile(const std::string& file);
    double& AddOneTerm(const Node& term); // add one term into m_Polynomial


private:
    std::list<Node> m_Polynomial;
};

#endif // POLYNOMIALLIST_H
