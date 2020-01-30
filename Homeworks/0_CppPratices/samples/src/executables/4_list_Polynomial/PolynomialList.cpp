#include "PolynomialList.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <math.h>
#define EPSILON 1.0e-10	// zero double
using namespace std;
PolynomialList::PolynomialList()
{
    /// do nothing
}

PolynomialList::PolynomialList(const PolynomialList &other)
{
    list<Node>::const_iterator itr = other.m_Polynomial.begin();
    for(;itr != other.m_Polynomial.end(); itr++)
    {
        AddOneTerm(*itr);
    }
}

PolynomialList::PolynomialList(const string &file)
{
    ReadFromFile(file);
}

PolynomialList::PolynomialList(const double *cof, const int *deg, int n)
{
    for(int i=0; i < n; i++)
    {
        AddOneTerm(Node(deg[i],cof[i]));
    }
}

PolynomialList::PolynomialList(const vector<int> &deg, const vector<double> &cof)
{
    if(cof.size() != deg.size())
    {
        cout<<"(cof.size() != deg.size()"<<endl;

    }
    size_t size = cof.size() < deg.size() ? cof.size() : deg.size();
    for(size_t i=0; i < size; i++)
    {
        AddOneTerm(Node(deg[i],cof[i]));
    }
}

PolynomialList::~PolynomialList()
{

}

double PolynomialList::coff(int i) const
{
    list<Node>::const_iterator itr = m_Polynomial.begin();
    for(; itr != m_Polynomial.end(); itr++)
    {
        if((*itr).deg == i)
        {
            return (*itr).cof;
        }
        if((*itr).deg > i)
        {
            break;
        }
    }
    return 0;
}

double &PolynomialList::coff(int i)
{
    return AddOneTerm(Node(i,0));
}

void PolynomialList::compress()
{
    list<Node>::iterator itr = m_Polynomial.begin();
    for(; itr != m_Polynomial.end();)
    {
        if(fabs((*itr).cof) < EPSILON)
        {
            itr = m_Polynomial.erase(itr);
        }
        else
        {
            itr++;
        }
    }

}

PolynomialList PolynomialList::operator+(const PolynomialList &right) const
{
    PolynomialList poly(right);
    list<Node>::const_iterator itr = m_Polynomial.begin();
    for(;itr!=m_Polynomial.end();itr++)
    {
        poly.AddOneTerm(*itr);
    }
    poly.compress();
    return poly;
}

PolynomialList PolynomialList::operator-(const PolynomialList &right) const
{
    PolynomialList poly(right);
    list<Node>::const_iterator itr = m_Polynomial.begin();
    for(;itr!=m_Polynomial.end();itr++)
    {
        poly.AddOneTerm(Node((*itr).deg,-(*itr).cof));
    }
    poly.compress();
    return poly;
}

PolynomialList PolynomialList::operator*(const PolynomialList &right) const
{
    PolynomialList tmpPoly;

    list<Node> p1,p2;
    p1=this->m_Polynomial;
    p2=right.m_Polynomial;


    list<Node>::const_iterator it1,it2;
    for (it1=p1.begin();it1!=p1.end();it1++)
    {
        for (it2=p2.begin();it2!=p2.end();it2++)
        {
            Node tn1,tn2;
            tn1 = *it1;
            tn2=*it2;
            tn1.cof*=tn2.cof;
            tn1.deg+=tn2.deg;
            tmpPoly.AddOneTerm(tn1);
        }
    }
    return tmpPoly;
}

PolynomialList &PolynomialList::operator=(const PolynomialList &right)
{
    m_Polynomial.clear();
    list<Node>::const_iterator itr = right.m_Polynomial.begin();
    for(;itr != right.m_Polynomial.end(); itr++)
    {
        AddOneTerm(*itr);
    }
    return *this;
}

void PolynomialList::Print() const
{
    list<Node>::const_iterator itr = m_Polynomial.begin();
    if(itr == m_Polynomial.end())
    {
        cout<<"0"<<endl;
    }
    for(; itr != m_Polynomial.end(); itr++)
    {
        if(itr != m_Polynomial.begin() && (*itr).cof > 0)
        {
            cout<<" +";
        }
        cout<<(*itr).cof;
        if((*itr).deg > 0)
        {
            cout<<"x^"<<(*itr).deg;
        }
    }
    cout<<endl;
}

bool PolynomialList::ReadFromFile(const string &file)
{
    m_Polynomial.clear();
    ifstream inp;
    inp.open(file.c_str());
    if (!inp.is_open()) {
        cout << "ERROR::PolynomialList::ReadFromFile:" << endl
            << "\t" << "file [" << file << "] opens failed" << endl;
        return false;
    }
    char ch;
    int n;
    inp>>ch;
    inp>>n;
    for (int i=0;i<n;i++)
    {
        Node nd;
        inp>>nd.deg;
        inp>>nd.cof;

        AddOneTerm(nd);
    }
    inp.close();
    return true;
}

double &PolynomialList::AddOneTerm(const Node &term)
{
   list<Node>::iterator itr = m_Polynomial.begin();
   if(itr == m_Polynomial.end() || (*itr).deg > term.deg)
   {
        m_Polynomial.push_front(term);
        return (*m_Polynomial.begin()).cof;
   }
   for(; itr != m_Polynomial.end(); itr++)
   {
        if((*itr).deg > term.deg)
        {
            break;
        }
   }
   itr--;
   if((*itr).deg == term.deg)
   {
       (*itr).cof += term.cof;
       return (*itr).cof;
   }
   else
   {
       itr++;
       if(itr == m_Polynomial.end())
       {
           m_Polynomial.push_back(term);
           return m_Polynomial.back().cof;
       }
       else
       {
            return (*m_Polynomial.insert(itr,term)).cof;
       }
   }
}


