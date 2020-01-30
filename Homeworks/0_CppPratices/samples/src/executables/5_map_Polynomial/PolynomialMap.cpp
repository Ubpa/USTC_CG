#include "PolynomialMap.h"
#include <iostream>
#include <fstream>
#include <math.h>
#define EPSILON 1.0e-10	// zero double

PolynomialMap::PolynomialMap()
{
}

PolynomialMap::PolynomialMap(const PolynomialMap &other)
{
    m_Polynomial = other.m_Polynomial;
}

PolynomialMap::PolynomialMap(const std::string &file)
{
    ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double *cof, const int *deg, int n)
{
    for(int i=0; i < n; i++)
    {
        coff(deg[i]) = cof[i];
    }
}

PolynomialMap::PolynomialMap(const std::vector<int> &deg, const std::vector<double> &cof)
{
    if(cof.size() != deg.size())
    {
        std::cout<<"(cof.size() != deg.size()"<<std::endl;

    }
    size_t size = cof.size() < deg.size() ? cof.size() : deg.size();
    for(size_t i=0; i < size; i++)
    {
        coff(deg[i]) = cof[i];
    }
}

PolynomialMap::~PolynomialMap()
{

}

double PolynomialMap::coff(int i) const
{
    if(m_Polynomial.count(i))
    {
        return m_Polynomial.at(i);
    }
    else
    {
        return 0;
    }
}

double &PolynomialMap::coff(int i)
{
    return m_Polynomial[i];
}

void PolynomialMap::compress()
{
    /// Safe but not effective method--- to study stl for  getting more effective method
    std::map<int,double> tmpPoly = m_Polynomial;
    m_Polynomial.clear();
    for(std::map<int,double>::iterator itr = tmpPoly.begin(); itr != tmpPoly.end(); itr++)
    {
        if(fabs(itr->second) > EPSILON)
        {
            m_Polynomial[itr->first] = itr->second;
        }
    }
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap &right) const
{
    PolynomialMap poly(right);
    std::map<int,double>::const_iterator itr = m_Polynomial.begin();
    for(; itr != m_Polynomial.end(); itr++)
    {
        poly.coff(itr->first) += itr->second;
    }
    poly.compress();
    return poly;

}

PolynomialMap PolynomialMap::operator-(const PolynomialMap &right) const
{
    PolynomialMap poly(right);
    std::map<int,double>::const_iterator itr = m_Polynomial.begin();
    for(; itr != m_Polynomial.end(); itr++)
    {
        poly.coff(itr->first) -= itr->second;
    }
    poly.compress();
    return poly;
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap &right) const
{
    PolynomialMap poly;
    std::map<int,double>::const_iterator itr0 = m_Polynomial.begin();
    for(;itr0 != m_Polynomial.end(); itr0++)
    {
        std::map<int,double>::const_iterator itr1 = right.m_Polynomial.begin();
        for(; itr1 != right.m_Polynomial.end(); itr1++)
        {
            poly.coff(itr0->first + itr1->first) += itr0->second * itr1->second;
        }
    }
    return poly;

}

PolynomialMap &PolynomialMap::operator=(const PolynomialMap &right)
{
    m_Polynomial = right.m_Polynomial;
    return *this;
}

void PolynomialMap::Print() const
{
    std::map<int,double>::const_iterator itr = m_Polynomial.begin();
    if(itr == m_Polynomial.end())
    {
        std::cout<<"0"<<std::endl;
    }
    for(;itr!=m_Polynomial.end();itr++)
    {
        if(itr != m_Polynomial.begin() && (itr)->second >= 0)
        {
            std::cout<<" +";
        }
        std::cout<<itr->second;
        if((*itr).first != 0 )
        {
            std::cout<<"x^"<<(itr)->first;
        }
    }
    std::cout<<std::endl;

}

void PolynomialMap::ReadFromFile(const std::string &file)
{
    m_Polynomial.clear();
    std::ifstream inp;
    inp.open(file.c_str());
    char ch;
    int n;
    inp>>ch;
    inp>>n;
    for (int i=0;i<n;i++)
    {
        int deg;
        double cof;
        inp>>deg;
        inp>>cof;
        coff(deg) = cof;

    }
    inp.close();
}
